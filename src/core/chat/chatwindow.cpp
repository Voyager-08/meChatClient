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
#include <QFile>
#include "ui_chatwindow.h"
#include <QDir>


//自定义头文件
#include "chatwindow.h"
#include "src/modelView/contactmodel.h"
#include "src/modelView/messagemodel.h"
#include "src/modelView/contactdelegate.h"
#include "src/modelView/messagedelegate.h"
#include "src/thread/network/networkmanager.h"
#include "src/core/extra/messagebubble.h"

using namespace MeChat;

struct Message{ //消息结构体
    MessageBubble::Role role;  // 消息角色（自己/他人）
    QString text;              // 消息文本
    QDateTime time;            // 消息时间
};

ChatWindow::ChatWindow(QString userID,QWidget *parent,NetworkManager *networkManager) 
    : QWidget(parent), networkManager(networkManager), heartbeatTimer(nullptr), dataLoader(nullptr), ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    //初始化
    registerCustomTypes();// 注册struct.h中的自定义类型
    initialUserInfo(userID); // 初始化用户信息
    initialUI();// 初始化界面
    initialModelView();// 初始化模型视图
    initialStackWideget();// 初始化区域
    connectUISignals();// 连接UI信号槽
    connectServerSignals();// 连接服务器信号槽函数
    
    onHeartbeat();// 发送心跳消息
}

ChatWindow::~ChatWindow()
{
    if (heartbeatTimer) {
        heartbeatTimer->stop();
        delete heartbeatTimer;
        heartbeatTimer = nullptr;
    }
    delete ui;
}

void ChatWindow::initialUI() // 初始化界面
{
    QIcon windowIcon(":/images/10.png");
    setWindowIcon(windowIcon);
    
    setMinimumSize(850, 600); // 设置窗口最小尺寸
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
    ui->extendBtn->setIcon(QIcon(":/images/12.png"));
    ui->extendBtn->setIconSize(QSize(21, 21));
    ui->extendBtn->setStyleSheet(
    "QToolButton {"
        "border: 1px solid transparent;"
        "background-color: transparent;"
    "}"
    "QToolButton:hover {"
    "    border: rgba(111, 111, 111, 1) 1px solid;"
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
    // 设置头像图片
    ui->avatar->setFixedSize(50, 50);
    paintRdiusPixmap(ui->avatar,userInfo->userAvatar, 5, 5);

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

void ChatWindow::initialStackWideget()//初始化主窗口stackwidget
{
    ui->centerStack->setCurrentWidget(ui->centerContactPage);
    ui->sideBarStack->setCurrentWidget(ui->chatPage);
    ui->chatStack->setCurrentWidget(ui->defaultChatPage);
    ui->messageArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏垂直滚动条
    ui->messageArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏水平滚动条
    ui->messageArea->setFrameShape(QFrame::NoFrame); // 去掉边框
}

void ChatWindow::connectUISignals()// 连接UI信号槽
{
    // 连接发送按钮
    connect(ui->sent, &QPushButton::clicked, this, &ChatWindow::sendMessage);
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
    // 当点击收藏按钮时显示收藏页面
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
    // 连接发送按钮点击事件
    connect(ui->sentBtn, &QPushButton::clicked, this, &ChatWindow::onSentBtnClicked);
}

void ChatWindow::initialUserInfo(QString userID)//初始化用户信息
{
    userInfo = new UserInfo();
    userInfo->userID = userID;
    userInfo->userAvatar = "./images/avatar/"+userID+".png";
    
    //先连接接收用户信息信号槽，确保在发送请求前有处理函数
    connect(networkManager, &NetworkManager::receiveUserInfo, this, [&](UserInfo *userInfo) 
    {
        this->userInfo->userNick=userInfo->userNick;
        this->userInfo->userEmail=userInfo->userEmail;
        this->userInfo->userMotto=userInfo->userMotto;
        this->userInfo->userStatus=userInfo->userStatus;
        this->userInfo->userRegistrationDate=userInfo->userRegistrationDate;
        this->userInfo->userSex=userInfo->userSex;
        
        qDebug() << "用户ID:" << this->userInfo->userID;
        qDebug() << "用户头像:" << this->userInfo->userAvatar;
        qDebug() << "用户昵称:" << this->userInfo->userNick;
        qDebug() << "用户邮箱:" << this->userInfo->userEmail;
        qDebug() << "用户个性签名:" << this->userInfo->userMotto;
        qDebug() << "用户状态:" << this->userInfo->userStatus;
        qDebug() << "用户注册日期:" << this->userInfo->userRegistrationDate;
    });
    //请求用户信息
    QMetaObject::invokeMethod(networkManager, "requestUserInfo", Qt::QueuedConnection,
                              Q_ARG(QString, userID));
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

bool ChatWindow::isNonDraggableWidget(QWidget *w)//判断是否为不可拖动的组件
{
        if (!w) return false;
    return w->inherits("QPushButton") ||
           w->inherits("QLabel") ||
           w->inherits("QPlainTextEdit") ||
           w->inherits("QScrollArea");
           // 可继续添加：|| w->inherits("QLineEdit") 等
}

void ChatWindow::initialModelView()//初始化模型视图
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
    
    // 连接消息列表点击事件
    connect(ui->messageListView, &QListView::clicked, this, &ChatWindow::onMessageListClicked);
    // 连接单击信号到槽函数
    connect(ui->contactListView, &QListView::clicked, this, &ChatWindow::onContactListClicked);
    // 连接双击信号到槽函数
    connect(ui->contactListView, &QListView::doubleClicked, this, &ChatWindow::onContactListDoubleClicked);
    //好友列表视图更新
    connect(networkManager, &NetworkManager::receiveFriendInfo, this, &ChatWindow::showContactList, Qt::QueuedConnection);
    //消息列表视图更新
    connect(networkManager, &NetworkManager::receiveMessage, this, &ChatWindow::showMessageList, Qt::QueuedConnection);
    //请求好友列表
    QMetaObject::invokeMethod(networkManager, "sendFriendRequest", Qt::QueuedConnection,
                              Q_ARG(QString, userInfo->userID));
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

void ChatWindow::refreshContactList()// 刷新联系人列表
{

}

void ChatWindow::paintRdiusPixmap(QLabel*label,const QString paintPath,int xRdius=0,int yRdius=0)
{
    int w = label->width();
    int h = label->height();
    //创建一个圆角矩形头像
    QPixmap pixmap = QPixmap(paintPath);
    if (pixmap.isNull())
    {
        qDebug() << "头像路径为空，使用默认头像";
        pixmap = QPixmap(QDir::currentPath() + "/images/avatar/default.png");
    } 
    qDebug() << "加载头像路径：" << paintPath;
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

void ChatWindow::showMessageList(const messageData& message)
{
    // 检查消息列表中是否已存在该好友的消息
    bool found = false;
    for (int i = 0; i < messageModel->rowCount(); ++i) {
        QModelIndex index = messageModel->index(i, 0);
        QString senderID = messageModel->data(index, static_cast<int>(MessageRoles::SenderIDRole)).toString();
        if (senderID == message.senderId) {
            // 更新已存在的消息
            messageModel->updateMessage(message.senderId, message.content, message.timestamp);
            found = true;
            break;
        }
    }
    
    // 如果不存在，才添加新消息
    if (!found) {
        Model_Message m_message;
        m_message.note=contactList[message.senderId].friendNick;
        m_message.avatarPath=contactList[message.senderId].avatarPath;
        m_message.message=message.content;
        m_message.time=message.timestamp;
        m_message.senderID=message.senderId;
        messageModel->addMessage(m_message);
    }
    
    // 同时更新 messageDataMap 中的消息
    Message msg;
    msg.role = MessageBubble::Other;
    msg.text = message.content;
    msg.time = message.timestamp;
    messageDataMap[message.senderId].append(msg);
}

void ChatWindow::showContactList(const FriendInfo& friendInfo)
{
    qDebug() << "好友列表更新：";
    contactList[friendInfo.friendId]=friendInfo;
    Model_Contact m_contact;
    m_contact.name=friendInfo.friendNick;
    m_contact.avatarPath=friendInfo.avatarPath;
    m_contact.id=friendInfo.friendId;
    qDebug() << "好友昵称：" << friendInfo.friendNick <<"好友头像路径：" << friendInfo.avatarPath<<"Id:"<<friendInfo.friendId;
    contactModel->addContact(m_contact);
    addSampleBubbleMessages(friendInfo.friendId);
}

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
    
    // 创建网络消息数据
    messageData data;
    data.senderId = userInfo->userID;
    data.receiverId = receiverID;
    data.content = text;
    data.timestamp = QDateTime::currentDateTime();
    
    // 通过网络管理器发送消息
    QMetaObject::invokeMethod(networkManager, "sendMessage", Qt::QueuedConnection,
                              Q_ARG(messageData, data));

    // 自动滚动到底部
    QScrollBar *scrollBar = ui->messageArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void ChatWindow::addFriend()// 添加联系人
{

}

void ChatWindow::onCheckContactList_to_MessageList(const QModelIndex &index) // 添加联系人到消息列表
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
    qDebug() << "好友头像路径：" << avatarpath;
    QString message=contactList[contactID].motto;//个性签名或留言
    QDateTime time = QDateTime::currentDateTime(); // 默认就是本地时间（Qt::LocalTime）
    
    // 更新UI
    ui->nickname->setText(contactNote); // 显示好友备注作为昵称
    receiverID= contactID;// 更新当前接收者ID

    // 如果是第一次与该联系人聊天，添加初始消息
    if (!contactSelected.contains(contactID)) {
        // 检查是否有历史消息
        QString lastMessage = "(暂无消息)";
        QDateTime lastMessageTime = time;
        
        if (messageDataMap.contains(contactID) && !messageDataMap[contactID].isEmpty()) {
            // 获取最后一条消息
            const Message &lastMsg = messageDataMap[contactID].last();
            lastMessage = lastMsg.text;
            lastMessageTime = lastMsg.time;
        }
        
        // 添加联系人到消息模型，作为对话的标识
        messageModel->addMessage(contactNote, avatarpath, lastMessage, lastMessageTime, contactID);
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
    showContactChat(contactID);
}

void ChatWindow::onContactListDoubleClicked(const QModelIndex &index)
{
    onCheckContactList_to_MessageList(index);
}

void ChatWindow::addSampleBubbleMessages() // 添加示例消息
{
    // 为每个联系人添加一些示例消息
    for(QString friendID: contactList.keys())// 遍历联系人列表的键值
    {
        QList<Message> messages;
        Message msg1;
        msg1.role = MessageBubble::Other;
        msg1.text = "你好！我是"+contactList[friendID].friendNick+"，很高兴认识你！";//通过键值ID获取FriendInfo列表中的昵称
        msg1.time = QDateTime::currentDateTime().addSecs(-60);
        messages.append(msg1);
        Message msg2;
        msg2.role = MessageBubble::Self;
        msg2.text = "你好！我是"+userInfo->userNick+"，也很高兴认识你！";
        msg2.time = QDateTime::currentDateTime().addSecs(-30);
        messages.append(msg2);
        messageDataMap[friendID] = messages;
    }
}

void ChatWindow::addSampleBubbleMessages(QString friendID)
{
    QList<Message> messages;
    Message msg1;
    msg1.role = MessageBubble::Other;
    msg1.text = "你好！我是"+contactList[friendID].friendNick+"，很高兴认识你！";//通过键值ID获取FriendInfo列表中的昵称 
    msg1.time = QDateTime::currentDateTime().addSecs(-60);
    messages.append(msg1);
    Message msg2;
    msg2.role = MessageBubble::Self;
    msg2.text = "你好！我是"+userInfo->userNick+"，也很高兴认识你！";
    msg2.time = QDateTime::currentDateTime().addSecs(-30);
    messages.append(msg2);
    messageDataMap[friendID] = messages;
}

void ChatWindow::showContactChat(QString receiverID) // 显示选中联系人的所有消息
{
    // 清空现有消息
    // 删除布局中的所有控件
    QLayoutItem *item;
    while ((item = ui->messageVBox->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // 显示内存中的消息（如果有的话）
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

void ChatWindow::onMessageListClicked(const QModelIndex &index) // 点击消息列表
{
    // 取消之前通过联系人点击设置的高亮状态
    messageDelegate->setCheckedRow(-1);
    
    QString receiverID = index.data(static_cast<int>(MessageRoles::SenderIDRole)).toString();
    qDebug()<<"接收者ID："<<receiverID;
    if(this->receiverID != receiverID)//换了一个好友聊天
    {
        qDebug()<<"换了一个好友聊天";
        // 切换到消息聊天页面
        ui->chatStack->setCurrentWidget(ui->messageChatPage);
    }
    else 
    {
        if(ui->chatStack->currentWidget() == ui->defaultChatPage)ui->chatStack->setCurrentWidget(ui->messageChatPage);
        else ui->chatStack->setCurrentWidget(ui->defaultChatPage);
    }
    this->receiverID = receiverID;
    QString friendNote = contactList[receiverID].friendNote;
    ui->nickname->setText(friendNote); // 更新昵称
    qDebug() << "点击了消息列表中的好友：" << friendNote;
    showContactChat(receiverID);
}

void ChatWindow::onUserStatusChanged(const QString &userId, bool online)// 处理用户状态改变
{
    qDebug() << "用户" << userId << "的在线状态已改变：" << online;
}

void ChatWindow::onMessageReceived(const messageData &data)// 处理接收消息
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

void ChatWindow::connectServerSignals()//绑定信号槽函数
{
    /*
    初始化网络管理器并连接信号与槽函数
    设置NetworkManager实例，并将以下信号连接到对应的槽函数:
    - receiveMessage: 当收到网络消息时触发，连接到onMessageReceived槽函数
    - connected: 当网络连接建立时触发，连接到onNetworkConnected槽函数
    - disconnected: 当网络连接断开时触发，连接到onNetworkDisconnected槽函数
    - error: 当网络发生错误时触发，连接到onNetworkError槽函数
     */
    connect(networkManager, &NetworkManager::receiveMessage,
            this, &ChatWindow::onMessageReceived, Qt::QueuedConnection);
    //用户状态改变时调用onUserStatusChanged函数
    connect(networkManager, &NetworkManager::userStatusChanged,
            this, &ChatWindow::onUserStatusChanged, Qt::QueuedConnection);
}



void ChatWindow::onHeartbeat()
{
    qDebug() << "心跳包";
    // 用定时器每过30s发送心跳消息
    if (!heartbeatTimer) {
        heartbeatTimer = new QTimer(this);
        heartbeatTimer->setInterval(30000); // 30秒发送一次心跳消息
        // 连接定时器超时信号到槽函数
        connect(heartbeatTimer, &QTimer::timeout, this, [this](){
            if (networkManager && userInfo && networkManager->isConnected()) {
                QMetaObject::invokeMethod(networkManager, "sendHeartbeatMessage", Qt::QueuedConnection,
                                          Q_ARG(QString, userInfo->userID));
            }
        });
    }
    
    if (!heartbeatTimer->isActive()) {
        heartbeatTimer->start();
        qDebug() << "心跳定时器已启动";
    }
}

// DataLoader 槽函数
void ChatWindow::onUserInfoLoaded(const UserInfo &loadedUserInfo)
{
    if (!loadedUserInfo.userNick.isEmpty()) {
        userInfo->userNick = loadedUserInfo.userNick;
        userInfo->userEmail = loadedUserInfo.userEmail;
        userInfo->userMotto = loadedUserInfo.userMotto;
        userInfo->userAvatar = loadedUserInfo.userAvatar;
        userInfo->userSex = loadedUserInfo.userSex;
        qDebug() << "从数据库加载用户信息成功";
    }
}

void ChatWindow::onFriendListLoaded(const QList<FriendInfo> &friendList)
{
    for (const FriendInfo &friendInfo : friendList) {
        contactList[friendInfo.friendId] = friendInfo;
        Model_Contact m_contact;
        m_contact.name = friendInfo.friendNick;
        m_contact.avatarPath = friendInfo.avatarPath;
        m_contact.id = friendInfo.friendId;
        contactModel->addContact(m_contact);
    }
    qDebug() << "从数据库加载好友列表成功，共" << friendList.size() << "个好友";
}

void ChatWindow::onMessagesLoaded(const QString &friendId, const QList<messageData> &messages)
{
    if (!messages.isEmpty()) {
        for (const messageData &msg : messages) {
            Message message;
            message.role = (msg.senderId == userInfo->userID) ? MessageBubble::Self : MessageBubble::Other;
            message.text = msg.content;
            message.time = msg.timestamp;
            messageDataMap[friendId].append(message);
        }
        qDebug() << "从数据库加载好友" << friendId << "的消息成功，共" << messages.size() << "条消息";
        
        // 如果当前正在查看的就是这个好友的聊天记录，更新UI
        if (receiverID == friendId) {
            // 清空现有消息
            QLayoutItem *item;
            while ((item = ui->messageVBox->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
            
            // 显示消息
            for (const Message &msg : messageDataMap[friendId]) {
                MessageBubble *messageBubble = new MessageBubble(msg.role, msg.text, msg.time);
                messageBubble->startAnimation(); // 启动动画效果
                ui->messageVBox->addWidget(messageBubble);
            }
            
            // 自动滚动到底部
            QScrollBar *scrollBar = ui->messageArea->verticalScrollBar();
            scrollBar->setValue(scrollBar->maximum());
        }
    }
}

void ChatWindow::onLoadingFinished()
{
    qDebug() << "数据加载全部完成";
    // 数据加载完成后，可以更新UI或执行其他操作
}

void ChatWindow::onSentBtnClicked()
{
    // 获取当前选中的联系人
    QModelIndex currentIndex = ui->contactListView->currentIndex();
    if (!currentIndex.isValid())return;// 如果没有选中的联系人，直接返回
    // 从当前选中的联系人获取信息并切换到聊天页面
    onCheckContactList_to_MessageList(currentIndex);
}

void ChatWindow::onContactListClicked(const QModelIndex &index)//单击联系人列表
{
    ui->sideBarStack->setCurrentWidget(ui->contactPage);
    // 更新联系人详细信息页面,获取选中的联系人信息
    QString contactID = index.data(static_cast<int>(ContactRoles::IDRole)).toString();
    QString contactNick=contactList[contactID].friendNick;//昵称
    QString contactNote=contactList[contactID].friendNote;//备注
    QString avatarpath=contactList[contactID].avatarPath;//头像路径
    QString friendMotto=contactList[contactID].motto;//个性签名或留言
    ui->friendNickInfo->setText(contactNick);
    ui->friendNoteInfo->setText(contactNote);
    ui->friendMottoInfo->setText(friendMotto);
    if (!avatarpath.isEmpty()) {
        paintRdiusPixmap(ui->friendAvatar, avatarpath, 10, 10);
    }
    ui->contactStack->setCurrentWidget(ui->InfoContactPage);
}
