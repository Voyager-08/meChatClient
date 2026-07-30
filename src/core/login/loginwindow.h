#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QPropertyAnimation>
class QLineEdit;
class QCheckBox;
class QPushButton;
class RegisterWidget;
class QLabel;

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    // 对外暴露的信号 - 供 EventBus 绑定
    void loginRequested(const QString &userID, const QString &password); // 登录请求
    void registerRequested(const QString &userID, const QString &userNick, const QString &password); // 注册请求
    void loginSuccess(const QString &userID); // 登录成功
    void sendAvatarFileRequest(const QString &filePath, const QString &userId); // 发送头像文件到服务器
    
    // 内部注册信号
    void registerSuccess(const QString &userID); // 注册成功
    void registerFailed(const QString &errorString); // 注册失败
    
public slots:
    void onLoginSuccess(const QString &userID); // 登录成功处理
    void onLoginFailed(const QString &errorString); // 登录失败处理
    void onRegisterSuccess(const QString &userID); // 注册成功处理
    void onRegisterFailed(const QString &errorString); // 注册失败处理
    void onNetworkErrorOccurred(const QString &errorString); // 网络错误处理
    void onSendAvatarFileRequest(const QString &filePath, const QString &userId); // 发送头像文件到服务器处理
private slots:
    void resetLineEditStyle(QLineEdit* lineEdit); // 重置输入框样式
    bool checkInputFields(); // 检查输入框
    void showAvatar(); // 显示头像
    void togglePasswordVisibility(); // 切换密码可见性
    void onForgetPassword(); // 忘记密码处理
    bool login(); // 登录功能
    void loadUserHistoryInfo(); // 加载用户历史登录信息
    void loginError(const QString &errorString); // 登录错误处理
    void handleSwitchToRegister(); // 内部切换到注册界面
    void handleSwitchToLogin(); // 内部切换到登录界面
    void loginChatwindow(const QString userID); // 登录成功后打开聊天窗口

private:
    void initUI(); // 初始化界面
    void setBackground(); // 设置背景
    void connectUISignals(); // 信号槽连接
    void handleRegisterSuccess(); // 注册成功处理（内部槽）

protected:
    bool eventFilter(QObject *obj, QEvent *event) override; // 事件过滤器重写
    void mousePressEvent(QMouseEvent *event) override; // 鼠标按下事件

private:
    // UI 组件
    QStackedWidget* stackedWidget; // 用于切换登录和注册界面
    QWidget* loginWidget; // 主登录窗口组件
    RegisterWidget* registerWidget; // 主注册窗口组件
    QLabel* userAvatar; // 用户头像标签
    QLineEdit* userIDLineEdit; // 用户名输入框
    QLineEdit* passwordLineEdit; // 密码输入框
    QCheckBox* showPasswordButton; // 显示密码复选框
    QPushButton* forgetPasswordButton; // 忘记密码按钮
    QPushButton* loginButton; // 登录按钮
    QPushButton* registerButton; // 注册按钮
    
    // 状态变量
    bool resetLineEdit = false; // 是否重置输入框
    QString originalStyle; // 保存原始样式
    QString showPasswordButtonStyle; // 保存显示密码复选框样式
    QString savedUserID; // 保存用户ID
    QString savedPassword; // 保存密码
};

#endif // LOGINWINDOW_H
