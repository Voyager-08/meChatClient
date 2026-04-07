#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>// 主机地址类
#include "src/custom/struct.h"

class NetworkManager : public QObject
{
    Q_OBJECT

public:
/**
 * @brief NetworkManager类的构造函数，explicit关键字防止隐式转换
 * @param parent 父对象指针，默认为nullptr
 */
    explicit NetworkManager(QObject *parent = nullptr);
    bool isConnected() const { return m_socket && m_socket->state() == QTcpSocket::ConnectedState; }// 检查是否已连接到服务器

public slots:
    void connectToServer();// 连接到服务器
    void sendHeartbeatMessage(const QString &userId); // 发送心跳消息
    void sendMessage(const MeChat::messageData &data); // 发送消息结构体
    void sendLoginRequest(const QString &userId, const QString &password); // 登录请求
    void sendLogoutRequest(const QString &userId); // 退出登录请求
    void requestUserInfo(const QString &userId); // 请求用户信息
    void sendFriendRequest(const QString &userId); // 发送好友列表请求
    void registerUser(const QString &userId, const QString &userNick, const QString &password); // 注册用户
    void sendAddFriendRequest(const QString &userId); // 发送添加好友请求
    void sendFriendStrListRequest(const QString &friendStr); // 根据好友字符串发送搜索好友列表请求 
signals:
    void connected();
    void disconnected();
    void error(const QString &errorString);
    void userStatusChanged(const QString &userId, bool online);
    void loginSuccess(const QString &userId); // 登录成功信号，携带用户ID
    void loginFailed(const QString &errorString); // 登录失败信号，携带错误信息
    void registerSuccess(const QString &userId); // 注册成功信号，携带用户ID
    void registerFailed(const QString &errorString); // 注册失败信号，携带错误信息
    void receiveUserInfo(MeChat::UserInfo *userInfo);// 接收用户信息
    void receiveMessage(const MeChat::messageData &data);// 接收消息结构体
    void receiveFriendInfo(const MeChat::FriendInfo &friendInfo);// 接收好友信息
    void receiveAddFriendList(const QString &friendId, const QString &friendNick);// 接收添加好友列表信号
    void clearAddFriendList();// 清空添加好友列表信号
    void addFriendSuccess(const MeChat::FriendInfo &friendInfo);// 添加好友成功信号，携带好友ID
    void addFriendFailed(const QString &errorString);// 添加好友失败信号，携带错误信息
private slots:
    void onReadyRead(); // 处理可读数据
    void onDisconnected();//处理断开连接
    void onError(); // 处理网络错误
    void sendRawData(const QJsonObject &data); // 发送原始数据
    void onConnected(); // 处理连接成功


private:
    QTcpSocket *m_socket;//用于客户端网络通信的TCP套接字
};

#endif // NETWORKMANAGER_H
