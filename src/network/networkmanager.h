#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QList>
#include "src/custom/struct.h"
#include "packetcodec.h"

class QTcpSocket;
using namespace chat;

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);//explicit构造函数，防止隐式类型转换
    bool isConnected() const;// 检查是否已连接到服务器

public slots:
    void initNetwork();// 初始化网络管理器
    void connectToServer();// 连接到服务器
    void sendAvatarFile(const QString &filePath, const QString &userId); // 发送头像文件到服务器
    void sendHeartbeatMessage(const QString &userId); // 发送心跳消息
    void sendMessage(const MessageInfo &messageInfo); // 发送消息结构体
    void sendLoginRequest(const QString &userId, const QString &password); // 发送登录请求
    void sendLogoutRequest(const QString &userId); // 发送退出登录请求
    void requestUserInfo(const QString &userId); // 发送请求用户信息请求
    void sendFriendRequest(const QString &userId); // 发送好友列表请求
    void sendRegisterRequest(const QString &userId, const QString &userNick, const QString &password); // 发送注册用户请求
    void sendAddFriendRequest(const QString &userId); // 发送添加好友请求
    void sendFriendStrListRequest(const QString &friendStr); // 根据好友字符串发送搜索好友列表请求 
    void sendDeleteFriendRequest(const QString &friendId);// 发送删除好友请求

signals:
    void connected();
    void disconnected();
    void deleteFriendResult(const QString &friendId,bool isSuccess);// 删除好友结果信号
    void errorOccurred(const QString &errorString);// 错误发生信号，携带错误信息
    void error(const QString &errorString);
    void userStatusChanged(const QString &userId, bool online);
    void loginSuccess(const QString &userId); // 登录成功信号，携带用户ID
    void loginFailed(const QString &errorString); // 登录失败信号，携带错误信息
    void registerSuccess(const QString &userId); // 注册成功信号，携带用户ID
    void registerFailed(const QString &errorString); // 注册失败信号，携带错误信息
    void receiveUserInfo(const UserInfo &userInfo);// 接收用户信息
    void receiveMessage(const MessageInfo &messageInfo);// 接收消息结构体
    void receiveFriendInfo(const FriendInfo &friendInfo);// 接收好友信息
    void receiveAddFriendList(const QString &friendId, const QString &friendNick);// 接收添加好友列表信号
    void clearAddFriendList();// 清空添加好友列表信号
    void addFriendSuccess(const FriendInfo &friendInfo);// 添加好友成功信号，携带好友信息
    void addFriendFailed(const QString &errorString);// 添加好友失败信号，携带错误信息

private slots:
    void onReadyRead(); // 处理可读数据
    void onDisconnected();//处理断开连接
    void onError(); // 处理网络错误
    bool sendRawData(const mechat::Packet &packet); // 发送原始数据
    void onConnected(); // 处理连接成功

private:
    QTcpSocket *m_socket = nullptr;//用于客户端网络通信的TCP套接字
    PacketCodec m_codec; // 二进制协议编解码器
    QList<QByteArray> m_pendingWrites;
};

#endif // NETWORKMANAGER_H
