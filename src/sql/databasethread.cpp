#include "databasethread.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

// 在匿名命名空间中定义DatabaseWorker，或者直接在实现文件中定义
class DatabaseWorker : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseWorker(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    void processQuery(const QString &query);

signals:
    void queryCompleted(const QList<QHash<QString, QVariant>> &data);
    void queryError(const QString &error);
    void finished();
};

void DatabaseWorker::processQuery(const QString &query)
{
    // 创建专用的数据库连接用于此线程
    QSqlDatabase threadDb = QSqlDatabase::addDatabase("QMYSQL", QString("thread_%1").arg(QString::number((qulonglong)QThread::currentThreadId())));
    threadDb.setDatabaseName("mechat");
    threadDb.setHostName("8.129.134.106");
    threadDb.setUserName("root_mechat");
    threadDb.setPassword("rootmechat");
    threadDb.setPort(3306);

    if (!threadDb.open()) {
        emit queryError(QString("数据库连接失败: %1").arg(threadDb.lastError().text()));
        emit finished();
        return;
    }

    QSqlQuery sqlQuery(threadDb);
    if (!sqlQuery.prepare(query)) {
        emit queryError(QString("SQL预处理失败: %1").arg(sqlQuery.lastError().text()));
        threadDb.close();
        QSqlDatabase::removeDatabase(QString("thread_%1").arg(QString::number((qulonglong)QThread::currentThreadId())));
        emit finished();
        return;
    }

    if (!sqlQuery.exec()) {
        emit queryError(QString("SQL执行失败: %1").arg(sqlQuery.lastError().text()));
        threadDb.close();
        QSqlDatabase::removeDatabase(QString("thread_%1").arg(QString::number((qulonglong)QThread::currentThreadId())));
        emit finished();
        return;
    }

    // 将查询结果转换为哈希列表
    QList<QHash<QString, QVariant>> resultData;
    QSqlRecord record = sqlQuery.record();

    while (sqlQuery.next()) {
        QHash<QString, QVariant> row;
        for (int i = 0; i < record.count(); ++i) {
            row[record.fieldName(i)] = sqlQuery.value(i);
        }
        resultData.append(row);
    }

    emit queryCompleted(resultData);
    threadDb.close();
    QSqlDatabase::removeDatabase(QString("thread_%1").arg(QString::number((qulonglong)QThread::currentThreadId())));
    emit finished();
}

DatabaseThread::DatabaseThread(QObject *parent) : QObject(parent)
{
    workerThread = new QThread();
    databaseWorker = new DatabaseWorker();  // 声明一个成员变量来存储worker引用
    databaseWorker->moveToThread(workerThread);

    connect(workerThread, &QThread::finished, databaseWorker, &QObject::deleteLater);
    
    // 连接worker的信号到当前对象的信号
    connect(databaseWorker, &DatabaseWorker::queryCompleted, this, &DatabaseThread::queryCompleted);
    connect(databaseWorker, &DatabaseWorker::queryError, this, &DatabaseThread::queryError);
    connect(databaseWorker, &DatabaseWorker::finished, this, &DatabaseThread::finished);
    
    // 连接executeQuery槽到worker的处理函数
    connect(this, &DatabaseThread::executeQuery, databaseWorker, &DatabaseWorker::processQuery);

    workerThread->start();
}

DatabaseThread::~DatabaseThread()
{
    workerThread->quit();
    workerThread->wait();
}

void DatabaseThread::executeQuery(const QString &query)
{
    // 使用Qt的元对象系统在工作线程上下文中调用worker的processQuery槽
    QMetaObject::invokeMethod(databaseWorker, "processQuery", Qt::QueuedConnection,
                              Q_ARG(const QString&, query));
}

