#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>
#include "src/custom/struct.h"
using namespace chat;
class QThread;
class Database;

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);//explicit构造函数，防止隐式类型转换
    virtual ~DatabaseManager();//析构函数，确保数据库连接被关闭

signals:
    void loadUserInfo(const UserInfo &userInfo);
    void loadFriendInfo(const QList<FriendInfo> &friendList);
    void loadFriendMessage(const QString &friendId, const QList<MessageInfo> &messages);
    void loadFinished();

public slots:
    // 加载数据的槽函数
    void onLoadUserInfo(const QString &userId);
    void onLoadFriendMessage(const QString &userId, const QString &friendId);
    void onSaveFriendMessage(const MessageInfo &data);
    void initDatabase();
    void startLoading(const QString &userId);
    void stopLoading();

private:
    std::unique_ptr<Database> m_database;//使用智能指针管理数据库实例，确保在对象销毁时自动关闭连接
    std::atomic<bool> m_startLoading{false};//用于开始加载数据的原子标志位，确保在多线程环境下安全操作，默认值为false，表示未开始加载数据
};

#endif // DATALOADER_H
