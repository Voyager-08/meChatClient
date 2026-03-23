#ifndef LoginWindow_H
#define LoginWindow_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QWidget>
#include <QStackedWidget>       // 用于切换登录和注册界面
#include <QPropertyAnimation>   // 用于切换登录和注册界面
#include <QMap>                 // 用于存储用户ID到头像路径的映射



QT_BEGIN_NAMESPACE
QT_END_NAMESPACE
class ChatWindow;// 前向声明聊天窗口类

struct UserInfo//用户ID, 昵称, 密码, 头像路径
{
    QString userID;
    QString userName;
    QString userPassword;
    QString userAvatar;
};

class NetworkManager;
class RegisterWidget;
class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr,
                         NetworkManager *networkManager = nullptr);
    QString getUserId() const{ return savedUserID; };
    
//类成员变量
private:
//鼠标对象
    QMouseEvent* mouseEvent;
// 网络管理器
    NetworkManager *m_networkManager; 
// UI 组件
    QStackedWidget*stackedWidget;// 用于切换登录和注册界面
    QWidget*loginWidget; // 主登录窗口组件
    RegisterWidget *registerWidget; // 主注册窗口组件
    QLabel* userAvatar; // 用户头像标签
    QLineEdit* userIDLineEdit; // 用户名输入框
    QLineEdit* passwordLineEdit; // 密码输入框
    QCheckBox* showPasswordButton; // 显示密码复选框
    QPushButton* forgetPasswordButton; // 忘记密码按钮
    QPushButton* loginButton; // 登录按钮
    QPushButton* registerButton; // 注册按钮
    bool resetLineEdit=0;// 是否重置输入框
    QString originalStyle;// 保存原始样式
    QString showPasswordButtonStyle;// 保存显示密码复选框样式
    QString savedUserID;// 保存用户ID
    QString savedPassword;// 保存密码
    QString savedUserNick; // 保存用户昵称
    QMap<QString, QString> userAvatarMap; // 存储用户ID到头像路径的映射

    // 动画相关
    QPropertyAnimation *m_flipAnimation;// 窗口翻转动画
    
signals:
    void userLoggedIn(const QString &userID, const QString &password); // 登录成功信号，按了布局，发送给主窗口
//槽函数
private slots:
    void resetLineEditStyle(QLineEdit* lineEdit);// 重置输入框样式
    bool checkInputFields();// 检查输入框
    void showAvatar();// 显示头像
    void togglePasswordVisibility(); // 切换密码可见性
    void onForgetPassword(); // 忘记密码处理
    bool login();//登录功能
    void loginError(const QString &errorString);// 登录错误处理
    void switchToRegister();//切换到注册界面
    void switchToLogin();//切换到登录界面
    void loginMainwindow(const QString userID);// 登录成功后打开主窗口
//类函数
private:
    void linkServer();// 连接服务器
    void initUI();// 初始化界面
    void setBackground();// 设置背景
    void signalConnect();// 信号槽连接
    void registerSuccess();// 注册成功
    void loadUserAvatarData();// 预加载用户头像数据
protected:
    bool eventFilter(QObject *obj, QEvent *event)override;// 事件过滤器重写
};

#endif // LoginWindow_H