// addcontactmodel.h
#ifndef ADDCONTACTMODEL_H
#define ADDCONTACTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QString>
#include "src/custom/struct.h"

class AddContactModel : public QAbstractListModel
{
public:
    explicit AddContactModel(QObject *parent = nullptr);//显示构造函数
    // 重写抽象类QAbstractListModel中的方法
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;//返回行数
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;//返回数据
    QHash<int, QByteArray> roleNames() const override;//返回角色名称映射

public:
    void addContact(const Model_Contact &contact);//添加联系人
    void addContact(const QString &name, const QString &avatarPath,const QString &id);//添加联系人重载
    void clearContacts();//清空联系人列表

private:
    QList<Model_Contact> m_contacts;//联系人列表
};

#endif // ADDCONTACTMODEL_H