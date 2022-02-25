# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# Qt源码解析5-对象模型-QVariant源码

# 1、QVariant详细说明

​	因为 C++ 禁止联合包含具有非默认构造函数或析构函数的类型，所以大多数有趣的 Qt 类不能在联合中使用。如果没有 QVariant，这对于QObject::property () 和数据库工作等来说都是一个问题。

​	QVariant 对象一次保存单个type()的单个值。（某些type() 是多值的，例如字符串列表。）您可以找出变体持有的类型， T，使用convert () 将其转换为不同的类型，使用toT()函数 之一获取其值() 函数（例如，toSize ()）,并检查是否可以使用canConvert ()将类型转换为特定类型。

​	名称为  toT() 的方法（例如，toInt ()、toString ()）是 const。如果您要求存储类型，它们会返回存储对象的副本。如果您要求可以从存储的类型生成的类型，toT() 复制并转换并保持对象本身不变。如果请求的类型不能从存储的类型中生成，则结果取决于类型；有关详细信息，请参阅功能文档。

下面是一些示例代码来演示 QVariant 的使用：

```C++
QDataStream out(...);
QVariant v(123);                // The variant now contains an int
int x = v.toInt();              // x = 123
out << v;                       // Writes a type tag and an int to out
v = QVariant("hello");          // The variant now contains a QByteArray
v = QVariant(tr("hello"));      // The variant now contains a QString
int y = v.toInt();              // y = 0 since v cannot be converted to an int
QString s = v.toString();       // s = tr("hello")  (see QObject::tr())
out << v;                       // Writes a type tag and a QString to out
...
QDataStream in(...);            // (opening the previously written stream)
in >> v;                        // Reads an Int variant
int z = v.toInt();              // z = 123
qDebug("Type is %s",            // prints "Type is int"
        v.typeName());
v = v.toInt() + 100;            // The variant now hold the value 223
v = QVariant(QStringList());
```

​	您甚至可以将QList <QVariant> 和QMap < QString , QVariant> 值存储在变体中，因此您可以轻松构建任意类型的任意复杂数据结构。
这是非常强大和通用的，但可能证明比在标准数据结构中存储特定类型的内存和速度效率低。

QVariant 还支持空值的概念，您可以在其中定义没有值集的类型。但是，请注意，QVariant 类型只有在设置了值时才能强制转换。

```c++
QVariant x, y(QString()), z(QString(""));
x.convert(QVariant::Int);
// x.isNull() == true
// y.isNull() == true, z.isNull() == false
```

QVariant 可以扩展以支持除Type枚举中提到的类型之外的其他类型。有关详细信息，请参阅创建自定义 Qt 类型。

# 2、关于 GUI 类型的说明


​	因为 QVariant 是 Qt Core 模块的一部分，它不能提供对 Qt GUI 中定义的数据类型的转换功能，例如QColor、QImage和QPixmap。换句话说，没有toColor()功能。相反，您可以使用QVariant::value () 或qvariant_cast () 模板函数。例如：

```c++
QVariant variant;
...
QColor color = variant.value<QColor>();
```

反向转换（例如，从QColor到 QVariant）是自动的，包括 QVariant 支持的所有数据类型，包含与 GUI 相关的类型：

```c++
QColor color = palette().background().color();
QVariant variant = color;
```

## 2.1连续使用 canConvert() 和 convert()

​	连续使用canConvert ()和convert ()时，canConvert ()有可能返回true，而convert ()返回false。这通常是因为canConvert () 仅报告 QVariant 在给定合适数据的类型之间转换的一般能力；仍然可以提供实际上无法转换的数据。

​	例如，当对包含字符串的变体调用时， canConvert (Int) 将返回 true，因为原则上，QVariant 能够将数字字符串转换为整数。但是，如果字符串包含非数字字符，则无法将其转换为整数，并且任何转换它的尝试都将失败。因此，重要的是让两个函数都返回 true 才能成功转换。

# 3、QVariant 内部结构---了解设计 QVariant 所涉及的各种问题

​	了解QVariant源码，或者自己实现这个这个功能。需要了解内部结构。

​	在C+*中，变量的类型需要在编译期就是已知的。但是在一些情况下我们需要处理一些类型在运行期才可知的变量。比如，假设你有一个函数，其返回从数据库中获得的值。那么其返回值什么呢？在C中，这将是一个 "void\*" 和一个额外的提供它的类型的信息。再或者假设你允许用户附加任意信息到那你的自定义表格控件的单元格。那么这个函数 setUserCellData(type X)的参数类型是什么呢？*

QVariant可用于上述情况，它可用于保存任何类型的值。你可以向QVariant询问类型和它内部存储的值。

在理解Qt的实现之前，为了更好地欣赏最终的解决方案我们会先尝试看看在QVariant的设计中所涉及的各种问题。

## 3.1 QVariant功能要求

- QVariant 不应当(也不能)是一个模板类。如果 QVariant 是一个模板，每一个存储 QVariant 的类也需要成为一个模板。
-  QVariant 必须对POD 和 非POD 数据类型都能工作。它应当对每一个可拷贝的和有着值行为的数据类型都能工作。请注意，对非POD类型的支持意味着我们不能使用C union来存储所有已知的类型，因为C不允许在union中放置非POD类型 。
-  QVariant 必须能够存放用户自定义类型。所以，如果你写了一个 Mytruct，你应当能够将其放入到QVariant 。
-  QVariant 不应当(也不能)使用 C*+ 的 RTTI。在 C++ 中 RTTI 只对多态的类有效* 。

## 3.2 将值存储为“void*”

​	一个想法是在 QVariant 中保存一个“void*”和一个类型。我们可以有一个 QVariant 构造函数作为一个函数模板，它可以适合任何类型。例如：

```c++
class QVariant { 
private:
	union {
 	//// POD 数据类型
 	int i;
 	float f;
 	// 所有 非POD 数据类型存储为 void *.
 	void *v;
	} data;
	enum DataType { Integer, Float, NonPod }; // 数据的类型
	int type;
    
public:
    // 适合 内置/POD 数据类型的构造函数 
QVariant(int i) {
 data.i = i;
 type = Integer;
}
    
    // 为所有非 non-POD 类型准备的模板构造函数
template <typename T>
QVariant(const T &t) {
 data.v = (void*) new T (t);
 type = NonPod;
//...
```

但是，我们将难以编写析构函数。如何“删除” void 指针？在 C + 中，您不能删除“void*”。

## 3.3 到达解决方案
​	正如我们上面得出的，要删除 void **我们只需要简单地知道它的类型。此外，由于 QVariant 需要支持用户定义类型，我们不可能放置一个巨大的switch case 来将 void \* 通过 reinterpret_cast 转换成特定的类型并删除这个指针。**

当试图访问QVariant的内容时我们也会遇到问题。variant.value <MyStruct>()在任何情况下都不应当崩溃。如果转换是不可用，它应该返回一个默认构造的MyStruct。

解决的办法是有一个这样的系统：可以从void **构造一个值，delete一个void**，并可通过转换 void * 类型来访问其值。如果对 *每一个* 存储在 QVariant 中的类型都有这些函数，那么QVariant就可以使用这些函数在void* 上工作。

qMetaTypeConstructHelper 和 qMetaTypeDeleteHelper 是模板构造助手 qRegisterMetaType<MyStruct>("MyStruct")

- QMetaType::registerType("MyStruct", ctr, dtr);

- 在内部，它全部存储在 QVector<QCustomTypeInfo>

如果您为所有类型执行 qRegisterMetaType，我们就完成了！但它很痛苦吗？Q_DECLARE_METATYPE(类型)

- 创建一个类 QMetaTypeId，它提供的 qt_metatype_id() 可根据要求进行注册 。
- 提供一个功能



# 4 QVariant源码

## 4.1 qRegisterMetaType 

函数原型为：

```c++
template <typename T>
int qRegisterMetaType(const char *typeName
#ifndef Q_CLANG_QDOC
....
#endif
)
{
#ifdef QT_NO_QOBJECT
    ....
#else
    QT_PREPEND_NAMESPACE(QByteArray) normalizedTypeName = QMetaObject::normalizedType(typeName);
#endif
    return qRegisterNormalizedMetaType<T>(normalizedTypeName, dummy, defined);
}
```



## 4.2 QVariant构造函数

从构造函数入手分析源码

```C++
QVariant() Q_DECL_NOTHROW : d() {}//不带参数构造

/*具体类型构造，采用重载函数方式，此方法包含多种数据类型，构造方式相同。
X类型的重载函数，给d.data的具体联合X对那个的联合变量赋值
*/
QVariant::QVariant(int val)
    : d(Int)
{ d.data.i = val; }

....
Private d;
....
struct Private
    {
    	/**/
        inline Private() Q_DECL_NOTHROW : type(Invalid), is_shared(false), is_null(true)
        { data.ptr = nullptr; }

        // Internal constructor for initialized variants.
        explicit inline Private(uint variantType) Q_DECL_NOTHROW
            : type(variantType), is_shared(false), is_null(false)
        {}
		....
        union Data
        {
            char c;
            uchar uc;
            short s;
            signed char sc;
            ushort us;
            int i;
            uint u;
            long l;
            ulong ul;
            bool b;
            double d;
            float f;
            qreal real;
            qlonglong ll;
            qulonglong ull;
            QObject *o;
            void *ptr;
            PrivateShared *shared;
        } data;
        uint type : 30;
        uint is_shared : 1;
        uint is_null : 1;
    };    
```

