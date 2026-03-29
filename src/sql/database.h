#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QSqlError>
#include <QUuid>
#include <QList>
#include <QDebug>
#include <QDateTime>
#include "src/custom/struct.h"

using namespace MeChat;

class Database
{
public:
    Database();
    ~Database();

    QSqlDatabase database() const { return db; }
    void connect(const QString &dbName);
    void close();
    QString connectionName() const;
    bool isOpen() const { return db.isOpen(); }

    // 用户相关操作
    bool saveUserInfo(const UserInfo &userInfo);
    UserInfo getUserInfo(const QString &userId);
    bool userExists(const QString &userId);

    // 好友相关操作
    bool saveFriendship(const QString &userId, const FriendInfo &friendInfo);
    QList<FriendInfo> getFriendList(const QString &userId);
    bool deleteFriendship(const QString &userId, const QString &friendId);

    // 消息相关操作
    bool saveMessage(const messageData &message);
    QList<messageData> getMessages(const QString &userId, const QString &friendId);
    bool markMessageAsRead(const QString &messageId);

    // 初始化数据库（创建表）
    bool init();

private:
    QSqlDatabase db;
    QString dbPath;
    QString m_connectionName;

    bool openDatabase();
    void closeDatabase();
    bool createTables();
};

#endif // DATABASE_H
