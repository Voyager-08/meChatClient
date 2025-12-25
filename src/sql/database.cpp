#include "database.h"
#include <QSqlRecord>


Database::Database()
{
}

Database::~Database()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "Database connection" << connectionName << "closed in destructor";
    }
}

bool Database::connect(QString connectDbName)
{
    // 如果已经存在相同名称的连接，先关闭并移除它
    if (QSqlDatabase::contains(connectDbName)) {
        QSqlDatabase existingDb = QSqlDatabase::database(connectDbName);
        if (existingDb.isOpen()) {
            existingDb.close();
            qDebug() << "Closed existing database connection:" << connectDbName;
        }
    }
    
    connectionName = connectDbName;// 数据库连接名称
    // 为每个连接创建唯一的连接名称
    db = QSqlDatabase::addDatabase("QMYSQL", connectDbName);// 使用 MySQL 数据库驱动
    db.setDatabaseName("mechat");// 数据库名称
    db.setHostName("127.0.0.1");// 数据库服务器地址
    db.setUserName("mechat_user");// 数据库用户名
    db.setPassword("mechatuser");// 数据库密码
    db.setPort(3306);
    
    if (!db.open()) {
        qCritical() << "连接名:" << connectionName << "数据库" << db.lastError().text() << "打开失败！";
        return false;
    }
    
    qDebug() << "连接名:" << connectionName << "数据库" << db.databaseName() << "打开成功！";
    return true;
}

void Database::close()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "MySql数据库连接：" << connectionName << "已关闭";
    }
}

QString Database::ConnectionName()
{
    return connectionName;
}
