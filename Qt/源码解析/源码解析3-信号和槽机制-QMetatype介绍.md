# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# Qt源码解析3-信号和槽机制-QMetaType 知道你的类型


​	QMetaType 是 Qt 获取有关您的类型的运行时动态信息的方式。它支持诸如自定义类型的 QVariant 包装、排队连接参数的复制等。

​	如果您想知道做什么Q_DECLARE_META_TYPE 或qRegisterMetaType做什么以及何时使用（或不使用）它们，请继续阅读。本文将描述您需要了解的关于 QMetaType 的知识：它的用途是什么；如何使用它; 以及它是如何工作的。

# 1 为什么 Qt 需要运行时动态类型信息？

​	让我们从一点历史开始。QMetaType在 Qt 4.0 中引入。创建它是为了有可能拥有异步信号和插槽 ( Qt::QueuedConnection)。为了使排队的插槽工作，我们必须复制参数并将它们存储在稍后将处理的事件中。我们还需要在调用完插槽后删除这些副本。（注意：使用Qt::DirectConnection: 时不需要这样做，直接在堆栈上使用指向参数的指针。）

​	调度信号的代码 有一个指向 arguments 的指针数组。但是，当时，Qt 对参数类型的所有了解都是它们作为字符串的名称，由 moc 提取。 QMetaObject::activate void*

QMetaType提供了一种从字符串（例如"QPoint"）获取复制或销毁对象的方法。Qt 然后会使用`void *QMetaType::create(int type, void *copy)`和`QMetaType::destroy(int type, void *data)`来复制和销毁参数，其中int type 是使用QMetaType::type(const char *typeName)参数的类型名称获得的，由 moc 提供。QMetaType 还为开发人员提供了一种在元类型数据库中注册任何类型的方法。

​	QMetaType 的另一个用例是 QVariant。Qt 3.x 中的 QVariant 仅支持内置 Qt 类型，因为包含的任意类型也需要与包装 QVariant 一起复制或销毁。但是在 QMetaType 的帮助下，QVariant 能够包含任何已注册的类型，因为 QVariant 现在可以复制和销毁包含的对象实例。

# 2 QMetaType 保留了哪些信息？

​	自 Qt 4.0 以来，发生了很多变化。我们现在有 QtScript 和 QML，它们正在大量使用动态类型集成。我们必须进行很多优化。

以下是元类型系统中为每种类型保留的信息列表：

- 注册的类型名称。有一个用于快速查找元类型 id 的名称索引。从 Qt 4.7 开始，甚至可以使用不同的名称注册相同的类型（对 typedef 很有用）。

- （复制）构造函数和析构函数（就地与否）。

- 大小以了解为堆栈或内联成员构造分配多少空间。

- 指定与QTypeInfo（见下文）或转换类型相同的信息的标志。

- 自定义转换函数，由QMetaType::registerConverter.

- QMetaObject，包含与类型关联的元 QObject 数据（如果存在）。

- …

**QTypeInfo**
QTypeInfo是与 QMetaType 正交的 trait 类，它允许开发人员手动指定（使用Q_DECLARE_TYPEINFO）类型是可移动的（使用 memmove），或者是否需要运行其构造函数/析构函数。这主要用于像 QVector 这样的容器中的优化。

例如，可以使用 memmove 移动隐式共享的类。而普通副本应该首先使用复制构造函数增加引用计数，然后在析构函数中减少它。

C++11 引入了移动构造函数和标准类型特征来解决这个问题，但是由于 QTypeInfo 是在 C++11 之前设计的，并且 Qt 仍然需要使用旧的编译器，所以我们不得不这样做。

# 3 它是如何工作的？
​	由于历史原因，内置类型和自定义类型之间存在很大差异。对于 QtCore 中的内置类型，每个元类型函数基本上都是一个开关，每种类型都有特殊的代码。在 Qt 5.0 中，这被重构为大量使用模板。（请参阅QMetaTypeSwitcher。）但本文让我们感兴趣的是它如何用于自定义注册类型。

有一个简单的 包含所有信息和一堆函数指针。QVector<QCustomTypeInfo>

**Q_DECLARE_METATYPE宏。**
	该宏专门QMetaTypeId针对特定类型的模板类。（事实上，它实际上专门化了这个类QMetaTypeId2，并且大部分代码都使用了 QMetaTypeId2。我不知道 QMetaTypeId2 背后的确切原因。也许这样 Qt 可以在不破坏以前使用的代码的情况下添加更多内置类型Q_DECLARE_METATYPE。）

QMetaTypeId用于在编译时确定类型的元类型 ID。
QMetaTypeId::qt_metatype_id是调用的函数qMetaType<T>()。在第一次调用这个函数时，它会调用 QMetaType 中的一些内部函数来为这个类型注册和分配一个元类型 id，使用宏中指定的名称。然后它将该 id 存储在静态变量中。

除了名称之外，所有其他信息都由编译器使用模板自动推断。

**qRegisterMetaType**
使用注册的类型Q_DECLARE_METATYPE将在第一次使用时实际注册（并分配一个 id）qMetaTypeId()。例如，当一个类型被包装在 QVariant 中时就是这种情况。但这在连接信号和插槽时尚未注册。在这种情况下，您需要使用强制第一次使用 qRegisterMetaType

# 4 自动注册

​	开发人员经常忘记注册他们的元类型，直到他们看到编译错误或运行时错误告诉他们这样做。但是，如果没有必要，那不是很好吗？唯一Q_DECLARE_METATYPE需要的原因是获得名称。但是在某些情况下，我们可以在没有该宏的情况下在运行时知道名称。例如，QList<T>如果 T 已经注册，我们可以查询元类型系统并使用 构造名称"QList<" + QMetaType::name(qMetaTypeId<T>()) + ">"
我们对一堆模板类执行此操作，例如：QList, QVector, QSharedPointer, QPointer, QMap, QHash, ...
我们还可以确定指向 QObject 子类的指针的名称，这要归功于moc 提供的信息： T::staticMetaObject.className() + "*"
而且从 Qt 5.5 开始，我们也会自动声明Q_GADGETand Q_ENUM。

​	就是这样Q_DECLARE_METATYPE，但您仍然需要调用qRegisterMetaType以在 a 中使用这些类型Q_PROPERTY或作为信号/插槽排队连接中的参数。但是，从 Qt 5.x 开始，qRegisterMetaType如果 moc 可以确定类型可能注册为元类型，则 moc 生成的代码将调用您。

# 5 研究

在 Qt 5.0 之前，我试图调查我们是否无法摆脱Q_DECLARE_METATYPE不需要名称的情况。这以某种方式像这样工作：

```c++
template<typename T> QMetaTypeId {
    static int qt_metatype_id() {
        static int typeId = QMetaType::registerMetaType(/*...*/);
        return typeId;
    }
};
```


根据 C++ 标准，QMetaTypeId::qt_metatype_id()::typeId每种类型都应该有一个变量的实例。但在实践中，一些编译器或链接器不遵守这条规则。特别是在 Windows 上，即使使用正确的导出宏，每个库也会有一个实例。因此，我们总是需要一个我们没有的名称标识符。（而且我们不想依赖 RTTI）。因此，我们只注册在 Qt 5 中我们可以知道名称的类型。

