#include "database.h"
#include <QFile>
#include <QDir>
#include <QUuid>
#include <QSqlQuery>
#include <QDebug>
#include <QDateTime>
#include <QSqlError>

Database::Database()
{
    
}

Database::~Database()
{
    close();
}

QString Database::connectionName() const
{
    return m_connectionName;
}

void Database::connect(const QString &dbName)
{
    // 生成唯一连接名（避免冲突）
    QString newConnectionName = "SQLITE_" + QUuid::createUuid().toString(QUuid::Id128).replace("{", "").replace("}", "").replace("-", "_");

    // 创建新的 SQLite 连接
    db = QSqlDatabase::addDatabase("QSQLITE", newConnectionName);
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qCritical() << "SQLite database打开失败:" << dbName
                    << "Error:" << db.lastError().text();
        return;
    }

    m_connectionName = newConnectionName;
    qDebug() << "SQLite database 打开成功:"
             << dbName << "(connection:" << m_connectionName << ")";

    // 测试数据输出
    QSqlQuery query(db);
    if (query.exec("SELECT name FROM sqlite_master WHERE type='table';")) {
        qInfo() << "数据库中的表:";
        while (query.next()) {
            qInfo() << " - " << query.value(0).toString();
        }
    } else {
        qWarning() << "查询数据库表失败:" << query.lastError().text();
    }
}

void Database::close()
{
    if (!m_connectionName.isEmpty()) {
        //先关闭物理连接
        if (db.isOpen()) {
            db.close();
        }
        //将成员变量置为无效状态（解除对连接的引用）
        db = QSqlDatabase();
        //从全局注册表中移除（必须在所有副本销毁后调用）
        QSqlDatabase::removeDatabase(m_connectionName);
        m_connectionName.clear();
        qDebug() << "SQLite数据库连接已删除";
    }
}

bool Database::init()
{

    // 设置数据库路径
    if (!setDbPath()) {
        return false;
    }
    // 连接数据库
    connect(dbPath);
    //打开数据库
    if (!isOpen()) {
        return false;
    }
    // 是否创建表，否则创建
    if (!createTables()) {
        return false;
    }
    // 创建成功
    return true;
}

bool Database::createTables()
{
    QSqlQuery query(db);

    // 创建users表
    QString createUsersTable = "CREATE TABLE IF NOT EXISTS users ("
                             "user_id TEXT PRIMARY KEY, "
                             "user_nick TEXT, "
                             "password TEXT DEFAULT '000000', "
                             "email TEXT, "
                             "created_at TEXT DEFAULT CURRENT_TIMESTAMP, "
                             "motto TEXT DEFAULT '欢迎来到 mechat', "
                             "avatar_path TEXT, "
                             "sex INTEGER DEFAULT 0 "
                             ");";

    if (!query.exec(createUsersTable)) {
        qDebug() << "创建users表失败:" << query.lastError().text();
        return false;
    }

    // 创建friendships表
    QString createFriendshipsTable = "CREATE TABLE IF NOT EXISTS friendships ("
                                   "user_id TEXT NOT NULL, "
                                   "friend_id TEXT NOT NULL, "
                                   "created_at TEXT DEFAULT CURRENT_TIMESTAMP, "
                                   "friend_note TEXT, "
                                   "PRIMARY KEY (user_id, friend_id) "
                                   ");";

    if (!query.exec(createFriendshipsTable)) {
        qDebug() << "创建friendships表失败:" << query.lastError().text();
        return false;
    }

    // 创建messages表
    QString createMessagesTable = "CREATE TABLE IF NOT EXISTS messages ("
                                "message_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "sender_id TEXT NOT NULL, "
                                "receiver_id TEXT NOT NULL, "
                                "datetime TEXT DEFAULT CURRENT_TIMESTAMP, "
                                "comment TEXT, "
                                "message_type TEXT DEFAULT 'text', "
                                "is_read BOOLEAN DEFAULT 0 "
                                ");";

    if (!query.exec(createMessagesTable)) {
        qDebug() << "创建messages表失败:" << query.lastError().text();
        return false;
    }

    qDebug() << "数据库表创建成功";
    return true;
}

bool Database::setDbPath()
{
    dbPath = QDir::currentPath() + "/sqlite/mechat.sqlite";
    // 确保sqlite目录存在
    QDir dir(QDir::currentPath() + "/sqlite");
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "创建sqlite目录失败";
            return false;
        }
    }
    return true;
}

bool Database::saveUserInfo(const UserInfo &userInfo)
{
    QSqlQuery query(db);

    // UPSERT: 插入新用户，若 user_id 冲突则更新指定字段
    query.prepare(
        "INSERT INTO users (user_id, user_nick, email, motto, avatar_path, sex) "
        "VALUES (?, ?, ?, ?, ?, ?) "
        "ON CONFLICT(user_id) DO UPDATE SET "
        "  user_nick   = excluded.user_nick, "
        "  email       = excluded.email, "
        "  motto       = excluded.motto, "
        "  avatar_path = excluded.avatar_path, "
        "  sex         = excluded.sex"
    );

    query.addBindValue(userInfo.userID);
    query.addBindValue(userInfo.userNick);
    query.addBindValue(userInfo.userEmail);
    query.addBindValue(userInfo.userMotto);
    query.addBindValue(userInfo.userAvatar);
    query.addBindValue(static_cast<int>(userInfo.userSex));

    if (!query.exec()) {
        qDebug() << "保存用户信息失败:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::userExists(const QString &userId)
{
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM users WHERE user_id = ?");
    query.addBindValue(userId);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

bool Database::saveFriendship(const QString &userId, const FriendInfo &friendInfo)
{
    QSqlQuery query(db);

    // 检查好友关系是否存在
    query.prepare("SELECT COUNT(*) FROM friendships WHERE user_id = ? AND friend_id = ?");
    query.addBindValue(userId);
    query.addBindValue(friendInfo.friendId);

    if (query.exec() && query.next() && query.value(0).toInt() > 0) {
        // 更新好友信息
        query.prepare("UPDATE friendships SET friend_note = ? WHERE user_id = ? AND friend_id = ?");
        query.addBindValue(friendInfo.friendNote);
        query.addBindValue(userId);
        query.addBindValue(friendInfo.friendId);
    } else {
        // 插入新好友
        query.prepare("INSERT INTO friendships (user_id, friend_id, friend_note) VALUES (?, ?, ?)");
        query.addBindValue(userId);
        query.addBindValue(friendInfo.friendId);
        query.addBindValue(friendInfo.friendNote);
    }

    if (!query.exec()) {
        qDebug() << "保存好友关系失败:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::deleteFriendship(const QString &userId, const QString &friendId)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM friendships WHERE user_id = ? AND friend_id = ?");
    query.addBindValue(userId);
    query.addBindValue(friendId);

    if (!query.exec()) {
        qDebug() << "删除好友关系失败:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::saveMessage(const MessageInfo &messageInfo)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO messages (sender_id, receiver_id, datetime, comment, message_type, is_read) VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(messageInfo.senderId);
    query.addBindValue(messageInfo.receiverId);
    query.addBindValue(messageInfo.timestamp.toString(Qt::ISODate));
    query.addBindValue(messageInfo.content);
    query.addBindValue("text");
    query.addBindValue(0);

    if (!query.exec()) {
        qDebug() << "保存消息失败:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::markMessageAsRead(const QString &messageId)
{
    QSqlQuery query(db);
    query.prepare("UPDATE messages SET is_read = 1 WHERE message_id = ?");
    query.addBindValue(messageId);

    if (!query.exec()) {
        qDebug() << "标记消息为已读失败:" << query.lastError().text();
        return false;
    }

    return true;
}

QList<MessageInfo> Database::getMessages(const QString &userId, const QString &friendId)
{
    QList<MessageInfo> messageInfoList;

    QSqlQuery query(db);
    query.prepare("SELECT sender_id, receiver_id, comment, datetime FROM messages WHERE (sender_id = ? AND receiver_id = ?) OR (sender_id = ? AND receiver_id = ?) ORDER BY datetime ASC");
    query.addBindValue(userId);
    query.addBindValue(friendId);
    query.addBindValue(friendId);
    query.addBindValue(userId);

    if (query.exec()) {
        while (query.next()) {
            MessageInfo messageInfo;
            messageInfo.senderId = query.value(0).toString();
            messageInfo.receiverId = query.value(1).toString();
            messageInfo.content = query.value(2).toString();
            messageInfo.timestamp = QDateTime::fromString(query.value(3).toString(), Qt::ISODate);
            messageInfoList.append(messageInfo);
        }
    }

    return messageInfoList;
}

QList<FriendInfo> Database::getFriendList(const QString &userId)
{
    QList<FriendInfo> friendInfoList;

    QSqlQuery query(db);
    query.prepare("SELECT friend_id, friend_note FROM friendships WHERE user_id = ?");
    query.addBindValue(userId);

    if (query.exec()) {
        while (query.next()) {
            FriendInfo friendInfo;
            friendInfo.friendId = query.value(0).toString();
            friendInfo.friendNote = query.value(1).toString();
            friendInfoList.append(friendInfo);
        }
    }

    return friendInfoList;
}

UserInfo Database::getUserInfo(const QString &userId)
{
    UserInfo userInfo;
    userInfo.userID = userId;

    QSqlQuery query(db);
    query.prepare("SELECT user_nick, email, motto, avatar_path, sex FROM users WHERE user_id = ?");
    query.addBindValue(userId);

    if (query.exec() && query.next()) {
        userInfo.userNick = query.value(0).toString();
        userInfo.userEmail = query.value(1).toString();
        userInfo.userMotto = query.value(2).toString();
        userInfo.userAvatar = query.value(3).toString();
        userInfo.userSex = static_cast<Sex>(query.value(4).toInt());
    }

    return userInfo;
}
