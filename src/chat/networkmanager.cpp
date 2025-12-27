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
    , m_socket(new QTcpSocket(this))  // 创建QTcpSocket对象作为成员变量
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
    if (m_socket->state() == QTcpSocket::ConnectedState)// 检查socket是否已连接
    {
        QJsonObject messageObj;
        messageObj["type"] = "message"; //  设置消息类型为"message"
        messageObj["sender"] = data.senderId; //  设置消息发送者ID
        messageObj["receiver"] = data.receiverId; //  设置消息接收者ID
        messageObj["content"] = data.content; //  设置消息内容
        messageObj["timestamp"] = data.timestamp.toString(Qt::ISODate); //  设置消息时间戳为当前时间的ISO格式字符串

        QJsonDocument doc(messageObj);// 创建JSON文档对象
        // 将JSON文档转换为字节数组并发送到服务器
        m_socket->write(doc.toJson() + "\n"); // 添加换行符作为消息分隔符
    }
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
        
        // 处理消息类型为"message"的情况
        if (msgType == "message") {
            // 提取发送者ID
            QString senderId = obj["sender"].toString();
            // 提取接收者ID
            QString receiverId = obj["receiver"].toString();
            // 提取消息内容
            QString content = obj["content"].toString();
            // 提取时间戳并转换为QDateTime对象
            QDateTime time = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
            // 发送消息接收信号
            emit messageReceived(networkData{content, receiverId, senderId, time});
        }
        // 处理用户状态变更消息
        else if (msgType == "user_status") 
        {  // 判断消息类型是否为"user_status"
            QString userId = obj["user_id"].toString();  // 从JSON对象中获取用户ID并转换为QString类型
            bool online = obj["online"].toBool();  // 从JSON对象中获取在线状态并转换为bool类型
            emit userStatusChanged(userId, online);  // 发射用户状态变更信号，传递用户ID和在线状态
        }
    }
}

void NetworkManager::onError()
{
    emit error(m_socket->errorString());
}