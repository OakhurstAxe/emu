/****************************************************************************
** Meta object code from reading C++ file 'oaemumainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../headers/oaemumainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'oaemumainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EmuMainWindow_t {
    QByteArrayData data[10];
    char stringdata0[154];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EmuMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EmuMainWindow_t qt_meta_stringdata_EmuMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "EmuMainWindow"
QT_MOC_LITERAL(1, 14, 14), // "startNextFrame"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 23), // "leftControllerLeftRight"
QT_MOC_LITERAL(4, 54, 5), // "value"
QT_MOC_LITERAL(5, 60, 20), // "leftControllerUpDown"
QT_MOC_LITERAL(6, 81, 20), // "leftControllerSelect"
QT_MOC_LITERAL(7, 102, 19), // "leftControllerStart"
QT_MOC_LITERAL(8, 122, 15), // "leftControllerA"
QT_MOC_LITERAL(9, 138, 15) // "leftControllerB"

    },
    "EmuMainWindow\0startNextFrame\0\0"
    "leftControllerLeftRight\0value\0"
    "leftControllerUpDown\0leftControllerSelect\0"
    "leftControllerStart\0leftControllerA\0"
    "leftControllerB"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EmuMainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    1,   50,    2, 0x0a /* Public */,
       5,    1,   53,    2, 0x0a /* Public */,
       6,    1,   56,    2, 0x0a /* Public */,
       7,    1,   59,    2, 0x0a /* Public */,
       8,    1,   62,    2, 0x0a /* Public */,
       9,    1,   65,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    4,
    QMetaType::Void, QMetaType::Double,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,

       0        // eod
};

void EmuMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EmuMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startNextFrame(); break;
        case 1: _t->leftControllerLeftRight((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->leftControllerUpDown((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->leftControllerSelect((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->leftControllerStart((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->leftControllerA((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->leftControllerB((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject EmuMainWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_EmuMainWindow.data,
    qt_meta_data_EmuMainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *EmuMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EmuMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EmuMainWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int EmuMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
