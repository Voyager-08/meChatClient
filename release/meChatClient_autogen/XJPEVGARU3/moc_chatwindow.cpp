/****************************************************************************
** Meta object code from reading C++ file 'chatwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/core/chat/chatwindow.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSChatWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSChatWindowENDCLASS = QtMocHelpers::stringData(
    "ChatWindow",
    "exitLogin",
    "",
    "userID",
    "avatarLoaded",
    "pixmap",
    "addFriendId",
    "friendId",
    "searchAddFriendStr",
    "friendStr",
    "onUserInfoLoaded",
    "MeChat::UserInfo",
    "userInfo",
    "onFriendListLoaded",
    "QList<MeChat::FriendInfo>",
    "friendList",
    "onMessagesLoaded",
    "QList<MeChat::messageData>",
    "messages",
    "onLoadingFinished"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSChatWindowENDCLASS_t {
    uint offsetsAndSizes[40];
    char stringdata0[11];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[13];
    char stringdata5[7];
    char stringdata6[12];
    char stringdata7[9];
    char stringdata8[19];
    char stringdata9[10];
    char stringdata10[17];
    char stringdata11[17];
    char stringdata12[9];
    char stringdata13[19];
    char stringdata14[26];
    char stringdata15[11];
    char stringdata16[17];
    char stringdata17[27];
    char stringdata18[9];
    char stringdata19[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSChatWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSChatWindowENDCLASS_t qt_meta_stringdata_CLASSChatWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "ChatWindow"
        QT_MOC_LITERAL(11, 9),  // "exitLogin"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 6),  // "userID"
        QT_MOC_LITERAL(29, 12),  // "avatarLoaded"
        QT_MOC_LITERAL(42, 6),  // "pixmap"
        QT_MOC_LITERAL(49, 11),  // "addFriendId"
        QT_MOC_LITERAL(61, 8),  // "friendId"
        QT_MOC_LITERAL(70, 18),  // "searchAddFriendStr"
        QT_MOC_LITERAL(89, 9),  // "friendStr"
        QT_MOC_LITERAL(99, 16),  // "onUserInfoLoaded"
        QT_MOC_LITERAL(116, 16),  // "MeChat::UserInfo"
        QT_MOC_LITERAL(133, 8),  // "userInfo"
        QT_MOC_LITERAL(142, 18),  // "onFriendListLoaded"
        QT_MOC_LITERAL(161, 25),  // "QList<MeChat::FriendInfo>"
        QT_MOC_LITERAL(187, 10),  // "friendList"
        QT_MOC_LITERAL(198, 16),  // "onMessagesLoaded"
        QT_MOC_LITERAL(215, 26),  // "QList<MeChat::messageData>"
        QT_MOC_LITERAL(242, 8),  // "messages"
        QT_MOC_LITERAL(251, 17)   // "onLoadingFinished"
    },
    "ChatWindow",
    "exitLogin",
    "",
    "userID",
    "avatarLoaded",
    "pixmap",
    "addFriendId",
    "friendId",
    "searchAddFriendStr",
    "friendStr",
    "onUserInfoLoaded",
    "MeChat::UserInfo",
    "userInfo",
    "onFriendListLoaded",
    "QList<MeChat::FriendInfo>",
    "friendList",
    "onMessagesLoaded",
    "QList<MeChat::messageData>",
    "messages",
    "onLoadingFinished"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSChatWindowENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    1 /* Public */,
       4,    1,   65,    2, 0x06,    3 /* Public */,
       6,    1,   68,    2, 0x06,    5 /* Public */,
       8,    1,   71,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      10,    1,   74,    2, 0x08,    9 /* Private */,
      13,    1,   77,    2, 0x08,   11 /* Private */,
      16,    2,   80,    2, 0x08,   13 /* Private */,
      19,    0,   85,    2, 0x08,   16 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QPixmap,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    9,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 17,    7,   18,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ChatWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSChatWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSChatWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSChatWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ChatWindow, std::true_type>,
        // method 'exitLogin'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'avatarLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPixmap, std::false_type>,
        // method 'addFriendId'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'searchAddFriendStr'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onUserInfoLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const MeChat::UserInfo &, std::false_type>,
        // method 'onFriendListLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<MeChat::FriendInfo> &, std::false_type>,
        // method 'onMessagesLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<MeChat::messageData> &, std::false_type>,
        // method 'onLoadingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ChatWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChatWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->exitLogin((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->avatarLoaded((*reinterpret_cast< std::add_pointer_t<QPixmap>>(_a[1]))); break;
        case 2: _t->addFriendId((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->searchAddFriendStr((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->onUserInfoLoaded((*reinterpret_cast< std::add_pointer_t<MeChat::UserInfo>>(_a[1]))); break;
        case 5: _t->onFriendListLoaded((*reinterpret_cast< std::add_pointer_t<QList<MeChat::FriendInfo>>>(_a[1]))); break;
        case 6: _t->onMessagesLoaded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<MeChat::messageData>>>(_a[2]))); break;
        case 7: _t->onLoadingFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ChatWindow::*)(QString );
            if (_t _q_method = &ChatWindow::exitLogin; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ChatWindow::*)(QPixmap );
            if (_t _q_method = &ChatWindow::avatarLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ChatWindow::*)(const QString & );
            if (_t _q_method = &ChatWindow::addFriendId; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ChatWindow::*)(const QString & );
            if (_t _q_method = &ChatWindow::searchAddFriendStr; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *ChatWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSChatWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ChatWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void ChatWindow::exitLogin(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ChatWindow::avatarLoaded(QPixmap _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ChatWindow::addFriendId(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ChatWindow::searchAddFriendStr(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
