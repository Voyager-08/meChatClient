// messagemodel.h
#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QString>
#include <QDateTime>
#include "src/custom/struct.h"

class MessageModel : public QAbstractListModel
{
public:
    explicit MessageModel(QObject *parent = nullptr);//显示构造函数
    // 重写抽象类QAbstractListModel中的方法
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;//返回行数
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;//返回数据
    QHash<int, QByteArray> roleNames() const override;//返回角色名称映射

public:
    void addMessage(const Model_Message &message);//添加消息
    void addMessage(const QString &name, const QString &avatarPath, const QString &message, const QDateTime &time, const QString &ID);//添加消息重载
    void updateMessage(const QString &senderID, const QString &message, const QDateTime &time);//更新消息
    void clearMessages();//清空消息列表

private:
    QList<Model_Message> m_messages;//消息列表
};

#endif // MESSAGEMODEL_H
