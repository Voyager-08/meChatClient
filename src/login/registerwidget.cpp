#include "registerwidget.h"
#include "../sql/database.h"// 包含数据库头文件
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QGraphicsEffect>
#include <QFileDialog>
#include <QPropertyAnimation>// 用于动画效果
#include <QRandomGenerator>
#include <QEvent>
#include <QTimer>
#include <QPainter>
#include <QGraphicsDropShadowEffect> // 添加阴影效果支持

RegisterWidget::RegisterWidget(QWidget *parent)
    : QWidget(parent)
{
    initUI();
    signalConnection();
}

RegisterWidget::~RegisterWidget()
{
}
QString RegisterWidget::getUserID() const
{
    return userID;// 返回用户ID
}

void RegisterWidget::initUI()
{
    // 设置窗口标题
    setWindowTitle("用户注册");
    // 设置窗口固定大小
    setFixedSize(700, 450);
    // 设置背景
    setBackground();

    // 创建主容器
    QFrame *container = new QFrame(this);
    container->setGeometry(150, 40, 410, 370); // 居中显示

    // 添加容器阴影效果
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(20); //  设置阴影效果的模糊半径为20像素
    shadowEffect->setColor(QColor(0, 0, 0, 100)); //  设置阴影颜色为黑色，透明度为100（半透明）
    shadowEffect->setOffset(0, 5); //  设置阴影的偏移量，水平方向为0，垂直方向为5像素
    container->setGraphicsEffect(shadowEffect); //  将设置好的阴影效果应用到容器组件上

    avatarButton = new QPushButton(container);
    avatarButton->setGeometry(150, 0, 80, 80);
    avatarButton->setFixedSize(80, 80);  // 设置固定大小
    QPixmap pixmap(avatarPath);
    // 将图片缩放到按钮大小，使用IgnoreAspectRatio确保完全填充
    QPixmap scaledPixmap = pixmap.scaled(avatarButton->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QIcon icon(scaledPixmap);
    avatarButton->setIcon(icon);
    avatarButton->setIconSize(avatarButton->size());

    // 用户图标
    QLabel *iconLabel = new QLabel(container);
    QPixmap userPixmap(":/images/icon_user.png");
    userPixmap = userPixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconLabel->setPixmap(userPixmap);
    iconLabel->setGeometry(40, 105, 24, 24);

    // 用户名输入框
    userIDLineEdit = new QLineEdit(container);
    userIDLineEdit->setPlaceholderText("请输入用户名");
    QString lineEditStyle =
        "QLineEdit {"
        "height: 40px;"
        "padding: 0 15px 0 35px;"
        "border: 1px solid rgba(255, 255, 255, 40%);"
        "border-radius: 20px;"
        "background-color: rgba(255, 255, 255, 10%);"
        "color: white;"
        "font-family: 'Microsoft YaHei', 'SimHei';"
        "font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "border: 1px solid #2196F3;"
        "background-color: rgba(255, 255, 255, 20%);"
        "}";
    originalStyleSheet=lineEditStyle;
    userIDLineEdit->setStyleSheet(lineEditStyle);
    userIDLineEdit->setGeometry(30, 100, 340, 40);

    // 密码图标
    QLabel *passwordIconLabel = new QLabel(container);
    QPixmap passwordPixmap(":/images/icon_password.png");
    passwordPixmap = passwordPixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    passwordIconLabel->setPixmap(passwordPixmap);
    passwordIconLabel->setGeometry(40, 160, 24, 24);

    // 密码输入框
    passwordLineEdit = new QLineEdit(container);
    passwordLineEdit->setPlaceholderText("请输入密码");
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setStyleSheet(lineEditStyle);
    passwordLineEdit->setGeometry(30, 155, 340, 40);

    // 确认密码图标
    QLabel *confirmPasswordIconLabel = new QLabel(container);
    QPixmap confirmPasswordPixmap(":/images/icon_password.png");
    confirmPasswordPixmap = confirmPasswordPixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    confirmPasswordIconLabel->setPixmap(confirmPasswordPixmap);
    confirmPasswordIconLabel->setGeometry(40, 215, 24, 24);

    // 确认密码输入框
    confirmPasswordLineEdit = new QLineEdit(container);
    confirmPasswordLineEdit->setPlaceholderText("请确认密码");
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordLineEdit->setStyleSheet(lineEditStyle);
    confirmPasswordLineEdit->setGeometry(30, 210, 340, 40);

    // 显示密码复选框
    showPasswordCheckBox = new QCheckBox("显示密码", container);
    showPasswordCheckBox->setGeometry(50, 255, 100, 20);

    // 注册按钮
    registerButton = new QPushButton("注册", container);
    QString registerButtonStyle =
        "QPushButton {"
        "background-color: #2196F3;"
        "color: white;"
        "padding: 10px 28px;"
        "border-radius: 20px;"
        "font-family: 'Microsoft YaHei', 'SimHei';"
        "font-size: 16px;"
        "font-weight: bold;"
        "border: none;"
        "}"
        "QPushButton:hover {"
        "background-color: #1E88E5;"
        "}"
        "QPushButton:pressed {"
        "background-color: #0D47A1;"
        "padding-top: 12px;  /* 微下沉效果 */"
        "padding-bottom: 8px;"
        "}";
    registerButton->setStyleSheet(registerButtonStyle);
    registerButton->setGeometry(30, 309, 160, 40);
    
    // 返回登录按钮
    backButton = new QPushButton("返回", container);
    QString backButtonStyle =
        "QPushButton {"
        "background-color: transparent;"
        "color: rgba(250, 250, 250, 1);"
        "padding: 10px 28px;"
        "border-radius: 20px;"
        "font-family: 'Microsoft YaHei', 'SimHei';"
        "font-size: 16px;"
        "border: 2px solid rgba(255, 255, 255, 0.5);"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(247, 218, 0, 1);"
        "font-weight: bold;"
        "border: 1px solid rgba(247, 218, 0, 1);"
        "color: white;"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(220, 190, 0, 1);"
        "padding-top: 12px;  /* 微下沉效果 */"
        "padding-bottom: 8px;"
        "}";
    backButton->setStyleSheet(backButtonStyle);
    backButton->setGeometry(210, 309, 160, 40);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(20);//模糊半径
    shadow->setColor(QColor(0, 0, 0, 80));//颜色
    shadow->setOffset(0, 5);//偏移
    backButton->setGraphicsEffect(shadow);
    

    // 错误提示标签
    errorLabel = new QLabel(container);
    QString errorLabelStyle ="QLabel { "
    "color: rgba(255, 0, 0, 0.58); "
    "font-family: 'KaiTi', 'SimKai'; "
    "font-size: 14px; "
    "font-weight: bold;"
     "}";
    errorLabel->setStyleSheet(errorLabelStyle);
    errorLabel->setGeometry(30, 285, 340, 20);
    errorLabel->hide();
}

void RegisterWidget::setBackground()
{
    // 加载并缩放背景图以适应窗口大小
    QPixmap bg("D:/cxdownload/夸克文件下载/8.png");
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

void RegisterWidget::signalConnection()
{
    connect(registerButton, &QPushButton::clicked, this, &RegisterWidget::onRegisterClicked);
    connect(showPasswordCheckBox, &QCheckBox::toggled, this, [=](bool checked) {
        passwordLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
        confirmPasswordLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    });
    connect(passwordLineEdit, &QLineEdit::textChanged, this, &RegisterWidget::onPasswordChanged);
    connect(avatarButton, &QPushButton::clicked, this, &RegisterWidget::onSelectAvatar);
    // 为可点击控件添加悬停事件处理
    registerButton->installEventFilter(this);
    // 注意：backButton 的连接将在 LoginWindow 类中进行处理，以支持动画效果
    showPasswordCheckBox->installEventFilter(this);
}

bool RegisterWidget::eventFilter(QObject *obj, QEvent *event)
{
    // 为按钮添加悬停效果
    if (obj == registerButton || obj == backButton) {
        if (event->type() == QEvent::Enter) {
            QPushButton *button = qobject_cast<QPushButton*>(obj);
            if (button) {
                button->setCursor(Qt::PointingHandCursor);// 设置鼠标指针为手形
            }
            return true;
        }
    }

    // 为复选框添加悬停效果
    if (obj == showPasswordCheckBox) {
        if (event->type() == QEvent::Enter) {
            showPasswordCheckBox->setCursor(Qt::PointingHandCursor);// 设置鼠标指针为手形
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);// 调用基类处理事件
}

bool RegisterWidget::onRegisterClicked()
{
    qDebug() << "Register clicked";
    if (!validateInput()) // 验证输入，如果验证失败，则返回
    {
        return false;
    }

    QString userID = userIDLineEdit->text();
    QString password = passwordLineEdit->text();
    qDebug()<<"      ID:"<<userID;
    qDebug()<<"password:"<<password;
    Database registerDb;
    if (!registerDb.connect("register")) {
        qDebug() << "数据库连接失败!";
        return false;
    }
    QSqlQuery query(registerDb.database());
    qDebug()<<"数据库语句执行";
    query.prepare("SELECT user_id FROM users WHERE user_id = ?");
    query.addBindValue(userID);

    if (!query.exec()) {
        showError("数据库查询出错: " + query.lastError().text());
        return false;
    }

    if (query.next()) {
        showError("用户名已存在，请选择其他用户名");
        // 添加输入框红色闪烁效果
        highlightError(userIDLineEdit);
        userIDLineEdit->setFocus();
        return false;
    }
    QString email="NULL";
    // 插入新用户
    query.prepare("INSERT INTO users (user_id, password,username,email,avatarpath) VALUES (?, ?,?,?,?)");
    query.addBindValue(userID);
    query.addBindValue(password);
    query.addBindValue(userID);
    query.addBindValue(email);
    query.addBindValue(avatarPath);

    if (!query.exec()) {
        qDebug() << "Error inserting user_id into database: " << query.lastError().text();
        showError("注册失败: " + query.lastError().text());
        return false;
    }

    // 保存用户名以便返回给调用者
    this->userID = userID;
    userIDLineEdit->clear();
    passwordLineEdit->clear();
    confirmPasswordLineEdit->clear();
    showPasswordCheckBox->setChecked(false);
    QMessageBox::information(this, "用户注册", "注册成功！");
    emit registrationSucceeded(); // 发送注册成功的信号
    return true;
}

void RegisterWidget::onPasswordChanged(const QString &text)
{
    // 使用Q_UNUSED宏标记未使用的参数text，避免编译器警告
    Q_UNUSED(text);
    // 如果错误提示出现以及提示中是否包含错误两个字，则确认密码错误
    if (!errorLabel->isHidden() && errorLabel->text().contains("密码")) 
    {
        clearErrors();
    }
}

void RegisterWidget::onSelectAvatar()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("选择头像"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));

    // 检查文件名是否为空
    if (!fileName.isEmpty()) 
    {
        // 根据文件名创建QPixmap对象
        QPixmap pixmap(fileName);
        // 检查pixmap是否有效（非空）
        if (!pixmap.isNull())
        {
            // 将图片缩放为80x80大小，保持宽高比，并使用平滑变换
            QPixmap scaledPixmap = pixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            // 设置按钮的图标为缩放后的图片
            avatarButton->setIcon(QIcon(scaledPixmap));
            // 设置按钮图标的显示大小为80x80
            avatarButton->setIconSize(QSize(80, 80));
        }
    }
    if(fileName.isEmpty())return;
    avatarPath=fileName;
}

bool RegisterWidget::validateInput()// 验证输入
{
    clearErrors();// 先清除错误标签以及信息

    QString userID = userIDLineEdit->text().trimmed();// 获取用户ID,去除空格
    QString password = passwordLineEdit->text();
    QString confirmPassword = confirmPasswordLineEdit->text();

    if (userID.isEmpty()) {
        showError("用户名不能为空!");
        userIDLineEdit->setFocus();// 设置焦点
        highlightError(userIDLineEdit);
        return false;
    }

    if (password.isEmpty()) {
        showError("密码不能为空!");
        passwordLineEdit->setFocus();
        highlightError(passwordLineEdit);
        return false;
    }

    if (confirmPassword.isEmpty()) {
        showError("请确认密码!");
        confirmPasswordLineEdit->setFocus();
        highlightError(confirmPasswordLineEdit);
        return false;
    }

    if (password != confirmPassword) {
        showError("两次输入的密码不一致!");
        passwordLineEdit->selectAll();
        passwordLineEdit->setFocus();
        highlightError(passwordLineEdit);
        highlightError(confirmPasswordLineEdit);
        return false;
    }

    if (password.length() < 6) {
        showError("密码长度不能少于6位!");
        passwordLineEdit->setFocus();
        passwordLineEdit->selectAll();
        highlightError(passwordLineEdit);
        return false;
    }

    return true;
}

void RegisterWidget::showError(const QString &message)
{
    errorLabel->setText(message);
    errorLabel->show();
    // 添加动画效果使错误信息更明显 (调整颜色)
    flashErrorLabel(errorLabel);
    errorLabel->setFixedHeight(20);
}

void RegisterWidget::clearErrors()
{
    errorLabel->hide();// 清除错误信息
    errorLabel->setFixedHeight(20);// 设置错误信息高度
}

void RegisterWidget::highlightError(QLineEdit* lineEdit) {
    // 保存原始调色板
    QString errorStyleSheet =
    "QLineEdit {"
    "height: 40px;"
    "padding: 0 15px 0 35px;"
    "border: 1px solid rgba(255, 255, 255, 40%);"
    "border-radius: 20px;"
    "background-color: rgba(255, 255, 255, 10%);"
    "color: white;"
    "font-family: 'Microsoft YaHei', 'SimHei';"
    "font-size: 14px;"
    "}"
    "QLineEdit:focus {"
    "border: 1px solid rgba(243, 33, 33, 1);"
    "background-color: rgba(255, 255, 255, 20%);"
    "}";

    int flashCount = 0;
    const int totalFlashes = 4; // 闪两次 = 变红 → 白 → 红 → 白

    QTimer *timer = new QTimer(lineEdit);
    QObject::connect(timer, &QTimer::timeout, [=]()mutable  {
        if (flashCount % 2 == 0) {
            lineEdit->setStyleSheet(errorStyleSheet);   // 红色背景
        } else {
            lineEdit->setStyleSheet(originalStyleSheet); // 恢复正常
        }
        flashCount++;
        if (flashCount >= totalFlashes) {
            timer->stop();
            lineEdit->setStyleSheet(originalStyleSheet); // 确保最终恢复
            delete timer;
        }
    });
    timer->start(250); // 每250ms切换一次，总共约1秒完成2次闪烁
}

void RegisterWidget::flashErrorLabel(QLabel* label, int loopCount, int durationPerLoop ) {
    if (!label) return;
    // 保存原始样式
    static QHash<QLabel*, QString> originalStyles;
    if (!originalStyles.contains(label)) {
        originalStyles[label] = label->styleSheet();
    }

    QString style1 = "QLabel { color: rgba(55, 0, 255, 0.57); font-family: 'KaiTi', 'SimKai'; font-size: 14px; font-weight: bold; }";
    QString style2 = "QLabel { color: rgba(255, 0, 0, 1); font-family: 'KaiTi', 'SimKai'; font-size: 14px; font-weight: bold; }";

    int totalSteps = loopCount * 2; // 每次循环：style1 → style2
    int step = 0;

    QTimer* timer = new QTimer(label);
    int interval = durationPerLoop / 2; // 每 half-loop 切换一次

    QObject::connect(timer, &QTimer::timeout, [=]() mutable {
        if (step % 2 == 0) {
            label->setStyleSheet(style1);
        } else {
            label->setStyleSheet(style2);
        }
        step++;
        if (step >= totalSteps) {
            timer->stop();
            label->setStyleSheet(originalStyles[label]); // 恢复原始样式
        }
    });

    timer->start(interval);
}