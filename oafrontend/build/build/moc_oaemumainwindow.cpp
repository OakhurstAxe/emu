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
struct qt_meta_stringdata_oa__emu__MainWindow_t {
    QByteArrayData data[17];
    char stringdata0[226];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_oa__emu__MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_oa__emu__MainWindow_t qt_meta_stringdata_oa__emu__MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 19), // "oa::emu::MainWindow"
QT_MOC_LITERAL(1, 20, 15), // "VcsGameNameEdit"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 4), // "text"
QT_MOC_LITERAL(4, 42, 17), // "VcsGameMapperEdit"
QT_MOC_LITERAL(5, 60, 21), // "VcsGameControllerEdit"
QT_MOC_LITERAL(6, 82, 18), // "VcsGameCompanyEdit"
QT_MOC_LITERAL(7, 101, 17), // "VcsGameScreenEdit"
QT_MOC_LITERAL(8, 119, 18), // "VcsGameRomFileEdit"
QT_MOC_LITERAL(9, 138, 15), // "VcsGameSelected"
QT_MOC_LITERAL(10, 154, 11), // "QModelIndex"
QT_MOC_LITERAL(11, 166, 5), // "index"
QT_MOC_LITERAL(12, 172, 11), // "EditClicked"
QT_MOC_LITERAL(13, 184, 7), // "checked"
QT_MOC_LITERAL(14, 192, 11), // "SaveClicked"
QT_MOC_LITERAL(15, 204, 10), // "AddClicked"
QT_MOC_LITERAL(16, 215, 10) // "RunClicked"

    },
    "oa::emu::MainWindow\0VcsGameNameEdit\0"
    "\0text\0VcsGameMapperEdit\0VcsGameControllerEdit\0"
    "VcsGameCompanyEdit\0VcsGameScreenEdit\0"
    "VcsGameRomFileEdit\0VcsGameSelected\0"
    "QModelIndex\0index\0EditClicked\0checked\0"
    "SaveClicked\0AddClicked\0RunClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_oa__emu__MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x08 /* Private */,
       4,    1,   72,    2, 0x08 /* Private */,
       5,    1,   75,    2, 0x08 /* Private */,
       6,    1,   78,    2, 0x08 /* Private */,
       7,    1,   81,    2, 0x08 /* Private */,
       8,    1,   84,    2, 0x08 /* Private */,
       9,    1,   87,    2, 0x08 /* Private */,
      12,    1,   90,    2, 0x08 /* Private */,
      14,    1,   93,    2, 0x08 /* Private */,
      15,    1,   96,    2, 0x08 /* Private */,
      16,    1,   99,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Bool,   13,

       0        // eod
};

void oa::emu::MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->VcsGameNameEdit((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->VcsGameMapperEdit((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->VcsGameControllerEdit((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->VcsGameCompanyEdit((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->VcsGameScreenEdit((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->VcsGameRomFileEdit((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->VcsGameSelected((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 7: _t->EditClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->SaveClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->AddClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->RunClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject oa::emu::MainWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_oa__emu__MainWindow.data,
    qt_meta_data_oa__emu__MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *oa::emu::MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *oa::emu::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_oa__emu__MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int oa::emu::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
