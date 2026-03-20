#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QStackedWidget>
#include <QModelIndex>
#include <QHash>
#include <QPropertyAnimation>
#include <QDate>

namespace Ui {class ChatWindow;}
class MessageModel;
class ContactModel;
class ContactDelegate;
class MessageDelegate;
class ContactMessageBubble;
class NetworkManager;
class ClickableLabel;
class QLabel;
//枚举enum会定义一系列新的类型
enum class ResizeMode;
struct Friend;
struct Message;
struct UserInfo;
struct networkData;
enum class Sex { other=0, male=1, female=2}; // 性别枚举


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

struct currentUserInfo//当前用户信息
{
    //用户
    QString userID; // 当前用户ID
    QString userNick;// 当前用户昵称
    QString userAvatar;// 当前用户头像
    QString userPassword; // 当前用户密码
    QString userMotto; // 当前用户个性签名
    QString userStatus; // 当前用户状态
    QString userEmail; // 当前用户邮箱
    QDate userBirthday; // 当前用户生日
    QString userPhone; // 当前用户电话
    QString userAddress; // 当前用户地址
    QDate userRegistrationDate; // 当前用户注册日期
    Sex userSex; // 当前用户性别
    QString receiverID ;//接收ID
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
    // bool isDragging;// 鼠标是否在拖动窗口
    // bool isLeftButtonPressed;// 鼠标是否按下左键
    // QPoint dragPosition;// 用于存储鼠标拖动位置
    // ResizeMode resizeMode = ResizeMode::None;// 窗口拉伸模式
    // int borderWidth = 5; // 增大窗口边框宽度，用于边缘检测，改善用户体验
    // bool isMaximize = false; // 用于跟踪窗口是否最大化
    QList<QWidget*> buttonWidgets;// 存储按钮的列表
    
    //信息数据结构
    currentUserInfo userInfo; // 当前用户信息
    QString receiverID; // 当前消息接收者ID
    ContactModel *contactModel;// 联系人模型
    MessageModel *messageModel;// 消息模型
    ContactDelegate *contactDelegate; // 联系人委托
    MessageDelegate *messageDelegate; // 消息委托
    QHash<QString, QList<Message>> messageDataMap; // 存储消息数据映射
    QHash<QString, Friend> contactList; // 存储联系人
    QHash<QString, bool> contactSelected; // 存储联系人是否被选中

    //网络
    NetworkManager *m_networkManager; // 网络管理器

signals:
    void exitLogin(); // 退出登录信号

private:
    void initialUI();// 初始化UI
    void initialStackWideget();// 初始化堆栈窗口
    void initialModelView();// 初始化模型视图
    void initialUserInfo(QString userID);// 初始化用户信息
    QString toStringSex(Sex s) ;// 将性别枚举转换为字符串
    bool isNonDraggableWidget(QWidget* w);
    void connectSignals();// 连接信号槽

    // 窗口状态相关函数
    // void showMaximize();//显示最大化窗口
    void showMessage();//显示消息界面
    void showContact();//显示联系人列表
    void showCollect();//显示收藏界面
    void showMoments();//显示朋友圈
    void showSearch();//显示搜索界面
    void sendMessage(); // 发送消息
    void addFriend(); // 添加联系人
    void onSentBtnClicked();//发送按钮点击
    void onContactClicked(const QModelIndex &index); // 单击联系人
    void onContactDoubleClicked(const QModelIndex &index); // 双击联系人
    void onCheckContact(const QModelIndex &index); // 点击发消息按钮
    void addSampleMessages(); // 添加示例消息
    void addSampleMessages(QString friendID);//添加示例消息
    void showContactMessages(QString receiverID); // 显示联系人消息
    void onMessageClicked(const QModelIndex &index); // 点击消息
    void onUserStatusChanged(const QString &userId, bool online); // 处理用户状态改变
    void onMessageReceived(const networkData &data); // 处理接收消息
    void animatePageTransition(QWidget *widget); // 页面切换动画
    // void loadContactsFromDatabase(); // 从数据库加载联系人
    void refreshContacts(); // 刷新联系人
    void paintRdiusPixmap(QLabel*label,const QString paintPath, int xRdius,int yRdius);// 绘制图片

    // 网络相关函数
    void linkServer(); // 连接服务器
    void onNetworkConnected(); // 处理网络连接
    void onNetworkDisconnected(); // 处理网络断开
    void onNetworkError(const QString &error); // 处理网络错误

protected:
    // 鼠标事件处理函数
    // ResizeMode getResizeMode(const QPoint &pos); // 获取当前鼠标所在的边缘位置
    // void resizeWindow(const QPoint &globalPos); // 根据鼠标位置调整窗口大小
    // void updateCursor(ResizeMode mode); // 更新鼠标光标样式
    // void mousePressEvent(QMouseEvent *event) override;// 鼠标按下事件
    // void mouseMoveEvent(QMouseEvent *event) override;// 鼠标移动事件
    // void mouseReleaseEvent(QMouseEvent *event) override;// 鼠标释放事件
    // void leaveEvent(QEvent *event) override;// 鼠标离开事件

};

#endif // CHATWINDOW_H