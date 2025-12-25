#include <QWidget>
#include <QMouseEvent>
#include <QStackedWidget>
#include <QModelIndex>
#include <QHash>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include "messagebubble.h"
#ifndef CHATWINDOW_H
#define CHATWINDOW_H

namespace Ui {class ChatWindow;}
class ContactModel;
class ContactDelegate;
class ContactMessageBubble;
class NetworkManager;

//枚举enum会定义一系列新的类型
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

struct Friend{ //好友备注，好友头像路径，好友个性签名
    QString friendNote; //  好友备注，用于唯一标识用户
    QString avatarPath; //  头像路径，存储用户头像文件的路径
    QString message; //  用户个性签名或留言
    QString friendID; //  好友ID，用于唯一标识好友
};

struct Message{ //消息结构体
    MessageBubble::Role role;  // 消息角色（自己/他人）
    QString text;              // 消息文本
    QDateTime time;            // 消息时间
};

class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(QString userID,QWidget *parent = nullptr);
    ~ChatWindow();
    void setUserId(QString userId);

private:
    Ui::ChatWindow *ui;
    bool isDragging;// 鼠标是否在拖动窗口
    bool isLeftButtonPressed;// 鼠标是否按下左键
    QPoint dragPosition;// 用于存储鼠标拖动位置
    ResizeMode resizeMode = ResizeMode::None;// 窗口拉伸模式
    int borderWidth = 8; // 增大窗口边框宽度，用于边缘检测，改善用户体验
    bool isMaximize = false; // 用于跟踪窗口是否最大化
    ContactModel *contactModel;// 联系人模型
    ContactModel *messageModel;// 消息模型
    ContactDelegate *contactDelegate; // 联系人委托
    QString userID; // 当前登录用户ID
    QString receiverID; // 当前聊天对象ID
    QHash<QString, QList<Message>> messageDataMap; // 存储消息数据映射
    QList<Friend> contactList; // 存储联系人
    QList<QString> contactIndexList; // 存储联系人索引
    QHash<QString, bool> contactSelected; // 存储联系人是否被选中
    NetworkManager *m_networkManager; // 网络管理器

private:
    void initUI();// 初始化UI
    void connectSignals();// 连接信号槽
    void setBackgroundImage(const QString &imagePath); // 新增设置背景图片函数
    ResizeMode getResizeMode(const QPoint &pos); // 获取当前鼠标所在的边缘位置
    void resizeWindow(const QPoint &globalPos); // 根据鼠标位置调整窗口大小
    void updateCursor(ResizeMode mode); // 更新鼠标光标样式
    void showMaximize();//显示最大化窗口
    void showContact();//显示联系人列表
    void showCollect();//显示收藏界面
    void showMessage();//显示消息
    void showMoments();//显示朋友圈
    void showSearch();//显示搜索界面
    void showMore();//显示更多界面
    void sendMessage(); // 发送消息
    void onContactClicked(const QModelIndex &index); // 点击联系人
    void addSampleMessages(); // 添加示例消息
    void displayContactMessages(QString receiverID); // 显示联系人消息
    void onMessageClicked(const QModelIndex &index); // 点击消息
    void loadContactMessages(QString receiverID); // 加载联系人消息（兼容旧接口）
    void onMessageReceived(const QString &senderId, const QString &content, const QDateTime &time); // 处理接收消息
    void onNetworkConnected(); // 处理网络连接
    void onNetworkDisconnected(); // 处理网络断开
    void onNetworkError(const QString &error); // 处理网络错误
    void animatePageTransition(QWidget *widget); // 页面切换动画

protected:
    void mousePressEvent(QMouseEvent *event) override;// 鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;// 鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event) override;// 鼠标释放事件
    void leaveEvent(QEvent *event) override;// 鼠标离开事件

};

#endif // CHATWINDOW_H