#ifndef STRUCT_H
#define STRUCT_H

#include <QString>
#include <QDate>
#include <QDateTime>
#include <QMetaType>

namespace MeChat {

// 自定义枚举类型
enum class Sex { other = 0, male = 1, female = 2 };

// 当前用户信息结构体
struct UserInfo {
    QString userID;
    QString userNick;
    QString userAvatar;
    QString userMotto;
    QString userStatus;
    QString userEmail;
    QDate userRegistrationDate;
    Sex userSex;
    QString receiverID;
};

// 好友信息结构体
struct FriendInfo {
    QString friendNote;
    QString friendNick;
    QString friendId;
    QString avatarPath;
    QString motto;
    QString email;
};

// 网络数据结构体
struct messageData {
    QString senderId;
    QString receiverId;
    QString content;
    QDateTime timestamp;
};

// 内联函数：注册自定义类型
inline void registerCustomTypes()
{
    qRegisterMetaType<FriendInfo>("MeChat::FriendInfo");
    qRegisterMetaType<messageData>("MeChat::messageData");
}

} // namespace MeChat

// 全局命名空间中的类型定义（用于Model/View系统）
// 消息模型结构体
struct Model_Message {
    QString note;
    QString avatarPath;
    QString message;
    QDateTime time;
    QString senderID;
};

// 消息角色枚举
enum class MessageRoles {
    NoteRole = Qt::UserRole + 1,
    AvatarPathRole = Qt::UserRole + 2,
    MessageRole = Qt::UserRole + 3,
    TimeRole = Qt::UserRole + 4,
    SenderIDRole = Qt::UserRole + 5
};

// 联系人模型结构体
struct Model_Contact {
    QString name;
    QString avatarPath;
    QString id;
};

// 联系人角色枚举
enum class ContactRoles {
    NameRole = Qt::UserRole + 1,
    AvatarPathRole = Qt::UserRole + 2,
    IDRole = Qt::UserRole + 3
};

#endif // STRUCT_H
