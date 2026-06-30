/****************************************************************************
** Meta object code from reading C++ file 'loginwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/core/login/loginwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loginwindow.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSLoginWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSLoginWindowENDCLASS = QtMocHelpers::stringData(
    "LoginWindow",
    "userLoggedIn",
    "",
    "userID",
    "password",
    "resetLineEditStyle",
    "QLineEdit*",
    "lineEdit",
    "checkInputFields",
    "showAvatar",
    "togglePasswordVisibility",
    "onForgetPassword",
    "login",
    "loadUserHistoryInfo",
    "loginError",
    "errorString",
    "switchToRegister",
    "switchToLogin",
    "loginChatwindow"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSLoginWindowENDCLASS_t {
    uint offsetsAndSizes[38];
    char stringdata0[12];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[9];
    char stringdata5[19];
    char stringdata6[11];
    char stringdata7[9];
    char stringdata8[17];
    char stringdata9[11];
    char stringdata10[25];
    char stringdata11[17];
    char stringdata12[6];
    char stringdata13[20];
    char stringdata14[11];
    char stringdata15[12];
    char stringdata16[17];
    char stringdata17[14];
    char stringdata18[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSLoginWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSLoginWindowENDCLASS_t qt_meta_stringdata_CLASSLoginWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11),  // "LoginWindow"
        QT_MOC_LITERAL(12, 12),  // "userLoggedIn"
        QT_MOC_LITERAL(25, 0),  // ""
        QT_MOC_LITERAL(26, 6),  // "userID"
        QT_MOC_LITERAL(33, 8),  // "password"
        QT_MOC_LITERAL(42, 18),  // "resetLineEditStyle"
        QT_MOC_LITERAL(61, 10),  // "QLineEdit*"
        QT_MOC_LITERAL(72, 8),  // "lineEdit"
        QT_MOC_LITERAL(81, 16),  // "checkInputFields"
        QT_MOC_LITERAL(98, 10),  // "showAvatar"
        QT_MOC_LITERAL(109, 24),  // "togglePasswordVisibility"
        QT_MOC_LITERAL(134, 16),  // "onForgetPassword"
        QT_MOC_LITERAL(151, 5),  // "login"
        QT_MOC_LITERAL(157, 19),  // "loadUserHistoryInfo"
        QT_MOC_LITERAL(177, 10),  // "loginError"
        QT_MOC_LITERAL(188, 11),  // "errorString"
        QT_MOC_LITERAL(200, 16),  // "switchToRegister"
        QT_MOC_LITERAL(217, 13),  // "switchToLogin"
        QT_MOC_LITERAL(231, 15)   // "loginChatwindow"
    },
    "LoginWindow",
    "userLoggedIn",
    "",
    "userID",
    "password",
    "resetLineEditStyle",
    "QLineEdit*",
    "lineEdit",
    "checkInputFields",
    "showAvatar",
    "togglePasswordVisibility",
    "onForgetPassword",
    "login",
    "loadUserHistoryInfo",
    "loginError",
    "errorString",
    "switchToRegister",
    "switchToLogin",
    "loginChatwindow"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSLoginWindowENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   86,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,   91,    2, 0x08,    4 /* Private */,
       8,    0,   94,    2, 0x08,    6 /* Private */,
       9,    0,   95,    2, 0x08,    7 /* Private */,
      10,    0,   96,    2, 0x08,    8 /* Private */,
      11,    0,   97,    2, 0x08,    9 /* Private */,
      12,    0,   98,    2, 0x08,   10 /* Private */,
      13,    0,   99,    2, 0x08,   11 /* Private */,
      14,    1,  100,    2, 0x08,   12 /* Private */,
      16,    0,  103,    2, 0x08,   14 /* Private */,
      17,    0,  104,    2, 0x08,   15 /* Private */,
      18,    1,  105,    2, 0x08,   16 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject LoginWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSLoginWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSLoginWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSLoginWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LoginWindow, std::true_type>,
        // method 'userLoggedIn'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'resetLineEditStyle'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        // method 'checkInputFields'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'showAvatar'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'togglePasswordVisibility'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onForgetPassword'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'login'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'loadUserHistoryInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loginError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'switchToRegister'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'switchToLogin'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loginChatwindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString, std::false_type>
    >,
    nullptr
} };

void LoginWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LoginWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->userLoggedIn((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->resetLineEditStyle((*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[1]))); break;
        case 2: { bool _r = _t->checkInputFields();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->showAvatar(); break;
        case 4: _t->togglePasswordVisibility(); break;
        case 5: _t->onForgetPassword(); break;
        case 6: { bool _r = _t->login();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->loadUserHistoryInfo(); break;
        case 8: _t->loginError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->switchToRegister(); break;
        case 10: _t->switchToLogin(); break;
        case 11: _t->loginChatwindow((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLineEdit* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LoginWindow::*)(const QString & , const QString & );
            if (_t _q_method = &LoginWindow::userLoggedIn; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *LoginWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LoginWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSLoginWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LoginWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void LoginWindow::userLoggedIn(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
