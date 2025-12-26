#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
/**
 * @brief NetworkManager类的构造函数，explicit关键字防止隐式转换
 * @param parent 父对象指针，默认为nullptr
 */
    explicit NetworkManager(QObject *parent = nullptr);
/**
 * @brief 连接到指定主机和端口的服务器
 * @param host 服务器主机地址
 * @param port 服务器端口号
 */
    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QString &receiverId, const QString &content, const QString &senderId);
    bool isConnected() const { return m_socket && m_socket->state() == QTcpSocket::ConnectedState; }

signals:
    void messageReceived(const QString &senderId, const QString &content, const QDateTime &time);
    void connected();
    void disconnected();
    void error(const QString &errorString);
    void userStatusChanged(const QString &userId, bool online);
private slots:
    void onReadyRead();
    void onError();

private:
    QTcpSocket *m_socket;
};

#endif // NETWORKMANAGER_H