#include "networkmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>

/**
 * @brief NetworkManager类的构造函数
 * @param parent QObject父对象指针，用于对象父子关系管理
 */
NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)  
    , m_socket(new QTcpSocket(this))  // 在构造函数创建QTcpSocket对象作为成员变量，是为了在整个NetworkManager生命周期内保持网络连接的状态和管理网络通信
{
    /* 连接QTcpSocket的readyRead信号到NetworkManager的onReadyRead槽函数
    当有数据可读时触发 */
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);

    /* 连接QTcpSocket的errorOccurred信号到NetworkManager的onError槽函数
    当发生错误时触发 */
    connect(m_socket, &QTcpSocket::errorOccurred,this, &NetworkManager::onError);
    
    /* 创建QTcpSocket的connected信号到NetworkManager的connected槽函数
    当连接成功时触发 */
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManager::connected);

    /* 创建QTcpSocket的disconnected信号到NetworkManager的disconnected槽函数
    当连接断开时触发 */
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkManager::disconnected);
}

void NetworkManager::connectToServer(const QString &host, quint16 port)
{
    m_socket->connectToHost(host, port);//连接到服务器，IP地址和端口
}

void NetworkManager::sendMessage(const networkData &data)
{
    if (m_socket->state() != QTcpSocket::ConnectedState) 
    {        
        qDebug() << "网络未连接，无法发送消息！";
        return;
    }

    QJsonObject messageObj;// 创建一个JSON对象来存储消息数据
    messageObj["type"] = "message";
    messageObj["sender"] = data.senderId;
    messageObj["receiver"] = data.receiverId;
    messageObj["content"] = data.content;
    messageObj["timestamp"] = data.timestamp.toString(Qt::ISODate);

    QJsonDocument doc(messageObj);// 将JSON对象转换为JSON文档，以便序列化为字符串
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact); // 使用紧凑格式，节省带宽

    qDebug() << "发送消息:" << jsonData; // qDebug 能直接打印 QByteArray

    m_socket->write(jsonData + "\n"); // 发送 + 换行符
}

void NetworkManager::sendRawData(const QByteArray &data)
{
    if (m_socket->state() != QTcpSocket::ConnectedState) 
    {        
        qDebug() << "网络未连接，无法发送数据！";
        return;
    }

    m_socket->write(data);
    m_socket->flush(); // 确保数据立即发送
    qDebug() << "客户端发送原始数据:" << data;
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
        // 获取JSON对象
        QJsonObject obj = doc.object();
        // 获取消息类型字段
        QString msgType = obj["type"].toString();
        qDebug() << "解析消息类型:" << msgType;
        // 处理消息类型为"message"的情况
        if (msgType == "message") {
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
            emit messageReceived(networkData{senderId,receiverId,content,time});
        }
        else if (msgType == "login_result") 
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
        // 处理用户状态变更消息
        else if (msgType == "user_status") 
        {
            qDebug() << "收到消息类型为user_status的消息";
            QString userID = obj["user_id"].toString();  // 从JSON对象中获取用户ID并转换为QString类型
            bool online = obj["online"].toBool();  // 从JSON对象中获取在线状态并转换为bool类型
            emit userStatusChanged(userID, online);  // 发射用户状态变更信号，传递用户ID和在线状态
        }
    }
}

void NetworkManager::onError()
{
    emit error(m_socket->errorString());
}


