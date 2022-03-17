# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# Qt源码解析3-信号和槽机制-QMetaType

# 类QMetaType

该类用作在 QVariant 以及排队的信号和槽连接中编组类型的帮助程序。它将类型名称与类型相关联，以便可以在运行时动态创建和销毁类型。使用 Q_DECLARE_METATYPE（） 声明新类型，使其可用于 QVariant 和其他基于模板的函数。调用 qRegisterMetaType（） 以使类型可用于非基于模板的函数，例如排队信号和插槽连接。

可以注册具有公有默认构造函数、公有复制构造函数和公有析构函数的任何类或结构。

下面的代码分配并销毁 MyClass 的实例：

```c++
int id = QMetaType::type("MyClass");//没有注册的类型获取为空
if (id != QMetaType::UnknownType) {
    void *myClassPtr = QMetaType::create(id);
    ...
    QMetaType::destroy(id, myClassPtr);
    myClassPtr = 0;
}
```

如果我们希望流运算符运算符<<（） 和运算符>>（） 处理存储自定义类型的 QVariant 对象，则自定义类型必须提供运算符<<（） 和运算符>>（） 运算符。

## 辅助类QtMetaTypePrivate

```c++
namespace QtMetaTypePrivate {
template <typename T, bool Accepted = true>
struct QMetaTypeFunctionHelper {
    static void Destruct(void *t)
    {
        Q_UNUSED(t) // Silence MSVC that warns for POD types.
        static_cast<T*>(t)->~T();
    }

    static void *Construct(void *where, const void *t)
    {
        if (t)
            return new (where) T(*static_cast<const T*>(t));
        return new (where) T;/*new表达式调用适当的分配函数分配存储，where类型如果是非数组类型，那么调用operator new。若是数组类型，则函数名为operator new[]。此处是布置new，若提供了布置参数，则将他们作为额外实参传递给分配函数。这些分配函数都被称为“布置 new”,这来源于标准分配函数void *operator new(std::size_t, void *).它直接返回未修改的第二个实参*/
    }
    ....
};
```



## 成员函数qMetaTypeId

在编译时返回类型 T 的元类型 ID。如果未使用 Q_DECLARE_METATYPE（） 声明类型，则编译将失败。

典型用法：

```c++
int id = qMetaTypeId<QString>();    // id is now QMetaType::QString
id = qMetaTypeId<MyStruct>();       // compile error if MyStruct not declared
/*判断语句为Q_STATIC_ASSERT_X(QMetaTypeId2<T>::Defined, "Type is not registered, please use the Q_DECLARE_METATYPE macro to make it known to Qt's meta-object system");
    */
```

QMetaType：：type（） 返回与 qMetaTypeId（） 相同的 ID，但会根据类型的名称在运行时进行查找。QMetaType：：type（） 有点慢，但如果未注册类型，编译会成功。

## 成员函数type

```c++

template <bool tryNormalizedType>
static inline int qMetaTypeTypeImpl(const char *typeName, int length)
{
    if (!length)
        return QMetaType::UnknownType;
    int type = qMetaTypeStaticType(typeName, length);
    if (type == QMetaType::UnknownType) {
        QReadLocker locker(customTypesLock());
        type = qMetaTypeCustomType_unlocked(typeName, length);
#ifndef QT_NO_QOBJECT
        if ((type == QMetaType::UnknownType) && tryNormalizedType) {
            const NS(QByteArray) normalizedTypeName = QMetaObject::normalizedType(typeName);
            type = qMetaTypeStaticType(normalizedTypeName.constData(),
                                       normalizedTypeName.size());
            if (type == QMetaType::UnknownType) {
                type = qMetaTypeCustomType_unlocked(normalizedTypeName.constData(),
                                                    normalizedTypeName.size());
            }
        }
#endif
    }
    return type;
}
int QMetaType::type(const char *typeName)
{
    return qMetaTypeTypeImpl</*tryNormalizedType=*/true>(typeName, qstrlen(typeName));
}

```

qMetaTypeStaticType内置类型存储在静态变量

```c++
static const struct { const char * typeName; int typeNameLength; int type; } types[] = {
    QT_FOR_EACH_STATIC_TYPE(QT_ADD_STATIC_METATYPE)
    QT_FOR_EACH_STATIC_ALIAS_TYPE(QT_ADD_STATIC_METATYPE_ALIASES_ITER)
    QT_FOR_EACH_STATIC_HACKS_TYPE(QT_ADD_STATIC_METATYPE_HACKS_ITER)
    {0, 0, QMetaType::UnknownType}
};
```

中。

qMetaTypeCustomType_unlocked自定义类型存储

```c++
const QVector<QCustomTypeInfo> * const ct = customTypes();
//全局静态变量定义
Q_GLOBAL_STATIC(QVector<QCustomTypeInfo>, customTypes)
```



## 成员函数create

```c++
void *QMetaType::create(int type, const void *copy)
{
    QMetaType info(type);
    if (int size = info.sizeOf())
        return info.construct(operator new(size), copy);//调用辅助类的构造函数
    return 0;
}
```

## 成员函数destroy

```c++
void QMetaType::destroy(int type, void *data)
{
    QMetaType info(type);
    info.destruct(data);//调用辅助类的析构函数
    operator delete(data);//清空创建对象大小的指针。
}
```

## 相关宏Q_DECLARE_METATYPE

```c++
#define Q_DECLARE_METATYPE(TYPE) Q_DECLARE_METATYPE_IMPL(TYPE)
#define Q_DECLARE_METATYPE_IMPL(TYPE)                                   \
    QT_BEGIN_NAMESPACE                                                  \
    template <>                                                         \
    struct QMetaTypeId< TYPE >                                          \
    {                                                                   \
        enum { Defined = 1 };                                           \
        static int qt_metatype_id()                                     \
            {                                                           \
                static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0); \
                if (const int id = metatype_id.loadAcquire())           \
                    return id;                                          \
                const int newId = qRegisterMetaType< TYPE >(#TYPE,      \
                              reinterpret_cast< TYPE *>(quintptr(-1))); \
                metatype_id.storeRelease(newId);                        \
                return newId;                                           \
            }                                                           \
    };                                                                  \
    QT_END_NAMESPACE
```

Q_DECLARE_METATYPE 展开后是一个特化后的类 QMetaTypeId<TYPE>，特化的struct QMetaTypeId< TYPE >  调用qRegisterMetaType 函数将类型注册到MetaType 系统中 

- 如果要使自定义类型或其他非QMetaType内置类型在QVaiant中使用，必须使用该宏。
- 该类型必须有公有的 构造、析构、复制构造 函数

## 相关非成员函数qRegisterMetaType 

```c++
template <typename T>
int qRegisterNormalizedMetaType(const QT_PREPEND_NAMESPACE(QByteArray) &normalizedTypeName
#ifndef Q_CLANG_QDOC
    , T * dummy = 0
    , typename QtPrivate::MetaTypeDefinedHelper<T, QMetaTypeId2<T>::Defined && !QMetaTypeId2<T>::IsBuiltIn>::DefinedType defined = QtPrivate::MetaTypeDefinedHelper<T, QMetaTypeId2<T>::Defined && !QMetaTypeId2<T>::IsBuiltIn>::Defined
#endif
)
{
#ifndef QT_NO_QOBJECT
    Q_ASSERT_X(normalizedTypeName == QMetaObject::normalizedType(normalizedTypeName.constData()), "qRegisterNormalizedMetaType", "qRegisterNormalizedMetaType was called with a not normalized type name, please call qRegisterMetaType instead.");
#endif
    const int typedefOf = dummy ? -1 : QtPrivate::QMetaTypeIdHelper<T>::qt_metatype_id();
    if (typedefOf != -1)
        return QMetaType::registerNormalizedTypedef(normalizedTypeName, typedefOf);

    QMetaType::TypeFlags flags(QtPrivate::QMetaTypeTypeFlags<T>::Flags);

    if (defined)
        flags |= QMetaType::WasDeclaredAsMetaType;

    const int id = QMetaType::registerNormalizedType(normalizedTypeName,
                                   QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Destruct,
                                   QtMetaTypePrivate::QMetaTypeFunctionHelper<T>::Construct,
                                   int(sizeof(T)),
                                   flags,
                                   QtPrivate::MetaObjectForType<T>::value());

    if (id > 0) {
        QtPrivate::SequentialContainerConverterHelper<T>::registerConverter(id);
        QtPrivate::AssociativeContainerConverterHelper<T>::registerConverter(id);
        QtPrivate::MetaTypePairHelper<T>::registerConverter(id);
        QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter(id);
    }

    return id;
}

template <typename T>
int qRegisterMetaType(const char *typeName
#ifndef Q_CLANG_QDOC
    , T * dummy = nullptr
    , typename QtPrivate::MetaTypeDefinedHelper<T, QMetaTypeId2<T>::Defined && !QMetaTypeId2<T>::IsBuiltIn>::DefinedType defined = QtPrivate::MetaTypeDefinedHelper<T, QMetaTypeId2<T>::Defined && !QMetaTypeId2<T>::IsBuiltIn>::Defined
#endif
)
{
#ifdef QT_NO_QOBJECT
    QT_PREPEND_NAMESPACE(QByteArray) normalizedTypeName = typeName;
#else
    QT_PREPEND_NAMESPACE(QByteArray) normalizedTypeName = QMetaObject::normalizedType(typeName);
#endif
    return qRegisterNormalizedMetaType<T>(normalizedTypeName, dummy, defined);
}
```

必须使用该函数的两种情况

- 如果非QMetaType内置类型要在 Qt 的属性系统中使用
- 如果非QMetaType内置类型要在 queued 信号与槽 中使用