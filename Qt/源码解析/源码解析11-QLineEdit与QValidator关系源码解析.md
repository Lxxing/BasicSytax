# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# Qt源码解析11-QLineEdit与QValidator关系源码解析

### 测试

本想了解QRegExpValidator的正则表达式如何生效的，发现分析起来比想象的复杂。

测试实例：

```c++
// regexp: optional '-' followed by between 1 and 3 digits
	QRegExp rx("-?\\d{1,3}");
	QValidator *validator = new QRegExpValidator(rx, this);

	ui.lineEdit->setValidator(validator);
```

测试界面：

键盘操作输入 ***-*** 号，数字，输入其他字母和字符是输入不了的。

![](D:\Work\Code\luxiang\CSDN\QLineEdit使用.gif)

问题：QRegExpValidator是如何生效的。

问题2：每次输入后，字符输入不了的原因。这个是在分析过程中产生的。分析前认为每次都会调用QRegExpValidator表达式去判断输入的正确性。

### 分析

#### **步骤0：**

查看设置setValidator函数，分析control变量对QValidator的处理。在输入字符串后并没有调用获取QValidator的函数（实际上搜索出错，没有把所有函数所有全面）。产生了上面的问题2。

```c++
void QLineEdit::setValidator(const QValidator *v)
{
    Q_D(QLineEdit);
    d->control->setValidator(v);
}
```

共有三个函数应用了QValidator变量。对窗口路径不感兴趣的同学可以直接直接挑战步骤4

```c++
bool QWidgetLineControl::fixup() 

bool QWidgetLineControl::finishChange(int validateFromState, bool update, bool edited)

bool QWidgetLineControl::hasAcceptableInput(const QString &str) const
```

#### **步骤1：**

分析问题2需要跟踪窗口处理流程，参见windows窗口处理函数

跟踪后发现，Qt把所有的windows事件放在WM_QT_SENDPOSTEDEVENTS事件中处理

```cpp
    WM_QT_SOCKETNOTIFIER = WM_USER,
    WM_QT_SENDPOSTEDEVENTS = WM_USER + 1,
    WM_QT_ACTIVATENOTIFIERS = WM_USER + 2,
```

#### **步骤2：**

进一步跟踪，要想知道输入后的操作，跟踪到函数中，找到处理输入的操作。

```c++
bool QWidget::event(QEvent *event)
{
    //Step 1.ignore mouse and key events when disabled.忽略失能的widget事件处理
    ...
    //Step 2
    case QEvent::KeyPress:
    	QKeyEvent *k = (QKeyEvent *)event;
        bool res = false;
        if (!(k->modifiers() & (Qt::ControlModifier | Qt::AltModifier))) {  //### Add MetaModifier?
            if (k->key() == Qt::Key_Backtab
                || (k->key() == Qt::Key_Tab && (k->modifiers() & Qt::ShiftModifier)))
                res = focusNextPrevChild(false);
            else if (k->key() == Qt::Key_Tab)
                res = focusNextPrevChild(true);
            if (res)
                break;
        }
        keyPressEvent(k);//进入QLineEdit的重载函数
    	break;
}
```

**总结：**

QLineEdit有个很重要的QWidgetLineControl类型成员变量，setValidator这个函数最终也是通过此成员变量实现。所以两个问题有合并为一个问题----QWidgetLineControl对输入的处理。

#### **步骤3：**

真实处理插入字符的函数

```c++
/*!
    \internal

    Inserts the given \a newText at the current cursor position.
    If there is any selected text it is removed prior to insertion of
    the new text.
*/
void QWidgetLineControl::insert(const QString &newText)
{
    int priorState = m_undoState;
    removeSelectedText();
    internalInsert(newText);
    finishChange(priorState);//此函数产生效果需要参考QLineEdit信号textChanged和textEdited区别
}
```

#### **步骤4：** 

**finishChange**函数中处理字符有效性问题

```c++
m_validator->validate(textCopy, cursorCopy)
```

最终调用的还是:

```c++
bool QRegExp::exactMatch(const QString &str) const
```

```c++
/*QRegExp类中，结构 QRegExpPrivate 包含除自动机之外的正则表达式的私有数据。它使许多 QRegExp 对象可以将相同的 QRegExpEngine 对象与不同的 QRegExpPrivate 对象一起使用。*/
private:
    QRegExpPrivate *priv;
```

到此步骤才是真正想分析的。标题都改了，QValidator源码分析单独出一期。

# QLineEdit控件信号textChanged和textEdited区别

```c++
[signal] void QLineEdit::textChanged(const QString &text)
    
This signal is emitted whenever the text changes. The text argument is the new text.
Unlike textEdited(), this signal is also emitted when the text is changed programmatically, for example, by calling setText().
Note: Notifier signal for property text. 
/*每当文本更改时，都会发出此信号。文本参数是新文本。
与 textEdited（） 不同，当以编程方式更改文本时，也会发出此信号，例如，通过调用 setText（）。
注意：属性文本的通知程序信号。*/
    
[signal] void QLineEdit::textEdited(const QString &text)
    
This signal is emitted whenever the text is edited. The text argument is the new text.
Unlike textChanged(), this signal is not emitted when the text is changed programmatically, for example, by calling setText().
    /*每当编辑文本时，都会发出此信号。文本参数是新文本。
与 textChanged（） 不同，当以编程方式更改文本（例如，通过调用 setText（））时，不会发出此信号。*/
    
```



# windows窗口处理函数

## 一、注册

```c++
	WNDCLASS wc;
    wc.style = 0;
    wc.lpfnWndProc = qt_internal_proc;
	...
    atom = RegisterClass(&wc);
```

lpfnWndProc的声明原型：

```c++
LRESULT (CALLBACK* WNDPROC) (HWND, UINT, WPARAM, LPARAM);
```

Qt通过qt_internal_proc函数处理事件消息。

## 参考：callWindowProcA 函数(winuser.h)

 将消息信息传递给指定的窗口过程。 

### 语法

C++复制

```cpp
LRESULT CallWindowProcA(
  [in] WNDPROC lpPrevWndFunc,
  [in] HWND    hWnd,
  [in] UINT    Msg,
  [in] WPARAM  wParam,
  [in] LPARAM  lParam
);
```

### 参数

```cpp
[in] lpPrevWndFunc
```

类型： **WNDPROC**

上一个窗口过程。 如果通过调用设置为**GWL_WNDPROC**或**DWL_DLGPROC**的 *nIndex* 参数的 GetWindowLong 函数来获取此值，则它实际上是窗口或对话框过程的地址，或者仅对 **CallWindowProc** 有意义的特殊内部值。

```
[in] hWnd
```

类型：**HWND**

用于接收消息的窗口过程的句柄。

```
[in] Msg
```

类型： **UINT**

消息。

```
[in] wParam
```

类型： **WPARAM**

其他的消息特定信息。 此参数的内容取决于 *Msg* 参数的值。

```
[in] lParam
```

类型： **LPARAM**

其他的消息特定信息。 此参数的内容取决于 *Msg* 参数的值。

### 返回值

类型： **LRESULT**

返回值指定消息处理的结果，具体取决于发送的消息。