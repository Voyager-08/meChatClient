#ifndef DATABASETHREAD_H
#define DATABASETHREAD_H

#include <QThread>
#include <QObject>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QHash>
#include <QMutex>
#include <QMutexLocker>
#include <QString>  // 添加QString头文件

class DatabaseWorker; // 前向声明

class DatabaseThread : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseThread(QObject *parent = nullptr);
    ~DatabaseThread();

signals:
    // 查询完成信号，返回查询结果
    void queryCompleted(const QList<QHash<QString, QVariant>> &data);
    // 查询错误信号，返回错误信息
    void queryError(const QString &error);
    // 线程完成信号
    void finished();

public slots:
    // 执行数据库查询的槽函数
    void executeQuery(const QString &query);

private:
    QThread *workerThread;
    DatabaseWorker *databaseWorker;  // 添加worker指针
};

#endif // DATABASETHREAD_H