// contactmodel.cpp
#include "contactmodel.h"
#include <QPixmap>
#include <QFile>
#include <QDebug>  


ContactModel::ContactModel(QObject *parent)// 显示构造函数，必须传父对象指针
    : QAbstractListModel(parent)// 调用父类构造函数
{
    
}

int ContactModel::rowCount(const QModelIndex &parent) const// 获取行数
{
    Q_UNUSED(parent);  // 明确标记未使用的参数，避免编译器警告
    return m_contacts.count();// 返回联系人数量
}

QVariant ContactModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_contacts.size())
        return QVariant(); //  返回一个空的QVariant对象，表示无效或未找到的数据

    const Model_Contact &contact = m_contacts.at(index.row()); //  获取指定索引位置的联系人引用

    switch (role) { //  根据不同的角色(role)返回对应的数据
    case static_cast<int>(ContactRoles::NameRole): //  当角色为姓名角色时，返回联系人的姓名
        return contact.name;
    case static_cast<int>(ContactRoles::AvatarPathRole): //  当角色为头像路径角色时，返回联系人的头像路径
        return contact.avatarPath;
    case static_cast<int>(ContactRoles::IDRole): //  当角色为ID角色时，返回联系人的ID
        return contact.id;
    case Qt::DisplayRole: //  当角色为默认显示角色时，返回联系人的姓名
        return contact.name;
    default: //  对于其他未定义的角色，返回空的QVariant对象
        return QVariant();
    }
}

QHash<int, QByteArray> ContactModel::roleNames() const
{
    QHash<int, QByteArray> roles; //  创建一个QHash<int, QByteArray>类型的哈希表，用于存储角色名称映射
    roles[static_cast<int>(ContactRoles::NameRole)] = "name"; //  添加姓名角色映射
    roles[static_cast<int>(ContactRoles::AvatarPathRole)] = "avatarPath"; //  添加头像路径角色映射
    roles[static_cast<int>(ContactRoles::IDRole)] = "id"; //  添加ID角色映射
    return roles; //  返回填充好的角色名称映射哈希表
}

void ContactModel::addContact(const Model_Contact &contact)
{ //  添加联系人到模型中的实现函数
    beginInsertRows(QModelIndex(), rowCount(), rowCount()); //  通知视图即将插入新行，插入位置为当前最后一行之后
    m_contacts.append(contact); //  将新的联系人添加到联系人列表末尾
    endInsertRows(); //  通知视图插入行操作完成
}

void ContactModel::addContact(const QString &name, const QString &avatarPath,const QString &ID)
{
    Model_Contact contacts{ name, avatarPath,ID}; //  创建一个Contact对象，并使用传入的参数进行初始化
    m_contacts.append(contacts); //  将新的联系人添加到联系人列表末尾
}

void ContactModel::clearContacts()
{
    beginResetModel(); //  通知视图即将重置模型
    m_contacts.clear(); //  清空联系人列表
    endResetModel(); //  通知视图重置模型操作完成
}
