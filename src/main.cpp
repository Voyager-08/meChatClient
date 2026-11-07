#include <QApplication>
#include <QThread>
#include <QLoggingCategory>
#include "src/core/main/mainwindow.h"
#include "src/event/eventbus.h"
#include "src/network/networkmanager.h"
#include "src/sql/databasemanager.h"

int main(int argc, char *argv[])
{
#ifdef QT_NO_DEBUG
    QLoggingCategory::setFilterRules("*.debug=false");
#endif

    QApplication app(argc, argv);
    
    // 创建事件总线（Composition Root）
    EventBus eventBus;

    // 数据加载器（在子线程中运行）
    QThread dataLoaderThread;
    DatabaseManager databaseManager;
    databaseManager.moveToThread(&dataLoaderThread);
    QMetaObject::invokeMethod(&databaseManager, &DatabaseManager::initDatabase, Qt::QueuedConnection);
    dataLoaderThread.start();
    
    // 网络管理器（在子线程中运行）
    QThread networkThread;
    NetworkManager networkManager;
    networkManager.moveToThread(&networkThread);
    QMetaObject::invokeMethod(&networkManager, &NetworkManager::initNetwork, Qt::QueuedConnection);
    networkThread.start();

    // 主窗口（Composition Root，负责组装所有依赖）
    MainWindow window(&eventBus, &networkManager, &databaseManager);

    window.show();
    int ret = app.exec();
    
    // 等待线程退出
    networkThread.quit();
    networkThread.wait(3000);
    dataLoaderThread.quit();
    dataLoaderThread.wait(3000);

    return ret;
}