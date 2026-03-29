#include "dataloader.h"
#include <QDebug>

using namespace MeChat;

DataLoader::DataLoader(QObject *parent) : QObject(parent), m_database(nullptr), m_stopLoading(false)
{
    // 创建数据库实例
    m_database = new Database();
    
    // 创建子线程
    m_thread = new QThread(this);
    // 将当前对象移到子线程
    this->moveToThread(m_thread);
    // 启动线程
    m_thread->start();
}

DataLoader::~DataLoader()
{
    // 停止线程
    if (m_thread->isRunning()) {
        m_thread->quit();
        m_thread->wait();
    }
    delete m_thread;
    
    // 关闭数据库
    if (m_database) {
        m_database->close();
        delete m_database;
        m_database = nullptr;
    }
}

void DataLoader::startLoading(const QString &userId)
{
    m_stopLoading = false;
    // 在子线程中加载数据
    QMetaObject::invokeMethod(this, "loadData", Qt::QueuedConnection, Q_ARG(QString, userId));
}

void DataLoader::stopLoading()
{
    m_stopLoading = true;
}

void DataLoader::loadData(const QString &userId)
{
    qDebug() << "开始在子线程中加载数据，用户ID:" << userId;
    
    // 初始化数据库（在子线程中）
    if (!m_database->init()) {
        qCritical() << "数据库初始化失败";
        return;
    }
    
    // 加载用户信息
    UserInfo userInfo = m_database->getUserInfo(userId);
    if (!m_stopLoading) {
        emit userInfoLoaded(userInfo);
        qDebug() << "用户信息加载完成";
    }
    
    // 加载好友列表
    QList<FriendInfo> friendList = m_database->getFriendList(userId);
    if (!m_stopLoading) {
        emit friendListLoaded(friendList);
        qDebug() << "好友列表加载完成，共" << friendList.size() << "个好友";
    }
    
    // 加载每个好友的消息
    for (const FriendInfo &friendInfo : friendList) {
        if (m_stopLoading) break;
        
        QList<messageData> messages = m_database->getMessages(userId, friendInfo.friendId);
        if (!messages.isEmpty()) {
            emit messagesLoaded(friendInfo.friendId, messages);
            qDebug() << "好友" << friendInfo.friendId << "的消息加载完成，共" << messages.size() << "条消息";
        }
    }
    
    if (!m_stopLoading) {
        emit loadingFinished();
        qDebug() << "数据加载全部完成";
    }
}

void DataLoader::loadMessages(const QString &userId, const QString &friendId)
{
    qDebug() << "开始在子线程中加载好友消息，用户ID:" << userId << "好友ID:" << friendId;
    
    // 加载好友消息
    QList<messageData> messages = m_database->getMessages(userId, friendId);
    if (!m_stopLoading) {
        emit messagesLoaded(friendId, messages);
        qDebug() << "好友" << friendId << "的消息加载完成，共" << messages.size() << "条消息";
    }
}
