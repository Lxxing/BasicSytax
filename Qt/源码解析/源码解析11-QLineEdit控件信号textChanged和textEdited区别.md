# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# Qt源码解析11-QLineEdit控件信号textChanged和textEdited区别

### 测试

基于分析QValidator的测试用例。

测试界面：



<img src="D:\Work\Code\luxiang\CSDN\QLineEdit使用1.gif" style="zoom:200%;" />

1. 输入正常数据，textChanged与textEdited函数都会触发
2. 输入无效字符，inputRejected函数触发
3. 调用setText，textChanged函数触发
4. 设置QRegExpValidator表达式，调用setText，依然能设置成功字符，接受的字符只触发textChanged，不接受的字符同时触发textChanged与inputRejected

# 

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



