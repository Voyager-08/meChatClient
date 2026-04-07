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

namespace Ui {class ChatWindow;}
class MessageModel;
class ContactModel;
class AddContactModel;
class ContactDelegate;
class MessageDelegate;
class ContactMessageBubble;
class NetworkManager;
class ClickableLabel;
class QLabel;
class DataLoader;
//枚举enum会定义一系列新的类型
enum class ResizeMode;
struct Message;

enum class ResizeMode { // 窗口拉伸模式枚举
    None,  //  无方向或位置
    Left,  //  左方向
    Right,  //  右方向
    Top,  //  上方向
    Bottom,  //  下方向
    TopLeft,  //  左上方向
    TopRight,  //  右上方向
    BottomLeft,  //  左下方向
    BottomRight  //  右下方向
};

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(QString userID,QWidget *parent = nullptr,NetworkManager *networkManager = nullptr);
    ~ChatWindow();

private:

    //界面组件
    Ui::ChatWindow *ui;
    QList<QWidget*> buttonWidgets;// 存储按钮的列表
    
    //信息数据结构
    MeChat::UserInfo *userInfo; // 当前用户信息
    QString receiverID; // 当前消息接收者ID
    ContactModel *contactModel;// 联系人模型
    AddContactModel *addContactModel;// 添加联系人模型
    MessageModel *messageModel;// 消息模型
    ContactDelegate *contactDelegate; // 联系人委托
    MessageDelegate *messageDelegate; // 消息委托
    QHash<QString, QList<Message>> messageDataMap; // 存储消息数据映射
    QHash<QString, MeChat::FriendInfo> contactList; // 存储联系人
    QHash<QString, bool> contact_addMessage; // 存储联系人是否已被添加消息

    //网络
    NetworkManager *networkManager; // 网络管理器
    QTimer *heartbeatTimer;          // 心跳定时器
    DataLoader *dataLoader;          // 数据加载器

signals:
    void exitLogin(QString userID); // 退出登录信号
    void avatarLoaded(QPixmap pixmap);
    void addFriendId(const QString &friendId);
    void searchAddFriendStr(const QString &friendStr);
private slots:
    // DataLoader 信号槽
    void onUserInfoLoaded(const MeChat::UserInfo &userInfo);
    void onFriendListLoaded(const QList<MeChat::FriendInfo> &friendList);
    void onMessagesLoaded(const QString &friendId, const QList<MeChat::messageData> &messages);
    void onLoadingFinished();
private:
    void initialUI();// 初始化UI
    void initialStackWideget();// 初始化堆栈窗口
    void initialModelView();// 初始化模型视图
    void initialUserInfo(QString userID);// 初始化用户信息
    void connectUISignals();// 连接信号槽

    // 窗口状态相关函数
    void animatePageTransition(QWidget *widget); // 页面切换动画
    void addSampleBubbleMessages(); // 添加示例消息
    void addSampleBubbleMessages(QString friendID);//添加示例消息
    void addFriend(); // 添加联系人

    void createNote();// 创建笔记
    void clickMessageList(const QModelIndex &index); // 点击消息列表
    void clickSentBtn();//发送按钮点击
    void clickContactList(const QModelIndex &index); // 单击联系人列表
    void clickDoubleContactList(const QModelIndex &index); // 双击联系人列表
    void clickContactList_to_MessageList(const QModelIndex &index); // 联系人列表添加到消息列表

    void onUserStatusChanged(const QString &userId, bool online); // 处理用户状态改变
    void onMessageReceived(const MeChat::messageData &data); // 处理接收消息

    void sendMessage(); // 发送消息
    void showContactList(const MeChat::FriendInfo& friendInfo);//显示联系人列表
    void showCollect();//显示收藏界面
    void showMoments();//显示朋友圈
    void showSearch();//显示搜索界面
    void showContact_historyMessage(QString receiverID); // 显示选中联系人的所有消息

    void refreshContactList(); // 刷新联系人列表
    void paintRdiusPixmap(QLabel*label,const QString paintPath, int xRdius,int yRdius);// 绘制图片

    // 网络相关函数
    void connectServerSignals(); // 连接服务器信号槽函数
    void onHeartbeat(); // 处理心跳消息

};

#endif // CHATWINDOW_H
