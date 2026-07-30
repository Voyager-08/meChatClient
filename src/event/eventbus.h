#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <QObject>
#include "src/custom/struct.h"

class LoginWindow;
class ChatWindow;
class NetworkManager;
class DatabaseManager;

using namespace chat;

class EventBus : public QObject
{
    Q_OBJECT
public:
    explicit EventBus(QObject *parent = nullptr);

    void bindLoginWindowSignals(LoginWindow *loginWindow);// 绑定登录窗口信号槽
    void bindChatWindowSignals(ChatWindow *chatWindow);// 绑定聊天窗口信号槽
    void bindNetworkSignals(NetworkManager *networkManager);// 绑定网络信号槽
    void bindDatabaseSignals(DatabaseManager *databaseManager);// 绑定数据库信号槽

signals:
    // ========== 错误发生信号 ==========
    void publishNetworkErrorOccurred(const QString &errorString);// 网络错误发生信号，携带错误信息
    // ========== UI -> EventBus (请求信号) ==========
    // 登录/注册
    void sendLoginRequest(const QString &userID, const QString &password);// 登录请求信号
    void sendRegisterRequest(const QString &userID, const QString &userNick, const QString &password);// 注册请求信号
    // 聊天
    void sendChatMessage(const MessageInfo &messageInfo);// 发送聊天消息
    void sendLogoutRequest(const QString &userId);// 发送退出登录请求
    void sendHeartbeatRequest(const QString &userId);// 发送心跳请求
    void sendUserInfoRequest(const QString &userId);// 请求用户信息
    void sendFriendListRequest(const QString &userId);// 请求好友列表
    void sendAddFriendRequest(const QString &friendId);// 添加好友请求
    void sendSearchFriendRequest(const QString &text);// 搜索好友请求
    void sendDeleteFriendRequest(const QString &friendId);// 删除好友请求
    void sendAvatarFileRequest(const QString &filePath, const QString &userId);// 发送头像文件请求
    void sendFileRequest(const QString &filePath, const QString &userId);// 发送文件请求
    // 数据加载
    void loadUserInfoRequest(const QString &userId);// 加载用户信息请求
    void loadFriendMessagesRequest(const QString &userId, const QString &friendId);// 加载好友消息请求
    void loadLocalDataRequest(const QString &userId);// 加载本地数据请求（登录成功后触发）

    // ========== EventBus -> UI (响应信号) ==========
    // 登录/注册
    void publishLoginSuccess(const QString &userID);// 登录成功信号，携带用户ID
    void publishLoginFailed(const QString &errorString);// 登录失败信号，携带错误信息
    void publishRegisterSuccess(const QString &userID);// 注册成功信号，携带用户ID
    void publishRegisterFailed(const QString &errorString);// 注册失败信号，携带错误信息
    
    // 聊天
    void publishReceiveMessage(const MessageInfo &messageInfo);// 接收消息信号
    void publishReceiveUserInfo(const UserInfo &userInfo);// 接收用户信息信号
    void publishReceiveFriendInfo(const FriendInfo &friendInfo);// 接收好友信息信号
    void publishAddFriendSuccess(const FriendInfo &friendInfo);// 添加好友成功信号
    void publishAddFriendFailed(const QString &errorString);// 添加好友失败信号
    void publishDeleteFriendResult(const QString &friendId,bool isSuccess);// 删除好友结果信号
    void publishReceiveAddFriendList(const QString &friendId, const QString &friendNick);// 接收添加好友列表信号
    void publishClearAddFriendList();// 清空添加好友列表信号
    void publishUserStatusChanged(const QString &userId, bool online);// 用户状态改变信号
    // 数据加载
    void publishLoadedUserInfo(const UserInfo &userInfo);// 加载用户信息完成信号
    void publishLoadedFriendInfo(const QList<FriendInfo> &friendList);// 加载好友列表完成信号
    void publishLoadedFriendMessages(const QString &friendId, const QList<MessageInfo> &messages);// 加载好友消息完成信号
    void publishLoadFinished();// 数据加载完成信号
    
private slots:
    // ========== 错误发生信号槽 ==========
    void onNetworkErrorOccurred(const QString &errorString);// 网络错误发生信号
    // ========== 登录/注册转发 ==========
    void onSendLoginRequest(const QString &userID, const QString &password);// 登录请求槽，转发给 NetworkManager
    void onSendRegisterRequest(const QString &userID, const QString &userNick, const QString &password);// 注册请求槽，转发给 NetworkManager
    void onPublishLoginSuccess(const QString &userID);// 登录成功槽
    void onPublishLoginFailed(const QString &errorString);// 登录失败槽
    void onPublishRegisterSuccess(const QString &userID);// 注册成功槽
    void onPublishRegisterFailed(const QString &errorString);// 注册失败槽
    
    // ========== 聊天请求转发 (UI -> NetworkManager) ==========
    void onSendChatMessage(const MessageInfo &messageInfo);// 转发发送消息请求
    void onSendLogoutRequest(const QString &userId);// 转发退出登录请求
    void onSendHeartbeatRequest(const QString &userId);// 转发心跳请求
    void onSendUserInfoRequest(const QString &userId);// 转发用户信息请求
    void onSendFriendListRequest(const QString &userId);// 转发好友列表请求
    void onSendAddFriendRequest(const QString &friendId);// 转发添加好友请求
    void onSendSearchFriendRequest(const QString &text);// 转发搜索好友请求
    void onSendDeleteFriendRequest(const QString &friendId);// 转发删除好友请求
    void onSendAvatarFileRequest(const QString &filePath, const QString &userId);// 转发发送头像文件请求
    void onSendFileRequest(const QString &filePath, const QString &userId);// 转发发送文件请求
    // ========== 聊天响应转发 (NetworkManager -> UI) ==========
    void onPublishReceiveMessage(const MessageInfo &messageInfo);// 转发接收消息
    void onPublishReceiveUserInfo(const UserInfo &userInfo);// 转发接收用户信息
    void onPublishReceiveFriendInfo(const FriendInfo &friendInfo);// 转发接收好友信息
    void onPublishAddFriendSuccess(const FriendInfo &friendInfo);// 转发添加好友成功
    void onPublishAddFriendFailed(const QString &errorString);// 转发添加好友失败
    void onPublishDeleteFriend(const QString &friendId,bool isSuccess);// 转发删除好友结果
    void onPublishReceiveAddFriendList(const QString &friendId, const QString &friendNick);// 转发接收添加好友列表
    void onPublishClearAddFriendList();// 转发清空添加好友列表
    void onPublishUserStatusChanged(const QString &userId, bool online);// 转发用户状态改变
    
    // ========== 数据加载请求转发 (UI -> DatabaseManager) ==========
    void onLoadUserInfoRequest(const QString &userId);// 转发加载用户信息请求
    void onLoadFriendMessagesRequest(const QString &userId, const QString &friendId);// 转发加载好友消息请求
    void onLoadLocalDataRequest(const QString &userId);// 转发加载本地数据请求（登录成功后触发）
    
    // ========== 数据加载响应转发 (DatabaseManager -> UI) ==========
    void onPublishLoadedUserInfo(const UserInfo &userInfo);// 转发加载用户信息完成
    void onPublishLoadedFriendInfo(const QList<FriendInfo> &friendList);// 转发加载好友列表完成
    void onPublishLoadedFriendMessages(const QString &friendId, const QList<MessageInfo> &messages);// 转发加载好友消息完成
    void onPublishLoadFinished();// 转发数据加载完成
};

#endif // EVENTBUS_H