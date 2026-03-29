#include <QDebug>
#include <QVBoxLayout>
#include <QIcon>
#include <QTimer>
#include <QThread>

#include "mainwindow.h"
#include "src/core/login/loginwindow.h"
#include "src/core/chat/chatwindow.h"
#include "src/thread/network/networkmanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      loginWindow(nullptr),
      chatWindow(nullptr),
      networkManager(nullptr),
      networkThread(nullptr)
{
    initializeUI();
    connectSignals();
    linkServer();
}

MainWindow::~MainWindow()
{
    if (networkThread) {
        networkThread->quit();
        networkThread->wait();
        delete networkThread;
        networkThread = nullptr;
    }
}

void MainWindow::initializeUI()
{
    
    // 设置主窗口属性
    setWindowTitle("meChat");
    setWindowIcon(QIcon(":/images/10.png"));  // 设置窗口图标
    setGeometry(100, 100, 700, 450);
    setMouseTracking(true);  // 允许鼠标跟踪
    
    // 创建栈式窗口作为中心组件
    stackedWidget = new QStackedWidget(this);
    
    // 使用 QVBoxLayout 来管理布局（QWidget 没有 setCentralWidget）
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);  // 设置边距为0，使 stackedWidget 充满整个窗口
    layout->addWidget(stackedWidget);
    setLayout(layout);
    
    // 创建网络线程
    networkThread = new QThread(this);
    networkManager = new NetworkManager();
    networkManager->moveToThread(networkThread);
    
    // 启动网络线程
    networkThread->start();
    
    // 创建登录窗口
    loginWindow = new LoginWindow(this, networkManager);
    stackedWidget->addWidget(loginWindow);
    
    // 初始显示登录页面
    stackedWidget->setCurrentWidget(loginWindow);
    setFixedSize(700, 450); // 设置主窗口大小与登录窗口一致
}

void MainWindow::connectSignals()
{
    // 连接登录窗口的登录成功信号
    connect(loginWindow, &LoginWindow::userLoggedIn, this, &MainWindow::onUserLoggedIn);
}

void MainWindow::linkServer()
{
    if(networkManager->isConnected())
    {
        qDebug() << "已连接到服务器";
        return;
    }
    // 使用队列连接确保在网络线程中执行
    QMetaObject::invokeMethod(networkManager, &NetworkManager::connectToServer, Qt::QueuedConnection);
}


void MainWindow::onUserLoggedIn(const QString &userID, const QString &password)
{
    
    qDebug() << "主窗口: 用户已登录，ID=" << userID;
    
    // 如果之前已有聊天窗口，先删除
    if (chatWindow != nullptr) {
        stackedWidget->removeWidget(chatWindow);
        delete chatWindow;
        chatWindow = nullptr;
    }

    // 创建新的聊天窗口，传入网络管理器
    chatWindow = new ChatWindow(userID, this, networkManager);
    // 设置 ChatWindow 充满整个 stackedWidget（重要！否则会看到 MainWindow 的框架）
    chatWindow->setGeometry(0, 0, stackedWidget->width(), stackedWidget->height());
    chatWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(850, 600); // 设置主窗口最小尺寸，确保聊天窗口有足够空间显示
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX); // 取消最大尺寸限制，允许用户调整窗口大小
    // 连接退出登录信号
    connect(chatWindow, &ChatWindow::exitLogin, this, &MainWindow::onUserLoggedOut);
    
    // 添加到栈式窗口并设置为当前窗口
    stackedWidget->addWidget(chatWindow);
    stackedWidget->setCurrentWidget(chatWindow);

    show();  // 重新显示窗口使标志改变生效
}

void MainWindow::onUserLoggedOut()
{
    qDebug() << "主窗口: 用户已退出登录";
    setFixedSize(this->size());
    // 删除聊天窗口
    if (chatWindow != nullptr) {
        stackedWidget->removeWidget(chatWindow);
        delete chatWindow;
        chatWindow = nullptr;
    }
    
    show();  // 重新显示窗口使标志改变生效
    
    // 清空登录表单，返回登录页面
    stackedWidget->setCurrentWidget(loginWindow);
}