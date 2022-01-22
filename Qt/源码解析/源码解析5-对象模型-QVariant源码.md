# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# Qt源码解析5-对象模型-QVariant源码

# 1、详细说明

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

您甚至可以将QList <QVariant> 和QMap < QString , QVariant> 值存储在变体中，因此您可以轻松构建任意类型的任意复杂数据结构。
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


因为 QVariant 是 Qt Core 模块的一部分，它不能提供对 Qt GUI 中定义的数据类型的转换功能，例如QColor、QImage和QPixmap。换句话说，没有toColor()功能。相反，您可以使用QVariant::value () 或qvariant_cast () 模板函数。例如：

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

连续使用canConvert ()和convert ()时，canConvert ()有可能返回true，而convert ()返回false。这通常是因为canConvert () 仅报告 QVariant 在给定合适数据的类型之间转换的一般能力；仍然可以提供实际上无法转换的数据。

例如，当对包含字符串的变体调用时， canConvert (Int) 将返回 true，因为原则上，QVariant 能够将数字字符串转换为整数。但是，如果字符串包含非数字字符，则无法将其转换为整数，并且任何转换它的尝试都将失败。因此，重要的是让两个函数都返回 true 才能成功转换。

