#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>
#include <QThread>
#include "src/sql/database.h"
#include "src/custom/struct.h"

class DataLoader : public QObject
{
    Q_OBJECT

public:
    explicit DataLoader(QObject *parent = nullptr);
    ~DataLoader();

    // 开始加载数据
    void startLoading(const QString &userId);
    // 停止加载数据
    void stopLoading();

    // 获取数据库实例（用于外部保存操作）
    Database* database() const { return m_database; }

signals:
    // 加载完成信号
    void userInfoLoaded(const MeChat::UserInfo &userInfo);
    void friendListLoaded(const QList<MeChat::FriendInfo> &friendList);
    void messagesLoaded(const QString &friendId, const QList<MeChat::messageData> &messages);
    void loadingFinished();

public slots:
    // 加载数据的槽函数
    void loadData(const QString &userId);
    void loadMessages(const QString &userId, const QString &friendId);

private:
    QThread *m_thread;
    Database *m_database;
    bool m_stopLoading;
};

#endif // DATALOADER_H
