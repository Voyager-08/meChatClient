#include <QPixmap>
#include <QPalette>         // 包含调色板头文件
#include <QPainter>         // 包含绘图头文件
#include <QPainterPath>    // 包含路径头文件
#include <QGraphicsDropShadowEffect>     // 包含阴影效果头文件
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFocusEvent>
#include <QMouseEvent>
#include <QPropertyAnimation>       // 用于切换登录和注册界面
#include <QEasingCurve>             // 用于动画缓动曲线
#include <QTransform>               // 用于图形变换
#include <QFile>

#include "loginwindow.h"
#include "src/sql/database.h"// 包含数据库头文件
#include "registerwidget.h"

LoginWindow::LoginWindow(QDialog *parent)
    : QDialog(parent)
    , m_currentWidgetIndex(0)
{
    initUI();// 初始化界面
    setBackground();// 设置背景
    signalConnect();// 信号槽连接
    showAvatar();// 显示头像
}

void LoginWindow::initUI()
{
    // 设置窗口标题
    setWindowTitle("meChat");
    // 设置窗口固定大小
    setFixedSize(700, 450);
    setWindowIcon(QIcon(":/images/10.png"));

    loginWidget=new QWidget(this);// 主登录窗口组件
    registerWidget=new RegisterWidget(this);// 主注册窗口组件
    stackedWidget=new QStackedWidget(this);// 用于切换登录和注册界面
    stackedWidget->addWidget(loginWidget);
    stackedWidget->addWidget(registerWidget);
    stackedWidget->setGeometry(0,0,this->width(),this->height());
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);

    // 设用户头像
    userAvatar = new QLabel(loginWidget);
    userAvatar->setPixmap(QPixmap(registerWidget->avatarPath));
    userAvatar->setGeometry(315, 30, 80, 80);// 设置位置和大小
    userAvatar->setScaledContents(true);  // 自动缩放图片适应标签大小
    
    //设置阴影
    QGraphicsDropShadowEffect *shadowEf = new QGraphicsDropShadowEffect(loginWidget);
    shadowEf->setBlurRadius(10);// 阴影模糊大小
    shadowEf->setColor(QColor(0, 0, 0, 100));// 阴影颜色
    shadowEf->setOffset(5, 5);// 阴影偏移量(水平, 垂直)
    userAvatar->setGraphicsEffect(shadowEf);// 设置阴影效果

    // 添加用户名输入框+图标
    // 添加阴影效果
    QGraphicsDropShadowEffect *shadowEf_user = new QGraphicsDropShadowEffect(loginWidget);
    shadowEf_user->setBlurRadius(10);// 阴影模糊大小
    shadowEf_user->setColor(QColor(0, 0, 0, 100));// 阴影颜色
    shadowEf_user->setOffset(0, 5);// 阴影偏移量(水平, 垂直)

    userIDLineEdit = new QLineEdit(loginWidget);
    userIDLineEdit->setText("23631642510");// 测试用默认填充用户名
    userIDLineEdit->installEventFilter(this);// 安装事件过滤器,为后续点击做准备
    userIDLineEdit->setPlaceholderText("账号");
    userIDLineEdit->setGeometry(200, 150, 300, 33);// 设置距离窗口的坐标为(90px, 100px)和大小宽350px，高33px
    
    originalStyle ="QLineEdit {"
    "font: 17px 'KaiTi', '楷体';"
    "padding: 5px;"// 设置内边距
    "border: 2px solid rgba(255, 255, 255, 0.7);"
    "border-radius: 8px;"
    "background-color: rgba(255, 255, 255, 0.1);"
    "color: rgba(0, 0, 0, 1);"
    "}"
    "QLineEdit:focus {"
    "border-color: rgba(255, 255, 255, 1);"
    "outline: none;"
    "}";
    userIDLineEdit->setStyleSheet(originalStyle);
    userIDLineEdit->setGraphicsEffect(shadowEf_user);// 设置阴影效果

    // 添加用户账号输入框标签图标
    // 创建独立的 QLabel 作为图标容器
    QLabel *iconLabel = new QLabel(loginWidget);
    QPixmap userIDPixmap(":/images/icon_user.png");
    userIDPixmap = userIDPixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconLabel->setPixmap(userIDPixmap);
    iconLabel->setGeometry(173, 155, 24, 24); // 手动定位到输入框内左侧

    // 添加密码输入框+图标
    QGraphicsDropShadowEffect *shadowEf_password = new QGraphicsDropShadowEffect(loginWidget);
    shadowEf_password->setBlurRadius(10);// 阴影模糊大小
    shadowEf_password->setColor(QColor(0, 0, 0, 100));// 阴影颜色
    shadowEf_password->setOffset(0, 5);// 阴影偏移量(水平, 垂直)

    passwordLineEdit = new QLineEdit(loginWidget);
    passwordLineEdit->setText("642510");
    passwordLineEdit->installEventFilter(this);// 安装事件过滤器,为后续点击做准备
    passwordLineEdit->setPlaceholderText("密码");
    passwordLineEdit->setGeometry(200, 210, 300, 33);
    passwordLineEdit->setEchoMode(QLineEdit::Password);// 设置密码输入框
    // 设置回车键触发登录
    
    passwordLineEdit->setStyleSheet(originalStyle);
    // 添加阴影效果
    passwordLineEdit->setGraphicsEffect(shadowEf_password);// 将阴影效果应用到输入框
    // 添加密码图标
    QLabel *iconPasswordLabel = new QLabel(loginWidget);
    QPixmap passwordPixmap(":/images/icon_password.png");
    passwordPixmap = passwordPixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconPasswordLabel->setPixmap(passwordPixmap);
    iconPasswordLabel->setGeometry(173, 215, 24, 24); // 手动定位到输入框内左侧

    // 添加显示密码复选框
    showPasswordButton = new QCheckBox("显示密码", loginWidget);
    showPasswordButton->setGeometry(200, 267, 80, 25);
    showPasswordButton->setStyleSheet(
    "QCheckBox {"
    "font: 15px 'KaiTi', '楷体';"
    "color: rgba(0, 0, 0, 0.7);"
    "border: none;"
    "}"
    "QCheckBox:hover {"
    "color: rgba(0, 38, 255, 1);"
    "}");
    showPasswordButton->setCursor(Qt::PointingHandCursor); // 设置鼠标悬停时的手型光标

    // 添加忘记密码按钮
    forgetPasswordButton = new QPushButton(loginWidget);
    forgetPasswordButton->setText("忘记密码？");
    forgetPasswordButton->setGeometry(420, 267, 80, 25);
    forgetPasswordButton->setStyleSheet(
    "QPushButton {"
    "font: 15px 'KaiTi', '楷体';"
    "color: rgba(1, 1, 1, 0.7);"
    "border: none;"
    "}"
    "QPushButton:hover {"
    "color: rgba(0, 38, 255, 1);"
    "}");
    forgetPasswordButton->setAutoDefault(false); // 防止按钮响应回车键
    forgetPasswordButton->setCursor(Qt::PointingHandCursor); // 设置鼠标悬停时的手型光标

    // 添加登录按钮
    // 创建阴影效果
    QGraphicsDropShadowEffect *shadowEf_login = new QGraphicsDropShadowEffect(loginWidget);
    shadowEf_login->setBlurRadius(10);// 阴影模糊大小
    shadowEf_login->setColor(QColor(0, 0, 0, 100));
    shadowEf_login->setOffset(0, 5);// 阴影偏移量(水平, 垂直)
    
    loginButton = new QPushButton(loginWidget);
    loginButton->setText("登录");
    loginButton->setGeometry(200, 350, 100, 33);
    loginButton->setStyleSheet(
    "QPushButton {"
    "font: 17px;"
    "font-family: 'KaiTi', '楷体'; "
    "color: rgba(250, 250, 250, 0.8);"
    "border: none;"
    "border-radius: 5px;"
    "border: 2px solid rgba(255, 255, 255, 0.6);"
    "border-radius: 8px;"
    "}"
    "QPushButton:hover {"
    "border-color: rgba(255, 255, 255, 1);"
    "color: rgba(255, 255, 255, 1);"
    "}"
    "QPushButton:pressed {"
    "padding-top: 8px;  /* 微下沉效果 */"
    "padding-bottom: 4px;"
    "background-color: rgba(255, 238, 0, 0.45);"
    "}");
    loginButton->setGraphicsEffect(shadowEf_login);// 将阴影效果应用到按钮
    loginButton->setAutoDefault(true); // 设置为默认按钮

    // 添加注册按钮
    QGraphicsDropShadowEffect *shadowEf_register = new QGraphicsDropShadowEffect(loginWidget);
    shadowEf_register->setBlurRadius(10);// 阴影模糊大小
    shadowEf_register->setColor(QColor(0, 0, 0, 100));
    shadowEf_register->setOffset(0, 5);// 阴影偏移量(水平, 垂直)
    registerButton = new QPushButton(loginWidget);
    registerButton->setText("注册");
    registerButton->setGeometry(400, 350, 100, 33);
    registerButton->setStyleSheet(
    "QPushButton {"
    "font: 17px;"
    "font-family: 'KaiTi', '楷体'; "
    "color: rgba(255, 255, 255, 0.8);"
    "border: none;"
    "border-radius: 5px;"
    "border: 2px solid rgba(255, 255, 255, 0.6);"
    "border-radius: 8px;"
    "}"
    "QPushButton:hover {"
    "border-color: rgba(255, 255, 255, 1);"
    "color: rgba(255, 255, 255, 1);"
    "}"
    "QPushButton:pressed {"
    "padding-top: 10px;  /* 微下沉效果 */"
    "padding-bottom: 4px;"
    "background-color: rgba(255, 238, 0, 0.45);"
    "}"); 
    registerButton->setGraphicsEffect(shadowEf_register);// 将阴影效果应用到按钮
    registerButton->setAutoDefault(false); // 防止按钮响应回车键
}

void LoginWindow::setBackground()
{
    // 加载并缩放背景图以适应窗口大小
    QPixmap bg(":/images/6.png");
    if (bg.isNull()) {
        qWarning() << "Failed to load background image!";
        return;
    }

    // 缩放图片以填充整个窗口（保持比例可选）
    bg = bg.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // 设置为窗口背景
    QPalette palette;
    palette.setBrush(QPalette::Window, bg);
    setPalette(palette);
    setAutoFillBackground(true);
}

bool LoginWindow::login()//登录功能
{
    if(resetLineEdit==1) return false;// 如果是重置输入框状态，则不执行登录操作
    //点击登录就需要记住输入
    savedUserID = userIDLineEdit->text();
    savedPassword = passwordLineEdit->text();
    qDebug() << "用户ID：" << savedUserID << "密码：" << savedPassword;
    if(checkInputFields()) return false;// 检查输入框
    // 在这里添加验证用户名和密码的逻辑
    Database loginDb;
    if (!loginDb.connect("login")) {
        qDebug() << "数据库连接失败!";
        return false;
    }
    
    QSqlQuery query(loginDb.database());// 使用当前数据库连接创建查询对象
    QString sql = "SELECT user_id FROM users WHERE user_id = ? AND password = ?";
    query.prepare(sql);
    query.addBindValue(savedUserID);
    query.addBindValue(savedPassword);
    
    if(query.exec() && query.first()) 
    {
        qDebug() << "登录成功!";
        savedUserID = query.value(0).toString();// 保存当前用户ID
        accept();
        // 登录成功，打开主窗口
    }
    else
    {
        qDebug() << "查询执行失败:" << query.lastError().text();
        resetLineEdit=1;// 设置重置输入框标志
        qDebug() << "用户ID或密码错误!";
        // 清空输入框以显示错误提示
        userIDLineEdit->clear();
        passwordLineEdit->clear();
        // 在账号输入框中显示错误提示
        userIDLineEdit->setPlaceholderText("账号或者密码错误");
        userIDLineEdit->setStyleSheet("QLineEdit {"
        "font: 16px 'KaiTi', '楷体';"
        "padding: 5px;"
        "border: 2px solid rgba(255, 0, 0, 1);"
        "border-radius: 8px;"
        "background-color: rgba(255, 255, 255, 0.6);"
        "color: rgba(255, 0, 0, 1);"
        "}");
        passwordLineEdit->setPlaceholderText("账号或者密码错误");
        passwordLineEdit->setStyleSheet("QLineEdit {"
        "font: 16px 'KaiTi', '楷体';"
        "padding: 5px;"
        "border: 2px solid rgba(255, 0, 0, 1);"
        "border-radius: 8px;"
        "background-color: rgba(255, 255, 255, 0.6);"
        "color: rgba(255, 0, 0, 1);"
        "}");
        // 设置显示密码复选框为高亮不可用状态
        showPasswordButton->setStyleSheet("QCheckBox {"
        "font: 14px 'KaiTi', '楷体';"
        "color: rgba(255, 255, 255, 0.7);"
        "border: none;"
        "}");
        showPasswordButton->setEnabled(false);
        return false;
    }
    return false;
}

void LoginWindow::signalConnect()// 信号槽连接
{
    connect(registerWidget->backButton, &QPushButton::clicked, this, &LoginWindow::switchToLogin);
    connect(registerWidget, &RegisterWidget::registrationSucceeded,this, &LoginWindow::registerSuccess);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::login);
    connect(userIDLineEdit, &QLineEdit::returnPressed, this, &LoginWindow::login);
    connect(passwordLineEdit, &QLineEdit::returnPressed, this, &LoginWindow::login);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::login);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::switchToRegister);
    connect(showPasswordButton, &QCheckBox::toggled, this, &LoginWindow::togglePasswordVisibility);
    connect(forgetPasswordButton, &QPushButton::clicked, this, &LoginWindow::onForgetPassword);
    //账号输入框内容改变时显示头像
    connect(userIDLineEdit, &QLineEdit::textChanged, this, &LoginWindow::showAvatar);
}

void LoginWindow::registerSuccess()// 注册成功
{
    switchToLogin();
    userIDLineEdit->setText(registerWidget->getUserID()); // 将注册成功后返回的用户ID设置到登录窗口的用户名输入框中
    passwordLineEdit->setText("");
    userAvatar->setPixmap(QPixmap(registerWidget->avatarPath));// 显示注册成功后的用户头像
}

void LoginWindow::togglePasswordVisibility()// 切换密码可见性
{
    if (showPasswordButton->isChecked())passwordLineEdit->setEchoMode(QLineEdit::Normal);
    else passwordLineEdit->setEchoMode(QLineEdit::Password);
}

void LoginWindow::onForgetPassword()// 忘记密码处理
{
    QMessageBox::information(this, "忘记密码", "请通过注册邮箱或手机号找回密码。");
}

bool LoginWindow::eventFilter(QObject *obj, QEvent *event)
{
    // 检查是否是监控的 QLineEdit 且事件是焦点进入
    if ((obj == userIDLineEdit|| obj == passwordLineEdit  )  &&event->type() == QEvent::FocusIn) {
        resetLineEditStyle((qobject_cast<QLineEdit*>(obj)));
        return true; // 表示事件已处理（但通常返回 false 更安全）
    }
    // 其他事件交给父类处理
    return QDialog::eventFilter(obj, event);
    // 如果返回 true，则表示事件已处理，不再传递给父类
}

void LoginWindow::resetLineEditStyle(QLineEdit *lineEdit )// 重置输入框样式
{
    showPasswordButton->setStyleSheet("QCheckBox {"
    "font: 14px 'KaiTi', '楷体';"
    "color: rgba(0, 0, 0, 1);"
    "border: none;"
    "}"
    "QCheckBox:hover {"
    "color:rgba(0, 38, 255, 0.7) ;"
    "}");
    showPasswordButton->setEnabled(true);
    userIDLineEdit->setPlaceholderText("用户名");
    passwordLineEdit->setPlaceholderText("密码");
    userIDLineEdit->setStyleSheet(originalStyle);
    passwordLineEdit->setStyleSheet(originalStyle);
    if(resetLineEdit==1)
    {
        resetLineEdit=0;// 重置标志清零
        userIDLineEdit->setText(savedUserID);
        passwordLineEdit->setText(savedPassword);
        // 恢复显示密码按钮状态
    }
}

bool LoginWindow::checkInputFields()// 检查输入框
{
    if(savedUserID.isEmpty())
    {
        userIDLineEdit->setPlaceholderText("账号不能为空！");
        userIDLineEdit->setStyleSheet("QLineEdit {"
        "font: 16px 'KaiTi', '楷体';"
        "padding: 5px;"
        "border: 2px solid rgba(255, 0, 0, 1);"
        "border-radius: 8px;"
        "background-color: rgba(255, 255, 255, 0.6);"
        "color: rgba(255, 0, 0, 1);"
        "}");
    }
    if(savedPassword.isEmpty())
    {
        passwordLineEdit->setPlaceholderText("密码不能为空！");
        passwordLineEdit->setStyleSheet("QLineEdit {"
        "font: 16px 'KaiTi', '楷体';"
        "padding: 5px;"
        "border: 2px solid rgba(255, 0, 0, 1);"
        "border-radius: 8px;"
        "background-color: rgba(255, 255, 255, 0.4);"
        "color: rgba(255, 0, 0, 1);"
        "}");
        // 设置显示密码复选框为高亮不可用状态
        showPasswordButton->setStyleSheet("QCheckBox {"
        "font: 14px 'KaiTi', '楷体';"
        "color: rgba(255, 255, 255, 1);"
        "border: none;"
        "}");
        showPasswordButton->setEnabled(false);
    }
    if(userIDLineEdit->text().isEmpty()||passwordLineEdit->text().isEmpty())
    {
        qDebug() << "请填写账号和密码！";
        return true;// 返回true,说明有一个没有填写，则不执行登录操作
    }
    return false;// 返回false，表示两个输入框都已填写，继续执行登录操作
}

void LoginWindow::showAvatar()
{
    Database db;
    if (!db.connect("login")) {
        qDebug() << "数据库连接失败";
        return;
    }

    QSqlQuery query(db.database());
    query.prepare("SELECT `avatar_path` FROM `users` WHERE `user_id` = :userID");
    query.bindValue(":userID", userIDLineEdit->text());

    if (!query.exec()) {
        qDebug() << "SQL 查询失败:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        QString avatarPath = query.value("avatar_path").toString();
        QPixmap pixmap;
        
        if (QFile::exists(avatarPath)) {
            pixmap = QPixmap(avatarPath);
        } else {
            pixmap = QPixmap(":/images/default_avatar.png");
        }

        // 创建圆角头像
        QPixmap roundedAvatar(80, 80);
        roundedAvatar.fill(Qt::transparent);
        
        QPainter painter(&roundedAvatar);
        painter.setRenderHint(QPainter::Antialiasing);
        
        // 创建圆形路径
        QPainterPath path;

        
        path.addRoundedRect(0, 0, 80, 80,5,5);  // 完整圆形
        painter.setClipPath(path);// 设置剪切路径

        // 绘制缩放后的头像
        painter.drawPixmap(0, 0, pixmap.scaled(80, 80, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        
        userAvatar->setPixmap(roundedAvatar);
    }
}

void LoginWindow::switchToRegister()//切换到注册界面
{
    // 创建动画
    QPropertyAnimation *animation = new QPropertyAnimation(stackedWidget, "geometry");
    animation->setDuration(600);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    
    // 获取当前几何信息
    QRect startGeometry = stackedWidget->geometry();
    QRect endGeometry = startGeometry;
    
    // 向上翻转到注册页面
    endGeometry.moveTop(-stackedWidget->height()/2);
    
    animation->setStartValue(startGeometry);
    animation->setEndValue(endGeometry);
    
    // 连接动画完成信号，在动画结束后切换页面
    connect(animation, &QPropertyAnimation::finished, [=]() {
        stackedWidget->setCurrentWidget(registerWidget);
        
        // 创建第二个动画，从上往下翻回来
        QPropertyAnimation *animation2 = new QPropertyAnimation(stackedWidget, "geometry");
        animation2->setDuration(200);
        animation2->setEasingCurve(QEasingCurve::InCubic);
        
        QRect startGeom = endGeometry;
        QRect endGeom = startGeometry;
        startGeom.moveTop(height());
        endGeom.moveTop(0);
        
        animation2->setStartValue(startGeom);
        animation2->setEndValue(endGeom);
        
        animation2->start(QAbstractAnimation::DeleteWhenStopped);
    });
    
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoginWindow::switchToLogin()//切换到登录界面
{
    // 创建动画
    QPropertyAnimation *animation = new QPropertyAnimation(stackedWidget, "geometry");
    animation->setDuration(600);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    
    // 获取当前几何信息
    QRect startGeometry = stackedWidget->geometry();
    QRect endGeometry = startGeometry;
    
    // 向下翻转到登录页面
    endGeometry.moveTop(stackedWidget->height());
    
    animation->setStartValue(startGeometry);
    animation->setEndValue(endGeometry);
    
    // 连接动画完成信号，在动画结束后切换页面
    connect(animation, &QPropertyAnimation::finished, [=]() {
        stackedWidget->setCurrentWidget(loginWidget);
        
        // 创建第二个动画，从下往上翻回来
        QPropertyAnimation *animation2 = new QPropertyAnimation(stackedWidget, "geometry");
        animation2->setDuration(200);
        animation2->setEasingCurve(QEasingCurve::InCubic);
        
        QRect startGeom = endGeometry;
        QRect endGeom = startGeometry;
        startGeom.moveTop(-height()/2);
        endGeom.moveTop(0);
        
        animation2->setStartValue(startGeom);
        animation2->setEndValue(endGeom);
        
        animation2->start(QAbstractAnimation::DeleteWhenStopped);
    });
    
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}