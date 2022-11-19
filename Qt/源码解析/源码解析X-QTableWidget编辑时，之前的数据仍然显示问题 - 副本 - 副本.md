# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# Qt源码解析-QTableWidget表格内容编辑有影子

## 问题描述与复现

QTableWidget中编辑状态时，以前的数据还是在背后显示。双击编辑的时候，删除掉以前的内容，但是还是显示在里面，影响后面输入的显示

![](D:\Work\Code\luxiang\CSDN\QtTable使用.gif)

## 问题分析

使用原始的QTableWidget测试，并没有这个问题，博主分析了所有的QSS配置，最后定位是某一条QSS配置问题

```css
QLineEdit{
	background:transparent;
}
```

### 问题分析2

```css
QLineEdit{
	border-image:url(:/*.png);
    background-image:url(:/*.png);
}
```

查看Qt官方说明：使用背景色依然会出现此类问题

```css

background-image  Url
The background image used for the widget. Semi-transparent parts of the image let the background-color shine through.
Example:

  QFrame { background-image: url(:/images/hydro.png) }
```



## 问题解决方法

```bash
修改此类配置
```



