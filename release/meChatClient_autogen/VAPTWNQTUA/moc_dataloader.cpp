/****************************************************************************
** Meta object code from reading C++ file 'dataloader.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/thread/dataLoad/dataloader.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dataloader.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSDataLoaderENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSDataLoaderENDCLASS = QtMocHelpers::stringData(
    "DataLoader",
    "userInfoLoaded",
    "",
    "MeChat::UserInfo",
    "userInfo",
    "friendListLoaded",
    "QList<MeChat::FriendInfo>",
    "friendList",
    "messagesLoaded",
    "friendId",
    "QList<MeChat::messageData>",
    "messages",
    "loadingFinished",
    "loadData",
    "userId",
    "loadMessages"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSDataLoaderENDCLASS_t {
    uint offsetsAndSizes[32];
    char stringdata0[11];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[17];
    char stringdata4[9];
    char stringdata5[17];
    char stringdata6[26];
    char stringdata7[11];
    char stringdata8[15];
    char stringdata9[9];
    char stringdata10[27];
    char stringdata11[9];
    char stringdata12[16];
    char stringdata13[9];
    char stringdata14[7];
    char stringdata15[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSDataLoaderENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSDataLoaderENDCLASS_t qt_meta_stringdata_CLASSDataLoaderENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "DataLoader"
        QT_MOC_LITERAL(11, 14),  // "userInfoLoaded"
        QT_MOC_LITERAL(26, 0),  // ""
        QT_MOC_LITERAL(27, 16),  // "MeChat::UserInfo"
        QT_MOC_LITERAL(44, 8),  // "userInfo"
        QT_MOC_LITERAL(53, 16),  // "friendListLoaded"
        QT_MOC_LITERAL(70, 25),  // "QList<MeChat::FriendInfo>"
        QT_MOC_LITERAL(96, 10),  // "friendList"
        QT_MOC_LITERAL(107, 14),  // "messagesLoaded"
        QT_MOC_LITERAL(122, 8),  // "friendId"
        QT_MOC_LITERAL(131, 26),  // "QList<MeChat::messageData>"
        QT_MOC_LITERAL(158, 8),  // "messages"
        QT_MOC_LITERAL(167, 15),  // "loadingFinished"
        QT_MOC_LITERAL(183, 8),  // "loadData"
        QT_MOC_LITERAL(192, 6),  // "userId"
        QT_MOC_LITERAL(199, 12)   // "loadMessages"
    },
    "DataLoader",
    "userInfoLoaded",
    "",
    "MeChat::UserInfo",
    "userInfo",
    "friendListLoaded",
    "QList<MeChat::FriendInfo>",
    "friendList",
    "messagesLoaded",
    "friendId",
    "QList<MeChat::messageData>",
    "messages",
    "loadingFinished",
    "loadData",
    "userId",
    "loadMessages"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSDataLoaderENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    1 /* Public */,
       5,    1,   53,    2, 0x06,    3 /* Public */,
       8,    2,   56,    2, 0x06,    5 /* Public */,
      12,    0,   61,    2, 0x06,    8 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      13,    1,   62,    2, 0x0a,    9 /* Public */,
      15,    2,   65,    2, 0x0a,   11 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 10,    9,   11,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   14,    9,

       0        // eod
};

Q_CONSTINIT const QMetaObject DataLoader::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSDataLoaderENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSDataLoaderENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSDataLoaderENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DataLoader, std::true_type>,
        // method 'userInfoLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const MeChat::UserInfo &, std::false_type>,
        // method 'friendListLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<MeChat::FriendInfo> &, std::false_type>,
        // method 'messagesLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<MeChat::messageData> &, std::false_type>,
        // method 'loadingFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'loadMessages'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void DataLoader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DataLoader *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->userInfoLoaded((*reinterpret_cast< std::add_pointer_t<MeChat::UserInfo>>(_a[1]))); break;
        case 1: _t->friendListLoaded((*reinterpret_cast< std::add_pointer_t<QList<MeChat::FriendInfo>>>(_a[1]))); break;
        case 2: _t->messagesLoaded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<MeChat::messageData>>>(_a[2]))); break;
        case 3: _t->loadingFinished(); break;
        case 4: _t->loadData((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->loadMessages((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DataLoader::*)(const MeChat::UserInfo & );
            if (_t _q_method = &DataLoader::userInfoLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DataLoader::*)(const QList<MeChat::FriendInfo> & );
            if (_t _q_method = &DataLoader::friendListLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DataLoader::*)(const QString & , const QList<MeChat::messageData> & );
            if (_t _q_method = &DataLoader::messagesLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DataLoader::*)();
            if (_t _q_method = &DataLoader::loadingFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *DataLoader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataLoader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSDataLoaderENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DataLoader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void DataLoader::userInfoLoaded(const MeChat::UserInfo & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataLoader::friendListLoaded(const QList<MeChat::FriendInfo> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DataLoader::messagesLoaded(const QString & _t1, const QList<MeChat::messageData> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DataLoader::loadingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
