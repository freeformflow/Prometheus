/****************************************************************************
** Meta object code from reading C++ file 'cpsviewer.h'
**
** Created: Sun Mar 17 11:24:42 2013
**      by: The Qt Meta Object Compiler version 67 (Qt 5.0.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Prometheus/cpsviewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cpsviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CPSviewer_t {
    QByteArrayData data[25];
    char stringdata[339];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CPSviewer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CPSviewer_t qt_meta_stringdata_CPSviewer = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 19),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 8),
QT_MOC_LITERAL(4, 40, 21),
QT_MOC_LITERAL(5, 62, 18),
QT_MOC_LITERAL(6, 81, 22),
QT_MOC_LITERAL(7, 104, 5),
QT_MOC_LITERAL(8, 110, 11),
QT_MOC_LITERAL(9, 122, 13),
QT_MOC_LITERAL(10, 136, 11),
QT_MOC_LITERAL(11, 148, 13),
QT_MOC_LITERAL(12, 162, 14),
QT_MOC_LITERAL(13, 177, 18),
QT_MOC_LITERAL(14, 196, 18),
QT_MOC_LITERAL(15, 215, 25),
QT_MOC_LITERAL(16, 241, 12),
QT_MOC_LITERAL(17, 254, 17),
QT_MOC_LITERAL(18, 272, 5),
QT_MOC_LITERAL(19, 278, 13),
QT_MOC_LITERAL(20, 292, 9),
QT_MOC_LITERAL(21, 302, 10),
QT_MOC_LITERAL(22, 313, 5),
QT_MOC_LITERAL(23, 319, 6),
QT_MOC_LITERAL(24, 326, 11)
    },
    "CPSviewer\0dragAndDropFilename\0\0filename\0"
    "requestShallowRefresh\0requestDeepRefresh\0"
    "updateProjectionMethod\0index\0useViewTool\0"
    "useSelectTool\0useAtomTool\0useDeleteTool\0"
    "useMeasureTool\0useIncrementBOTool\0"
    "useDecrementBOTool\0requestBuildElementChange\0"
    "atomicNumber\0requestLabelStyle\0label\0"
    "setBitmapMode\0isChecked\0setBitGrid\0"
    "width\0height\0printBitmap\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CPSviewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       4,    0,   97,    2, 0x0a,
       5,    0,   98,    2, 0x0a,
       6,    1,   99,    2, 0x0a,
       8,    0,  102,    2, 0x0a,
       9,    0,  103,    2, 0x0a,
      10,    0,  104,    2, 0x0a,
      11,    0,  105,    2, 0x0a,
      12,    0,  106,    2, 0x0a,
      13,    0,  107,    2, 0x0a,
      14,    0,  108,    2, 0x0a,
      15,    1,  109,    2, 0x0a,
      17,    1,  112,    2, 0x0a,
      19,    1,  115,    2, 0x0a,
      21,    2,  118,    2, 0x0a,
      24,    0,  123,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Bool,   20,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   22,   23,
    QMetaType::Void,

       0        // eod
};

void CPSviewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CPSviewer *_t = static_cast<CPSviewer *>(_o);
        switch (_id) {
        case 0: _t->dragAndDropFilename((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->requestShallowRefresh(); break;
        case 2: _t->requestDeepRefresh(); break;
        case 3: _t->updateProjectionMethod((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->useViewTool(); break;
        case 5: _t->useSelectTool(); break;
        case 6: _t->useAtomTool(); break;
        case 7: _t->useDeleteTool(); break;
        case 8: _t->useMeasureTool(); break;
        case 9: _t->useIncrementBOTool(); break;
        case 10: _t->useDecrementBOTool(); break;
        case 11: _t->requestBuildElementChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->requestLabelStyle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->setBitmapMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->setBitGrid((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->printBitmap(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CPSviewer::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CPSviewer::dragAndDropFilename)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CPSviewer::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_CPSviewer.data,
      qt_meta_data_CPSviewer,  qt_static_metacall, 0, 0}
};


const QMetaObject *CPSviewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CPSviewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPSviewer.stringdata))
        return static_cast<void*>(const_cast< CPSviewer*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int CPSviewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void CPSviewer::dragAndDropFilename(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
