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
#include <QDir>
#include "ui_chatwindow.h"

//自定义头文件
#include "chatwindow.h"
#include "src/modelView/contactmodel.h"
#include "src/modelView/addcontactmodel.h"
#include "src/modelView/messagemodel.h"
#include "src/modelView/contactdelegate.h"
#include "src/modelView/messagedelegate.h"
#include "src/modelView/addcontactdelegate.h"
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

    ui->lineEdit->setPlaceholderText("搜索");
    ui->lineEdit_2->setPlaceholderText("搜索");//设置文本框占位符
    ui->lineEdit->setStyleSheet(R"(
        QLineEdit {
            background-color: rgba(255, 255, 255, 0.4);
            color: rgba(100, 100, 100, 1);
            border: 1px solid #e0e0e0;
            border-radius: 5px;
            padding: 5px;
            font-family: "KaiTi", "楷体";
            font-size: 14px;
        }
        QLineEdit:hover {
            border: 2px solid rgba(255, 255, 255, 0.6);
        }
        QLineEdit:focus {
            border: 2px solid rgba(255, 255, 255, 1);
            background-color: rgba(255, 255, 255, 220); /* 半透明背景以显示背景图 */
        }
    )");
    ui->lineEdit_2->setStyleSheet(R"(
        QLineEdit {
            background-color: rgba(255, 255, 255, 0.4);
            color: rgba(100, 100, 100, 1);
            border: 1px solid #e0e0e0;
            border-radius: 5px;
            padding: 5px;
            font-family: "KaiTi", "楷体";
            font-size: 14px;
        }
        QLineEdit:hover {
            border: 2px solid rgba(255, 255, 255, 0.6);
        }
        QLineEdit:focus {
            border: 2px solid rgba(255, 255, 255, 1);
            background-color: rgba(255, 255, 255, 220); /* 半透明背景以显示背景图 */
        }
    )");
    
    ui->extendBtn->setIcon(QIcon(":/images/12.png"));
    ui->extendBtn->setIconSize(QSize(21, 21));
    ui->extendBtn->setStyleSheet(
    "QToolButton {"
        "border: 1px solid transparent;"
        "background-color: transparent;"
    "}"
    "QToolButton:hover {"
    "    border: rgba(111, 111, 111, 1) 1px solid;"
    "}"
    "QToolButton:pressed {"
        "background-color: rgba(200,200,200,100);" // 按下时半透明效果
    "}"
    "QToolButton::menu-indicator { image: none; }"
    "QMenu {"
        "background-color: rgba(110, 187, 255, 1); /* 半透明背景以显示背景图 */"
        "border: 1px solid rgba(110, 187, 255, 1);"
        "border-radius: 3px;"
        "padding: 5px;"
        "font-family: 'KaiTi', '楷体';"
        "font-size: 14px;"
    "}"
    "QMenu::item:hover {"
        "background-color: rgba(62, 225, 84, 1);"
    "}"
    "QMenu::item:selected {" 
        "background-color: rgba(62, 225, 84, 1);"
    "}"
    );
    QAction *addFriendAction = ui->extendBtn->addAction("添加好友");
    connect(addFriendAction, &QAction::triggered, this, &ChatWindow::addFriend);
    QAction *createNoteAction = ui->extendBtn->addAction("创建笔记");
    connect(createNoteAction, &QAction::triggered, this, &ChatWindow::createNote);

    // 将菜单设置为按钮的弹出菜单
    ui->extendBtn->setMenu(ui->extendBtn->menu());
    // 设置头像图片
    ui->avatar->setFixedSize(50, 50);
    paintRdiusPixmap(ui->avatar,userInfo->userAvatar, 5, 5);

    // 设置占位符图片并使其铺满整个可用区域
    QPixmap messagePhotoPixmap(":/images/click_message.png");
    ui->defaultMessagePhoto->setPixmap(messagePhotoPixmap);
    ui->defaultMessagePhoto->setScaledContents(true); // 使图片铺满标签区域
    ui->defaultMessagePhoto->setAlignment(Qt::AlignCenter); // 居中显示图片
    ui->defaultMessagePhoto->setFixedSize(100, 100);

    QPixmap contactPhotoPixmap(":/images/click_contact.png");
    ui->defaultContactPhoto->setPixmap(contactPhotoPixmap);
    ui->defaultContactPhoto->setScaledContents(true); // 使图片铺满标签区域
    ui->defaultContactPhoto->setAlignment(Qt::AlignCenter); // 居中显示图片
    ui->defaultContactPhoto->setFixedSize(100, 100);
    
    QPixmap collectPhotoPixmap(":/images/click_collect.png");
    ui->defaultCollectPhoto->setPixmap(collectPhotoPixmap);
    ui->defaultCollectPhoto->setScaledContents(true); // 使图片铺满标签区域
    ui->defaultCollectPhoto->setAlignment(Qt::AlignCenter); // 居中显示图片
    ui->defaultCollectPhoto->setFixedSize(100, 100);
    
    QPixmap momentPhotoPixmap(":/images/click_moment.png");
    ui->defaultMomentPhoto->setPixmap(momentPhotoPixmap);
    ui->defaultMomentPhoto->setScaledContents(true); // 使图片铺满标签区域
    ui->defaultMomentPhoto->setAlignment(Qt::AlignCenter); // 居中显示图片
    ui->defaultMomentPhoto->setFixedSize(100, 100);
    
    QPixmap searchPhotoPixmap(":/images/click_search.png");
    ui->defaultSearchPhoto->setPixmap(searchPhotoPixmap);
    ui->defaultSearchPhoto->setScaledContents(true); // 使图片铺满标签区域
    ui->defaultSearchPhoto->setAlignment(Qt::AlignCenter); // 居中显示图片
    ui->defaultSearchPhoto->setFixedSize(100, 100);
    
    // 美化左侧导航栏
    ui->leftBox->setStyleSheet(R"(
        QFrame {
            background-color: rgba(190,216,239,1);  /* 半透明背景以显示背景图 */
            border-right: 1px solid rgba(197, 197, 197, 1);
            }
    )");

    // 为 frameContact 和 frameMessage 单独设置样式，避免被覆盖
    ui->frameContact->setStyleSheet(R"(
        QFrame {
            border-bottom: 1px solid rgba(197, 197, 197, 1);
        }
    )");
    
    ui->frameMessage->setStyleSheet(R"(
        QFrame {
            border-bottom: 1px solid rgba(197, 197, 197, 1);
        }
    )");
    
    // 设置 listViewStack 样式
    ui->listViewStack->setStyleSheet(
        R"(
            QStackedWidget  {
            background-color: rgba(190,216,239,1);  /* 半透明背景以显示背景图 */ 
            border-radius: 5px;
            }
    )");

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
    ui->message->setIcon(QIcon(":/images/click_message.png"));
    ui->message->setIconSize(QSize(32, 32));
    ui->message->setText("");// 只显示图标不显示文字
    ui->message->setToolTip("消息");
    
    ui->contact->setStyleSheet(navButtonStyle);
    ui->contact->setIcon(QIcon(":/images/click_contact.png"));
    ui->contact->setIconSize(QSize(32, 32));
    ui->contact->setText("");
    ui->contact->setToolTip("联系人");
    
    ui->collect->setStyleSheet(navButtonStyle);
    ui->collect->setIcon(QIcon(":/images/click_collect.png"));
    ui->collect->setIconSize(QSize(32, 32));
    ui->collect->setText("");
    ui->collect->setToolTip("收藏");
    
    ui->moments->setStyleSheet(navButtonStyle);
    ui->moments->setIcon(QIcon(":/images/click_moment.png"));
    ui->moments->setIconSize(QSize(32, 32));
    ui->moments->setText("");
    ui->moments->setToolTip("朋友圈");
    
    ui->search->setStyleSheet(navButtonStyle);
    ui->search->setIcon(QIcon(":/images/click_search.png"));
    ui->search->setIconSize(QSize(32, 32));
    ui->search->setText("");
    ui->search->setToolTip("搜索");
    
    ui->more->setStyleSheet(navButtonStyle);
    ui->more->setIcon(QIcon(":/images/setting.png"));
    ui->more->setIconSize(QSize(20, 20));
    ui->more->setText("");
    ui->more->setToolTip("更多");

    // 设置联系人列表样式
    ui->contactListView->setStyleSheet(R"(
        QListView {
            background-color: rgba(255, 255, 255, 1);
            font-family: 'Microsoft YaHei';
            font-size: 14px;
            outline: none;
            border-radius: 4px;
        }
        QListView:hover {
        }
        QListView::item:hover {
            border-radius: 4px;
            border-bottom: 3px solid rgba(161, 161, 161, 1);
            border-right: 3px solid rgba(161, 161, 161, 1);
        }
    )");
    // 设置消息列表样式
    ui->messageListView->setStyleSheet(
    R"(
        QListView {
            background-color: rgba(255, 255, 255, 1);
            font-family: 'Microsoft YaHei';
            font-size: 14px;
            outline: none;
            border-radius: 4px;
        }
        QListView:hover {
        }
        QListView::item:hover {
            border-radius: 8px;
        }
    )");

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
            background-color: rgba(232, 232, 232, 1);
            border-radius: 4px;
            border-bottom: 2px solid #e0e0e0;
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
            border-radius: 6px;
        }
        QPushButton:hover {
            background-color: rgba(230, 240, 255, 0.8);
            color: #1a73e8;
        }
        QPushButton:pressed {
            background-color: rgba(200, 220, 255, 0.9);
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
    
    // 设置输入框样式
    ui->inputBox->setStyleSheet(R"(
        QPlainTextEdit {
            background-color: rgba(245, 245, 245, 0);
            border-radius: 8px;
            padding: 10px;
            font-family: 'Microsoft YaHei';
            font-size: 14px;
        }
        QPlainTextEdit:hover {
            border-color: rgba(26, 115, 232, 0.5);
        }
        QPlainTextEdit:focus {
            border-color: #1a73e8;
            background-color: rgba(255, 255, 255, 1);
            outline: none;
        }
    )");

    // 优化信息框动画
    ui->information->setStyleSheet(R"(
        QSplitter::handle {
            background-color: rgba(187, 187, 187, 0.47);
            height: 1px;
        }
        QSplitter::handle:hover {
            background-color: rgba(26, 115, 232, 0.5);
        }
    )");

    

    // 设置发送按钮样式
    ui->sent->setEnabled(false);//禁用发送按钮
    ui->sent->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(240, 240, 240, 0.8);
            border: 1px solid rgba(156, 155, 155, 0.5);
            color: rgba(116, 116, 116, 1);
            font-family: 'KaiTi', '楷体';
            font-size: 14px;
            font-weight: 500;
            border-radius: 6px;
            padding: 8px 16px;
            margin: 10px;
        }
    )");
    
    ui->sentBtn->setStyleSheet(
        R"(
            QPushButton:hover {
                border: 1px solid rgba(57, 218, 86, 0.98);
                background-color: rgba(57, 218, 86, 0.98);
                color: rgba(255, 255, 255, 1);
            }
        )"
    );
    ui->callBtn->setStyleSheet(
        R"(
            QPushButton:hover {
                border: 1px solid rgba(248, 69, 69, 0.98);
                background-color: rgba(248, 69, 69, 0.98);
                color: rgba(255, 255, 255, 1);
            }
        )"
    );

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
    ui->listViewStack->setCurrentWidget(ui->listViewContactPage);
    ui->mainStack->setCurrentWidget(ui->ContactPage);
    ui->messageStack->setCurrentWidget(ui->defaultMessagePage);
    ui->contactStack->setCurrentWidget(ui->defaultContactPage);
    ui->messageArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏垂直滚动条
    ui->messageArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏水平滚动条
    ui->messageArea->setFrameShape(QFrame::NoFrame); // 去掉边框
}

void ChatWindow::connectUISignals()// 连接UI信号槽
{
    // 连接发送按钮
    connect(ui->sent, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    connect(ui->message, &QPushButton::clicked,[=](){
        ui->mainStack->setCurrentWidget(ui->MessagePage);
        ui->messageStack->setCurrentWidget(ui->defaultMessagePage);
        ui->listViewStack->setCurrentWidget(ui->listViewMessagePage);
        animatePageTransition(ui->mainStack);
        ui->leftBox->setStyleSheet(
            "QFrame {"
                "background-color: rgba(163,227,206,1); /* 半透明背景以显示背景图 */"
                "border-right: 1px solid rgba(197, 197, 197, 1);"
            "}"
        );
        ui->listViewStack->setStyleSheet(R"(
            QStackedWidget {
                background-color: rgba(163,227,206,1);
            }
        )");
    });
    // 当点击联系人按钮时显示联系人页面
    connect(ui->contact, &QPushButton::clicked,[=](){
        ui->mainStack->setCurrentWidget(ui->ContactPage);
        ui->contactStack->setCurrentWidget(ui->defaultContactPage);
        ui->listViewStack->setCurrentWidget(ui->listViewContactPage);
        ui->leftBox->setStyleSheet(
            "QFrame {"
                "background-color: rgba(190,216,239,1); /* 半透明背景以显示背景图 */"
                "border-right: 1px solid rgba(197, 197, 197, 1);"
            "}"
        );
        ui->listViewStack->setStyleSheet(R"(
            QStackedWidget {
                background-color: rgba(190,216,239,1);
            }
        )");
        animatePageTransition(ui->mainStack);
    });
    // 当点击收藏按钮时显示收藏页面
    connect(ui->collect, &QPushButton::clicked,[=](){
        ui->mainStack->setCurrentWidget(ui->CollectPage);
        ui->collectStack->setCurrentWidget(ui->defaultCollectPage);
        ui->listViewStack->setCurrentWidget(ui->listViewCollectPage);
        ui->leftBox->setStyleSheet(
            "QFrame {"
                "background-color: rgba(248, 171, 165, 1); /* 半透明背景以显示背景图 */"
                "border-right: 1px solid rgba(197, 197, 197, 1);"
            "}"
        );
        ui->listViewStack->setStyleSheet(R"(
            QStackedWidget {
                background-color: rgba(248, 171, 165, 1);
            }
        )");
        animatePageTransition(ui->CollectPage);
    });
    // 当点击朋友圈按钮时显示朋友圈页面
    connect(ui->moments, &QPushButton::clicked,[=](){
        ui->mainStack->setCurrentWidget(ui->MomentPage);
        ui->momentStack->setCurrentWidget(ui->defaultMomentPage);
        ui->listViewStack->setCurrentWidget(ui->listViewMomentPage);
        ui->leftBox->setStyleSheet(
            "QFrame {"
                "background-color: rgba(107, 255, 146, 0.5); /* 半透明背景以显示背景图 */"
                "border-right: 1px solid rgba(197, 197, 197, 1);"
            "}"
        );
        ui->listViewStack->setStyleSheet(R"(
            QStackedWidget {
                background-color: rgba(107, 255, 146, 0.5);
            }
        )");
        animatePageTransition(ui->MomentPage);
    });
    //点击发现按钮时显示搜索页面
    connect(ui->search, &QPushButton::clicked,[=](){
        ui->mainStack->setCurrentWidget(ui->SearchPage);
        ui->searchStack->setCurrentWidget(ui->defaultSearchPage);
        ui->listViewStack->setCurrentWidget(ui->listViewSearchPage);
        ui->leftBox->setStyleSheet(
            "QFrame {"
                "background-color: rgba(255, 196, 147, 1); /* 半透明背景以显示背景图 */"
                "border-right: 1px solid rgba(197, 197, 197, 1);"
            "}"
        );
        ui->listViewStack->setStyleSheet(R"(
            QStackedWidget {
                background-color: rgba(255, 196, 147, 1);
            }
        )");
        animatePageTransition(ui->SearchPage);
    });
    
    // 连接更多按钮的点击信号
    connect(ui->more, &QPushButton::clicked, [=]() {
        QMenu *menu = new QMenu(this);
        menu->addAction("设置", this, []() {
            // 这里可以添加设置功能
        });
        menu->addAction("退出登录", this, [=]() {
            // 发送退出登录信号
            emit exitLogin(userInfo->userID);
            // 清空用户信息
            delete userInfo;
            userInfo = nullptr;
            // 关闭当前窗口
            this->close();
        });
        menu->exec(QCursor::pos());
        delete menu;
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
            ui->sent->setStyleSheet(R"(
            QPushButton {
            background-color: #07C160;
            border: 1px solid rgba(156, 155, 155, 0.5);
            color: rgba(255, 255, 255, 1);
            font-family: 'KaiTi', '楷体';
            font-size: 14px;
            font-weight: 500;
            border-radius: 6px;
            padding: 8px 16px;
            margin: 10px;
            })");
        }
        else
        {
            ui->sent->setEnabled(false);//禁用发送按钮
            ui->sent->setStyleSheet(R"(
            QPushButton {
            background-color: rgba(240, 240, 240, 0.8);
            border: 1px solid rgba(156, 155, 155, 0.5);
            color: rgba(116, 116, 116, 1);
            font-family: 'KaiTi', '楷体';
            font-size: 14px;
            font-weight: 500;
            border-radius: 6px;
            padding: 8px 16px;
            margin: 10px;
            })");
        }
    });
    // 连接发送按钮点击事件
    connect(ui->sentBtn, &QPushButton::clicked, this, &ChatWindow::clickSentBtn);
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

void ChatWindow::initialModelView()//初始化模型视图
{
    messageDelegate=new MessageDelegate(this);
    contactDelegate = new ContactDelegate(this);
    messageModel = new MessageModel(this);
    contactModel = new ContactModel(this);
    addContactModel = new AddContactModel(this);
    ui->contactListView->setItemDelegate(contactDelegate);
    ui->contactListView->setModel(contactModel);
    ui->contactListView->setUniformItemSizes(true);
    ui->messageListView->setItemDelegate(messageDelegate);
    ui->messageListView->setModel(messageModel);
    ui->messageListView->setUniformItemSizes(true);// 设置所有项的尺寸相同
    
    // 连接消息列表点击事件
    connect(ui->messageListView, &QListView::clicked, this, &ChatWindow::clickMessageList);
    // 连接单击信号到槽函数
    connect(ui->contactListView, &QListView::clicked, this, &ChatWindow::clickContactList);
    // 连接双击信号到槽函数
    connect(ui->contactListView, &QListView::doubleClicked, this, &ChatWindow::clickDoubleContactList);
    //添加好友成功后好友列表视图更新
    connect(networkManager, &NetworkManager::addFriendSuccess, this, &ChatWindow::showContactList, Qt::QueuedConnection);
    //好友列表视图更新
    connect(networkManager, &NetworkManager::receiveFriendInfo, this, &ChatWindow::showContactList, Qt::QueuedConnection);
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
        pixmap = QPixmap(QDir::currentPath() + "/images/avatar/default.png");
    }
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

void ChatWindow::createNote()
{
    //打开系统自带记事本
    system("notepad.exe");
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
    ui->mainStack->setCurrentIndex(2);
}

void ChatWindow::showMoments()
{
    ui->mainStack->setCurrentIndex(3);
}

void ChatWindow::showSearch()
{
    ui->mainStack->setCurrentIndex(4);
}

void ChatWindow::sendMessage() // 发送消息
{
    // 确保输入框获取焦点
    ui->inputBox->setFocus();
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
    QDialog dialog;
    dialog.setWindowTitle("添加好友");
    dialog.setWindowIcon(QIcon(":/images/click_contact.png"));
    dialog.setFixedSize(700, 450);
    
    // 设置现代化样式
    dialog.setStyleSheet(
        "QDialog { "
        "    background-color: #f0f2f5; "
        "    border-radius: 10px; "
        "}"
    );

    // 主垂直布局
    QVBoxLayout mainLayout;
    mainLayout.setContentsMargins(20, 20, 20, 20);
    mainLayout.setSpacing(15);
    dialog.setLayout(&mainLayout);

    // 搜索区域容器
    QWidget searchContainer(&dialog);
    searchContainer.setObjectName("searchContainer");
    searchContainer.setStyleSheet(
        "#searchContainer { "
        "    background-color: transparent; "
        "}"
    );
    QHBoxLayout searchLayout;
    searchLayout.setContentsMargins(10, 10, 10, 10);
    searchLayout.setSpacing(10);
    searchContainer.setLayout(&searchLayout);

    // 搜索框 - 现代化样式
    QLineEdit addFriendEdit(&searchContainer);
    addFriendEdit.setPlaceholderText("请输入ID或昵称搜索...");
    addFriendEdit.setMinimumHeight(40);
    addFriendEdit.setStyleSheet(
        "QLineEdit { "
        "    border: 2px solid #e0e0e0; "
        "    border-radius: 20px; "
        "    padding: 8px 15px; "
        "    font-size: 14px; "
        "    background-color: #fafafa; "
        "} "
        "QLineEdit:hover { "
        "    border-color: rgba(117, 219, 139, 1); "
        "    background-color: white; "
        "} "
        "QLineEdit:focus { "
        "    border-color: #0078d4; "
        "    background-color: white; "
        "}"
    );

    // 搜索按钮 - 现代化样式
    QPushButton searchButton("🔍 搜索", &searchContainer);
    searchButton.setMinimumSize(100, 40);
    searchButton.setStyleSheet(
        "QPushButton { "
        "    background-color: #e0e0e0; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 20px; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    padding: 8px 16px; "
        "} "
        "QPushButton:hover { "
        "    background-color: rgba(117, 219, 139, 1); "
        "} "
        "QPushButton:pressed { "
        "    background-color: #0078d4; "
        "    padding: 8px 16px; "
        "}"
    );

    searchLayout.addWidget(&addFriendEdit);
    searchLayout.addWidget(&searchButton);

    // 联系人列表 - 现代化样式
    QListView addContactListView(&dialog);
    addContactListView.setStyleSheet(
        "QListView { "
        "    background-color: white; "
        "    border: 1px solid #e0e0e0; "
        "    border-radius: 12px; "
        "    padding: 10px; "
        "    outline: none; "
        "} "
        "QListView::item { "
        "    border-radius: 8px; "
        "    margin: 2px 0; "
        "    padding: 8px; "
        "} "
        "QListView::item:selected { "
        "    background-color: #e6f4ff; "
        "    border-radius: 8px; "
        "} "
        "QListView::item:hover { "
        "    background-color: #f0f8ff; "
        "    border-radius: 8px; "
        "} "
        "QListView::vertical-scrollbar { "
        "    width: 8px; "
        "    background: transparent; "
        "    border-radius: 4px; "
        "} "
        "QListView::vertical-slider { "
        "    background: #c0c0c0; "
        "    border-radius: 4px; "
        "} "
        "QListView::vertical-slider:hover { "
        "    background: #a0a0a0; "
        "} "
        "QListView::vertical-slider:pressed { "
        "    background: #808080; "
        "} "
        "QListView::horizontal-scrollbar { "
        "    height: 8px; "
        "    background: transparent; "
        "    border-radius: 4px; "
        "} "
        "QListView::horizontal-slider { "
        "    background: #c0c0c0; "
        "    border-radius: 4px; "
        "} "
        "QListView::horizontal-slider:hover { "
        "    background: #a0a0a0; "
        "} "
        "QListView::horizontal-slider:pressed { "
        "    background: #808080; "
        "}"
    );
    addContactListView.setSpacing(5);// 设置项间距为5px
    addContactListView.setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);// 垂直滚动条按像素滚动,变细一点
    addContactListView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);// 水平滚动条隐藏

    // 将搜索容器和列表添加到主布局
    mainLayout.addWidget(&searchContainer);
    mainLayout.addWidget(&addContactListView, 1); // 占据剩余空间

    // 使用AddContactDelegate
    AddContactDelegate addFriendDelegate(&dialog);
    addContactListView.setItemDelegate(&addFriendDelegate);
    addContactListView.setModel(addContactModel);
    addContactListView.setUniformItemSizes(true);// 设置所有项的尺寸相同
    
    // 点击添加好友按钮
    connect(&addFriendDelegate, &AddContactDelegate::addButtonClicked, this, [&](const QModelIndex &index){
        QString friendId = index.data(static_cast<int>(ContactRoles::IDRole)).toString();
        if (!friendId.isEmpty()) {
            emit addFriendId(friendId);//发送添加好友信号
        }
    });
    
    // 连接添加好友Id到网络管理器信号
    connect(this, &ChatWindow::addFriendId, networkManager, &NetworkManager::sendAddFriendRequest, Qt::QueuedConnection);
    
    // 回车事件搜索好友
    connect(&addFriendEdit, &QLineEdit::returnPressed, this, [this, &addFriendEdit](){
        if (!addFriendEdit.text().isEmpty()) {
            emit searchAddFriendStr(addFriendEdit.text());
        }
    });
    // 连接清空添加好友列表信号槽
    connect(networkManager, &NetworkManager::clearAddFriendList, this, [&](){
        addContactModel->clearContacts();
    });
    // 点击搜索按钮搜索好友
    connect(&searchButton, &QPushButton::clicked, this, [this, &addFriendEdit](){
        if (!addFriendEdit.text().isEmpty()) {
            emit searchAddFriendStr(addFriendEdit.text());
        }
    });
    connect(networkManager, &NetworkManager::receiveAddFriendList, this, [&](const QString &friendId, const QString &friendNick){
        addContactModel->addContact(friendNick,"./images/avatar/"+friendId+".png", friendId);
    },Qt::QueuedConnection);
    // 连接搜索用户str到网络管理器Str查找用户列表信号
    connect(this, &ChatWindow::searchAddFriendStr, networkManager, &NetworkManager::sendFriendStrListRequest, Qt::QueuedConnection);
    
    dialog.setModal(true);// 设置为模态对话框
    dialog.exec();
}
void ChatWindow::clickContactList_to_MessageList(const QModelIndex &index) // 添加联系人到消息列表
{
    // 切换到聊天页面
    ui->listViewStack->setCurrentWidget(ui->listViewMessagePage);
    ui->mainStack->setCurrentWidget(ui->MessagePage);  // 显示聊天主页面
    ui->messageStack->setCurrentWidget(ui->messageChatPage);  // 显示消息页面而不是联系人信息

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
    if (!contact_addMessage.contains(contactID)) {
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
    contact_addMessage[contactID] = true;// 标记该联系人已被添加消息
    
    // 查找该联系人在消息列表中的索引,并高亮显示
    for (int i = 0; i < messageModel->rowCount(); ++i) {
        QModelIndex msgIndex = messageModel->index(i, 0);
        if (messageModel->data(msgIndex, static_cast<int>(MessageRoles::SenderIDRole)).toString() == contactID) {
            QItemSelectionModel *selectionModel = ui->messageListView->selectionModel();
            selectionModel->clearSelection();
            selectionModel->select(msgIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            
            // 同时设置委托中的高亮行
            // messageDelegate->setCheckedRow(i);
            break;
        }
    }
    // 显示该联系人的消息
    showContact_historyMessage(contactID);
}

void ChatWindow::clickDoubleContactList(const QModelIndex &index)
{
    ui->leftBox->setStyleSheet(
        "QFrame {"
            "background-color: rgba(163,227,206,1); /* 半透明背景以显示背景图 */"
            "border-right: 1px solid rgba(197, 197, 197, 1);"
        "}"
    );
    ui->listViewStack->setStyleSheet(
        "QStackedWidget {"
            "background-color: rgba(163,227,206,1);"
        "}"
    );
    clickContactList_to_MessageList(index);
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

void ChatWindow::showContact_historyMessage(QString receiverID) // 显示选中联系人的所有消息
{
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

void ChatWindow::clickMessageList(const QModelIndex &index) // 点击消息列表
{
       
    QString receiverID = index.data(static_cast<int>(MessageRoles::SenderIDRole)).toString();
    qDebug()<<"接收者ID："<<receiverID;
    if(this->receiverID != receiverID)//换了一个好友聊天
    {
        qDebug()<<"换了一个好友聊天";
        // 切换到消息聊天页面
        ui->messageStack->setCurrentWidget(ui->messageChatPage);
    }
    else 
    {
        if(ui->messageStack->currentWidget() == ui->defaultMessagePage)
        {
            ui->messageStack->setCurrentWidget(ui->messageChatPage);
            //当前index对应的item作为选中状态
            QItemSelectionModel *selectionModel = ui->messageListView->selectionModel();
            selectionModel->clearSelection();
            selectionModel->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        }
        else 
        {
            ui->messageStack->setCurrentWidget(ui->defaultMessagePage);
            //当前index对应的item作为未选中状态
            QItemSelectionModel *selectionModel = ui->messageListView->selectionModel();
            selectionModel->clearSelection();
        }
    }
    this->receiverID = receiverID;
    QString friendNote = contactList[receiverID].friendNote;
    ui->nickname->setText(friendNote); // 更新昵称
    qDebug() << "点击了消息列表中的好友：" << friendNote;
    showContact_historyMessage(receiverID);
}

void ChatWindow::onUserStatusChanged(const QString &userId, bool online)// 处理用户状态改变
{
    qDebug() << "用户" << userId << "的在线状态已改变：" << online;
}

void ChatWindow::onMessageReceived(const messageData &data)// 处理接收消息
{
    // 如果不存在，才添加新消息
    if (!contact_addMessage.contains(data.senderId)) {
        Model_Message m_message;
        m_message.note=contactList[data.senderId].friendNick;
        m_message.avatarPath=contactList[data.senderId].avatarPath;
        m_message.message=data.content;
        m_message.time=data.timestamp;
        m_message.senderID=data.senderId;
        messageModel->addMessage(m_message);
        contact_addMessage[data.senderId] = true;// 标记该联系人已被添加消息
    }
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
    connect(networkManager, &NetworkManager::receiveMessage,
            this, &ChatWindow::onMessageReceived, Qt::QueuedConnection);
    //用户状态改变时调用onUserStatusChanged函数
    connect(networkManager, &NetworkManager::userStatusChanged,
        this, &ChatWindow::onUserStatusChanged, Qt::QueuedConnection);
    connect(this, &ChatWindow::exitLogin, 
        networkManager, &NetworkManager::sendLogoutRequest, Qt::QueuedConnection);
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

void ChatWindow::clickSentBtn()
{
    ui->leftBox->setStyleSheet(
        "QFrame {"
            "background-color: rgba(163,227,206,1); /* 半透明背景以显示背景图 */"
            "border-right: 1px solid rgba(197, 197, 197, 1);"
        "}"
    );
    ui->listViewStack->setStyleSheet(
        "QStackedWidget {"
            "background-color: rgba(163,227,206,1);"
        "}"
    );
    // 获取当前选中的联系人
    QModelIndex currentIndex = ui->contactListView->currentIndex();
    if (!currentIndex.isValid())return;// 如果没有选中的联系人，直接返回
    // 从当前选中的联系人获取信息并切换到聊天页面
    clickContactList_to_MessageList(currentIndex);
}

void ChatWindow::clickContactList(const QModelIndex &index)//单击联系人列表
{
    ui->mainStack->setCurrentWidget(ui->ContactPage);
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
    ui->contactStack->setCurrentWidget(ui->contactPage);
}
