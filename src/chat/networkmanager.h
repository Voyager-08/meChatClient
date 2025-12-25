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
    explicit NetworkManager(QObject *parent = nullptr);
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