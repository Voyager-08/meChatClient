#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QTimer>


class LoginWindow;
class ChatWindow;
class NetworkManager;
class QThread;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 登录成功，切换到聊天窗口
    void onUserLoggedIn(const QString &userID, const QString &password);
    // 用户退出登录，返回登录窗口
    void onUserLoggedOut();

private:
    void initializeUI();     // 初始化UI
    void connectSignals();   // 连接信号槽
    void linkServer();// 连接服务器

private:
    QStackedWidget *stackedWidget;  // 用于页面切换的栈式窗口
    LoginWindow *loginWindow;        // 登录页面
    ChatWindow *chatWindow;          // 聊天页面
    NetworkManager *networkManager;  // 全局网络管理器
    QThread *networkThread;          // 网络线程
};

#endif // MAINWINDOW_H
