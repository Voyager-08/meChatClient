#ifndef LoginWindow_H
#define LoginWindow_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QWidget>
#include <QStackedWidget>// 用于切换登录和注册界面
#include <QPropertyAnimation>


QT_BEGIN_NAMESPACE
QT_END_NAMESPACE
class RegisterWidget;
class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QDialog *parent = nullptr);
    ~LoginWindow();
    QString getUserID() const;
    
//类成员变量
private:
//鼠标对象
    QMouseEvent* mouseEvent;

// UI 组件
    QStackedWidget*stackedWidget;// 用于切换登录和注册界面
    QWidget*loginWidget; // 主登录窗口组件
    RegisterWidget *registerWidget; // 主注册窗口组件
    QLabel* userLabel; // 添加欢迎标签的声明
    QLineEdit* userIDLineEdit; // 用户名输入框
    QLineEdit* passwordLineEdit; // 密码输入框
    QCheckBox* showPasswordButton; // 显示密码复选框
    QPushButton* forgetPasswordButton; // 忘记密码按钮
    QPushButton* loginButton; // 登录按钮
    QPushButton* registerButton; // 注册按钮
    QLabel* icon_user; // 用户图标标签
    QLabel* icon_password; // 密码图标标签
    bool resetLineEdit=0;// 是否重置输入框
    QString originalStyle;// 保存原始样式
    QString savedUserID;// 保存用户ID
    QString savedPassword;// 保存密码

    // 动画相关
    QPropertyAnimation *m_flipAnimation;
    int m_currentWidgetIndex;
    
//槽函数
private slots:
    void resetLineEditStyle(QLineEdit* lineEdit);// 重置输入框样式
    bool checkInputFields();// 检查输入框
    void togglePasswordVisibility(); // 切换密码可见性
    void onForgetPassword(); // 忘记密码处理
    bool login();//登录功能
    void switchToRegister();//切换到注册界面
    void switchToLogin();//切换到登录界面
//类函数
private:
    void initUI();// 初始化界面
    void setBackground();// 设置背景
    void signalConnect();// 信号槽连接
    void registerSuccess();// 注册成功
protected:
    bool eventFilter(QObject *obj, QEvent *event)override;// 事件过滤器重写
};

#endif // LoginWindow_H