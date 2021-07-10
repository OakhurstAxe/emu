/****************************************************************************
** Meta object code from reading C++ file 'nesmainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../headers/nesmainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'nesmainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_oa__nes__NesMainWindow_t {
    QByteArrayData data[9];
    char stringdata0[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_oa__nes__NesMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_oa__nes__NesMainWindow_t qt_meta_stringdata_oa__nes__NesMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 22), // "oa::nes::NesMainWindow"
QT_MOC_LITERAL(1, 23, 23), // "leftControllerLeftRight"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 5), // "value"
QT_MOC_LITERAL(4, 54, 20), // "leftControllerUpDown"
QT_MOC_LITERAL(5, 75, 20), // "leftControllerSelect"
QT_MOC_LITERAL(6, 96, 19), // "leftControllerStart"
QT_MOC_LITERAL(7, 116, 15), // "leftControllerA"
QT_MOC_LITERAL(8, 132, 15) // "leftControllerB"

    },
    "oa::nes::NesMainWindow\0leftControllerLeftRight\0"
    "\0value\0leftControllerUpDown\0"
    "leftControllerSelect\0leftControllerStart\0"
    "leftControllerA\0leftControllerB"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_oa__nes__NesMainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       4,    1,   47,    2, 0x0a /* Public */,
       5,    1,   50,    2, 0x0a /* Public */,
       6,    1,   53,    2, 0x0a /* Public */,
       7,    1,   56,    2, 0x0a /* Public */,
       8,    1,   59,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void oa::nes::NesMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NesMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->leftControllerLeftRight((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->leftControllerUpDown((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->leftControllerSelect((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->leftControllerStart((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->leftControllerA((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->leftControllerB((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject oa::nes::NesMainWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_oa__nes__NesMainWindow.data,
    qt_meta_data_oa__nes__NesMainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *oa::nes::NesMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *oa::nes::NesMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_oa__nes__NesMainWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int oa::nes::NesMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
