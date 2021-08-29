/****************************************************************************
** Meta object code from reading C++ file 'vcsmainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../headers/vcsmainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vcsmainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_oa__vcs__VcsMainWindow_t {
    QByteArrayData data[8];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_oa__vcs__VcsMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_oa__vcs__VcsMainWindow_t qt_meta_stringdata_oa__vcs__VcsMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 22), // "oa::vcs::VcsMainWindow"
QT_MOC_LITERAL(1, 23, 15), // "LeftControllerA"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 5), // "value"
QT_MOC_LITERAL(4, 46, 19), // "LeftControllerReset"
QT_MOC_LITERAL(5, 66, 20), // "LeftControllerSelect"
QT_MOC_LITERAL(6, 87, 20), // "LeftControllerUpDown"
QT_MOC_LITERAL(7, 108, 23) // "LeftControllerLeftRight"

    },
    "oa::vcs::VcsMainWindow\0LeftControllerA\0"
    "\0value\0LeftControllerReset\0"
    "LeftControllerSelect\0LeftControllerUpDown\0"
    "LeftControllerLeftRight"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_oa__vcs__VcsMainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       4,    1,   42,    2, 0x0a /* Public */,
       5,    1,   45,    2, 0x0a /* Public */,
       6,    1,   48,    2, 0x0a /* Public */,
       7,    1,   51,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,

       0        // eod
};

void oa::vcs::VcsMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VcsMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->LeftControllerA((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->LeftControllerReset((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->LeftControllerSelect((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->LeftControllerUpDown((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->LeftControllerLeftRight((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject oa::vcs::VcsMainWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_oa__vcs__VcsMainWindow.data,
    qt_meta_data_oa__vcs__VcsMainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *oa::vcs::VcsMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *oa::vcs::VcsMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_oa__vcs__VcsMainWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int oa::vcs::VcsMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
