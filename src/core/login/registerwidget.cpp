#include <QVBoxLayout>                  // 垂直布局管理器，用于垂直排列控件
#include <QHBoxLayout>                  // 水平布局管理器，用于水平排列控件
#include <QFormLayout>                  // 表单布局管理器，用于创建标签-输入字段的表单
#include <QMessageBox>                  // 消息对话框，用于显示提示信息
#include <QSqlQuery>                    // SQL查询
#include <QSqlError>                    // SQL错误处理
#include <QGraphicsEffect>              // 图形效果
#include <QFileDialog>                  // 选择文件对话框
#include <QPropertyAnimation>           // 用于动画效果
#include <QRandomGenerator>             // 用于生成随机数
#include <QEvent>                       // 用于处理事件
#include <QTimer>                       // 定时器
#include <QPainter>                     // 绘图
#include <QGraphicsDropShadowEffect>    // 添加阴影效果支持
#include <QRegularExpression>           // 正则表达式
#include <QThread>                      // 线程，用于延迟关闭对话框

#include "registerwidget.h"
#include "src/custom/clickablelabel.h"
#include "src/thread/network/networkmanager.h"

RegisterWidget::RegisterWidget(QWidget *parent, NetworkManager *networkManager)
    : QWidget(parent), networkManager(networkManager)
{
    initUI();
    connectUISignals();
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
    container->setGeometry(150, 40, 410, 420); // 调整高度以容纳新的输入框

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

    // 创建头像提示标签
    avatarTipLabel = new ClickableLabel(container);
    avatarTipLabel->setObjectName("avatarTipLabel"); // 设置对象名以便后续引用
    avatarTipLabel->setText("点击选择头像");
    avatarTipLabel->setGeometry(150, 0, 80, 80); // 与头像按钮大小和位置一致
    avatarTipLabel->setAlignment(Qt::AlignCenter); // 文字居中
    
    // 设置样式表：半透明黑色背景，白色文字，圆角
    QString tipStyle = "QLabel#avatarTipLabel {"
                       "background-color: rgba(0, 0, 0, 100);"
                       "color: white;"
                       "font-size: 12px;"
                       "font-weight: bold;"
                       "font-family: 'Kaiti';"
                       "}";
    avatarTipLabel->setStyleSheet(tipStyle);
    
    // 确保标签在最上层
    avatarTipLabel->raise();


    // 用户图标
    QLabel *iconLabel = new QLabel(container);
    QPixmap userPixmap(":/images/icon_user.png");
    userPixmap = userPixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconLabel->setPixmap(userPixmap);
    iconLabel->setGeometry(40, 104, 24, 24);

    // 用户ID输入框
    userIDLineEdit = new QLineEdit(container);
    userIDLineEdit->setPlaceholderText("请输入用户ID");
    QString lineEditStyle =
        "QLineEdit {"
        "height: 40px;"
        "padding: 0 15px 0 35px;"
        "border: 2px solid rgba(255, 255, 255, 0.4);"
        "border-radius: 10px;"
        "background-color: rgba(255, 255, 255, 0.1);"
        "color: #000000;"
        "font-family: 'Kaiti';"
        "font-size: 17px;"
        "}"
        "QLineEdit:focus {"
        "border: 2px solid rgba(255, 255, 255, 0.8);"
        "background-color: rgba(255, 255, 255, 0.2);"
        "}";
    originalStyleSheet=lineEditStyle;
    userIDLineEdit->setStyleSheet(lineEditStyle);
    userIDLineEdit->setGeometry(30, 97, 340, 40);

    // 昵称图标
    QLabel *nickIconLabel = new QLabel(container);
    QPixmap nickPixmap(":/images/icon_user.png"); // 使用相同的用户图标
    nickPixmap = nickPixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    nickIconLabel->setPixmap(nickPixmap);
    nickIconLabel->setGeometry(40, 157, 24, 24);

    // 用户昵称输入框
    userNickLineEdit = new QLineEdit(container);
    userNickLineEdit->setPlaceholderText("请输入用户昵称");
    userNickLineEdit->setStyleSheet(lineEditStyle);
    userNickLineEdit->setGeometry(30, 150, 340, 40);

    // 密码图标
    QLabel *passwordIconLabel = new QLabel(container);
    QPixmap passwordPixmap(":/images/icon_password.png");
    passwordPixmap = passwordPixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    passwordIconLabel->setPixmap(passwordPixmap);
    passwordIconLabel->setGeometry(40, 212, 24, 24);

    // 密码输入框
    passwordLineEdit = new QLineEdit(container);
    passwordLineEdit->setPlaceholderText("请输入密码");
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setStyleSheet(lineEditStyle);
    passwordLineEdit->setGeometry(30, 205, 340, 40);

    // 确认密码图标
    QLabel *confirmPasswordIconLabel = new QLabel(container);
    QPixmap confirmPasswordPixmap(":/images/icon_password.png");
    confirmPasswordPixmap = confirmPasswordPixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    confirmPasswordIconLabel->setPixmap(confirmPasswordPixmap);
    confirmPasswordIconLabel->setGeometry(40, 267, 24, 24);

    // 确认密码输入框
    confirmPasswordLineEdit = new QLineEdit(container);
    confirmPasswordLineEdit->setPlaceholderText("请确认密码");
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordLineEdit->setStyleSheet(lineEditStyle);
    confirmPasswordLineEdit->setGeometry(30, 260, 340, 40);
    showPasswordButtonStyle =(
    "QCheckBox {"
    "    font: 14px 'KaiTi', '楷体';"
    "    color: rgba(0, 0, 0, 0.8);"
    "    spacing: 8px;"
    "}"
    "QCheckBox:hover {"
    "    color:  rgba(13, 134, 255, 1);"
    "}"
    "QCheckBox::indicator {"
    "    width: 14px;"
    "    height: 14px;"
    "    border-radius: 9px;"
    "    border: 2px solid #ccc;"
    "    background: white;"
    "}"
    "QCheckBox::indicator:hover {"
    "    background: rgba(13, 134, 255, 0.68);"
    "    border-color: rgba(13, 134, 255, 1);"
    "}"
    "QCheckBox::indicator:checked {"
    "    border: 2px solid rgba(112, 112, 112, 1);"
    "    image: url(:/images/icon_check.png);"
    "}"
    "QCheckBox::indicator:checked:hover {"
    "    border: 2px solid rgba(112, 112, 112, 1);"
    "    image: url(:/images/icon_check.png);"
    "}");
    // 显示密码复选框
    showPasswordCheckBox = new QCheckBox("显示密码", container);
    showPasswordCheckBox->setGeometry(50, 305, 100, 20);
    showPasswordCheckBox->setStyleSheet(showPasswordButtonStyle);
    
    // 注册按钮
    registerButton = new QPushButton("注册", container);
    QString registerButtonStyle =
        "QPushButton {"
        "background-color: #2196F3;"
        "color: white;"
        "padding: 10px 28px;"
        "border-radius: 20px;"
        "font-family: 'Kaiti';"
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
    registerButton->setGeometry(30, 345, 160, 40);
    
    // 返回登录按钮
    backButton = new QPushButton("返回", container);
    QString backButtonStyle =
        "QPushButton {"
        "background-color: transparent;"
        "color: rgba(250, 250, 250, 1);"
        "padding: 10px 28px;"
        "border-radius: 20px;"
        "font-family: 'Kaiti';"
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
    backButton->setGeometry(210, 345, 160, 40);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(20);//模糊半径
    shadow->setColor(QColor(0, 0, 0, 80));//颜色
    shadow->setOffset(0, 5);//偏移
    backButton->setGraphicsEffect(shadow);
    
    // 错误提示标签
    errorLabel = new QLabel(container);
    QString errorLabelStyle ="QLabel {"
    "color: rgba(255, 0, 0, 0.58); "
    "font-family: 'KaiTi', 'SimKai'; "
    "font-size: 14px; "
    "font-weight: bold;"
     "}";
    errorLabel->setStyleSheet(errorLabelStyle);
    errorLabel->setGeometry(230, 305, 340, 20);
    errorLabel->hide();
}


void RegisterWidget::setBackground()
{
    // 加载并缩放背景图以适应窗口大小
    QPixmap bg(":images/8.png");
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

void RegisterWidget::connectUISignals()
{
    connect(registerButton, &QPushButton::clicked, this, &RegisterWidget::onRegisterClicked);
    connect(showPasswordCheckBox, &QCheckBox::toggled, this, [=](bool checked) {
        passwordLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
        confirmPasswordLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    });
    connect(passwordLineEdit, &QLineEdit::textChanged, this, &RegisterWidget::onPasswordChanged);
    connect(avatarTipLabel, &ClickableLabel::clicked, this, &RegisterWidget::onSelectAvatar);
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
    qDebug() << "注册按钮点击事件触发";
    if (!validateInput()) // 验证输入，如果验证失败，则返回
    {
        return false;
    }
    // 使用队列连接确保在网络线程中执行
    QMetaObject::invokeMethod(networkManager, &NetworkManager::connectToServer, Qt::QueuedConnection);
    if(!networkManager->isConnected())// 检查网络连接状态
    {
        showError("服务器连接失败！");// 显示错误信息
        return false;
    }

    QDialog registerDialog(this);//创建栈对话框
    registerDialog.setWindowTitle("用户注册");
    registerDialog.setModal(true);//设置为模态对话框，用户必须先关闭对话框才能继续操作其他窗口
    registerDialog.setFixedSize(150, 80);
    // 创建标签并设置初始文本
    QLabel registerLabel("注册中.", &registerDialog);
    registerLabel.setGeometry(30, 30, 140, 20);// 设置标签位置和大小，左上角坐标为(30,30)，宽度为140，高度为20
    registerLabel.setAlignment(Qt::AlignLeft);// 左对齐
    registerLabel.setFont(QFont("KaiTi", 12, QFont::Bold));//设字体大小为12px，加粗
    // 点数计数器（初始为1，因为初始文本已包含一个点）
    int dotCount = 1;
    // 创建定时器
    QTimer timer(&registerDialog);
    timer.setInterval(400); // 400毫秒间隔
    // 连接定时器信号
    connect(&timer, &QTimer::timeout, &registerDialog, [&]() mutable {
        // 递增点数
        dotCount++;
        //qDebug()<<"dotCount:"<<dotCount;
        QString newText = "注册中" + QString(dotCount, '.');
        registerLabel.setText(newText);
        if (dotCount >=3) dotCount = 0; // 重置到1个点
    });
    timer.start();// 启动定时器
 
    QString userID = userIDLineEdit->text();
    QString userNick = userNickLineEdit->text(); // 获取用户昵称
    QString password = passwordLineEdit->text();
    qDebug()<<"   ID:"<<userID;
    qDebug()<<" Nickname:"<<userNick;
    qDebug()<<"password:"<<password;
    bool isSuccess = false;

    //显示定时器,显示结果1s后关闭对话框
    QTimer delayTimer(&registerDialog);
    delayTimer.setSingleShot(true);// 单次触发，注册成功或失败后关闭对话框
    // 注册成功定时器
    QTimer delaySuccTimer(&registerDialog);
    delaySuccTimer.setSingleShot(true);// 单次触发，注册成功或失败后关闭对话框
    // 注册失败定时器
    QTimer delayFailTimer(&registerDialog);
    delayFailTimer.setSingleShot(true);// 单次触发，注册成功或失败后关闭对话框

    //接收注册信号
    // 注册成功信号，触发注册成功定时器，让注册中显示2000毫秒
    connect(networkManager, &NetworkManager::registerSuccess, &registerDialog, [&]() {
        qDebug()<<"注册成功！";
        isSuccess = true;
        delaySuccTimer.start(2000); // 让注册中显示2000毫秒
    }, Qt::QueuedConnection);
    // 注册失败信号，触发注册失败定时器，让注册中显示2000毫秒
    connect(networkManager, &NetworkManager::registerFailed, &registerDialog, [&](QString errorString) {
        timer.stop();// 停止定时器
        registerDialog.close();// 关闭对话框
        qDebug()<<"注册失败!"<<errorString;// 显示注册失败信息
        showError(errorString);// 显示错误信息
        isSuccess = false;
        delayFailTimer.start(2000); // 让注册中显示2000毫秒
    }, Qt::QueuedConnection);
    
    // 注册成功定时器信号，显示注册成功结果1s后关闭对话框
    connect(&delaySuccTimer, &QTimer::timeout, &registerDialog, [&]() {
        timer.stop();// 停止定时器
        registerLabel.setText("注册成功！");
        qDebug()<<"成功显示";
        delayTimer.start(1000); // 显示注册成功1000毫秒
    });
    // 注册失败定时器信号，显示注册失败结果1s后关闭对话框
    connect(&delayFailTimer, &QTimer::timeout, &registerDialog, [&]() {
        timer.stop();// 停止定时器
        registerLabel.setText("注册失败！");// 显示注册失败信息
        qDebug()<<"失败显示";
        delayTimer.start(1000); // 显示注册失败1000毫秒
    });
    // 注册信号定时器信号，显示注册结果1s后关闭对话框
    connect(&delayTimer, &QTimer::timeout, &registerDialog, [&]() {
        registerDialog.close();// 关闭对话框
    });

    // 调用网络管理器注册用户,用信号槽传入参数到networkmanager
    QMetaObject::invokeMethod(networkManager, "registerUser",  Qt::QueuedConnection,
                              Q_ARG(QString, userID),
                              Q_ARG(QString, userNick),
                              Q_ARG(QString, password));// 注册用户，传递用户ID、昵称、密码
    registerDialog.exec();// 显示对话框，等待用户操作
    if(!isSuccess)return false;
    
    // 保存用户ID以便返回给调用者
    this->userID = userID;
    userIDLineEdit->clear();
    userNickLineEdit->clear(); // 清空昵称输入框
    passwordLineEdit->clear();
    confirmPasswordLineEdit->clear();
    showPasswordCheckBox->setChecked(false);
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
            QPixmap scaledPixmap = pixmap.scaled(80, 80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            // 保存用户头像到本地文件images/avatar/userID.png
            QString savePath = QDir::currentPath() + QString("/images/avatar/%1.png").arg(userID);
            scaledPixmap.save(savePath);
            // 设置按钮的图标为缩放后的图片
            avatarButton->setIcon(QIcon(scaledPixmap));
            // 设置按钮图标的显示大小为80x80
            avatarButton->setIconSize(QSize(80, 80));
        }
    }
    if(fileName.isEmpty())return;
    avatarTipLabel->hide();//隐藏提示
    avatarPath=fileName;
}

bool RegisterWidget::validateInput()// 验证输入
{
    clearErrors();// 先清除错误标签以及信息

    QString userID = userIDLineEdit->text().trimmed();// 获取用户ID,去除空格
    QString userNick = userNickLineEdit->text().trimmed(); // 获取用户昵称，去除空格
    QString password = passwordLineEdit->text();
    QString confirmPassword = confirmPasswordLineEdit->text();

    if (userID.isEmpty()) {
        showError("用户ID不能为空!");
        userIDLineEdit->setFocus();// 设置焦点
        lineEditHighlight(userIDLineEdit);
        return false;
    }

    if (userNick.isEmpty()) {
        showError("用户昵称不能为空!");
        userNickLineEdit->setFocus();// 设置焦点
        lineEditHighlight(userNickLineEdit);   
        return false;
    }

    QRegularExpression re("[^0-9]"); // 用户ID只能是数字
    //用户ID只能是数字
    if (userID.contains(re)) {
        showError("用户ID只能是数字!");
        userIDLineEdit->setFocus();
        lineEditHighlight(userIDLineEdit);
        return false;
    }
    
    if (password.isEmpty()) {
        showError("密码不能为空!");
        passwordLineEdit->setFocus();
        lineEditHighlight(passwordLineEdit);
        return false;
    }

    if (confirmPassword.isEmpty()) {
        showError("请确认密码!");
        confirmPasswordLineEdit->setFocus();
        lineEditHighlight(confirmPasswordLineEdit);
        return false;
    }

    if (password != confirmPassword) {
        showError("两次输入的密码不一致!");
        lineEditHighlight(passwordLineEdit);
        lineEditHighlight(confirmPasswordLineEdit);
        return false;
    }

    if (password.length() < 6) {
        showError("密码长度不能少于6位!");
        passwordLineEdit->setFocus();
        lineEditHighlight(passwordLineEdit);
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

void RegisterWidget::lineEditHighlight(QLineEdit* lineEdit) {
    // 保存原始调色板
    QString errorStyleSheet =
    "QLineEdit {"
    "height: 40px;"
    "padding: 0 15px 0 35px;"
    "border: 2px solid rgba(255, 14, 14, 0.66);"
    "border-radius: 10px;"
    "background-color: rgba(255, 255, 255, 0.1);"
    "color: white;"
    "font-family: 'Kaiti';"
    "font-size: 17px;"
    "}"
    "QLineEdit:focus {"
    "border: 2px solid rgba(243, 33, 33, 1);"
    "background-color: rgba(255, 255, 255, 0.2);"
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