/****************************************************************************
** Meta object code from reading C++ file 'QtGuiApplication1.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QtGuiApplication1.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtGuiApplication1.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QtGuiApplication1_t {
    QByteArrayData data[19];
    char stringdata0[294];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtGuiApplication1_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtGuiApplication1_t qt_meta_stringdata_QtGuiApplication1 = {
    {
QT_MOC_LITERAL(0, 0, 17), // "QtGuiApplication1"
QT_MOC_LITERAL(1, 18, 25), // "QTreeWidgetClicked_Recipe"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(4, 62, 4), // "item"
QT_MOC_LITERAL(5, 67, 6), // "column"
QT_MOC_LITERAL(6, 74, 23), // "QTreeWidgetClicked_Food"
QT_MOC_LITERAL(7, 98, 18), // "ResultIndexClicked"
QT_MOC_LITERAL(8, 117, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(9, 135, 15), // "removeFoodItem2"
QT_MOC_LITERAL(10, 151, 17), // "changeAmount_Food"
QT_MOC_LITERAL(11, 169, 10), // "currentRow"
QT_MOC_LITERAL(12, 180, 21), // "changeAmount_Nutrient"
QT_MOC_LITERAL(13, 202, 17), // "sliderRatioChange"
QT_MOC_LITERAL(14, 220, 5), // "value"
QT_MOC_LITERAL(15, 226, 12), // "applyClicked"
QT_MOC_LITERAL(16, 239, 18), // "runAllReipeClicked"
QT_MOC_LITERAL(17, 258, 18), // "runAllImageClicked"
QT_MOC_LITERAL(18, 277, 16) // "openImageClicked"

    },
    "QtGuiApplication1\0QTreeWidgetClicked_Recipe\0"
    "\0QTreeWidgetItem*\0item\0column\0"
    "QTreeWidgetClicked_Food\0ResultIndexClicked\0"
    "QTableWidgetItem*\0removeFoodItem2\0"
    "changeAmount_Food\0currentRow\0"
    "changeAmount_Nutrient\0sliderRatioChange\0"
    "value\0applyClicked\0runAllReipeClicked\0"
    "runAllImageClicked\0openImageClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtGuiApplication1[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x08 /* Private */,
       6,    2,   74,    2, 0x08 /* Private */,
       7,    1,   79,    2, 0x08 /* Private */,
       9,    1,   82,    2, 0x08 /* Private */,
      10,    1,   85,    2, 0x08 /* Private */,
      12,    1,   88,    2, 0x08 /* Private */,
      13,    1,   91,    2, 0x08 /* Private */,
      15,    0,   94,    2, 0x08 /* Private */,
      16,    0,   95,    2, 0x08 /* Private */,
      17,    0,   96,    2, 0x08 /* Private */,
      18,    0,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void, 0x80000000 | 8,    4,
    QMetaType::Void, 0x80000000 | 8,    4,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QtGuiApplication1::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QtGuiApplication1 *_t = static_cast<QtGuiApplication1 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->QTreeWidgetClicked_Recipe((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->QTreeWidgetClicked_Food((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->ResultIndexClicked((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->removeFoodItem2((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->changeAmount_Food((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->changeAmount_Nutrient((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->sliderRatioChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->applyClicked(); break;
        case 8: _t->runAllReipeClicked(); break;
        case 9: _t->runAllImageClicked(); break;
        case 10: _t->openImageClicked(); break;
        default: ;
        }
    }
}

const QMetaObject QtGuiApplication1::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QtGuiApplication1.data,
      qt_meta_data_QtGuiApplication1,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QtGuiApplication1::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtGuiApplication1::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QtGuiApplication1.stringdata0))
        return static_cast<void*>(const_cast< QtGuiApplication1*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QtGuiApplication1::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
