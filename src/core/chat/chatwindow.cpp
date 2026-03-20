#include <QTimer>
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QEasingCurve>// 缓动曲线头文件
#include <QPainter>// 绘图头文件
#include <QPainterPath>// 绘图路径头文件
#include <QMenu>
#include <QMovie>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include "ui_chatwindow.h"

//自定义头文件
#include "chatwindow.h"
#include "src/modelView/contactmodel.h"
#include "src/modelView/messagemodel.h"
#include "src/modelView/contactdelegate.h"
#include "src/modelView/messagedelegate.h"
#include "src/network/networkmanager.h"
#include "src/core/extra/messagebubble.h"

struct Friend{ //好友备注，好友昵称，好友ID，头像路径，信息
    QString friendNote; //  好友备注，用于唯一标识用户
    QString friendNick; //  好友昵称，用于显示用户昵称
    QString friendID; //  好友ID，用于唯一标识好友
    QString avatarPath; //  头像路径，存储用户头像文件的路径
    QString message; //  用户个性签名或留言
    QString email;      //  邮箱
};

struct Message{ //消息结构体
    MessageBubble::Role role;  // 消息角色（自己/他人）
    QString text;              // 消息文本
    QDateTime time;            // 消息时间
};

ChatWindow::ChatWindow(QString userID,QWidget *parent,NetworkManager *networkManager) 
    : QWidget(parent), m_networkManager(networkManager), ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    //初始化
    initialUserInfo(userID); // 初始化用户信息
    initialUI();// 初始化界面
    initialModelView();// 初始化模型视图
    initialStackWideget();// 初始化区域
    connectSignals();// 连接信号槽
    linkServer();// 连接服务器
    qDebug() << "ChatWindow initialized for user:" << userID;
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::initialUI()// 初始化界面
{
    QIcon windowIcon(":/images/10.png");
    setWindowIcon(windowIcon);
    // // 设置无边框，实现自定义标题栏和窗口边框以及拉伸窗口
    // setMouseTracking(true); //允许无按键时触发 mouseMoveEvent
    
    // 注意：当 ChatWindow 作为子 widget 时，不需要设置 setWindowFlags
    // setWindowFlags 只对顶级窗口有效
    // setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint);
    // setWindowFlags(Qt::FramelessWindowHint);
    
    setMinimumSize(700, 600); // 设置窗口最小尺寸

    // buttonWidgets  << ui->avatar<<ui->moreInformation
    //           << ui->message<<ui->nickname
    //           << ui->contact
    //           << ui->moments
    //           << ui->search
    //           << ui->more
    //           <<ui->messageArea<<ui->inputBox<<ui->sent;
    // // 设置聊天记录区域和输入框区域的高度比例
    QList<int> sizes;
    sizes << 400 << 100;  // 设置聊天记录区域和输入框区域的比例
    ui->information->setSizes(sizes);

    QString QSSmenuBarWidget =     
    "QPushButton {"
    "   background-color: transparent;"   // 透明背景
    "   border: none;"                    // 无边框
    "}"
    "QPushButton:hover {"
    "   background-color: rgba(255, 0, 0, 50);"
    "   border-radius: 15px;"
    "}"
    "QPushButton:pressed {"
    "   background-color: rgba(200,200,200,100);" // 按下时半透明效果
    "}";
    /*
    // 设置顶部菜单栏
    ui->closeButton->setToolTip("关闭");
    ui->closeButton->setText("");
    ui->minimizeButton->setToolTip("最小化");
    ui->minimizeButton->setText("");
    ui->minimizeButton->setBackgroundRole(QPalette::ButtonText);// 设置按钮背景角色
    ui->minimizeButton->setIconSize(QSize(30,30));
    ui->maximizeButton->setToolTip("最大化");// 设置按钮提示
    ui->maximizeButton->setText("");
    ui->maximizeButton->setIconSize(QSize(30,30));
    ui->closeButton->setIcon(QIcon(":/images/icon10.png"));
    ui->closeButton->setIconSize(QSize(30, 30));
    // 优化标题栏按钮
    ui->closeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   border: none;"
        "   border-radius: 15px;"
        "   width: 30px;"
        "   height: 30px;"
        "   font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 0, 0, 50);"
        "   border-radius: 15px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(255, 0, 0, 80);"
        "   border-radius: 15px;"
        "}"
    );
    ui->minimizeButton->setIcon(QIcon(":/images/icon6.png"));
    ui->minimizeButton->setIconSize(QSize(30, 30));
    ui->minimizeButton->setStyleSheet(QSSmenuBarWidget);
    ui->maximizeButton->setIcon(QIcon(":/images/icon7.png"));
    ui->maximizeButton->setIconSize(QSize(30, 30));
    ui->maximizeButton->setStyleSheet(QSSmenuBarWidget);// 设置按钮图标和大小
    */
    ui->extendBtn->setIcon(QIcon(":/images/12.png"));
    ui->extendBtn->setIconSize(QSize(21, 21));
    ui->extendBtn->setStyleSheet(
    "QToolButton {"
        "border: 1px solid transparent;"
        "background-color: transparent;"
    "}"
    "QToolButton:hover {"
    "    border: #65a3ff 1px solid;"
        "background-color: rgba(255,255,255,50);" // 鼠标悬停时半透明效果
    "}"
    "QToolButton:pressed {"
        "background-color: rgba(200,200,200,100);" // 按下时半透明效果
    "}"
    "QToolButton::menu-indicator { image: none; }"
    );
    ui->lineEdit->setPlaceholderText("搜索");
    ui->lineEdit_2->setPlaceholderText("搜索");
    QString QSSlineEdit = R"(
        QLineEdit {
            background-color: rgba(255, 255, 255, 200); /* 半透明背景以显示背景图 */
            border: 1px solid #e0e0e0;
            border-radius: 5px;
            padding: 5px;
            font-family: "KaiTi", "楷体";
            font-size: 14px;
        }
        QLineEdit:hover {
            border: 1px solid #727679;
        }
        QLineEdit:focus {
            border: 1px solid #71717b;
            background-color: rgba(255, 255, 255, 220); /* 半透明背景以显示背景图 */
        }
    )";
    ui->lineEdit->setStyleSheet(QSSlineEdit);
    ui->lineEdit_2->setStyleSheet(QSSlineEdit);
    //QMenu *extendMenu = new QMenu(ui->extendBtn);
    QAction *addFriendAction = ui->extendBtn->addAction("添加好友");
    connect(addFriendAction, &QAction::triggered, this, &ChatWindow::addFriend);
    ui->extendBtn->addAction("创建笔记");
    
    // 将菜单设置为按钮的弹出菜单
    ui->extendBtn->setMenu(ui->extendBtn->menu());

    ui->avatar->setFixedSize(50, 50);
    paintRdiusPixmap(ui->avatar,userInfo.userAvatar, 5, 5);

    // 设置占位符图片并使其铺满整个可用区域
    QPixmap placeholderPixmap(":/images/1.png");
    ui->placeholderLabel->setPixmap(placeholderPixmap);
    ui->placeholderLabel->setScaledContents(true); // 使图片铺满标签区域
    ui->placeholderLabel->setAlignment(Qt::AlignCenter); // 居中显示图片
    ui->placeholderLabel->setFixedSize(100, 100);

    ui->placeholderLabel_2->setPixmap(placeholderPixmap);
    ui->placeholderLabel_2->setScaledContents(true); // 使图片铺满标签区域
    ui->placeholderLabel_2->setAlignment(Qt::AlignCenter); // 居中显示图片
    ui->placeholderLabel_2->setFixedSize(100, 100);
    
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
            font-family: "KaiTi", "楷体";
            font-size: 14px;
            padding: 7px;
            text-align: center;
            border-radius: 7px;
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
    ui->message->setIcon(QIcon(":/images/icon2.png"));
    ui->message->setIconSize(QSize(32, 32));
    ui->message->setText("");// 只显示图标不显示文字
    ui->message->setToolTip("消息");
    
    ui->contact->setStyleSheet(navButtonStyle);
    ui->contact->setIcon(QIcon(":/images/icon5.png"));
    ui->contact->setIconSize(QSize(32, 32));
    ui->contact->setText("");
    ui->contact->setToolTip("联系人");
    
    ui->collect->setStyleSheet(navButtonStyle);
    ui->collect->setIcon(QIcon(":/images/icon1.png"));
    ui->collect->setIconSize(QSize(32, 32));
    ui->collect->setText("");
    ui->collect->setToolTip("收藏");
    
    ui->moments->setStyleSheet(navButtonStyle);
    ui->moments->setIcon(QIcon(":/images/2.png"));
    ui->moments->setIconSize(QSize(32, 32));
    ui->moments->setText("");
    ui->moments->setToolTip("朋友圈");
    
    ui->search->setStyleSheet(navButtonStyle);
    ui->search->setIcon(QIcon(":/images/icon8.png"));
    ui->search->setIconSize(QSize(32, 32));
    ui->search->setText("");
    ui->search->setToolTip("搜索");
    
    ui->more->setStyleSheet(navButtonStyle);
    ui->more->setIcon(QIcon(":/images/icon4.png"));
    ui->more->setIconSize(QSize(32, 32));
    ui->more->setText("");
    ui->more->setToolTip("更多");
    
    QString ListViewStyle="QListView {"
        "   background-color: rgba(255, 255, 255, 180);"
        "   border: none;"
        "   border-right: 1px solid #e0e0e0;"
        "   font-family: 'Microsoft YaHei';"
        "   font-size: 14px;"
        "   outline: none;"
        "}";

    // 优化联系人列表
    ui->contactListView->setStyleSheet(ListViewStyle);
    // 设置联系人列表样式
    ui->messageListView->setStyleSheet(ListViewStyle);

    // 优化消息区域动画
    ui->messageArea->setStyleSheet(
        "QScrollArea {"
        "   border: none;"
        "   background-color: transparent;"
        "   border-radius: 8px;"
        "}"
        "QWidget {"
        "   background-color: transparent;"
        "}"
    );
    
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
            font-family: "KaiTi", "楷体";
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
    
    // 设置按钮样式
    ui->messageArea->setStyleSheet(
        "QScrollArea {"
        "   border: none;"
        "   background-color: transparent;"
        "   border-radius: 8px;"
        "}"
        "QWidget {"
        "   background-color: transparent;"
        "}"
    );
    
    // 优化输入框动画
    ui->inputBox->setStyleSheet(
        "QPlainTextEdit {"
        "   background-color: transparent;"
        "   border: rgba(156, 155, 155, 1) 2px solid;"
        "}"
    );

    // 优化信息框动画
    ui->information->setStyleSheet(R"(
    QSplitter::handle {
        background-color: rgba(187, 187, 187, 0.47);
        height: 1px;
        }
    )");


    // 优化分割线动画
    ui->rightLine->setStyleSheet(R"(
    QSplitter::handle {
        background-color: rgba(187, 187, 187, 0.47);
        height: 1px;
        }
    )");

    // 优化联系人栏
    ui->contactBar->setStyleSheet(R"(
    QFrame {
        background-color: transparent;
        border-bottom: 1px solid #CCCCCC;  /* 添加1像素宽的灰色下划线 */
    }
    )");


    // 优化发送按钮
    ui->sent->setEnabled(false);//禁用发送按钮
    ui->sent->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   border: none;"
        "   color: rgba(116, 116, 116, 1);"
        "   font-family:KaiTi, 楷体;"
        "   font-size: 14px;"
        "   font-weight: 500;"
        "   border-radius: 6px;"
        "   padding: 5px 10px;"
        "   margin: 10px;"
        "   transition: background-color 0.2s;"
        "}"
    );
    
    // 美化占位符标签
    QString placeholderStyle = R"(
        QLabel {
            background-color: transparent; /* 透明背景以显示聊天背景图 */
            color: rgba(139, 139, 139, 0.65);
            font-family: "KaiTi", "楷体";
        }
    )";
    ui->placeholderLabel->setStyleSheet(placeholderStyle);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(255, 255, 255, 230));
    setAutoFillBackground(true);
    setPalette(palette);
    
    // 添加窗口阴影
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(15);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::black);
    setGraphicsEffect(shadow);
}

void ChatWindow::initialStackWideget()//初始化stackwidget
{
    ui->centerStack->setCurrentWidget(ui->centerContactPage);
    ui->sideBarStack->setCurrentWidget(ui->chatPage);
    ui->chatStack->setCurrentWidget(ui->defaultChatPage);
    ui->messageArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏垂直滚动条
    ui->messageArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏水平滚动条
    ui->messageArea->setFrameShape(QFrame::NoFrame); // 去掉边框
}

void ChatWindow::connectSignals()// 连接信号槽
{
    // 连接发送按钮
    connect(ui->sent, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    // 最小化
    // connect(ui->minimizeButton, &QPushButton::clicked, this, &QWidget::showMinimized);
    // // 最大化
    // connect(ui->maximizeButton, &QPushButton::clicked, this,&ChatWindow::showMaximize);
    // connect(ui->closeButton, &QPushButton::clicked, this, &ChatWindow::close);
    // 当点击消息按钮时显示聊天页面
    connect(ui->message, &QPushButton::clicked,[=](){
        ui->sideBarStack->setCurrentWidget(ui->chatPage);
        ui->centerStack->setCurrentWidget(ui->centerMessagePage);
        animatePageTransition(ui->sideBarStack);
    });
    // 当点击联系人按钮时显示联系人页面
    connect(ui->contact, &QPushButton::clicked,[=](){
        ui->sideBarStack->setCurrentWidget(ui->contactPage);
        ui->centerStack->setCurrentWidget(ui->centerContactPage);
        animatePageTransition(ui->sideBarStack);
    });
    // 当点击设置按钮时显示收藏页面
    connect(ui->collect, &QPushButton::clicked,[=](){
        ui->sideBarStack->setCurrentWidget(ui->collectPage);
        animatePageTransition(ui->collectPage);
    });
    // 当点击朋友圈按钮时显示朋友圈页面
    connect(ui->moments, &QPushButton::clicked,[=](){
        ui->sideBarStack->setCurrentWidget(ui->momentsPage);
        animatePageTransition(ui->momentsPage);
    });
    //点击发现按钮时显示搜索页面
    connect(ui->search, &QPushButton::clicked,[=](){
        ui->sideBarStack->setCurrentWidget(ui->searchPage);
        animatePageTransition(ui->searchPage);
    });
    // 单击联系人列表事件
    connect(ui->contactListView, &QListView::clicked, this, &ChatWindow::onContactClicked);
    // 双击联系人列表点击事件
    connect(ui->contactListView, &QListView::doubleClicked, this, &ChatWindow::onContactDoubleClicked);
    // 连接消息列表点击事件
    connect(ui->messageListView, &QListView::clicked, this, &ChatWindow::onMessageClicked);
    // 连接输入框的回车键发送消息信号
    connect(ui->inputBox, &MePlainTextEdit::enterPressed, this, &ChatWindow::sendMessage);
    
    // 连接输入框的焦点事件，实现样式变化效果
    connect(ui->inputBox, &MePlainTextEdit::focusIn, [=](){//输入框获取焦点
        ui->inputBox->setStyleSheet(
            "QPlainTextEdit {"
                "background-color: transparent;"
            "}"
        );
    });
    connect(ui->inputBox,&MePlainTextEdit::textChanged, [=]()
    {
        if (!ui->inputBox->toPlainText().isEmpty()) {
            ui->sent->setEnabled(true);
            ui->sent->setStyleSheet("QPushButton {"
            "   background-color: #07C160;"
            "   border: none;"
            "   color: rgba(255, 255, 255, 1);"
            "   font-family:KaiTi, 楷体;"
            "   font-size: 14px;"
            "   font-weight: 500;"
            "   padding: 5px 10px;"
            "   border-radius: 6px;"
            "   margin: 10px;"
            "}");
        }
        else
        {
            ui->sent->setEnabled(false);//禁用发送按钮
            ui->sent->setStyleSheet("QPushButton {"
            "   background-color: transparent;"
            "   border: none;"
            "   color: rgba(116, 116, 116, 1);"
            "   font-family:KaiTi, 楷体;"
            "   font-size: 14px;"
            "   font-weight: 500;"
            "   padding: 5px 10px;"
            "   border-radius: 6px;"
            "   margin: 10px;"
            "   }");
        }
    });
    // connect(ui->minimizeButton, &QPushButton::clicked, [=](){// 添加窗口最小化/最大化按钮动画
    //     ui->minimizeButton->setStyleSheet(
    //         "QPushButton {"
    //         "   background-color: transparent;"
    //         "   border: none;"
    //         "   border-radius: 15px;"
    //         "   width: 30px;"
    //         "   height: 30px;"
    //         "}"
    //         "QPushButton:hover {"
    //         "   background-color: rgba(0, 0, 0, 20);"
    //         "   border-radius: 15px;"
    //         "}"
    //     );
    //     QTimer::singleShot(100, [=](){
    //         ui->minimizeButton->setStyleSheet(
    //             "QPushButton {"
    //             "   background-color: transparent;"
    //             "   border: none;"
    //             "   border-radius: 15px;"
    //             "   width: 30px;"
    //             "   height: 30px;"
    //             "  }"
    //         );
    //     }); 
    // });
    
    // 连接"发消息"按钮
    connect(ui->sentBtn, &QPushButton::clicked, this, &ChatWindow::onSentBtnClicked);
}

void ChatWindow::initialUserInfo(QString userID)
{
    /*Database *db = new Database();
    
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
    
    //关闭数据库
    db->close();*/
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

bool ChatWindow::isNonDraggableWidget(QWidget *w)
{
        if (!w) return false;
    return w->inherits("QPushButton") ||
           w->inherits("QLabel") ||
           w->inherits("QPlainTextEdit") ||
           w->inherits("QScrollArea");
           // 可继续添加：|| w->inherits("QLineEdit") 等
}

void ChatWindow::initialModelView()
{
    messageDelegate=new MessageDelegate(this);
    contactDelegate = new ContactDelegate(this);
    messageModel = new MessageModel(this);
    contactModel = new ContactModel(this);
    ui->contactListView->setItemDelegate(contactDelegate);
    ui->contactListView->setModel(contactModel);
    ui->messageListView->setItemDelegate(messageDelegate);
    ui->messageListView->setModel(messageModel);
    ui->messageListView->setUniformItemSizes(true);// 设置所有项的尺寸相同
    ui->contactListView->setUniformItemSizes(true);
    
    // 连接双击信号到槽函数
    connect(ui->contactListView, &QListView::doubleClicked, this, &ChatWindow::onContactDoubleClicked);
    
    showContact();//显示联系人列表
    showMessage();//显示消息列表
}

void ChatWindow::animatePageTransition(QWidget *widget)
{
    // 确保widget可见
    widget->show();
    
    // 获取widget的初始几何信息
    QRect startGeometry = widget->geometry();
    QRect endGeometry = widget->geometry();
    
    // 如果是初始状态，设置起始位置在右侧
    if (startGeometry.x() < widget->parentWidget()->width()) {
        startGeometry.moveLeft(widget->parentWidget()->width());
        widget->setGeometry(startGeometry);
    }
    
    // 创建页面切换动画
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "geometry");
    animation->setDuration(300); // 300ms 动画时间
    animation->setEasingCurve(QEasingCurve::OutCubic); // 使用缓动曲线使动画更自然
    
    animation->setStartValue(startGeometry);
    animation->setEndValue(endGeometry);
    
    // 动画结束后清理资源
    connect(animation, &QAbstractAnimation::finished, [=]() {
        widget->setGeometry(endGeometry); // 确保最终位置正确
        animation->deleteLater();
    });
    
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ChatWindow::paintRdiusPixmap(QLabel*label,const QString paintPath,int xRdius=0,int yRdius=0)
{
    int w = label->width();
    int h = label->height();
    //创建一个圆角矩形头像
    QPixmap pixmap = QPixmap(paintPath);
    QPixmap pixmapPath(w, h);
    pixmapPath.fill(Qt::transparent);
    
    QPainter painter(&pixmapPath);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QPainterPath path;
    path.addRoundedRect(0, 0, w, h, xRdius, yRdius); // xRadius 和 yRadius 分别是 X 轴和 Y 轴的圆角半径
    painter.setClipPath(path);
    
    // 绘制缩放后的头像
    painter.drawPixmap(0, 0, pixmap.scaled(w, h, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    
    label->setPixmap(pixmapPath);
}

/*void ChatWindow::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "鼠标按下";
    if (event->button() == Qt::LeftButton) {
        isLeftButtonPressed = true;
        resizeMode = getResizeMode(event->pos());
        
        // 标题栏区域 (高度40像素)
        QRect titleBarRect(0, 0, width(), 40);
        bool inTitleBar = titleBarRect.contains(event->pos());// 检查鼠标是否在标题栏内
        
        // 检查是否在按钮区域（避免覆盖按钮）
        bool inButtonArea = isNonDraggableWidget(childAt(event->pos()));
        
        if (resizeMode != ResizeMode::None)//鼠标在窗口外
        {
            updateCursor(resizeMode);
            event->accept();
        }
        else if (inTitleBar||!inButtonArea)// 不在按钮区域或者不在窗口外
        {
            // 在标题栏区域且不在按钮上，允许拖动
            setCursor(Qt::SizeAllCursor); // 手型光标
            dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
            isDragging = true;
            event->accept();
        }
        else 
        {
            // 其他区域不处理
            isDragging = false;
            event->ignore();
        }
    }
    QWidget::mousePressEvent(event);
}

void ChatWindow::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "鼠标移动";
    bool eventHandled = false;
    
    if (resizeMode != ResizeMode::None && event->buttons() & Qt::LeftButton) {
        updateCursor(resizeMode);
        resizeWindow(event->globalPosition().toPoint());
        event->accept();
        eventHandled = true;
    }
    else if (event->buttons() & Qt::LeftButton && isDragging) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
        eventHandled = true;
    }
    else {
        // 仅在非拖动/调整大小时更新光标
        resizeMode = getResizeMode(event->pos());
        if (!isDragging) {
            updateCursor(resizeMode);
        }
    }
    if (!eventHandled) {
        QWidget::mouseMoveEvent(event);
    }
}

void ChatWindow::mouseReleaseEvent(QMouseEvent *event)
{
    //qDebug() << "鼠标释放";
    resizeMode = ResizeMode::None;
    isLeftButtonPressed = false;
    
    // 重置光标状态
    if (isDragging) {
        setCursor(Qt::ArrowCursor);
        isDragging = false;
    } else {
        updateCursor(ResizeMode::None);
    }
    
    QWidget::mouseReleaseEvent(event);
}

void ChatWindow::leaveEvent(QEvent *event)
{
    //qDebug() << "鼠标离开窗口";
    // 确保离开窗口时恢复默认光标
    if (!isDragging) {
        updateCursor(ResizeMode::None);
    }
    QWidget::leaveEvent(event);
}

// 获取当前鼠标所在的边缘位置
ResizeMode ChatWindow::getResizeMode(const QPoint &pos)// 根据鼠标位置获取调整模式
{
    QRect rect = this->rect();// 获取窗口矩形区域
    // 检查鼠标是否在窗口边缘区域
    // 在 Qt 的坐标系统中，鼠标（以及所有 widget）的坐标原点 (0, 0) 默认位于左上角，并且：
    // X 轴向右增大（→）
    // Y 轴向下增大（↓）
    // 这是 Qt 遵循的 标准屏幕坐标系，与大多数 GUI 框架一致。
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
    return ResizeMode::None;// 不在边缘区域
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

void ChatWindow::updateCursor(ResizeMode mode)
{
    //qDebug() << "更新光标: " << (int)mode;
    switch (mode) {
    case ResizeMode::Left:
    case ResizeMode::Right:
        setCursor(Qt::SizeHorCursor);
        break;
    case ResizeMode::Top:
    case ResizeMode::Bottom:
        setCursor(Qt::SizeVerCursor);
        break;
    case ResizeMode::TopRight:
    case ResizeMode::BottomLeft:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case ResizeMode::TopLeft:
    case ResizeMode::BottomRight:
        setCursor(Qt::SizeFDiagCursor);
        break;
    default:
        // 无论哪种情况都设置为箭头光标
        setCursor(Qt::ArrowCursor);
        break;
    }
}
*/

// void ChatWindow::showMaximize()
// {
//     if (isMaximize) {
//         showNormal();
//         ui->maximizeButton->setIcon(QIcon(":/images/icon7.png"));
//         ui->maximizeButton->setToolTip("最大化");
//     } else {
//         showMaximized();
//         ui->maximizeButton->setIcon(QIcon(":/images/icon9.png"));
//         ui->maximizeButton->setToolTip("还原");
//     }
//     isMaximize = !isMaximize;
// }

void ChatWindow::showMessage()
{   

}

void ChatWindow::showContact()
{
    // 检查contactModel是否为空，如果是则从数据库加载数据
    if (contactModel->rowCount() == 0) {
        //loadContactsFromDatabase();
    }
    addSampleMessages(); // 添加示例消息
    ui->contactListView->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
}

void ChatWindow::refreshContacts()//刷新联系人
{
    // 清空现有的联系人模型和列表
    contactModel->clearContacts();
    contactList.clear();
    
    // 重新加载联系人
    //loadContactsFromDatabase();
    
    // 更新UI显示
    ui->contactListView->setModel(contactModel);
    ui->contactListView->setItemDelegate(contactDelegate);
}

/*void ChatWindow::loadContactsFromDatabase()
{
    Database *db = new Database();
    if (!db->connect("chat_connection")) {
        delete db;
        return;
    }
    
    //使用预处理语句避免SQL注入风险，并通过JOIN一次性获取所有联系人信息
    QSqlQuery query(db->database());
    query.prepare(R"(
        SELECT f.friend_note,u.user_nick,f.friend_id, u.avatar_path, u.motto ,u.email
        FROM friendships f 
        JOIN users u ON f.friend_id = u.user_id 
        WHERE f.user_id = ?
    )");
    query.addBindValue(userInfo.userID);
    
    if (!query.exec()) 
    {
        qDebug() << "Query execution failed:" << query.lastError().text();
        delete db;
        return;
    }
    qDebug() << "好友信息: ";
    
    while (query.next())
    {
        QString friendNote = query.value(0).toString();
        QString friendNick = query.value(1).toString();
        QString friendID = query.value(2).toString();
        QString avatarPath = query.value(3).toString();
        QString message = query.value(4).toString();
        QString email = query.value(5).toString();

        //添加联系人到本地列表
        contactList[friendID] = Friend{friendNote,friendNick, friendID,avatarPath, message,email};
        qDebug() << "联系人信息 - 好友备注:" << friendNote << ", 头像路径:" << avatarPath << ", 信息:" << message;
        //将联系人信息添加到联系人模型中
        contactModel->addContact(friendNote, avatarPath,friendID);
    }
    delete db;
}*/

void ChatWindow::showCollect()
{
    ui->sideBarStack->setCurrentIndex(2);
}

void ChatWindow::showMoments()
{
    ui->sideBarStack->setCurrentIndex(3);
}

void ChatWindow::showSearch()
{
    ui->sideBarStack->setCurrentIndex(4);
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

    // 保存消息数据
    Message msg;
    msg.role = MessageBubble::Self;
    msg.text = text;
    msg.time = QDateTime::currentDateTime();
    messageDataMap[receiverID].append(msg);// 保存消息数据
    // 通过网络管理器发送消息
    m_networkManager->sendMessage(networkData{userInfo.userID, receiverID, text, QDateTime::currentDateTime()});

    // 自动滚动到底部
    QScrollBar *scrollBar = ui->messageArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void ChatWindow::addFriend()// 添加联系人
{
    /*Database *db = new Database();
    if (!db->connect("add_friend_connection")) {
        QMessageBox::critical(nullptr, QObject::tr("错误"), QObject::tr("数据库连接失败，无法添加好友！"));
        delete db;
        return;
    }
    QSqlDatabase database = db->database();

    bool ok;
    // 弹出输入对话框获取对方的用户ID
    QString friendInfo = QInputDialog::getText(nullptr, 
        QObject::tr("添加好友"), 
        QObject::tr("请输入好友ID或昵称"), 
        QLineEdit::Normal, 
        QString(), 
        &ok);
    
    if (!ok || friendInfo.isEmpty()) {
        // 用户取消操作或输入为空
        return;
    }
    
    // 先尝试按昵称查找用户ID
    QSqlQuery selectQuery(database);
    selectQuery.prepare("SELECT user_id, user_nick FROM users WHERE user_nick = ?");
    selectQuery.addBindValue(friendInfo);
    
    QString friendID = friendInfo;  // 默认认为输入的就是ID
    QString friendNick = friendInfo; // 默认使用输入作为昵称
    
    if (selectQuery.exec() && selectQuery.next()) {
        // 如果找到了对应的昵称，使用查询到的ID和昵称
        friendID = selectQuery.value("user_id").toString();
        friendNick = selectQuery.value("user_nick").toString();
    }
    
    // 检查是否已经是好友
    if (contactList.contains(friendID)) {
        QMessageBox::information(nullptr, QObject::tr("提示"), QObject::tr("该用户已经是您的好友！"));
        delete db;
        return;
    }
    
    // 检查自己不能添加自己为好友
    if (friendID == userInfo.userID) {
        QMessageBox::information(nullptr, QObject::tr("提示"), QObject::tr("不能添加自己为好友！"));
        delete db;
        return;
    }

    // 开始事务
    database.transaction();
    
    try {
        QSqlQuery query(database);
        // 检查对方用户是否存在
        query.prepare("SELECT COUNT(*) FROM users WHERE user_id = ?");
        query.addBindValue(friendID);
        if (!query.exec() || !query.next() || query.value(0).toInt() == 0) {
            QMessageBox::information(nullptr, QObject::tr("提示"), QObject::tr("该用户不存在！"));
            database.rollback();
            delete db;
            return;
        }
        
        // 检查是否已经是好友（双向检查）
        query.prepare("SELECT COUNT(*) FROM friendships WHERE (user_id = ? AND friend_id = ?) OR (user_id = ? AND friend_id = ?)");
        query.addBindValue(userInfo.userID);
        query.addBindValue(friendID);
        query.addBindValue(friendID);
        query.addBindValue(userInfo.userID);
        
        if (!query.exec() || !query.next() || query.value(0).toInt() > 0) {
            QMessageBox::information(nullptr, QObject::tr("提示"), QObject::tr("你们已经是好友了！"));
            database.rollback();
            delete db;
            return;
        }
        
        // 在friendships表中互相添加为好友
        // 添加当前用户到对方的好友列表
        query.prepare("INSERT INTO friendships (user_id, friend_id, friend_note, created_at) VALUES (?, ?, ?, NOW())");
        query.addBindValue(friendID);           // user_id (对方)
        query.addBindValue(userInfo.userID);    // friend_id (当前用户)
        query.addBindValue(userInfo.userNick);  // 默认使用当前用户的昵称作为对方看到的备注
        if (!query.exec()) {
            throw std::runtime_error("Failed to add current user to friend's list");
        }
        
        // 添加对方到当前用户的好友列表
        query.prepare("INSERT INTO friendships (user_id, friend_id, friend_note, created_at) VALUES (?, ?, ?, NOW())");
        query.addBindValue(userInfo.userID);    // user_id (当前用户)
        query.addBindValue(friendID);           // friend_id (对方)
        query.addBindValue(friendNick);         // 使用对方的昵称作为备注
        if (!query.exec()) {
            throw std::runtime_error("Failed to add friend to current user's list");
        }
        
        // 提交事务
        database.commit();
        
        QMessageBox::information(nullptr, QObject::tr("成功"), QObject::tr("好友添加成功！"));
        
        // 更新本地联系人列表
        // 重新加载联系人列表以包含新增的好友
        addSampleMessages(friendID);//添加好友后，添加示例消息
        contactModel->clearContacts(); // 清空当前联系人
        contactList.clear();          // 清空本地联系人缓存
        loadContactsFromDatabase();   // 重新加载联系人
        
        delete db;
    } catch (const std::exception &e) {
        // 回滚事务
        database.rollback();
        QMessageBox::critical(nullptr, QObject::tr("错误"), QObject::tr("添加好友失败：%1").arg(e.what()));
        delete db;
        return;
    }*/
}

void ChatWindow::onCheckContact(const QModelIndex &index) // 添加联系人到消息列表
{
    // 切换到聊天页面
    ui->centerStack->setCurrentWidget(ui->centerMessagePage);
    ui->sideBarStack->setCurrentWidget(ui->chatPage);  // 显示聊天主页面
    ui->chatStack->setCurrentWidget(ui->messageChatPage);  // 显示消息页面而不是联系人信息

    // 获取选中的联系人信息
    QString contactID = index.data(static_cast<int>(ContactRoles::IDRole)).toString();
    QString contactNick=contactList[contactID].friendNick;//昵称
    QString contactNote=contactList[contactID].friendNote;//备注
    QString avatarpath=contactList[contactID].avatarPath;//头像路径
    QString message=contactList[contactID].message;//信息
    QDateTime time = QDateTime::currentDateTime(); // 默认就是本地时间（Qt::LocalTime）
    
    // 更新UI
    ui->nickname->setText(contactNote); // 显示好友备注作为昵称
    receiverID= contactID;// 更新当前接收者ID

    // 如果是第一次与该联系人聊天，添加初始消息
    if (!contactSelected.contains(contactID)) {
        // 添加联系人到消息模型，作为对话的标识
        messageModel->addMessage(contactNote, avatarpath, "(暂无消息)", time, contactID);
    }
    contactSelected[contactID] = true;// 标记该联系人已被选中

    // 高亮显示对应联系人的消息项
    // 查找该联系人在消息列表中的索引
    for (int i = 0; i < messageModel->rowCount(); ++i) {
        QModelIndex msgIndex = messageModel->index(i, 0);
        if (messageModel->data(msgIndex, static_cast<int>(MessageRoles::SenderIDRole)).toString() == contactID) {
            QItemSelectionModel *selectionModel = ui->messageListView->selectionModel();
            selectionModel->clearSelection();
            selectionModel->select(msgIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            
            // 同时设置委托中的高亮行
            messageDelegate->setCheckedRow(i);
            break;
        }
    }
    // 显示该联系人的消息
    showContactMessages(contactID);
}

void ChatWindow::onContactDoubleClicked(const QModelIndex &index)
{
    onCheckContact(index);
}

void ChatWindow::addSampleMessages() // 添加示例消息
{
    // 为每个联系人添加一些示例消息
    for(QString friendID: contactList.keys())// 遍历联系人列表的键值
    {
        QList<Message> messages;
        Message msg1;
        msg1.role = MessageBubble::Other;
        msg1.text = "你好！我是"+contactList[friendID].friendNick+"，很高兴认识你！";//通过键值ID获取Friend列表中的昵称
        msg1.time = QDateTime::currentDateTime().addSecs(-60);
        messages.append(msg1);
        Message msg2;
        msg2.role = MessageBubble::Self;
        msg2.text = "你好！我是"+userInfo.userNick+"，也很高兴认识你！";
        msg2.time = QDateTime::currentDateTime().addSecs(-30);
        messages.append(msg2);
        messageDataMap[friendID] = messages;
    }
}

void ChatWindow::addSampleMessages(QString friendID)
{
    QList<Message> messages;
    Message msg1;
    msg1.role = MessageBubble::Other;
    msg1.text = "你好！我是"+contactList[friendID].friendNick+"，很高兴认识你！";//通过键值ID获取Friend列表中的昵称
    msg1.time = QDateTime::currentDateTime().addSecs(-60);
    messages.append(msg1);
    Message msg2;
    msg2.role = MessageBubble::Self;
    msg2.text = "你好！我是"+userInfo.userNick+"，也很高兴认识你！";
    msg2.time = QDateTime::currentDateTime().addSecs(-30);
    messages.append(msg2);
    messageDataMap[friendID] = messages;
}

void ChatWindow::showContactMessages(QString receiverID) // 显示联系人消息
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
            qDebug() << "showContactMessages: " << msg.text;
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
    // 取消之前通过联系人点击设置的高亮状态
    messageDelegate->setCheckedRow(-1);
    
    QString receiverID = index.data(static_cast<int>(MessageRoles::SenderIDRole)).toString();
    qDebug()<<"接收者ID："<<receiverID;
    
    if(ui->chatStack->currentWidget() != ui->messageChatPage)
    {
        ui->chatStack->setCurrentWidget(ui->messageChatPage);
    }
    else ui->chatStack->setCurrentWidget(ui->defaultChatPage);
    
    this->receiverID = receiverID;
    QString receiverName = index.data(static_cast<int>(MessageRoles::NoteRole)).toString();
    ui->nickname->setText(receiverName); // 更新昵称
    qDebug() << "点击了消息列表中的好友：" << receiverName;
    showContactMessages(receiverID);
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
    // 自动滚动到底部
    QScrollBar *scrollBar = ui->messageArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
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
    //m_networkManager = new NetworkManager(this);
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

}

void ChatWindow::onNetworkConnected()// 处理连接成功
{
    qDebug() << "服务器连接成功！";
    // 连接成功，无需认证，可以直接通信
}

void ChatWindow::onNetworkDisconnected()// 处理断开连接
{
    qDebug() << "服务器已断开！";
}

void ChatWindow::onNetworkError(const QString &error)// 处理网络错误
{
    qDebug() << "Network error网络错误：" << error;
}

void ChatWindow::onSentBtnClicked()
{
    // 获取当前选中的联系人
    QModelIndex currentIndex = ui->contactListView->currentIndex();
    if (!currentIndex.isValid())return;
    // 从当前选中的联系人获取信息并切换到聊天页面
    onCheckContact(currentIndex);
}

void ChatWindow::onContactClicked(const QModelIndex &index)//单击联系人
{
    ui->sideBarStack->setCurrentWidget(ui->contactPage);
    // 更新联系人详细信息页面
    // 获取选中的联系人信息
    QString contactID = index.data(static_cast<int>(ContactRoles::IDRole)).toString();
    QString contactNick=contactList[contactID].friendNick;//昵称
    QString contactNote=contactList[contactID].friendNote;//备注
    QString avatarpath=contactList[contactID].avatarPath;//头像路径
    QString friendMotto=contactList[contactID].message;//邮箱
    ui->friendNickInfo->setText(contactNick);
    ui->friendNoteInfo->setText(contactNote);
    ui->friendMottoInfo->setText(friendMotto);
    if (!avatarpath.isEmpty()) {
        paintRdiusPixmap(ui->friendAvatar, avatarpath, 10, 10);
    }
    ui->contactStack->setCurrentWidget(ui->InfoContactPage);
}
