#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QStackedWidget>
#include <QModelIndex>
#include <QHash>
#include <QPropertyAnimation>
#include <QDate>
#include <QTimer>
#include "src/custom/struct.h"

class MessageModel;
class ContactModel;
class AddContactModel;
class ContactDelegate;
class MessageDelegate;
class ContactMessageBubble;
class ClickableLabel;
class QLabel;
class Message;

using namespace chat;
namespace Ui { class ChatWindow; }  

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(QString userID,QWidget *parent = nullptr );
    ~ChatWindow();
    
    // 启动数据加载（登录成功后由 MainWindow 调用）
    void startDataLoading();

private:

    //界面组件
    Ui::ChatWindow *ui;
    QList<QWidget*> buttonWidgets;// 存储按钮的列表
    
    //信息数据结构
    UserInfo *userInfo; // 当前用户信息
    QString receiverID; // 当前消息接收者ID
    ContactModel *contactModel;// 联系人模型
    AddContactModel *addContactModel;// 添加联系人模型
    MessageModel *messageModel;// 消息模型
    ContactDelegate *contactDelegate; // 联系人委托
    MessageDelegate *messageDelegate; // 消息委托
    QHash<QString, QList<Message>> messageDataMap; // 存储消息数据映射
    QHash<QString, FriendInfo> contactList; // 存储联系人
    QHash<QString, bool> contact_addMessage; // 存储联系人是否已被添加消息
    QTimer *heartbeatTimer;          // 心跳定时器

signals:
    // ========== 网络请求信号 (UI -> EventBus -> NetworkManager) ==========
    void sendMessage(const MessageInfo &data);// 发送消息
    void sendLogoutRequest(const QString &userId); // 发送退出登录请求
    void sendHeartbeatMessage(const QString &userId); // 发送心跳消息
    void sendUserInfoRequest(const QString &userId);// 请求用户信息
    void sendFriendListRequest(const QString &userId);// 请求好友列表
    void sendAddFriendRequest(const QString &friendId);// 添加好友
    void sendSearchFriendRequest(const QString &text);// 搜索好友
    void sendDeleteFriendRequest(const QString &friendId);// 删除好友
    void sendFileRequest(const QString &filePath, const QString &userId);// 发送文件请求
    
    // ========== 数据加载请求信号 (UI -> EventBus -> DatabaseManager) ==========
    void loadUserInfoRequest(const QString &userId);// 加载用户信息请求
    void loadFriendMessagesRequest(const QString &userId, const QString &friendId);// 加载好友消息请求
    void loadLocalDataRequest(const QString &userId);// 加载本地数据请求（登录成功后触发）

    // ========== EventBus -> ChatWindow 响应槽 ==========
    void addFriendSuccess(const FriendInfo &friendInfo);
    void addFriendFailed(const QString &errorString);
    void receiveAddFriendList(const QString &friendId, const QString &friendNick); // 接收添加好友列表

public slots:
    // ========== EventBus -> ChatWindow 响应槽 ==========
    // 网络响应
    void onUserInfoLoaded(const UserInfo &userInfo);
    void onSendMessage(); // 发送消息
    void onMessageReceived(const MessageInfo &data); // 接收消息
    void onReceiveFriendInfo(const FriendInfo &friendInfo); // 接收好友信息
    void onAddFriendSuccess(FriendInfo friendInfo); // 添加好友成功
    void onAddFriendFailed(const QString &errorString); // 添加好友失败
    void onDeleteFriend(const QString &friendId,bool isSuccess); // 删除好友成功
    void onReceiveAddFriendList(const QString &friendId, const QString &friendNick); // 接收添加好友列表
    void onUserStatusChanged(const QString &userId, bool online); // 用户状态改变
    // 数据加载响应
    void onFriendListLoaded(const QList<FriendInfo> &friendList);
    void onMessagesLoaded(const QString &friendId, const QList<MessageInfo> &messages);
    void onLoadingFinished();

private slots:
    // UI 内部槽
    void onAddFriend(); // 添加联系人

private:
    void initialUI();// 初始化UI
    void initialStackWideget();// 初始化堆栈窗口
    void initialModelView();// 初始化模型视图
    void initialUserInfo(QString userID);// 初始化用户信息

    void connectUISignals();// 连接信号槽

    // 窗口状态相关函数
    void animatePageTransition(QWidget *widget); // 页面切换动画

    void createNote();// 创建笔记
    void clickMessageList(const QModelIndex &index); // 点击消息列表
    void clickSentBtn();//发送按钮点击
    void clickContactList(const QModelIndex &index); // 单击联系人列表
    void clickDoubleContactList(const QModelIndex &index); // 双击联系人列表
    void clickContactList_to_MessageList(const QModelIndex &index); // 联系人列表添加到消息列表
    void contextMenuRequested(const QPoint &pos); // 右键菜单请求槽函数
    QString getUserAvatar(QString userID);// 获取用户头像

    void addFriendSuccessMessage(FriendInfo friendInfo);// 发送添加好友成功消息 
    void showContact_historyMessage(QString receiverID); // 显示选中联系人的所有消息

    void refreshMessageList(const QString &friendID,QString text); // 刷新消息列表
    void paintRdiusPixmap(QLabel*label,const QString paintPath, int xRdius,int yRdius);// 绘制图片

    void onHeartbeat(); // 处理心跳消息

};

#endif // CHATWINDOW_H