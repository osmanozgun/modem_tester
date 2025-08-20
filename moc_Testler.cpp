/****************************************************************************
** Meta object code from reading C++ file 'Testler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "Testler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Testler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TestlerWidget_t {
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TestlerWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TestlerWidget_t qt_meta_stringdata_TestlerWidget = {
    {
QT_MOC_LITERAL(0, 0, 13) // "TestlerWidget"

    },
    "TestlerWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TestlerWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void TestlerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject TestlerWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_TestlerWidget.data,
    qt_meta_data_TestlerWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TestlerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TestlerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TestlerWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TestlerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_TabTestBaslat_t {
    QByteArrayData data[6];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TabTestBaslat_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TabTestBaslat_t qt_meta_stringdata_TabTestBaslat = {
    {
QT_MOC_LITERAL(0, 0, 13), // "TabTestBaslat"
QT_MOC_LITERAL(1, 14, 10), // "baslatTest"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 10), // "durdurTest"
QT_MOC_LITERAL(4, 37, 16), // "testEkraniBaslat"
QT_MOC_LITERAL(5, 54, 11) // "seriVeriOku"

    },
    "TabTestBaslat\0baslatTest\0\0durdurTest\0"
    "testEkraniBaslat\0seriVeriOku"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TabTestBaslat[] = {

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
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TabTestBaslat::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TabTestBaslat *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->baslatTest(); break;
        case 1: _t->durdurTest(); break;
        case 2: _t->testEkraniBaslat(); break;
        case 3: _t->seriVeriOku(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject TabTestBaslat::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_TabTestBaslat.data,
    qt_meta_data_TabTestBaslat,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TabTestBaslat::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TabTestBaslat::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TabTestBaslat.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TabTestBaslat::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_VirtualMeterPanel_t {
    QByteArrayData data[8];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VirtualMeterPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VirtualMeterPanel_t qt_meta_stringdata_VirtualMeterPanel = {
    {
QT_MOC_LITERAL(0, 0, 17), // "VirtualMeterPanel"
QT_MOC_LITERAL(1, 18, 11), // "connectPort"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 14), // "disconnectPort"
QT_MOC_LITERAL(4, 46, 12), // "refreshPorts"
QT_MOC_LITERAL(5, 59, 20), // "onSerialDataReceived"
QT_MOC_LITERAL(6, 80, 11), // "updateTimer"
QT_MOC_LITERAL(7, 92, 18) // "resetToDefaultBaud"

    },
    "VirtualMeterPanel\0connectPort\0\0"
    "disconnectPort\0refreshPorts\0"
    "onSerialDataReceived\0updateTimer\0"
    "resetToDefaultBaud"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VirtualMeterPanel[] = {

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
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    0,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VirtualMeterPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VirtualMeterPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectPort(); break;
        case 1: _t->disconnectPort(); break;
        case 2: _t->refreshPorts(); break;
        case 3: _t->onSerialDataReceived(); break;
        case 4: _t->updateTimer(); break;
        case 5: _t->resetToDefaultBaud(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject VirtualMeterPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_VirtualMeterPanel.data,
    qt_meta_data_VirtualMeterPanel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VirtualMeterPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VirtualMeterPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VirtualMeterPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int VirtualMeterPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_SerialReaderThread_t {
    QByteArrayData data[4];
    char stringdata0[38];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialReaderThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialReaderThread_t qt_meta_stringdata_SerialReaderThread = {
    {
QT_MOC_LITERAL(0, 0, 18), // "SerialReaderThread"
QT_MOC_LITERAL(1, 19, 12), // "dataReceived"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 4) // "data"

    },
    "SerialReaderThread\0dataReceived\0\0data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialReaderThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,

       0        // eod
};

void SerialReaderThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SerialReaderThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SerialReaderThread::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SerialReaderThread::dataReceived)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SerialReaderThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_SerialReaderThread.data,
    qt_meta_data_SerialReaderThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SerialReaderThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialReaderThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SerialReaderThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int SerialReaderThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void SerialReaderThread::dataReceived(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
