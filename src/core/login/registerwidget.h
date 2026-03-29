#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

class ClickableLabel;
class NetworkManager;

class RegisterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterWidget(QWidget *parent = nullptr, NetworkManager *networkManager = nullptr);
    ~RegisterWidget();
    QString getUserID() const;// 获取用户名

signals:
    void registrationSucceeded();// 返回登录信号

public slots :
    bool onRegisterClicked();// 注册按钮点击
    void onPasswordChanged(const QString &text);// 密码框内容改变
    void onSelectAvatar();// 选择头像   

private:
    void initUI();
    void setBackground(); // 添加背景设置函数
    void connectUISignals();// 信号连接
    bool validateInput();// 验证输入
    void showError(const QString &message);// 显示错误信息
    void clearErrors();// 清除错误信息
    void lineEditHighlight(QLineEdit *lineEdit);//错误输入框高亮
    void flashErrorLabel(QLabel* label, int loopCount = 2, int durationPerLoop = 500);// 错误信息显示闪动

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    
public:
    // UI 组件
    QPushButton *avatarButton; // 使用 QPushButton 以支持点击事件，作为标题显示
    QLabel *backgroundLabel; // 添加背景标签，用于设置背景图片
    QLabel *userIDLabel; // 用户名标签
    ClickableLabel *avatarTipLabel;// 头像提示标签
    QLineEdit *userIDLineEdit; // 用户名输入框
    QLineEdit *userNickLineEdit; // 用户昵称输入框
    QLabel *passwordLabel; // 密码标签
    QLineEdit *passwordLineEdit; // 密码输入框
    QLabel *confirmPasswordLabel; // 确认密码标签
    QLineEdit *confirmPasswordLineEdit; // 确认密码输入框
    QCheckBox *showPasswordCheckBox; // 显示密码复选框，用于切换密码的显示/隐藏
    QPushButton *registerButton; // 注册按钮
    QPushButton *backButton; // 返回按钮
    QLabel *errorLabel; // 错误信息标签，用于显示注册过程中的错误提示
    QString originalStyleSheet; // 保存原始样式表，用于恢复样式
    // 数据
    QString showPasswordButtonStyle; // 显示密码复选框样式
    QString userID; // 用户ID
    QString password;// 密码
    QString userNick; // 用户昵称
    QString avatarPath=":/images/avatar/default.png"; // 头像路径
    // 网络管理器
    NetworkManager *networkManager; // 全局网络管理器
};

#endif // REGISTERWIDGET_H