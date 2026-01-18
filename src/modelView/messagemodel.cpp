// messagemodel.cpp
#include "messagemodel.h"
#include <QPixmap>
#include <QFile>
#include <QDebug>  


MessageModel::MessageModel(QObject *parent)// 显示构造函数，必须传父对象指针
    : QAbstractListModel(parent)// 调用父类构造函数
{
    
}

int MessageModel::rowCount(const QModelIndex &parent) const// 获取行数
{
    Q_UNUSED(parent);  // 明确标记未使用的参数，避免编译器警告
    return m_messages.count();// 返回消息列表数量
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_messages.size())
        return QVariant(); //  返回一个空的QVariant对象，表示无效或未找到的数据

    const Model_Message &message = m_messages.at(index.row()); //  获取指定索引位置的消息引用

    switch (role) { //  根据不同的角色(role)返回对应的数据
    case static_cast<int>(MessageRoles::NoteRole): //  当角色为姓名角色时，返回消息的姓名
        return message.note;
    case static_cast<int>(MessageRoles::AvatarPathRole): //  当角色为头像路径角色时，返回消息的头像路径
        return message.avatarPath;
    case static_cast<int>(MessageRoles::MessageRole): //  当角色为消息角色时，返回消息
        return message.message;
    case static_cast<int>(MessageRoles::TimeRole): //  当角色为时间角色时，返回消息时间
        return message.time;
    case static_cast<int>(MessageRoles::SenderIDRole): //  当角色为发送者ID角色时，返回ID
        return message.senderID;
    case Qt::DisplayRole: //  当角色为默认显示角色时，返回消息的姓名
        return message.note;
    default: //  对于其他未定义的角色，返回空的QVariant对象
        return QVariant();
    }
}

QHash<int, QByteArray> MessageModel::roleNames() const
{
    QHash<int, QByteArray> roles; //  创建一个QHash<int, QByteArray>类型的哈希表，用于存储角色名称映射
    roles[static_cast<int>(MessageRoles::NoteRole)] = "note"; //  添加备注角色映射
    roles[static_cast<int>(MessageRoles::AvatarPathRole)] = "avatarPath"; //  添加头像路径角色映射
    roles[static_cast<int>(MessageRoles::MessageRole)] = "message";//  添加消息角色映射
    roles[static_cast<int>(MessageRoles::TimeRole)] = "time";//  添加时间角色映射
    roles[static_cast<int>(MessageRoles::SenderIDRole)] = "id";//  添加ID角色映射
    return roles; //  返回填充好的角色名称映射哈希表
}

void MessageModel::addMessage(const Model_Message &message)
{ //  添加消息到模型中的实现函数
    beginInsertRows(QModelIndex(), rowCount(), rowCount()); //  通知视图即将插入新行，插入位置为当前最后一行之后
    m_messages.append(message); //  将新的消息添加到消息列表末尾
    endInsertRows(); //  通知视图插入行操作完成
}

void MessageModel::addMessage(const QString &note, const QString &avatarPath, const QString &message, const QDateTime &time, const QString &id)
{
    Model_Message newMessage{ note, avatarPath, message, time, id }; //  创建一个Model_Message对象，并使用传入的参数进行初始化
    beginInsertRows(QModelIndex(), rowCount(), rowCount()); //  通知视图即将插入新行
    m_messages.append(newMessage); //  将新的消息添加到消息列表末尾
    endInsertRows(); //  通知视图插入行操作完成
}

void MessageModel::clearMessages()
{
    if (!m_messages.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, m_messages.size() - 1); //  通知视图即将删除所有行
        m_messages.clear(); //  清空消息列表
        endRemoveRows(); //  通知视图删除行操作完成
    }
}