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
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred,this, &NetworkManager::onError);
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManager::connected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkManager::disconnected);
}

void NetworkManager::connectToServer(const QString &host, quint16 port)
{
    m_socket->connectToHost(host, port);//连接到服务器，IP地址和端口
}

void NetworkManager::sendMessage(const QString &receiverId, const QString &content, const QString &senderId)
{
    if (m_socket->state() == QTcpSocket::ConnectedState) {
        QJsonObject messageObj;
        messageObj["type"] = "message";
        messageObj["receiver"] = receiverId;
        messageObj["sender"] = senderId;
        messageObj["content"] = content;
        messageObj["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        
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
        
        if (msgType == "message") {
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