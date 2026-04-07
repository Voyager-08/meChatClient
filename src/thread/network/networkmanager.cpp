#include "networkmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>
#include <QDir>

#include "src/custom/struct.h"

using namespace MeChat;

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)  
    , m_socket(new QTcpSocket(this))  // 在构造函数创建QTcpSocket对象作为成员变量，是为了在整个NetworkManager生命周期内保持网络连接的状态和管理网络通信
{
    registerCustomTypes();// 注册自定义类型
    /* 连接QTcpSocket的readyRead信号到NetworkManager的onReadyRead槽函数
    当有数据可读时触发 */
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);

    /* 连接QTcpSocket的errorOccurred信号到NetworkManager的onError槽函数
    当发生错误时触发 */
    connect(m_socket, &QTcpSocket::errorOccurred,this, &NetworkManager::onError);
    
    /* 连接QTcpSocket的connected信号到NetworkManager的onConnected槽函数
    当连接成功时触发 */
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);

    /* 连接QTcpSocket的disconnected信号到NetworkManager的onDisconnected槽函数
    当连接断开时触发 */
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);

    // 注意：不要在这里调用connectToServer()，因为此时NetworkManager可能还没有被移到子线程
}

void NetworkManager::connectToServer()
{
    m_socket->connectToHost("8.129.134.106", 6452);//连接到服务器，IP地址和端口
}

void NetworkManager::sendMessage(const messageData &data)
{
    if (m_socket->state() != QTcpSocket::ConnectedState) 
    {        
        connectToServer();
        if(m_socket->state() != QTcpSocket::ConnectedState)
        {
            qDebug() << "连接服务器失败！";
            return;
        }
    }

    QJsonObject messageObj;// 创建一个JSON对象来存储消息数据
    messageObj["type"] = "message";
    messageObj["sender"] = data.senderId;
    messageObj["receiver"] = data.receiverId;
    messageObj["content"] = data.content;
    messageObj["timestamp"] = data.timestamp.toString(Qt::ISODate);
    sendRawData(messageObj); // 发送 + 换行符
}

void NetworkManager::sendLoginRequest(const QString &userId, const QString &password)
{
    QJsonObject loginObj;// 创建一个JSON对象来存储登录消息数据
    loginObj["type"] = "login";
    loginObj["userId"] = userId;
    loginObj["password"] = password;
    sendRawData(loginObj);
}

void NetworkManager::sendLogoutRequest(const QString &userId)
{
    QJsonObject logoutObj;// 创建一个JSON对象来存储退出登录消息数据
    logoutObj["type"] = "logout";
    logoutObj["userId"] = userId;
    sendRawData(logoutObj);
}

void NetworkManager::requestUserInfo(const QString &userId)
{
    QJsonObject user_info;
    user_info["type"] = "user_info";
    user_info["userId"] = userId;
    sendRawData(user_info);
}

void NetworkManager::sendFriendRequest(const QString &userId)
{
    QJsonObject user_friends;
    user_friends["type"] = "user_friends";
    user_friends["userId"] = userId;
    sendRawData(user_friends);
}

void NetworkManager::sendRawData(const QJsonObject &data)
{
    if (m_socket->state() != QTcpSocket::ConnectedState) 
    {        
        qDebug() << "网络未连接，无法发送数据！";
        return;
    }
    QJsonDocument doc(data);// 将JSON对象转换为JSON文档，以便序列化为字符串
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact) + "\n"; // 必须加 \n！
    m_socket->write(jsonData);
    m_socket->flush(); // 确保数据立即发送
    qDebug() << "客户端发送原始数据:" << jsonData;
}

void NetworkManager::onConnected()
{
    qDebug() << "已连接到服务器！";
    emit connected();
}

void NetworkManager::registerUser(const QString &userId, const QString &userNick, const QString &password)
{
    if (m_socket->state() != QTcpSocket::ConnectedState) 
    {        
        qDebug() << "网络未连接，无法发送注册消息！";
        return;
    }
    QJsonObject messageObj;// 创建一个JSON对象来存储注册消息数据
    messageObj["type"] = "register";
    messageObj["userId"] = userId;
    messageObj["userNick"] = userNick;
    messageObj["password"] = password;
    sendRawData(messageObj);
}
// 发送添加好友请求
void NetworkManager::sendAddFriendRequest(const QString &friendId)
{
    QJsonObject add_friend;
    add_friend["type"] = "add_friend";
    add_friend["friendId"] = friendId;
    sendRawData(add_friend);
}
// 发送搜索好友Id请求
void NetworkManager::sendFriendStrListRequest(const QString &friendStr)
{
    QJsonObject add_friend_list;
    add_friend_list["type"] = "search_friend_list";
    add_friend_list["friendStr"] = friendStr;
    sendRawData(add_friend_list);
}

/**
 * @brief NetworkManager::onReadyRead
 * 处理网络数据就绪的槽函数，当有数据可读时被调用
 * 该函数会持续读取socket中的数据行，解析JSON格式消息，并根据消息类型触发相应的信号
 */
void NetworkManager::onReadyRead()
{
    // 循环读取socket中的所有可用行
    while (m_socket->canReadLine()) {
        // 读取一行数据
        QByteArray line = m_socket->readLine();
        // 将JSON格式的数据转换为QJsonDocument对象
        QJsonDocument doc = QJsonDocument::fromJson(line);
        
        // 检查JSON解析是否成功
        if (doc.isNull()) {
            qDebug() << "JSON解析失败，数据:" << line;
            continue;
        }
        
        // 获取JSON对象
        QJsonObject obj = doc.object();
        
        // 检查是否包含type字段
        if (!obj.contains("type")) {
            qDebug() << "JSON缺少type字段，数据:" << line;
            continue;
        }
        
        // 获取消息类型字段
        QString msgType = obj["type"].toString();
        qDebug() << "解析消息类型:" << msgType;
        if (msgType == "message") {// 处理消息类型为"message"的情况
            qDebug() << "收到消息类型为message的消息";
            // 提取发送者ID
            QString senderId = obj["sender"].toString();
            // 提取接收者ID
            QString receiverId = obj["receiver"].toString();
            // 提取消息内容
            QString content = obj["content"].toString();
            // 提取时间戳并转换为QDateTime对象
            QDateTime time = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
            // 发送消息接收信号
            emit receiveMessage(messageData{senderId,receiverId,content,time});
        }
        else if (msgType == "login_result") // 处理登录结果消息
        {
            qDebug() << "收到消息类型为login_result的消息";
            if (obj["result"].toBool())
            {
                QString userId = obj["userId"].toString();
                emit loginSuccess(userId);
            }
            else
            {
                QString errorString = obj["reason"].toString();
                emit loginFailed(errorString);
            }
        }
        else if (msgType == "offline_message") // 处理离线消息
        {
            qDebug() << "收到消息类型为offline_message的消息";
            // 提取发送者ID
            QString senderId = obj["sender"].toString();
            // 提取接收者ID
            QString receiverId = obj["receiver"].toString();
            // 提取消息内容
            QString content = obj["content"].toString();
            // 提取时间戳并转换为QDateTime对象
            QString timestampStr = obj["timestamp"].toString();
            QDateTime time = QDateTime::fromString(timestampStr, Qt::ISODate);
            
            // 如果时间戳无效，使用当前时间作为默认值
            if (!time.isValid()) {
                qDebug() << "离线消息时间戳无效";
            }
            
            // 发送消息接收信号
            emit receiveMessage(messageData{senderId,receiverId,content,time});
        }
        else if (msgType == "user_status")// 处理用户状态变更消息
        {
            qDebug() << "收到消息类型为user_status的消息";
            QString userID = obj["user_id"].toString();  // 从JSON对象中获取用户ID并转换为QString类型
            bool online = obj["online"].toBool();  // 从JSON对象中获取在线状态并转换为bool类型
            emit userStatusChanged(userID, online);  // 发射用户状态变更信号，传递用户ID和在线状态
        }
        else if (msgType == "register_result") 
        // 处理注册结果消息
        {
            qDebug() << "收到消息类型为register_result的消息";
            if (obj["result"].toBool())
            {
                QString userId = obj["userId"].toString();
                emit registerSuccess(userId);
            }
            else
            {
                QString errorString = obj["reason"].toString();
                emit registerFailed(errorString);
            }
        }
        else if(msgType == "user_info")
        {// 处理消息类型为"user_info"的情况
            qDebug() << "收到消息类型为user_info的消息";
            UserInfo *userInfo = new UserInfo();
            userInfo->userNick = obj["user_nick"].toString();
            userInfo->userMotto = obj["user_motto"].toString();
            userInfo->userStatus = obj["user_status"].toString();
            userInfo->userEmail = obj["user_email"].toString();
            userInfo->userRegistrationDate = QDate::fromString(obj["user_registration_date"].toString(), Qt::ISODate);
            userInfo->userSex = (Sex)obj["user_sex"].toInt();
            emit receiveUserInfo(userInfo);
        }
        else if(msgType == "friend_list")
        // 处理好友列表消息
        {
            qDebug() << "收到消息类型为friend_list的消息";
            // 注意：服务端发送的字段是 "friendList"
            QJsonArray friendList = obj["friendList"].toArray(); 
        
            for (const QJsonValue &friendValue : friendList)
            {
                QJsonObject friendObj = friendValue.toObject();

                // 从JSON对象中提取所有字段
                QString friendId = friendObj["friendId"].toString();
                QString friendNote = friendObj["friendNote"].toString();
                QString friendNick = friendObj["friendNick"].toString();
                QString motto = friendObj["motto"].toString();
                QString email = friendObj["email"].toString();
            
                qDebug() << "解析到好友:" << friendId << friendNick << friendNote;
                FriendInfo friendInfo;
                friendInfo.friendNote = friendNote;
                friendInfo.friendNick = friendNick;
                friendInfo.friendId = friendId;
                friendInfo.avatarPath = QDir::currentPath() + "/images/avatar/" + friendId + ".png";
                friendInfo.motto = motto;
                friendInfo.email = email;
                emit receiveFriendInfo(friendInfo);
            }
        }
        else if(msgType == "heartbeat_response")
        // 处理心跳响应消息
        {
            QString userId = obj["user_id"].toString().trimmed();
            if (userId.isEmpty()) {
                qDebug() << "心跳消息字段缺失";
                return;
            }
            qDebug() << "收到心跳响应消息，用户ID:" << userId;
        }
        else if(msgType == "search_friend_list")
        // 处理搜索好友列表消息
        {
            qDebug() << "收到消息类型为search_friend_list的消息";
            if (obj["result"].toBool())
            {
                emit clearAddFriendList();
                QString count = obj["count"].toString();
                QJsonArray friendList = obj["friendList"].toArray();
                for (const QJsonValue &friendValue : friendList)
                {
                    QJsonObject friendObj = friendValue.toObject();
                    QString friendId = friendObj["friendId"].toString();
                    QString friendNick = friendObj["friendNick"].toString();
                    qDebug() << "解析到好友:" << friendId << " . " << friendNick;
                    emit receiveAddFriendList(friendId, friendNick);
                }
            }
        }
        else if(msgType == "add_friend_result")
        // 处理添加好友结果消息
        {
            qDebug() << "收到消息类型为add_friend_result的消息";
            if (obj["result"].toBool())
            {
                FriendInfo friendInfo;
                friendInfo.friendId = obj["friendId"].toString();
                friendInfo.friendNick = obj["friendNick"].toString();
                friendInfo.avatarPath = "./images/avatar/" + friendInfo.friendId + ".png";
                qDebug() << "添加好友成功:" << friendInfo.friendId << friendInfo.friendNick;
                emit addFriendSuccess(friendInfo);
            }
            else
            {
                QString errorString = obj["reason"].toString();
                qDebug() << "添加好友失败:" << errorString;
                emit addFriendFailed(errorString);
            }
        }
    }
}

void NetworkManager::onDisconnected()
{
    qDebug() << "服务器已断开！";
    emit disconnected();
}

void NetworkManager::onError()
{
    qDebug() << "网络错误:" << m_socket->errorString();
    emit error(m_socket->errorString());
}

void NetworkManager::sendHeartbeatMessage(const QString &userId)
{
    qDebug() << "发送心跳消息";
    // 构建心跳消息JSON字符串
    QJsonObject heartbeatObj;
    heartbeatObj["type"] = "heartbeat";
    heartbeatObj["user_id"] = userId;
    //发送原始数据
    sendRawData(heartbeatObj);
}
