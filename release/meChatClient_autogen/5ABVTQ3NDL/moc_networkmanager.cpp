/****************************************************************************
** Meta object code from reading C++ file 'networkmanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/thread/network/networkmanager.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkmanager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSNetworkManagerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSNetworkManagerENDCLASS = QtMocHelpers::stringData(
    "NetworkManager",
    "connected",
    "",
    "disconnected",
    "deleteFriendSuccess",
    "friendId",
    "error",
    "errorString",
    "userStatusChanged",
    "userId",
    "online",
    "loginSuccess",
    "loginFailed",
    "registerSuccess",
    "registerFailed",
    "receiveUserInfo",
    "MeChat::UserInfo*",
    "userInfo",
    "receiveMessage",
    "MeChat::messageData",
    "data",
    "receiveFriendInfo",
    "MeChat::FriendInfo",
    "friendInfo",
    "receiveAddFriendList",
    "friendNick",
    "clearAddFriendList",
    "addFriendSuccess",
    "addFriendFailed",
    "connectToServer",
    "sendFile",
    "filePath",
    "sendHeartbeatMessage",
    "sendMessage",
    "sendLoginRequest",
    "password",
    "sendLogoutRequest",
    "requestUserInfo",
    "sendFriendRequest",
    "registerUser",
    "userNick",
    "sendAddFriendRequest",
    "sendFriendStrListRequest",
    "friendStr",
    "onReadyRead",
    "onDisconnected",
    "onError",
    "sendRawData",
    "onConnected",
    "deleteFriend"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSNetworkManagerENDCLASS_t {
    uint offsetsAndSizes[100];
    char stringdata0[15];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[20];
    char stringdata5[9];
    char stringdata6[6];
    char stringdata7[12];
    char stringdata8[18];
    char stringdata9[7];
    char stringdata10[7];
    char stringdata11[13];
    char stringdata12[12];
    char stringdata13[16];
    char stringdata14[15];
    char stringdata15[16];
    char stringdata16[18];
    char stringdata17[9];
    char stringdata18[15];
    char stringdata19[20];
    char stringdata20[5];
    char stringdata21[18];
    char stringdata22[19];
    char stringdata23[11];
    char stringdata24[21];
    char stringdata25[11];
    char stringdata26[19];
    char stringdata27[17];
    char stringdata28[16];
    char stringdata29[16];
    char stringdata30[9];
    char stringdata31[9];
    char stringdata32[21];
    char stringdata33[12];
    char stringdata34[17];
    char stringdata35[9];
    char stringdata36[18];
    char stringdata37[16];
    char stringdata38[18];
    char stringdata39[13];
    char stringdata40[9];
    char stringdata41[21];
    char stringdata42[25];
    char stringdata43[10];
    char stringdata44[12];
    char stringdata45[15];
    char stringdata46[8];
    char stringdata47[12];
    char stringdata48[12];
    char stringdata49[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSNetworkManagerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSNetworkManagerENDCLASS_t qt_meta_stringdata_CLASSNetworkManagerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14),  // "NetworkManager"
        QT_MOC_LITERAL(15, 9),  // "connected"
        QT_MOC_LITERAL(25, 0),  // ""
        QT_MOC_LITERAL(26, 12),  // "disconnected"
        QT_MOC_LITERAL(39, 19),  // "deleteFriendSuccess"
        QT_MOC_LITERAL(59, 8),  // "friendId"
        QT_MOC_LITERAL(68, 5),  // "error"
        QT_MOC_LITERAL(74, 11),  // "errorString"
        QT_MOC_LITERAL(86, 17),  // "userStatusChanged"
        QT_MOC_LITERAL(104, 6),  // "userId"
        QT_MOC_LITERAL(111, 6),  // "online"
        QT_MOC_LITERAL(118, 12),  // "loginSuccess"
        QT_MOC_LITERAL(131, 11),  // "loginFailed"
        QT_MOC_LITERAL(143, 15),  // "registerSuccess"
        QT_MOC_LITERAL(159, 14),  // "registerFailed"
        QT_MOC_LITERAL(174, 15),  // "receiveUserInfo"
        QT_MOC_LITERAL(190, 17),  // "MeChat::UserInfo*"
        QT_MOC_LITERAL(208, 8),  // "userInfo"
        QT_MOC_LITERAL(217, 14),  // "receiveMessage"
        QT_MOC_LITERAL(232, 19),  // "MeChat::messageData"
        QT_MOC_LITERAL(252, 4),  // "data"
        QT_MOC_LITERAL(257, 17),  // "receiveFriendInfo"
        QT_MOC_LITERAL(275, 18),  // "MeChat::FriendInfo"
        QT_MOC_LITERAL(294, 10),  // "friendInfo"
        QT_MOC_LITERAL(305, 20),  // "receiveAddFriendList"
        QT_MOC_LITERAL(326, 10),  // "friendNick"
        QT_MOC_LITERAL(337, 18),  // "clearAddFriendList"
        QT_MOC_LITERAL(356, 16),  // "addFriendSuccess"
        QT_MOC_LITERAL(373, 15),  // "addFriendFailed"
        QT_MOC_LITERAL(389, 15),  // "connectToServer"
        QT_MOC_LITERAL(405, 8),  // "sendFile"
        QT_MOC_LITERAL(414, 8),  // "filePath"
        QT_MOC_LITERAL(423, 20),  // "sendHeartbeatMessage"
        QT_MOC_LITERAL(444, 11),  // "sendMessage"
        QT_MOC_LITERAL(456, 16),  // "sendLoginRequest"
        QT_MOC_LITERAL(473, 8),  // "password"
        QT_MOC_LITERAL(482, 17),  // "sendLogoutRequest"
        QT_MOC_LITERAL(500, 15),  // "requestUserInfo"
        QT_MOC_LITERAL(516, 17),  // "sendFriendRequest"
        QT_MOC_LITERAL(534, 12),  // "registerUser"
        QT_MOC_LITERAL(547, 8),  // "userNick"
        QT_MOC_LITERAL(556, 20),  // "sendAddFriendRequest"
        QT_MOC_LITERAL(577, 24),  // "sendFriendStrListRequest"
        QT_MOC_LITERAL(602, 9),  // "friendStr"
        QT_MOC_LITERAL(612, 11),  // "onReadyRead"
        QT_MOC_LITERAL(624, 14),  // "onDisconnected"
        QT_MOC_LITERAL(639, 7),  // "onError"
        QT_MOC_LITERAL(647, 11),  // "sendRawData"
        QT_MOC_LITERAL(659, 11),  // "onConnected"
        QT_MOC_LITERAL(671, 12)   // "deleteFriend"
    },
    "NetworkManager",
    "connected",
    "",
    "disconnected",
    "deleteFriendSuccess",
    "friendId",
    "error",
    "errorString",
    "userStatusChanged",
    "userId",
    "online",
    "loginSuccess",
    "loginFailed",
    "registerSuccess",
    "registerFailed",
    "receiveUserInfo",
    "MeChat::UserInfo*",
    "userInfo",
    "receiveMessage",
    "MeChat::messageData",
    "data",
    "receiveFriendInfo",
    "MeChat::FriendInfo",
    "friendInfo",
    "receiveAddFriendList",
    "friendNick",
    "clearAddFriendList",
    "addFriendSuccess",
    "addFriendFailed",
    "connectToServer",
    "sendFile",
    "filePath",
    "sendHeartbeatMessage",
    "sendMessage",
    "sendLoginRequest",
    "password",
    "sendLogoutRequest",
    "requestUserInfo",
    "sendFriendRequest",
    "registerUser",
    "userNick",
    "sendAddFriendRequest",
    "sendFriendStrListRequest",
    "friendStr",
    "onReadyRead",
    "onDisconnected",
    "onError",
    "sendRawData",
    "onConnected",
    "deleteFriend"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSNetworkManagerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  212,    2, 0x06,    1 /* Public */,
       3,    0,  213,    2, 0x06,    2 /* Public */,
       4,    1,  214,    2, 0x06,    3 /* Public */,
       6,    1,  217,    2, 0x06,    5 /* Public */,
       8,    2,  220,    2, 0x06,    7 /* Public */,
      11,    1,  225,    2, 0x06,   10 /* Public */,
      12,    1,  228,    2, 0x06,   12 /* Public */,
      13,    1,  231,    2, 0x06,   14 /* Public */,
      14,    1,  234,    2, 0x06,   16 /* Public */,
      15,    1,  237,    2, 0x06,   18 /* Public */,
      18,    1,  240,    2, 0x06,   20 /* Public */,
      21,    1,  243,    2, 0x06,   22 /* Public */,
      24,    2,  246,    2, 0x06,   24 /* Public */,
      26,    0,  251,    2, 0x06,   27 /* Public */,
      27,    1,  252,    2, 0x06,   28 /* Public */,
      28,    1,  255,    2, 0x06,   30 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      29,    0,  258,    2, 0x0a,   32 /* Public */,
      30,    2,  259,    2, 0x0a,   33 /* Public */,
      32,    1,  264,    2, 0x0a,   36 /* Public */,
      33,    1,  267,    2, 0x0a,   38 /* Public */,
      34,    2,  270,    2, 0x0a,   40 /* Public */,
      36,    1,  275,    2, 0x0a,   43 /* Public */,
      37,    1,  278,    2, 0x0a,   45 /* Public */,
      38,    1,  281,    2, 0x0a,   47 /* Public */,
      39,    3,  284,    2, 0x0a,   49 /* Public */,
      41,    1,  291,    2, 0x0a,   53 /* Public */,
      42,    1,  294,    2, 0x0a,   55 /* Public */,
      44,    0,  297,    2, 0x08,   57 /* Private */,
      45,    0,  298,    2, 0x08,   58 /* Private */,
      46,    0,  299,    2, 0x08,   59 /* Private */,
      47,    1,  300,    2, 0x08,   60 /* Private */,
      48,    0,  303,    2, 0x08,   62 /* Private */,
      49,    1,  304,    2, 0x08,   63 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    9,   10,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,   25,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   31,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   35,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    9,   40,   35,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,   43,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,   20,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

Q_CONSTINIT const QMetaObject NetworkManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSNetworkManagerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSNetworkManagerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSNetworkManagerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<NetworkManager, std::true_type>,
        // method 'connected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'disconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deleteFriendSuccess'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'error'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'userStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'loginSuccess'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'loginFailed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'registerSuccess'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'registerFailed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'receiveUserInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<MeChat::UserInfo *, std::false_type>,
        // method 'receiveMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const MeChat::messageData &, std::false_type>,
        // method 'receiveFriendInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const MeChat::FriendInfo &, std::false_type>,
        // method 'receiveAddFriendList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'clearAddFriendList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addFriendSuccess'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const MeChat::FriendInfo &, std::false_type>,
        // method 'addFriendFailed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'connectToServer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'sendHeartbeatMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'sendMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const MeChat::messageData &, std::false_type>,
        // method 'sendLoginRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'sendLogoutRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'requestUserInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'sendFriendRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'registerUser'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'sendAddFriendRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'sendFriendStrListRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendRawData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'onConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deleteFriend'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void NetworkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->deleteFriendSuccess((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->error((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->userStatusChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 5: _t->loginSuccess((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->loginFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->registerSuccess((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->registerFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->receiveUserInfo((*reinterpret_cast< std::add_pointer_t<MeChat::UserInfo*>>(_a[1]))); break;
        case 10: _t->receiveMessage((*reinterpret_cast< std::add_pointer_t<MeChat::messageData>>(_a[1]))); break;
        case 11: _t->receiveFriendInfo((*reinterpret_cast< std::add_pointer_t<MeChat::FriendInfo>>(_a[1]))); break;
        case 12: _t->receiveAddFriendList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 13: _t->clearAddFriendList(); break;
        case 14: _t->addFriendSuccess((*reinterpret_cast< std::add_pointer_t<MeChat::FriendInfo>>(_a[1]))); break;
        case 15: _t->addFriendFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->connectToServer(); break;
        case 17: _t->sendFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 18: _t->sendHeartbeatMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 19: _t->sendMessage((*reinterpret_cast< std::add_pointer_t<MeChat::messageData>>(_a[1]))); break;
        case 20: _t->sendLoginRequest((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 21: _t->sendLogoutRequest((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 22: _t->requestUserInfo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 23: _t->sendFriendRequest((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 24: _t->registerUser((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 25: _t->sendAddFriendRequest((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 26: _t->sendFriendStrListRequest((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 27: _t->onReadyRead(); break;
        case 28: _t->onDisconnected(); break;
        case 29: _t->onError(); break;
        case 30: _t->sendRawData((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 31: _t->onConnected(); break;
        case 32: _t->deleteFriend((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkManager::*)();
            if (_t _q_method = &NetworkManager::connected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (_t _q_method = &NetworkManager::disconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (_t _q_method = &NetworkManager::deleteFriendSuccess; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (_t _q_method = &NetworkManager::error; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & , bool );
            if (_t _q_method = &NetworkManager::userStatusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (_t _q_method = &NetworkManager::loginSuccess; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (_t _q_method = &NetworkManager::loginFailed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (_t _q_method = &NetworkManager::registerSuccess; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (_t _q_method = &NetworkManager::registerFailed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(MeChat::UserInfo * );
            if (_t _q_method = &NetworkManager::receiveUserInfo; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const MeChat::messageData & );
            if (_t _q_method = &NetworkManager::receiveMessage; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const MeChat::FriendInfo & );
            if (_t _q_method = &NetworkManager::receiveFriendInfo; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & , const QString & );
            if (_t _q_method = &NetworkManager::receiveAddFriendList; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)();
            if (_t _q_method = &NetworkManager::clearAddFriendList; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const MeChat::FriendInfo & );
            if (_t _q_method = &NetworkManager::addFriendSuccess; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (NetworkManager::*)(const QString & );
            if (_t _q_method = &NetworkManager::addFriendFailed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 15;
                return;
            }
        }
    }
}

const QMetaObject *NetworkManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSNetworkManagerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetworkManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 33)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 33;
    }
    return _id;
}

// SIGNAL 0
void NetworkManager::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NetworkManager::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void NetworkManager::deleteFriendSuccess(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetworkManager::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NetworkManager::userStatusChanged(const QString & _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NetworkManager::loginSuccess(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void NetworkManager::loginFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void NetworkManager::registerSuccess(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void NetworkManager::registerFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void NetworkManager::receiveUserInfo(MeChat::UserInfo * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void NetworkManager::receiveMessage(const MeChat::messageData & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void NetworkManager::receiveFriendInfo(const MeChat::FriendInfo & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void NetworkManager::receiveAddFriendList(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void NetworkManager::clearAddFriendList()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void NetworkManager::addFriendSuccess(const MeChat::FriendInfo & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void NetworkManager::addFriendFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}
QT_WARNING_POP
