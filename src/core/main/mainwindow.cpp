#include <QDebug>
#include <QVBoxLayout>
#include <QIcon>
#include <QTimer>
#include <QThread>

#include "mainwindow.h"
#include "src/core/login/loginwindow.h"
#include "src/core/chat/chatwindow.h"
#include "src/event/eventbus.h"
#include "src/network/networkmanager.h"
#include "src/sql/databasemanager.h"

MainWindow::MainWindow(EventBus *eventBus, NetworkManager *networkManager, DatabaseManager *databaseManager, QWidget *parent)
    : QWidget(parent),
      eventBus(eventBus),
      networkManager(networkManager),
      databaseManager(databaseManager),
      loginWindow(nullptr),
      chatWindow(nullptr)
{
    initializeUI();
    connectSignals();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initializeUI()
{
    // 设置主窗口属性
    setWindowTitle("meChat");
    setWindowIcon(QIcon(":/images/10.png"));  // 设置窗口图标
    setBaseSize(700, 450);
    setMouseTracking(true);  // 允许鼠标跟踪
    
    // 创建栈式窗口作为中心组件
    stackedWidget = new QStackedWidget(this);
    
    // 使用 QVBoxLayout 来管理布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);  // 设置边距为0，使 stackedWidget 充满整个窗口
    layout->addWidget(stackedWidget);
    setLayout(layout);
    
    // 创建登录窗口
    loginWindow = new LoginWindow(this);
    stackedWidget->addWidget(loginWindow);
    
    // 初始显示登录页面
    stackedWidget->setCurrentWidget(loginWindow);
    setFixedSize(700, 450); // 设置主窗口大小与登录窗口一致
}

void MainWindow::connectSignals()
{
    // ========== EventBus 信号绑定 (Composition Root) ==========
    // 绑定登录窗口信号
    eventBus->bindLoginWindowSignals(loginWindow);
    // 绑定网络管理器信号
    eventBus->bindNetworkSignals(networkManager);
    // 绑定数据库管理器信号
    eventBus->bindDatabaseSignals(databaseManager);
    
    // ========== MainWindow 内部信号 ==========
    // 监听登录成功信号，切换到聊天窗口
    connect(loginWindow, &LoginWindow::loginSuccess, this, &MainWindow::onUserLoggedIn);
}

void MainWindow::onUserLoggedIn(const QString &userID)
{
    qDebug() << "主窗口: 用户已登录，ID=" << userID;
    
    // 隐藏主窗口
    this->hide();
    //位置往左上移动
    move(350, 140);
    // 如果之前已有聊天窗口，先删除
    if (chatWindow != nullptr) {
        // 断开 EventBus 绑定
        stackedWidget->removeWidget(chatWindow);
        delete chatWindow;
        chatWindow = nullptr;
    }

    // 创建新的聊天窗口
    chatWindow = new ChatWindow(userID, this);
    // 设置 ChatWindow 充满整个 stackedWidget
    chatWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(850, 600); // 设置主窗口最小尺寸，确保聊天窗口有足够空间显示
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX); // 取消最大尺寸限制，允许用户调整窗口大小
    
    // 绑定聊天窗口信号到 EventBus
    eventBus->bindChatWindowSignals(chatWindow);
    
    // 触发本地数据加载（信号链路：ChatWindow -> EventBus -> DatabaseManager）
    chatWindow->startDataLoading();
    
    // 连接退出登录信号 (ChatWindow::sendLogoutRequest -> MainWindow::onUserLoggedOut)
    connect(chatWindow, &ChatWindow::sendLogoutRequest, this, &MainWindow::onUserLoggedOut);
    // 添加到栈式窗口并设置为当前窗口
    stackedWidget->addWidget(chatWindow);
    stackedWidget->setCurrentWidget(chatWindow);
    
    // 显示主窗口
    this->show();
}

void MainWindow::onUserLoggedOut(const QString &userId)
{
    qDebug() << "主窗口: 用户已退出登录，ID=" << userId;
    setFixedSize(700, 450);
    
    // 切换到登录窗口
    stackedWidget->setCurrentWidget(loginWindow);
}