#include "eventbus.h"
#include "src/sql/databasemanager.h"
#include "src/network/networkmanager.h"
#include "src/core/login/loginwindow.h"
#include "src/core/chat/chatwindow.h"

EventBus::EventBus(QObject *parent) : QObject(parent)
{
}

// ========== 绑定信号槽 ==========

void EventBus::bindLoginWindowSignals(LoginWindow *loginWindow) // 绑定登录窗口信号槽
{
    // UI -> EventBus -> NetworkManager (请求)
    connect(loginWindow, &LoginWindow::loginRequested, this, &EventBus::onSendLoginRequest, Qt::QueuedConnection);
    connect(loginWindow, &LoginWindow::registerRequested, this, &EventBus::onSendRegisterRequest, Qt::QueuedConnection);
    connect(loginWindow, &LoginWindow::sendAvatarFileRequest, this, &EventBus::onSendAvatarFileRequest, Qt::QueuedConnection);
    // EventBus -> UI (响应)
    connect(this, &EventBus::publishLoginSuccess, loginWindow, &LoginWindow::onLoginSuccess, Qt::QueuedConnection);
    connect(this, &EventBus::publishRegisterSuccess, loginWindow, &LoginWindow::onRegisterSuccess, Qt::QueuedConnection);
    connect(this, &EventBus::publishLoginFailed, loginWindow, &LoginWindow::onLoginFailed, Qt::QueuedConnection);
    connect(this, &EventBus::publishRegisterFailed, loginWindow, &LoginWindow::onRegisterFailed, Qt::QueuedConnection);
    connect(this, &EventBus::publishNetworkErrorOccurred, loginWindow, &LoginWindow::onNetworkErrorOccurred, Qt::QueuedConnection);
}

void EventBus::bindChatWindowSignals(ChatWindow *chatWindow) // 绑定聊天窗口信号槽
{
    // ========== UI -> EventBus -> NetworkManager (请求) ==========
    connect(chatWindow, &ChatWindow::sendMessage, this, &EventBus::onSendChatMessage, Qt::QueuedConnection);
    connect(chatWindow, &ChatWindow::sendLogoutRequest, this, &EventBus::onSendLogoutRequest, Qt::QueuedConnection);
    connect(chatWindow, &ChatWindow::sendHeartbeatMessage, this, &EventBus::onSendHeartbeatRequest, Qt::QueuedConnection);
    connect(chatWindow, &ChatWindow::sendUserInfoRequest, this, &EventBus::onSendUserInfoRequest, Qt::QueuedConnection);
    connect(chatWindow, &ChatWindow::sendFriendListRequest, this, &EventBus::onSendFriendListRequest, Qt::QueuedConnection);
    connect(chatWindow, &ChatWindow::sendAddFriendRequest, this, &EventBus::onSendAddFriendRequest, Qt::QueuedConnection);
    connect(chatWindow, &ChatWindow::sendSearchFriendRequest, this, &EventBus::onSendSearchFriendRequest, Qt::QueuedConnection);
    connect(chatWindow, &ChatWindow::sendDeleteFriendRequest, this, &EventBus::onSendDeleteFriendRequest, Qt::QueuedConnection);
    connect(chatWindow, &ChatWindow::sendFileRequest, this, &EventBus::onSendFileRequest, Qt::QueuedConnection);
    
    // 数据加载请求
    connect(chatWindow, &ChatWindow::loadUserInfoRequest, this, &EventBus::onLoadUserInfoRequest, Qt::QueuedConnection);
    connect(chatWindow, &ChatWindow::loadFriendMessagesRequest, this, &EventBus::onLoadFriendMessagesRequest, Qt::QueuedConnection);
    connect(chatWindow, &ChatWindow::loadLocalDataRequest, this, &EventBus::onLoadLocalDataRequest, Qt::QueuedConnection);
    
    // ========== EventBus -> UI (响应) ==========
    connect(this, &EventBus::publishReceiveMessage, chatWindow, &ChatWindow::onMessageReceived, Qt::QueuedConnection);
    connect(this, &EventBus::publishReceiveUserInfo, chatWindow, &ChatWindow::onUserInfoLoaded, Qt::QueuedConnection);
    connect(this, &EventBus::publishReceiveFriendInfo, chatWindow, &ChatWindow::onReceiveFriendInfo, Qt::QueuedConnection);
    connect(this, &EventBus::publishAddFriendSuccess, chatWindow, &ChatWindow::onAddFriendSuccess, Qt::QueuedConnection);
    connect(this, &EventBus::publishAddFriendFailed, chatWindow, &ChatWindow::onAddFriendFailed, Qt::QueuedConnection);
    connect(this, &EventBus::publishDeleteFriendResult, chatWindow, &ChatWindow::onDeleteFriend, Qt::QueuedConnection);
    
    connect(this, &EventBus::publishReceiveAddFriendList, chatWindow, &ChatWindow::onReceiveAddFriendList, Qt::QueuedConnection);
    connect(this, &EventBus::publishUserStatusChanged, chatWindow, &ChatWindow::onUserStatusChanged, Qt::QueuedConnection);
    // 数据加载响应
    connect(this, &EventBus::publishLoadedUserInfo, chatWindow, &ChatWindow::onUserInfoLoaded, Qt::QueuedConnection);
    connect(this, &EventBus::publishLoadedFriendInfo, chatWindow, &ChatWindow::onFriendListLoaded, Qt::QueuedConnection);
    connect(this, &EventBus::publishLoadedFriendMessages, chatWindow, &ChatWindow::onMessagesLoaded, Qt::QueuedConnection);
    connect(this, &EventBus::publishLoadFinished, chatWindow, &ChatWindow::onLoadingFinished, Qt::QueuedConnection);
}

void EventBus::bindNetworkSignals(NetworkManager *networkManager) // 绑定网络信号槽
{
    // ========== EventBus -> NetworkManager (请求转发) ==========
    // 错误发生信号
    connect(networkManager, &NetworkManager::errorOccurred, this, &EventBus::onNetworkErrorOccurred, Qt::QueuedConnection);
    // 登录/注册
    connect(this, &EventBus::sendLoginRequest, networkManager, &NetworkManager::sendLoginRequest, Qt::QueuedConnection);
    connect(this, &EventBus::sendRegisterRequest, networkManager, &NetworkManager::sendRegisterRequest, Qt::QueuedConnection);
    // 聊天
    connect(this, &EventBus::sendChatMessage, networkManager, &NetworkManager::sendMessage, Qt::QueuedConnection);
    connect(this, &EventBus::sendLogoutRequest, networkManager, &NetworkManager::sendLogoutRequest, Qt::QueuedConnection);
    connect(this, &EventBus::sendHeartbeatRequest, networkManager, &NetworkManager::sendHeartbeatMessage, Qt::QueuedConnection);
    connect(this, &EventBus::sendUserInfoRequest, networkManager, &NetworkManager::requestUserInfo, Qt::QueuedConnection);
    connect(this, &EventBus::sendFriendListRequest, networkManager, &NetworkManager::sendFriendRequest, Qt::QueuedConnection);
    connect(this, &EventBus::sendAddFriendRequest, networkManager, &NetworkManager::sendAddFriendRequest, Qt::QueuedConnection);
    connect(this, &EventBus::sendSearchFriendRequest, networkManager, &NetworkManager::sendFriendStrListRequest, Qt::QueuedConnection);
    connect(this, &EventBus::sendDeleteFriendRequest, networkManager, &NetworkManager::sendDeleteFriendRequest, Qt::QueuedConnection);
    connect(this, &EventBus::sendAvatarFileRequest, networkManager, &NetworkManager::sendAvatarFile, Qt::QueuedConnection);
    
    // ========== NetworkManager -> EventBus (响应转发) ==========
    // 登录/注册
    connect(networkManager, &NetworkManager::loginSuccess, this, &EventBus::onPublishLoginSuccess, Qt::QueuedConnection);
    connect(networkManager, &NetworkManager::loginFailed, this, &EventBus::onPublishLoginFailed, Qt::QueuedConnection);
    connect(networkManager, &NetworkManager::registerSuccess, this, &EventBus::onPublishRegisterSuccess, Qt::QueuedConnection);
    connect(networkManager, &NetworkManager::registerFailed, this, &EventBus::onPublishRegisterFailed, Qt::QueuedConnection);
    // 聊天
    connect(networkManager, &NetworkManager::receiveMessage, this, &EventBus::onPublishReceiveMessage, Qt::QueuedConnection);
    connect(networkManager, &NetworkManager::receiveUserInfo, this, &EventBus::onPublishReceiveUserInfo, Qt::QueuedConnection);
    connect(networkManager, &NetworkManager::receiveFriendInfo, this, &EventBus::onPublishReceiveFriendInfo, Qt::QueuedConnection);
    connect(networkManager, &NetworkManager::addFriendSuccess, this, &EventBus::onPublishAddFriendSuccess, Qt::QueuedConnection);
    connect(networkManager, &NetworkManager::addFriendFailed, this, &EventBus::onPublishAddFriendFailed, Qt::QueuedConnection);
    connect(networkManager, &NetworkManager::deleteFriendResult, this, &EventBus::onPublishDeleteFriend, Qt::QueuedConnection);
    connect(networkManager, &NetworkManager::receiveAddFriendList, this, &EventBus::onPublishReceiveAddFriendList, Qt::QueuedConnection);
    connect(networkManager, &NetworkManager::clearAddFriendList, this, &EventBus::onPublishClearAddFriendList, Qt::QueuedConnection);
    connect(networkManager, &NetworkManager::userStatusChanged, this, &EventBus::onPublishUserStatusChanged, Qt::QueuedConnection);
}

void EventBus::bindDatabaseSignals(DatabaseManager *databaseManager) // 绑定数据库信号槽
{
    // ========== EventBus -> DatabaseManager (请求转发) ==========
    connect(this, &EventBus::loadUserInfoRequest, databaseManager, &DatabaseManager::onLoadUserInfo, Qt::QueuedConnection);
    connect(this, &EventBus::loadFriendMessagesRequest, databaseManager, &DatabaseManager::onLoadFriendMessage, Qt::QueuedConnection);
    connect(this, &EventBus::loadLocalDataRequest, databaseManager, &DatabaseManager::startLoading, Qt::QueuedConnection);
    connect(this, &EventBus::sendChatMessage, databaseManager, &DatabaseManager::onSaveFriendMessage, Qt::QueuedConnection);

    // ========== DatabaseManager -> EventBus (响应转发) ==========
    connect(databaseManager, &DatabaseManager::loadUserInfo, this, &EventBus::onPublishLoadedUserInfo, Qt::QueuedConnection);
    connect(databaseManager, &DatabaseManager::loadFriendInfo, this, &EventBus::onPublishLoadedFriendInfo, Qt::QueuedConnection);
    connect(databaseManager, &DatabaseManager::loadFriendMessage, this, &EventBus::onPublishLoadedFriendMessages, Qt::QueuedConnection);
    connect(databaseManager, &DatabaseManager::loadFinished, this, &EventBus::onPublishLoadFinished, Qt::QueuedConnection);
}

// ========== 网络错误发生信号槽 ==========
void EventBus::onNetworkErrorOccurred(const QString &errorString) // 网络错误发生槽，转发给 EventBus 错误信号
{
    emit publishNetworkErrorOccurred(errorString);
}

// ========== 登录/注册转发槽 ==========

void EventBus::onSendLoginRequest(const QString &userID, const QString &password) // 登录请求槽，转发给 NetworkManager
{
    emit sendLoginRequest(userID, password);
}

void EventBus::onSendRegisterRequest(const QString &userID, const QString &userNick, const QString &password) // 注册请求槽，转发给 NetworkManager
{
    emit sendRegisterRequest(userID, userNick, password);
}

void EventBus::onPublishLoginSuccess(const QString &userID) // 登录成功槽
{
    emit publishLoginSuccess(userID);
}

void EventBus::onPublishLoginFailed(const QString &errorString) // 登录失败槽
{
    emit publishLoginFailed(errorString);
}

void EventBus::onPublishRegisterSuccess(const QString &userID) // 注册成功槽
{
    emit publishRegisterSuccess(userID);
}

void EventBus::onPublishRegisterFailed(const QString &errorString) // 注册失败槽
{
    emit publishRegisterFailed(errorString);
}

// ========== 聊天请求转发槽 (UI -> NetworkManager) ==========

void EventBus::onSendChatMessage(const MessageInfo &messageInfo) // 转发发送消息请求
{
    emit sendChatMessage(messageInfo);
}

void EventBus::onSendLogoutRequest(const QString &userId) // 转发退出登录请求
{
    emit sendLogoutRequest(userId);
}

void EventBus::onSendHeartbeatRequest(const QString &userId) // 转发心跳请求
{
    emit sendHeartbeatRequest(userId);
}

void EventBus::onSendUserInfoRequest(const QString &userId) // 转发用户信息请求
{
    emit sendUserInfoRequest(userId);
}

void EventBus::onSendFriendListRequest(const QString &userId) // 转发好友列表请求
{
    emit sendFriendListRequest(userId);
}

void EventBus::onSendAddFriendRequest(const QString &friendId) // 转发添加好友请求
{
    emit sendAddFriendRequest(friendId);
}

void EventBus::onSendSearchFriendRequest(const QString &text) // 转发搜索好友请求
{
    emit sendSearchFriendRequest(text);
}

void EventBus::onSendDeleteFriendRequest(const QString &friendId) // 转发删除好友请求
{
    emit sendDeleteFriendRequest(friendId);
}

void EventBus::onSendAvatarFileRequest(const QString &filePath, const QString &userId) // 转发发送头像文件请求
{
    emit sendAvatarFileRequest(filePath, userId);
}

void EventBus::onSendFileRequest(const QString &filePath, const QString &userId) // 转发发送文件请求
{
    emit sendFileRequest(filePath, userId);
}

// ========== 聊天响应转发槽 (NetworkManager -> UI) ==========

void EventBus::onPublishReceiveMessage(const MessageInfo &messageInfo) // 转发接收消息
{
    emit publishReceiveMessage(messageInfo);
}

void EventBus::onPublishReceiveUserInfo(const UserInfo &userInfo) // 转发接收用户信息
{
    emit publishReceiveUserInfo(userInfo);
}

void EventBus::onPublishReceiveFriendInfo(const FriendInfo &friendInfo) // 转发接收好友信息
{
    emit publishReceiveFriendInfo(friendInfo);
}

void EventBus::onPublishAddFriendSuccess(const FriendInfo &friendInfo) // 转发添加好友成功
{
    emit publishAddFriendSuccess(friendInfo);
}

void EventBus::onPublishAddFriendFailed(const QString &errorString) // 转发添加好友失败
{
    emit publishAddFriendFailed(errorString);
}

void EventBus::onPublishDeleteFriend(const QString &friendId,bool isSuccess) // 转发删除好友结果
{
    emit publishDeleteFriendResult(friendId,isSuccess);
}

void EventBus::onPublishReceiveAddFriendList(const QString &friendId, const QString &friendNick) // 转发接收添加好友列表
{
    emit publishReceiveAddFriendList(friendId, friendNick);
}

void EventBus::onPublishClearAddFriendList() // 转发清空添加好友列表
{
    emit publishClearAddFriendList();
}

void EventBus::onPublishUserStatusChanged(const QString &userId, bool online) // 转发用户状态改变
{
    emit publishUserStatusChanged(userId, online);
}

// ========== 数据加载请求转发槽 (UI -> DatabaseManager) ==========

void EventBus::onLoadUserInfoRequest(const QString &userId) // 转发加载用户信息请求
{
    emit loadUserInfoRequest(userId);
}

void EventBus::onLoadFriendMessagesRequest(const QString &userId, const QString &friendId) // 转发加载好友消息请求
{
    emit loadFriendMessagesRequest(userId, friendId);
}

void EventBus::onLoadLocalDataRequest(const QString &userId) // 转发加载本地数据请求
{
    emit loadLocalDataRequest(userId);
}

// ========== 数据加载响应转发槽 (DatabaseManager -> UI) ==========

void EventBus::onPublishLoadedUserInfo(const UserInfo &userInfo) // 转发加载用户信息完成
{
    emit publishLoadedUserInfo(userInfo);
}

void EventBus::onPublishLoadedFriendInfo(const QList<FriendInfo> &friendList) // 转发加载好友列表完成
{
    emit publishLoadedFriendInfo(friendList);
}

void EventBus::onPublishLoadedFriendMessages(const QString &friendId, const QList<MessageInfo> &messages) // 转发加载好友消息完成
{
    emit publishLoadedFriendMessages(friendId, messages);
}

void EventBus::onPublishLoadFinished() // 转发数据加载完成
{
    emit publishLoadFinished();
}
