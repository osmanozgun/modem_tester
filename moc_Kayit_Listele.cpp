/****************************************************************************
** Meta object code from reading C++ file 'Kayit_Listele.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "Kayit_Listele.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Kayit_Listele.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TabKayitListele_t {
    QByteArrayData data[9];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TabKayitListele_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TabKayitListele_t qt_meta_stringdata_TabKayitListele = {
    {
QT_MOC_LITERAL(0, 0, 15), // "TabKayitListele"
QT_MOC_LITERAL(1, 16, 19), // "guncellePenceresiAc"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(4, 55, 4), // "item"
QT_MOC_LITERAL(5, 60, 3), // "ara"
QT_MOC_LITERAL(6, 64, 6), // "filter"
QT_MOC_LITERAL(7, 71, 13), // "seciliLoguSil"
QT_MOC_LITERAL(8, 85, 14) // "verileriGoster"

    },
    "TabKayitListele\0guncellePenceresiAc\0"
    "\0QTableWidgetItem*\0item\0ara\0filter\0"
    "seciliLoguSil\0verileriGoster"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TabKayitListele[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       5,    1,   37,    2, 0x08 /* Private */,
       7,    0,   40,    2, 0x08 /* Private */,
       8,    0,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TabKayitListele::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TabKayitListele *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->guncellePenceresiAc((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->ara((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->seciliLoguSil(); break;
        case 3: _t->verileriGoster(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TabKayitListele::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_TabKayitListele.data,
    qt_meta_data_TabKayitListele,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TabKayitListele::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TabKayitListele::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TabKayitListele.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TabKayitListele::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
