#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

class MainWindow;//前向声明

class Database
{
public:
    Database();
    ~Database();
    QSqlDatabase database() const { return db; }// 获取数据库连接

    bool connect(QString connectDbName);
    void close();
    QString ConnectionName();// 数据库连接名称

private:
    QSqlDatabase db;
    QString connectionName; // 数据库连接名称
};

#endif // DATABASE_H