#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QTimer>
#include <QPointer>

class LoginWindow;
class ChatWindow;
class EventBus;
class NetworkManager;
class DatabaseManager;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(EventBus *eventBus, NetworkManager *networkManager, DatabaseManager *databaseManager, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 登录成功，切换到聊天窗口
    void onUserLoggedIn(const QString &userID);
    // 用户退出登录，返回登录窗口
    void onUserLoggedOut(const QString &userId);

private:
    void initializeUI();     // 初始化UI
    void connectSignals();   // 连接信号槽

private:
    QStackedWidget *stackedWidget;  // 用于页面切换的栈式窗口
    LoginWindow *loginWindow;        // 登录注册页面
    QPointer<ChatWindow> chatWindow; // 聊天页面（动态创建/销毁，使用智能指针）
    QPointer<EventBus> eventBus;     // 事件总线（外部传入，使用智能指针防止悬空）
    QPointer<NetworkManager> networkManager;  // 网络管理器（外部传入）
    QPointer<DatabaseManager> databaseManager; // 数据库管理器（外部传入）
};

#endif // MAINWINDOW_H