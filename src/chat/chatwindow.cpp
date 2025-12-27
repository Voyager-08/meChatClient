#include <QTimer>
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include "ui_chatwindow.h"

//自定义头文件
#include "chatwindow.h"
#include "src/contact/contactmodel.h"
#include "src/contact/contactdelegate.h"
#include "src/sql/database.h"
#include "networkmanager.h"
#include "messagebubble.h"
#include "src/login/loginwindow.h"


struct Friend{ //好友备注，好友昵称，好友ID，头像路径，信息
    QString friendNote; //  好友备注，用于唯一标识用户
    QString friendNick; //  好友昵称，用于显示用户昵称
    QString friendID; //  好友ID，用于唯一标识好友
    QString avatarPath; //  头像路径，存储用户头像文件的路径
    QString message; //  用户个性签名或留言
};

struct Message{ //消息结构体
    MessageBubble::Role role;  // 消息角色（自己/他人）
    QString text;              // 消息文本
    QDateTime time;            // 消息时间
};

ChatWindow::ChatWindow(QString userID,QWidget *parent) : QWidget(parent)
    , ui(new Ui::ChatWindow)
{
    initialUserInfo(userID); // 初始化用户信息
    ui->setupUi(this);
    initUI();
    connectSignals();
    showContact();
    linkServer();
    // 初始化消息数据映射表
    messageDataMap.clear();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::initUI()// 初始化界面
{
    messageModel= new ContactModel(this);
    QIcon windowIcon(":/images/10.png");
    setWindowIcon(windowIcon);
    // 设置无边框，实现自定义标题栏和窗口边框以及拉伸窗口
    setMouseTracking(true); //允许无按键时触发 mouseMoveEvent
    // 允许最大化按钮
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint);
    ui->rightBox->setStretchFactor(0, 0); // contactList：不拉伸
    ui->rightBox->setStretchFactor(1, 1); // frame：可以拉伸（获得所有额外空间）
    setWindowFlags(Qt::FramelessWindowHint);// 去掉标题栏
    setMinimumSize(700, 600); // 设置窗口最小尺寸

    // 设置聊天记录区域和输入框区域的高度比例
    QList<int> sizes;
    sizes << 400 << 100;  // 设置聊天记录区域和输入框区域的比例
    ui->information->setSizes(sizes);
    
    // 设置顶部菜单栏
    ui->closeButton->setToolTip("关闭");
    ui->closeButton->setText("");
    ui->minimizeButton->setToolTip("最小化");
    ui->minimizeButton->setText("");
    ui->maximizeButton->setToolTip("恢复");// 设置按钮提示
    ui->maximizeButton->setText("");
    ui->closeButton->setIcon(QIcon(":/images/icon10.png"));
    ui->closeButton->setIconSize(QSize(30, 30));
    ui->minimizeButton->setIcon(QIcon(":/images/icon6.png"));
    ui->minimizeButton->setIconSize(QSize(30, 30));
    ui->maximizeButton->setIcon(QIcon(":/images/icon7.png"));
    ui->maximizeButton->setIconSize(QSize(30, 30));
    
    // 设置占位符图片并使其铺满整个可用区域
    QPixmap placeholderPixmap(":/images/1.png");
    ui->placeholderLabel->setPixmap(placeholderPixmap);
    ui->placeholderLabel->setScaledContents(true); // 使图片铺满标签区域
    ui->placeholderLabel->setAlignment(Qt::AlignCenter); // 居中显示图片
    ui->placeholderLabel->setFixedSize(100, 100);
    
    // 美化左侧导航栏
    QString leftBoxStyle = R"(
        QFrame {
            background-color: rgba(240, 240, 240, 180); /* 半透明背景以显示背景图 */
            border: none;
        }
    )";
    ui->leftBox->setStyleSheet(leftBoxStyle);
    
    // 美化左侧导航按钮 - 添加动画效果
    QString navButtonStyle = R"(
        QPushButton {
            background-color: transparent;
            border: none;
            color: #333;
            font-family: "Microsoft YaHei", "微软雅黑";
            font-size: 14px;
            padding: 10px;
            text-align: center;
            border-radius: 6px;
        }
        QPushButton:hover {
            background-color: rgba(224, 224, 224, 150); /* 半透明悬停效果 */
        }
        QPushButton:pressed {
            background-color: rgba(208, 208, 208, 150); /* 半透明按下效果 */
        }
    )";
    
    // 为导航按钮设置图标和样式
    ui->message->setStyleSheet(navButtonStyle);
    ui->message->setIcon(QIcon("D:\\cxdownload\\夸克文件下载\\13.png"));
    ui->message->setIconSize(QSize(32, 32));
    ui->message->setText("");// 只显示图标不显示文字
    ui->message->setToolTip("消息");
    
    ui->contact->setStyleSheet(navButtonStyle);
    ui->contact->setIcon(QIcon("D:\\cxdownload\\夸克文件下载\\16.png"));
    ui->contact->setIconSize(QSize(32, 32));
    ui->contact->setText("");
    ui->contact->setToolTip("联系人");
    
    ui->collect->setStyleSheet(navButtonStyle);
    ui->collect->setIcon(QIcon("D:\\cxdownload\\夸克文件下载\\11.png"));
    ui->collect->setIconSize(QSize(32, 32));
    ui->collect->setText("");
    ui->collect->setToolTip("收藏");
    
    ui->moments->setStyleSheet(navButtonStyle);
    ui->moments->setIcon(QIcon("D:\\cxdownload\\夸克文件下载\\15.png"));
    ui->moments->setIconSize(QSize(32, 32));
    ui->moments->setText("");
    ui->moments->setToolTip("朋友圈");
    
    ui->search->setStyleSheet(navButtonStyle);
    ui->search->setIcon(QIcon("D:\\cxdownload\\夸克文件下载\\12.png"));
    ui->search->setIconSize(QSize(32, 32));
    ui->search->setText("");
    ui->search->setToolTip("搜索");
    
    ui->more->setStyleSheet(navButtonStyle);
    ui->more->setIcon(QIcon(":/images/icon4.png"));
    ui->more->setIconSize(QSize(32, 32));
    ui->more->setText("");
    ui->more->setToolTip("更多");
    
    // 美化联系人列表
    QString contactListStyle = R"(
        QListView {
            background-color: rgba(250, 250, 250, 180); /* 半透明背景以显示背景图 */
            border: none;
            border-right: 1px solid #e0e0e0;
        }
        QListView::item {
            padding: 5px;
            border-radius: 4px;
        }
        QListView::item:hover {
            background-color: rgba(224, 224, 224, 150);
        }
        QListView::item:selected {
            background-color: rgba(0, 120, 212, 100);
        }
    )";
    ui->contactListView->setStyleSheet(contactListStyle);
    
    // 默认显示占位符页面
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
    

    // 美化聊天区域
    QString frameStyle = R"(
        QFrame {
            background-color: rgba(255, 255, 255, 200); /* 半透明背景以显示背景图 */
            border: none;
        }
        QScrollArea {
            border: none;
        }
        QWidget#messageContainer {
            background-color: transparent;
        }
    )";
    ui->chatPage->setStyleSheet(frameStyle);
    
    // 美化联系人栏
    QString contactBarStyle = R"(
        QFrame {
            background-color: rgba(248, 248, 248, 180); /* 半透明背景以显示背景图 */
            border: none;
            border-bottom: 1px solid #e0e0e0;
        }
    )";
    ui->contactBar->setStyleSheet(contactBarStyle);
    
    // 美化联系人栏按钮
    QString contactBarButtonStyle = R"(
        QPushButton {
            background-color: transparent;
            border: none;
            color: #333;
            font-family: "Microsoft YaHei", "微软雅黑";
            font-size: 14px;
            padding: 5px 10px;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: rgba(224, 224, 224, 150); /* 半透明悬停效果 */
        }
    )";
    ui->nickname->setStyleSheet(contactBarButtonStyle);
    ui->moreInformation->setStyleSheet(contactBarButtonStyle);
    
    // 美化消息区域
    QString messageAreaStyle = R"(
        QScrollArea {
            border: none;
            background-color: transparent; /* 透明背景以显示聊天背景图 */
        }
        QWidget {
            background-color: transparent; /* 透明背景以显示聊天背景图 */
        }
    )";
    ui->messageArea->setStyleSheet(messageAreaStyle);
    
    // 美化输入框
    QString inputBoxStyle = R"(
        QPlainTextEdit {
            border: 1px solid #e0e0e0;
            border-radius: 8px;
            padding: 10px;
            font-family: "Microsoft YaHei", "微软雅黑";
            font-size: 14px;
            background-color: rgba(250, 250, 250, 200); /* 半透明背景以显示背景图 */
        }
        QPlainTextEdit:focus {
            border: 1px solid #0078d4;
            background-color: rgba(255, 255, 255, 200); /* 半透明背景以显示背景图 */
        }
    )";
    ui->inputBox->setStyleSheet(inputBoxStyle);

    // 美化发送按钮
    QString sentButtonStyle = R"(
        QPushButton {
            background-color: #0078d4;
            border: none;
            color: white;
            font-family: "Microsoft YaHei", "微软雅黑";
            font-size: 14px;
            font-weight: bold;
            padding: 8px 20px;
            border-radius: 6px;
        }
        QPushButton:hover {
            background-color: #005a9e;
            
        }
        QPushButton:pressed {
            background-color: #004080;
            
        }
    )";
    ui->sent->setStyleSheet(sentButtonStyle);
    
    // 美化占位符标签
    QString placeholderStyle = R"(
        QLabel {
            background-color: transparent; /* 透明背景以显示聊天背景图 */
            color: #808080;
            font-family: "Microsoft YaHei", "微软雅黑";
        }
    )";
    ui->placeholderLabel->setStyleSheet(placeholderStyle);

}

void ChatWindow::connectSignals()// 连接信号槽
{
    // 连接发送按钮
    connect(ui->sent, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    // 最小化
    connect(ui->minimizeButton, &QPushButton::clicked, this, &QWidget::showMinimized);
    // 最大化
    connect(ui->maximizeButton, &QPushButton::clicked, this,&ChatWindow::showMaximize);
    connect(ui->closeButton, &QPushButton::clicked, this, &ChatWindow::close);
    // 当点击消息按钮时显示聊天页面
    connect(ui->message, &QPushButton::clicked,[=](){
        animatePageTransition(ui->messagePage);
        ui->stackedWidget->setCurrentIndex(0);
    });
    // 当点击联系人按钮时显示联系人页面
    connect(ui->contact, &QPushButton::clicked,[=](){
        animatePageTransition(ui->contactPage);
        ui->stackedWidget->setCurrentIndex(1);
    });
    // 当点击设置按钮时显示收藏页面
    connect(ui->collect, &QPushButton::clicked,[=](){
        animatePageTransition(ui->collectPage);
        ui->stackedWidget->setCurrentIndex(2);
    });
    // 当点击朋友圈按钮时显示朋友圈页面
    connect(ui->moments, &QPushButton::clicked,[=](){
        animatePageTransition(ui->momentsPage);
        ui->stackedWidget->setCurrentIndex(3);
    });
    //点击发现按钮时显示搜索页面
    connect(ui->search, &QPushButton::clicked,[=](){
        animatePageTransition(ui->searchPage);
        ui->stackedWidget->setCurrentIndex(4);
    });
    
    // 连接联系人列表点击事件
    connect(ui->contactListView, &QListView::clicked, this, &ChatWindow::onContactClicked);
    // 连接消息列表点击事件
    connect(ui->messageListView, &QListView::clicked, this, &ChatWindow::onMessageClicked);
    // 连接输入框的回车键发送消息信号
    connect(ui->inputBox, &MePlainTextEdit::enterPressed, this, &ChatWindow::sendMessage);
}

void ChatWindow::initialUserInfo(QString userID)
{
    Database *db = new Database();
    
    if (!db->connect("chat_connection"))
    {
        qDebug() << "Failed to connect to database in ChatWindow";
        return;
    }

    QSqlQuery query(db->database());
    query.prepare("SELECT * FROM users WHERE user_id = ?");
    query.addBindValue(userID);
    if (!query.exec())// 执行查询
    {
        qDebug() << "查询用户信息失败";
        return;
    }
    if (query.next())// 获取用户信息
    {
        userInfo.userID = query.value("user_id").toString();
        userInfo.userNick = query.value("user_nick").toString();
        userInfo.userPassword = query.value("password").toString();
        userInfo.userEmail = query.value("email").toString();
        userInfo.userRegistrationDate = query.value("created_at").toDate();
        userInfo.userMotto= query.value("motto").toString();
        userInfo.userAvatar = query.value("avatar_path").toString();
        userInfo.userSex = static_cast<Sex>(query.value("sex").toInt());
    }
    //输出用户信息
    qDebug() << "用户ID：" << userInfo.userID ;
    qDebug()<< "用户密码：" << userInfo.userPassword ;
    qDebug()<< "用户昵称：" << userInfo.userNick ;
    qDebug()<< "用户邮箱：" << userInfo.userEmail  ;
    qDebug()<< "用户注册时间：" << userInfo.userRegistrationDate  ;
    qDebug()<< "用户个性签名：" << userInfo.userMotto  ;
    qDebug()<< "用户头像：" << userInfo.userAvatar ;
    qDebug()<< "用户性别：" << toStringSex(userInfo.userSex);
}

QString ChatWindow::toStringSex(Sex s)
{
    switch (s) {
    case Sex::female:
        return "女";
    case Sex::male:
        return "男";
    case Sex::other:
        return "其他";
    default:
        return "未知";
    }
}

void ChatWindow::animatePageTransition(QWidget *widget)
{
    // 创建页面切换动画
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "geometry");
    animation->setDuration(300); // 300ms 动画时间
    animation->setEasingCurve(QEasingCurve::OutBack); // 使用缓动曲线使动画更自然
    
    // 从右侧滑入效果
    QRect startGeometry = widget->geometry();
    QRect endGeometry = widget->geometry();
    startGeometry.moveLeft(widget->parentWidget()->width()); // 起始位置在屏幕右侧
    widget->setGeometry(startGeometry);
    
    animation->setStartValue(startGeometry);
    animation->setEndValue(endGeometry);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ChatWindow::mousePressEvent(QMouseEvent *event)// 鼠标按下事件
{
    if (event->button() == Qt::LeftButton) //鼠标点击左键
    {
        isLeftButtonPressed = true;
        // 检查是否在边缘区域点击，如果是则准备调整窗口大小
        resizeMode = getResizeMode(event->pos());
        
        // 只有在顶部标题栏区域才允许拖动窗口 (标题栏高度大约为40像素)
        QRect titleBarRect(0, 0, width(), 40);
        bool inTitleBar = titleBarRect.contains(event->pos());// 检查鼠标位置是否在标题栏内
        
        // 如果不是在调整窗口大小模式，并且在标题栏内，也不是在关闭按钮上，则允许拖动
        if (resizeMode == ResizeMode::None && inTitleBar 
            && !ui->closeButton->geometry().contains(event->pos())
            && !ui->minimizeButton->geometry().contains(event->pos())
            && !ui->maximizeButton->geometry().contains(event->pos()))
        {
            qDebug()<<"移动窗口";
            dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            isDragging = true;
            event->accept();
        }
        else if (resizeMode != ResizeMode::None)
        {
            qDebug()<<"调整窗口大小";
            isDragging = false;
            // 在调整窗口大小模式下接受事件
            event->accept();
        }
        else
        {
            qDebug()<<"其他情况";
            isDragging = false;
            // 在其他情况下，拒绝事件
            event->ignore();
        }
    }
    QWidget::mousePressEvent(event);// 调用基类实现鼠标按下事件
}

void ChatWindow::mouseMoveEvent(QMouseEvent *event)// 鼠标移动事件
{
    if (resizeMode != ResizeMode::None) 
    {
        // 如果处于调整窗口大小模式，则调整窗口
        resizeWindow(event->globalPosition().toPoint());
        event->accept();
    }
    else if (event->buttons() & Qt::LeftButton && isDragging) 
    {
        // 如果处于移动窗口模式，则移动窗口
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
    else if(!isLeftButtonPressed)updateCursor(getResizeMode(event->pos()));
    // 否则更新鼠标光标样式
    QWidget::mouseMoveEvent(event);
}

void ChatWindow::mouseReleaseEvent(QMouseEvent *event)// 鼠标释放事件
{
    resizeMode = ResizeMode::None; //  设置调整模式为无调整
    isLeftButtonPressed = false; //  设置鼠标左键未按下
    // 只有在非调整大小模式下才取消光标设置
    if (resizeMode == ResizeMode::None) {
        unsetCursor(); //  取消鼠标光标设置
    }
    QWidget::mouseReleaseEvent(event); //  调用父类QWidget的鼠标释放事件处理函数
}

void ChatWindow::leaveEvent(QEvent *event)// 鼠标离开事件
{
    // 鼠标离开窗口时恢复默认光标
    unsetCursor();
    QWidget::leaveEvent(event);
}

// 获取当前鼠标所在的边缘位置
ResizeMode ChatWindow::getResizeMode(const QPoint &pos)// 根据鼠标位置获取调整模式
{
    QRect rect = this->rect();// 获取窗口矩形区域
    /* 检查鼠标是否在窗口边缘区域
    在 Qt 的坐标系统中，鼠标（以及所有 widget）的坐标原点 (0, 0) 默认位于左上角，并且：
    X 轴向右增大（→）
    Y 轴向下增大（↓）
    这是 Qt 遵循的 标准屏幕坐标系，与大多数 GUI 框架一致。*/
    if (pos.x() <= borderWidth && pos.y() <= borderWidth)// 在左上角
        return ResizeMode::TopLeft;
    else if (pos.x() >= rect.width() - borderWidth && pos.y() <= borderWidth)// 在右上角
        return ResizeMode::TopRight;
    else if (pos.x() <= borderWidth && pos.y() >= rect.height() - borderWidth)// 在左下角
        return ResizeMode::BottomLeft;
    else if (pos.x() >= rect.width() - borderWidth && pos.y() >= rect.height() - borderWidth)// 在右下角
        return ResizeMode::BottomRight;
    else if (pos.x() <= borderWidth)// 在左边缘
        return ResizeMode::Left;
    else if (pos.x() >= rect.width() - borderWidth)// 在右边缘
        return ResizeMode::Right;
    else if (pos.y() <= borderWidth)// 在上边缘
        return ResizeMode::Top;
    else if (pos.y() >= rect.height() - borderWidth)//  在下边缘
        return ResizeMode::Bottom;
        
    return ResizeMode::None;
}
void ChatWindow::resizeWindow(const QPoint &globalPos)// 根据鼠标位置调整窗口大小
{
    QRect rect = frameGeometry();// 获取窗口矩形区域逻辑窗口范围
    QPoint topLeft = rect.topLeft();// 获取左上角全局坐标
    QPoint bottomRight = rect.bottomRight();// 获取右下角全局坐标

    switch (resizeMode) {// 根据调整模式修改窗口矩形区域
    case ResizeMode::Left:// 左边缘
        rect.setLeft(globalPos.x());      // 修改rect获得的窗口左边缘位置
        break;
    case ResizeMode::Right:
        rect.setRight(globalPos.x());
        break;
    case ResizeMode::Top:
        rect.setTop(globalPos.y());
        break;
    case ResizeMode::Bottom:
        rect.setBottom(globalPos.y());
        break;
    case ResizeMode::TopLeft:
        rect.setTopLeft(globalPos);
        break;
    case ResizeMode::TopRight:
        rect.setTopRight(globalPos);
        break;
    case ResizeMode::BottomLeft:
        rect.setBottomLeft(globalPos);
        break;
    case ResizeMode::BottomRight:
        rect.setBottomRight(globalPos);
        break;
    default:
        return;
    }

    // 确保窗口不会变得太小
    if (rect.width() >= minimumWidth() && rect.height() >= minimumHeight()) {
        setGeometry(rect.normalized());
    }
}

void ChatWindow::updateCursor(ResizeMode mode)// 更新鼠标光标样式
{
    switch (mode) {
    case ResizeMode::Left:
    case ResizeMode::Right:
        setCursor(Qt::SizeHorCursor);
        break;
    case ResizeMode::Top:
    case ResizeMode::Bottom:
        setCursor(Qt::SizeVerCursor);
        break;
    case ResizeMode::TopRight://头右边
    case ResizeMode::BottomLeft:// 脚左边
        setCursor(Qt::SizeBDiagCursor); // / 方向
        break;
    case ResizeMode::TopLeft://头左边
    case ResizeMode::BottomRight://脚右边
        setCursor(Qt::SizeFDiagCursor); // \ 方向
        break;
    default:
        // 只有在非调整大小模式下才取消光标设置
        if (resizeMode == ResizeMode::None) {
            unsetCursor();
        }
        break;
    }
}

void ChatWindow::showMaximize()//显示最大化窗口
{
    if (isMaximize) {
        showNormal();//  显示正常窗口
        ui->maximizeButton->setIcon(QIcon(":/images/icon7.png"));//  设置最大化按钮图标
        ui->maximizeButton->setToolTip("最大化");//  设置最大化按钮提示
    } else {
        showMaximized();//  显示最大化窗口
        ui->maximizeButton->setIcon(QIcon(":/images/icon8.png"));//  设置还原按钮图标
        ui->maximizeButton->setToolTip("还原");//  设置还原按钮提示
    }
    isMaximize = !isMaximize;//  切换最大化状态
}

void ChatWindow::showContact()
{
    Database *db = new Database();
    contactModel = new ContactModel(this);
    contactDelegate = new ContactDelegate(this);
    
    if (!db->connect("chat_connection"))
    {
        qDebug() << "Failed to connect to database in ChatWindow";
        return;
    }
    
    //使用预处理语句避免SQL注入风险，并通过JOIN一次性获取所有联系人信息
    QSqlQuery query(db->database());
    query.prepare(R"(
        SELECT f.friend_note,u.user_nick,f.friend_id, u.avatar_path, u.motto 
        FROM friendships f 
        JOIN users u ON f.friend_id = u.user_id 
        WHERE f.user_id = ?
    )");
    query.addBindValue(userInfo.userID);
    
    if (!query.exec()) 
    {
        qDebug() << "Query execution failed:" << query.lastError().text();
        return;
    }

    qDebug() << "当前用户ID: " << userInfo.userID;
    qDebug() << "好友信息: ";
    
    while (query.next())
    {
        QString friendNote = query.value(0).toString();
        QString friendNick = query.value(1).toString();
        QString friendID = query.value(2).toString();
        QString avatarPath = query.value(3).toString();
        QString message = query.value(4).toString();

        //添加联系人到本地列表
        contactList[friendID] = Friend{friendNote,friendNick, friendID,avatarPath, message};
        qDebug() << "联系人信息 - 好友备注:" << friendNote << ", 头像路径:" << avatarPath << ", 信息:" << message;
        //将联系人信息添加到联系人模型中
        contactModel->addContact(friendNote, avatarPath, message,friendID);
    }
    ui->contactListView->setModel(contactModel);
    ui->contactListView->setItemDelegate(contactDelegate);
}

void ChatWindow::showCollect()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void ChatWindow::showMoments()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void ChatWindow::showSearch()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void ChatWindow::sendMessage() // 发送消息
{
    QString text = ui->inputBox->toPlainText().trimmed(); // 获取输入框文本并去除首尾空白
    if (text.isEmpty() || receiverID.isEmpty()) return; // 如果文本为空或接收者ID为空则返回

    // 创建并显示发送的消息气泡
    MessageBubble *messageBubble = new MessageBubble(MessageBubble::Self, text, QDateTime::currentDateTime());
    messageBubble->startAnimation(); // 启动动画效果
    
    
    ui->messageVBox->setAlignment(Qt::AlignTop); // 设置布局顶部对齐

    ui->messageVBox->addWidget(messageBubble);// 将消息气泡添加到消息布局中（显示消息）

    // 清空输入框
    ui->inputBox->clear();

    // 自动滚动到底部
    QScrollBar *scrollBar = ui->messageArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());

    // 保存消息数据
    Message msg;
    msg.role = MessageBubble::Self;
    msg.text = text;
    msg.time = QDateTime::currentDateTime();
    messageDataMap[receiverID].append(msg);// 保存消息数据
    m_networkManager->sendMessage(networkData{userInfo.userID, receiverID, text, QDateTime::currentDateTime()}); // 通过网络管理器发送消息
}

void ChatWindow::onContactClicked(const QModelIndex &index) // 点击联系人
{
    // 切换到聊天页面
    ui->stackedWidget->setCurrentWidget(ui->chatPage);
    ui->stackedWidget_2->setCurrentWidget(ui->messagePage);

    // 获取选中的联系人信息
    QString contactID = index.data(static_cast<int>(ContactRoles::IDRole)).toString();
    QString contactNick=contactList[contactID].friendNick;
    QString contactNote=contactList[contactID].friendNote;
    QString avatarpath=contactList[contactID].avatarPath;
    QString message=contactList[contactID].message;
    
    // 更新UI
    ui->nickname->setText(contactNote); // 显示好友备注作为昵称
    userInfo.userID = contactID;

    // 如果是第一次与该联系人聊天，初始化其消息列表
    if (!contactSelected.contains(contactID)) {
        messageModel->addContact(Contact{contactNick, avatarpath, message, contactID});
        ui->messageListView->setModel(messageModel);
        ui->messageListView->setItemDelegate(contactDelegate);
        addSampleMessages(contactNick,contactNick); // 添加示例消息
    }
    contactSelected[contactID] = true;// 标记该联系人已被选择过

    // 显示该联系人的消息
    displayContactMessages(contactID);
}

void ChatWindow::addSampleMessages(QString friendNick,QString userNick) // 添加示例消息
{
    // 仅当当前会话没有历史消息时才添加示例消息
    if (messageDataMap.contains(receiverID) && !messageDataMap[receiverID].isEmpty()) {
        return;
    }

    QList<Message> messages;
        
    Message msg1;
    msg1.role = MessageBubble::Other;
    msg1.text = "你好！我是"+friendNick+"，很高兴认识你！";
    msg1.time = QDateTime::currentDateTime().addSecs(-60);
    messages.append(msg1);
        
    Message msg2;
    msg2.role = MessageBubble::Self;
    msg2.text = "你好！我是"+userNick+"，很高兴认识你！";
    msg2.time = QDateTime::currentDateTime().addSecs(-30);
    messages.append(msg2);
        
    messageDataMap[receiverID] = messages;
}

void ChatWindow::displayContactMessages(QString receiverID) // 显示联系人消息
{
    // 清空现有消息
    // 删除布局中的所有控件
    QLayoutItem *item;
    while ((item = ui->messageVBox->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    // 显示联系人的消息
    if (messageDataMap.contains(receiverID)) {
        for (const Message &msg : messageDataMap[receiverID]) {
            MessageBubble *messageBubble = new MessageBubble(msg.role, msg.text, msg.time);
            messageBubble->startAnimation(); // 启动动画效果
            ui->messageVBox->addWidget(messageBubble);
        }
    }

    // 自动滚动到底部
    QScrollBar *scrollBar = ui->messageArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void ChatWindow::onMessageClicked(const QModelIndex &index) // 点击消息
{
    QString receiverID = index.data(static_cast<int>(ContactRoles::IDRole)).toString();
    qDebug()<<"接收者ID："<<receiverID;
    if(this->receiverID == receiverID)
    {
        qDebug()<<"切换默认页面与消息页面";
        if(ui->stackedWidget_2->currentWidget() == ui->messagePage)ui->stackedWidget_2->setCurrentWidget(ui->defaultPage);
        else ui->stackedWidget_2->setCurrentWidget(ui->messagePage);
    }
    else ui->stackedWidget_2->setCurrentWidget(ui->messagePage);
    this->receiverID = receiverID;
    QString receiverName = index.data(static_cast<int>(ContactRoles::NameRole)).toString();
    ui->nickname->setText(receiverName); // 更新昵称
    qDebug() << "点击了消息列表中的好友：" << receiverName;
    displayContactMessages(receiverID);
}

void ChatWindow::onUserStatusChanged(const QString &userId, bool online)// 处理用户状态改变
{
    qDebug() << "用户" << userId << "的在线状态已改变：" << online;
}

void ChatWindow::onMessageReceived(const networkData &data)// 处理接收消息
{
    qDebug() << "收到消息：" << data.senderId << " " << data.content << " " << data.timestamp;

    // 检查是否是当前聊天对象的消息
    if (data.senderId == receiverID) 
    {
        // 创建并显示接收的消息气泡
        MessageBubble *messageBubble = new MessageBubble(MessageBubble::Other, data.content, data.timestamp);
        messageBubble->startAnimation(); // 启动动画效果

        ui->messageVBox->setAlignment(Qt::AlignTop); // 设置布局顶部对齐
        ui->messageVBox->addWidget(messageBubble);

        // 自动滚动到底部
        QScrollBar *scrollBar = ui->messageArea->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());

        // 将消息添加到当前会话的消息列表中
        Message msg;
        msg.role = MessageBubble::Other;
        msg.text = data.content;
        msg.time = data.timestamp;
        messageDataMap[data.senderId].append(msg);
    } 
    //如果不是当前用户聊天对象的消息，则只保存消息数据
    else 
    {
        Message msg;
        msg.role = MessageBubble::Other;
        msg.text = data.content;
        msg.time = data.timestamp;
        messageDataMap[data.senderId].append(msg);
    }
}

void ChatWindow::linkServer()// 连接服务器
{
        /*
     * 初始化网络管理器并连接信号与槽函数
     * 设置NetworkManager实例，并将以下信号连接到对应的槽函数:
     * - messageReceived: 当收到网络消息时触发，连接到onMessageReceived槽函数
     * - connected: 当网络连接建立时触发，连接到onNetworkConnected槽函数
     * - disconnected: 当网络连接断开时触发，连接到onNetworkDisconnected槽函数
     * - error: 当网络发生错误时触发，连接到onNetworkError槽函数
     */
    qDebug() << "服务器连接中...";
    m_networkManager = new NetworkManager(this);
    //如果收到消息则调用onMessageReceived函数
    connect(m_networkManager, &NetworkManager::messageReceived,
            this, &ChatWindow::onMessageReceived);
    //连接成功时调用onNetworkConnected函数
    connect(m_networkManager, &NetworkManager::connected,
            this, &ChatWindow::onNetworkConnected);
    //断开连接时调用onNetworkDisconnected函数
    connect(m_networkManager, &NetworkManager::disconnected,
            this, &ChatWindow::onNetworkDisconnected);
    //发生错误时调用onNetworkError函数
    connect(m_networkManager, &NetworkManager::error,
            this, &ChatWindow::onNetworkError);
    //用户状态改变时调用onUserStatusChanged函数
    connect(m_networkManager, &NetworkManager::userStatusChanged,
            this, &ChatWindow::onUserStatusChanged);

    // 连接到服务器 
    m_networkManager->connectToServer("47.110.91.35", 6452);

}

void ChatWindow::onNetworkConnected()
{
    qDebug() << "服务器连接成功！";
    // 连接成功，无需认证，可以直接通信
}

void ChatWindow::onNetworkDisconnected()
{
    qDebug() << "服务器已断开！";
}

void ChatWindow::onNetworkError(const QString &error)
{
    qDebug() << "Network error网络错误：" << error;
}
