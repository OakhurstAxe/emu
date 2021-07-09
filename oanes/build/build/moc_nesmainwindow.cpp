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
    QByteArrayData data[10];
    char stringdata0[163];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_oa__nes__NesMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_oa__nes__NesMainWindow_t qt_meta_stringdata_oa__nes__NesMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 22), // "oa::nes::NesMainWindow"
QT_MOC_LITERAL(1, 23, 14), // "startNextFrame"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 23), // "leftControllerLeftRight"
QT_MOC_LITERAL(4, 63, 5), // "value"
QT_MOC_LITERAL(5, 69, 20), // "leftControllerUpDown"
QT_MOC_LITERAL(6, 90, 20), // "leftControllerSelect"
QT_MOC_LITERAL(7, 111, 19), // "leftControllerStart"
QT_MOC_LITERAL(8, 131, 15), // "leftControllerA"
QT_MOC_LITERAL(9, 147, 15) // "leftControllerB"

    },
    "oa::nes::NesMainWindow\0startNextFrame\0"
    "\0leftControllerLeftRight\0value\0"
    "leftControllerUpDown\0leftControllerSelect\0"
    "leftControllerStart\0leftControllerA\0"
    "leftControllerB"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_oa__nes__NesMainWindow[] = {

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

void oa::nes::NesMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NesMainWindow *>(_o);
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
