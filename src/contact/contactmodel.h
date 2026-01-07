// contactmodel.h
#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QString>

struct Contact ;
enum class ContactRoles ;

struct Contact {//昵称、头像路径、签名
    QString name;
    QString avatarPath;//头像路径
    QString message;//个性签名
    QString id; // 联系人ID
};
enum class ContactRoles {
    NameRole = Qt::UserRole + 1,      // 名字
    AvatarPathRole = Qt::UserRole + 2, // 头像路径（字符串）
    MessageRole = Qt::UserRole + 3,     // 签名
    IDRole = Qt::UserRole + 4           // ID
};

class ContactModel : public QAbstractListModel
{
public:
    explicit ContactModel(QObject *parent = nullptr);//显示构造函数
    // 重写抽象类QAbstractListModel中的方法
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;//返回行数
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;//返回数据
    QHash<int, QByteArray> roleNames() const override;//返回角色名称映射

public:
    void addContact(const Contact &contact);//添加联系人
    void addContact(const QString &name, const QString &avatarPath, const QString &message,const QString &ID);//添加联系人重载

private:
    QList<Contact> m_contacts;//联系人列表
};

#endif // CONTACTMODEL_H