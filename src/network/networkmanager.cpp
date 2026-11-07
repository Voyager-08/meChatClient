#include "networkmanager.h"
#include <QHostAddress>
#include <QThread>
#include <QTcpSocket>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtEndian>
#include <QTimer>

#include "src/event/eventbus.h"

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
{
    registerCustomTypes();
}

void NetworkManager::initNetwork()
{
    if (m_socket)
        return;
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &NetworkManager::onError);
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connectToServer();
}

bool NetworkManager::isConnected() const
{
    return m_socket && m_socket->state() == QTcpSocket::ConnectedState;
}

void NetworkManager::connectToServer()
{
    if (!m_socket || m_socket->state() != QAbstractSocket::UnconnectedState)
        return;

    const QString host = qEnvironmentVariable("MECHAT_SERVER_HOST", "127.0.0.1").trimmed();
    bool portOk = false;
    const uint port = qEnvironmentVariable("MECHAT_SERVER_PORT", "6452").toUInt(&portOk);
    if (host.isEmpty() || !portOk || port == 0 || port > 65535) {
        emit errorOccurred("MECHAT_SERVER_HOST or MECHAT_SERVER_PORT is invalid");
        return;
    }
    m_socket->connectToHost(host, static_cast<quint16>(port));
}

// ==================== 发送方法（Protobuf 序列化） ====================

void NetworkManager::sendMessage(const MessageInfo &messageInfo)
{
    if (!isConnected()) {
        qDebug() << "连接服务器失败！";
        return;
    }

    mechat::Packet packet;
    packet.set_type(mechat::MESSAGE);
    auto *msg = packet.mutable_chat_message();
    msg->set_sender(messageInfo.senderId.toStdString());
    msg->set_receiver(messageInfo.receiverId.toStdString());
    msg->set_content(messageInfo.content.toStdString());
    msg->set_timestamp(messageInfo.timestamp.toString(Qt::ISODate).toStdString());

    sendRawData(packet);
}

void NetworkManager::sendLoginRequest(const QString &userId, const QString &password)
{
    if (!isConnected() && (!m_socket || m_socket->state() != QAbstractSocket::ConnectingState)) {
        qDebug() << "登录请求发送失败！";
        emit loginFailed("网络未连接！");
        return;
    }

    mechat::Packet packet;
    packet.set_type(mechat::LOGIN_REQUEST);
    auto *login = packet.mutable_login_request();
    login->set_user_id(userId.toStdString());
    login->set_password(password.toStdString());
    sendRawData(packet);
}

void NetworkManager::sendLogoutRequest(const QString &userId)
{
    if (!isConnected()) {
        qDebug() << "登出请求发送失败！";
        return;
    }
    mechat::Packet packet;
    packet.set_type(mechat::LOGOUT_REQUEST);
    packet.mutable_logout_request()->set_user_id(userId.toStdString());
    sendRawData(packet);
}

void NetworkManager::requestUserInfo(const QString &userId)
{
    qDebug()<<"发送用户信息请求";
    qDebug()<<"用户ID:"<<userId;
    if (!isConnected()) {
        qDebug() << "请求用户信息发送失败！";
        return;
    }
    mechat::Packet packet;
    packet.set_type(mechat::USER_INFO_REQUEST);
    auto *req = packet.mutable_user_info_request();
    req->set_user_id(userId.toStdString());
    sendRawData(packet);
}

void NetworkManager::sendFriendRequest(const QString &userId)
{
    if (!isConnected()) {
        qDebug() << "请求好友列表发送失败！";
        return;
    }
    mechat::Packet packet;
    packet.set_type(mechat::FRIEND_LIST_REQUEST);
    auto *req = packet.mutable_friend_list_request();
    req->set_user_id(userId.toStdString());
    sendRawData(packet);
}

void NetworkManager::sendRegisterRequest(const QString &userId, const QString &userNick, const QString &password)
{
    if (!isConnected() && (!m_socket || m_socket->state() != QAbstractSocket::ConnectingState)) {
        qDebug() << "注册请求发送失败！";
        emit registerFailed("网络未连接！");
        return;
    }
    mechat::Packet packet;
    packet.set_type(mechat::REGISTER_REQUEST);
    auto *reg = packet.mutable_register_request();
    reg->set_user_id(userId.toStdString());
    reg->set_user_nick(userNick.toStdString());
    reg->set_password(password.toStdString());

    sendRawData(packet);
}

void NetworkManager::sendAddFriendRequest(const QString &friendId)
{
    if (!isConnected()) {
        qDebug() << "网络未连接，无法发送添加好友消息！";
        return;
    }
    mechat::Packet packet;
    packet.set_type(mechat::ADD_FRIEND);
    auto *add = packet.mutable_add_friend();
    add->set_friend_id(friendId.toStdString());
    sendRawData(packet);
}

void NetworkManager::sendFriendStrListRequest(const QString &friendStr)
{
    if (!isConnected()) {
        qDebug() << "网络未连接，无法发送搜索好友Id消息！";
        return;
    }
    mechat::Packet packet;
    packet.set_type(mechat::SEARCH_FRIEND);
    auto *search = packet.mutable_search_friend();
    search->set_text(friendStr.toStdString());
    sendRawData(packet);
}

void NetworkManager::sendDeleteFriendRequest(const QString &friendId)
{
    if (!isConnected()) {
        qDebug() << "网络未连接，无法发送删除好友消息！";
        return;
    }
    mechat::Packet packet;
    packet.set_type(mechat::DELETE_FRIEND);
    auto *del = packet.mutable_delete_friend();
    del->set_friend_id(friendId.toStdString());
    sendRawData(packet);
}

void NetworkManager::sendAvatarFile(const QString &filePath, const QString &userId)
{
    if (!isConnected()) {
        qDebug() << "网络未连接，无法发送文件！";
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件:" << filePath;
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    // 使用 Protobuf 的 AVATAR_DATA 类型打包
    // user_id 序列化，file_data 作为原始字节（Protobuf bytes 类型就是原始字节）
    mechat::Packet packet;
    packet.set_type(mechat::AVATAR_DATA);
    auto *avatar = packet.mutable_avatar_data();
    avatar->set_user_id(userId.toStdString());
    avatar->set_avatar_data(fileData.constData(), fileData.size());

    sendRawData(packet);

    qDebug() << "发送文件:" << filePath
             << "userId:" << userId
             << "size:" << fileData.size();
}

void NetworkManager::sendHeartbeatMessage(const QString &userId)
{
    if (!isConnected()) {
        qDebug() << "网络未连接，无法发送心跳消息！";
        return;
    }
    mechat::Packet packet;
    packet.set_type(mechat::HEARTBEAT);//设置消息类型为心跳
    auto *hb = packet.mutable_heartbeat();//获取心跳消息指针
    hb->set_user_id(userId.toStdString());//设置用户ID
    sendRawData(packet);//发送原始数据
}

// ==================== 发送原始数据（二进制打包） ====================

bool NetworkManager::sendRawData(const mechat::Packet &packet)
{
    if (!m_socket) {
        qDebug() << "网络套接字尚未初始化！";
        return false;
    }
    QByteArray packetData = m_codec.pack(packet);
    if (packetData.isEmpty()) {
        qDebug() << "数据包打包失败！";
        return false;
    }
    if (m_socket->state() == QAbstractSocket::ConnectingState) {
        m_pendingWrites.append(packetData);
        return true;
    }
    if (!isConnected()) {
        qDebug() << "网络未连接，无法发送数据！";
        return false;
    }
    qint64 bytesWritten = m_socket->write(packetData);
    if (bytesWritten != packetData.size()) {
        qDebug() << "数据发送不完整，期望:" << packetData.size() << "实际:" << bytesWritten;
        return false;
    }
    m_socket->flush();
    return true;
}

// ==================== 连接回调 ====================

void NetworkManager::onConnected()
{
    qDebug() << "已连接到服务器！";
    for (const QByteArray &frame : m_pendingWrites) {
        if (m_socket->write(frame) != frame.size()) {
            m_pendingWrites.clear();
            emit errorOccurred("发送连接期间排队的数据失败");
            return;
        }
    }
    m_pendingWrites.clear();
    emit connected();
}

void NetworkManager::onDisconnected()
{
    qDebug() << "服务器已断开！";
    m_codec.clearBuffer();
    emit errorOccurred("服务器已断开！");
    QTimer::singleShot(2000, this, &NetworkManager::connectToServer);
}

void NetworkManager::onError()
{
    qDebug() << "网络错误:" << m_socket->errorString();
    emit error(m_socket->errorString());
    QTimer::singleShot(2000, this, &NetworkManager::connectToServer);
}

// ==================== 接收数据（二进制解包 + Protobuf 反序列化） ====================

void NetworkManager::onReadyRead()
{
    if (!isConnected()) {
        qDebug() << "网络未连接，无法读取数据！";
        return;
    }

    QByteArray data = m_socket->readAll();
    QList<mechat::Packet> packets = m_codec.unpack(data);

    for (const mechat::Packet &packet : packets) {
        mechat::MessageType msgType = packet.type();

        switch (msgType) {
        case mechat::MESSAGE: {
            qDebug() << "收到消息类型为MESSAGE的消息";
            const auto &msg = packet.chat_message();
            QString senderId = QString::fromStdString(msg.sender());
            QString receiverId = QString::fromStdString(msg.receiver());
            QString content = QString::fromStdString(msg.content());
            QDateTime time = QDateTime::fromString(QString::fromStdString(msg.timestamp()), Qt::ISODate);
            emit receiveMessage(MessageInfo{senderId, receiverId, content, time});
            break;
        }
        case mechat::LOGIN_RESULT: {
            qDebug() << "收到消息类型为LOGIN_RESULT的消息";
            const auto &result = packet.login_result();
            if (result.result()) {
                emit loginSuccess(QString::fromStdString(result.user_id()));
            } else {
                emit loginFailed(QString::fromStdString(result.reason()));
            }
            break;
        }
        case mechat::OFFLINE_MESSAGE: {
            qDebug() << "收到消息类型为OFFLINE_MESSAGE的消息";
            const auto &offline = packet.offline_message();
            for (int i = 0; i < offline.messages_size(); ++i) {
                const auto &msg = offline.messages(i);
                QString senderId = QString::fromStdString(msg.sender());
                QString receiverId = QString::fromStdString(msg.receiver());
                QString content = QString::fromStdString(msg.content());
                QDateTime time = QDateTime::fromString(QString::fromStdString(msg.timestamp()), Qt::ISODate);
                if (!time.isValid()) {
                    qDebug() << "离线消息时间戳无效";
                }
                emit receiveMessage(MessageInfo{senderId, receiverId, content, time});
            }
            break;
        }
        case mechat::USER_STATUS: {
            qDebug() << "收到消息类型为USER_STATUS的消息";
            const auto &status = packet.user_status();
            QString userID = QString::fromStdString(status.user_id());
            bool online = status.online();
            emit userStatusChanged(userID, online);
            break;
        }
        case mechat::REGISTER_RESULT: {
            qDebug() << "收到消息类型为REGISTER_RESULT的消息";
            const auto &result = packet.register_result();
            if (result.result()) {
                emit registerSuccess(QString::fromStdString(result.user_id()));
            } else {
                emit registerFailed(QString::fromStdString(result.reason()));
            }
            break;
        }
        case mechat::USER_INFO: {
            qDebug() << "收到消息类型为USER_INFO的消息";
            const auto &info = packet.user_info();
            UserInfo userInfo;
            userInfo.userID = QString::fromStdString(info.user_id());
            userInfo.userNick = QString::fromStdString(info.user_nick());
            userInfo.userMotto = QString::fromStdString(info.user_motto());
            userInfo.userStatus = QString::fromStdString(info.user_status());
            userInfo.userEmail = QString::fromStdString(info.user_email());
            userInfo.userRegistrationDate = QDate::fromString(QString::fromStdString(info.user_registration_date()), Qt::ISODate);
            userInfo.userSex = static_cast<Sex>(info.user_sex());
            emit receiveUserInfo(userInfo);
            break;
        }
        case mechat::FRIEND_LIST: {
            qDebug() << "收到消息类型为FRIEND_LIST的消息";
            const auto &friendList = packet.friend_list();
            for (int i = 0; i < friendList.friends_size(); ++i) {
                const auto &f = friendList.friends(i);
                QString friendId = QString::fromStdString(f.friend_id());
                QString friendNick = QString::fromStdString(f.friend_nick());
                QString friendNote = QString::fromStdString(f.friend_note());
                QString motto = QString::fromStdString(f.motto());
                QString email = QString::fromStdString(f.email());

                qDebug() << "解析到好友:" << friendId << friendNick << friendNote;
                FriendInfo info;
                info.friendNote = friendNote;
                info.friendNick = friendNick;
                info.friendId = friendId;
                info.avatarPath = QDir::currentPath() + "/images/avatar/" + friendId + ".png";
                info.motto = motto;
                info.email = email;
                emit receiveFriendInfo(info);
            }
            break;
        }
        case mechat::HEARTBEAT_RESPONSE: {
            const auto &hb = packet.heartbeat_response();
            QString userId = QString::fromStdString(hb.user_id()).trimmed();
            if (userId.isEmpty()) {
                qDebug() << "心跳消息字段缺失";
                break;
            }
            qDebug() << "收到HEARTBEAT_RESPONSE消息，用户ID:" << userId;
            break;
        }
        case mechat::SEARCH_FRIEND_LIST: {
            qDebug() << "收到消息类型为SEARCH_FRIEND_LIST的消息";
            const auto &searchResult = packet.search_friend_list();
            if (searchResult.result()) {
                emit clearAddFriendList();
                for (int i = 0; i < searchResult.friends_size(); ++i) {
                    const auto &f = searchResult.friends(i);
                    QString friendId = QString::fromStdString(f.friend_id());
                    QString friendNick = QString::fromStdString(f.friend_nick());
                    qDebug() << "解析到好友:" << friendId << " . " << friendNick;
                    emit receiveAddFriendList(friendId, friendNick);
                }
            }
            break;
        }
        case mechat::ADD_FRIEND_RESULT: {
            qDebug() << "收到消息类型为ADD_FRIEND_RESULT的消息";
            const auto &result = packet.add_friend_result();
            if (result.result()) {
                FriendInfo friendInfo;
                friendInfo.friendId = QString::fromStdString(result.friend_id());
                friendInfo.friendNick = QString::fromStdString(result.friend_nick());
                friendInfo.avatarPath = "./images/avatar/" + friendInfo.friendId + ".png";
                qDebug() << "添加好友成功:" << friendInfo.friendId << friendInfo.friendNick;
                emit addFriendSuccess(friendInfo);
            } else {
                QString errorString = QString::fromStdString(result.reason());
                qDebug() << "添加好友失败:" << errorString;
                emit addFriendFailed(errorString);
            }
            break;
        }
        case mechat::AVATAR_DATA: {
            const auto &avatar = packet.avatar_data();
            QString userId = QString::fromStdString(avatar.user_id());
            QString avatarPath = QDir::currentPath() + "/images/avatar/" + userId + ".png";
            if (!QFile::exists(avatarPath)) {
                QByteArray avatarData(avatar.avatar_data().data(), static_cast<int>(avatar.avatar_data().size()));
                QDir().mkpath(QFileInfo(avatarPath).absolutePath());
                QFile avatarFile(avatarPath);
                if (!avatarFile.open(QIODevice::WriteOnly) || avatarFile.write(avatarData) != avatarData.size()) {
                    qWarning() << "Unable to save avatar:" << avatarFile.errorString();
                    break;
                }
                qDebug() << "收到消息类型为AVATAR_DATA的消息";
                qDebug() << "保存头像到文件:" << avatarPath;
            }
            break;
        }
        case mechat::DELETE_FRIEND_RESULT: {
            qDebug() << "收到消息类型为DELETE_FRIEND_RESULT的消息";
            const auto &result = packet.delete_friend_result();
            emit deleteFriendResult(QString::fromStdString(result.friend_id()), result.result());
            break;
        }
        default:
            qDebug() << "未知消息类型:" << static_cast<int>(msgType);
            break;
        }
    }
}
