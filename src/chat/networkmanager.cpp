#include "networkmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
{
    //  连接QTcpSocket的信号到NetworkManager的槽函数 当socket有数据可读时，调用NetworkManager的onReadyRead槽函数
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead); 
    //  当socket发生错误时，调用NetworkManager的onError槽函数
    connect(m_socket, &QTcpSocket::errorOccurred,this, &NetworkManager::onError); 
    //  当socket成功连接到服务器时，调用NetworkManager的connected槽函数
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManager::connected); 
     //  当socket与服务器断开连接时，调用NetworkManager的disconnected槽函数
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkManager::disconnected);
}

void NetworkManager::connectToServer(const QString &host, quint16 port)
{
    m_socket->connectToHost(host, port);//连接到服务器，IP地址和端口
}

void NetworkManager::sendMessage(const QString &receiverId, const QString &content, const QString &senderId)
{
    if (m_socket->state() == QTcpSocket::ConnectedState ) {
        QJsonObject messageObj;
        messageObj["type"] = "message"; //  设置消息类型为"message"
        messageObj["receiver"] = receiverId; //  设置消息接收者ID
        messageObj["sender"] = senderId; //  设置消息发送者ID
        messageObj["content"] = content; //  设置消息内容
        messageObj["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate); //  设置消息时间戳为当前时间的ISO格式字符串
        
        QJsonDocument doc(messageObj);
        m_socket->write(doc.toJson() + "\n"); // 添加换行符作为消息分隔符
    } 
}

void NetworkManager::onReadyRead()
{
    while (m_socket->canReadLine()) {
        QByteArray line = m_socket->readLine();
        QJsonDocument doc = QJsonDocument::fromJson(line);
        QJsonObject obj = doc.object();
        
        QString msgType = obj["type"].toString();
        
        if (msgType == "auth_response") {
            bool success = obj["success"].toBool();
            QString message = obj["message"].toString();

        } else if (msgType == "message") {
            QString senderId = obj["sender"].toString();
            QString content = obj["content"].toString();
            QDateTime time = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
            
            emit messageReceived(senderId, content, time);
        } else if (msgType == "user_status") {
            QString userId = obj["user_id"].toString();
            bool online = obj["online"].toBool();
            emit userStatusChanged(userId, online);
        }
    }
}

void NetworkManager::onError()
{
    emit error(m_socket->errorString());
}