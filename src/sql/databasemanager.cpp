#include <QDebug>
#include <QThread>

#include "databasemanager.h"
#include "database.h"
#include "src/event/eventbus.h"

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent), m_database(nullptr)
{
    // 注册自定义类型
    registerCustomTypes();
}

DatabaseManager::~DatabaseManager()
{
    //在这做防御性编程，~Database()已经调用了close()
    m_database->close();
}

void DatabaseManager::startLoading(const QString &userId)
{
    m_startLoading = true;//设置为true，通知子线程开始加载数据
    onLoadUserInfo(userId);
}

void DatabaseManager::stopLoading()
{
    m_startLoading = false;//设置为false，通知子线程停止加载数据
}

void DatabaseManager::onLoadUserInfo(const QString &userId) 
{
    if(!m_startLoading)
    {
        return;
    }
    qDebug() << "开始在子线程中加载用户信息,用户ID:" << userId;
    
    // 检查数据库是否已打开
    if (!m_database->isOpen()) {
        qCritical() << "数据库未打开";
        return;
    }
    
    // 加载用户信息
    UserInfo userInfo = m_database->getUserInfo(userId);
    if (m_startLoading) {
        emit loadUserInfo(userInfo);
        qDebug() << "用户信息加载完成";
    }
    QList<FriendInfo> friendList = m_database->getFriendList(userId);
    //加载好友信息
    if (m_startLoading) {
        emit loadFriendInfo(friendList);
        qDebug() << "好友信息加载完成";
    }
    //加载好友消息
    if (m_startLoading) {
        for(const FriendInfo &friendInfo : friendList)
        {
            onLoadFriendMessage(userId, friendInfo.friendId);
        }
        qDebug() << "好友消息加载完成";
    }
    if (m_startLoading) {
        emit loadFinished();
        qDebug() << "数据加载全部完成";
    }
}

void DatabaseManager::onLoadFriendMessage(const QString &userId, const QString &friendId)
{
    qDebug() << "开始在子线程中加载好友消息,用户ID:" << userId << "好友ID:" << friendId;
    // 加载好友消息
    QList<MessageInfo> messages = m_database->getMessages(userId, friendId);
    if (m_startLoading) {
        emit loadFriendMessage(friendId, messages);
        qDebug() << "好友" << friendId << "的消息加载完成，共" << messages.size() << "条消息";
    }
}

void DatabaseManager::onSaveFriendMessage(const MessageInfo &data)
{
    qDebug() << "开始在子线程中保存好友消息,用户ID:" << data.senderId << "好友ID:" << data.receiverId;
    // 保存好友消息
    m_database->saveMessage(data);
}

void DatabaseManager::initDatabase()//延迟初始化
{
    m_database = std::make_unique<Database>();//是为了在整个DatabaseManager生命周期内保持数据库连接的状态和管理数据库通信
    if(!m_database->init())
    {
        qCritical() << "数据库初始化失败";
        return;
    }
    qDebug() << "数据库初始化成功";
}