# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# Qt源码解析2---外部程序嵌入到Qt进程界面

Qt程序开发时经常需要启动外部程序，这个通常使用QProcess挺好解决的。

还有一种情况时需要启动外部程序，并嵌入到我们开发的Qt程序中，这样系统看起来更像一个整体。例如我们使用Visual studio开发或者qt开发windows应用程序，想嵌入Windows附带的计算器(calc.exe)或者已有的第三方的应用软件。

从windows程序角度看。可以将calc嵌入到您自己的应用程序中,但它仍然可以在它自己的进程空间中运行. UAC可能还会施加一些限制,但这取决于计算方法的推出方式.
 您需要做的就是更改主calc窗口的父级,并将其样式更改为WS_C。

方法有多种，第一种，如果使用 的是qt开发，在qt5以上版本中，封装了接口。如果qt5以下版本或者window桌面程序，可以使用原生的window接口。

# 方法一（Qt5）

## 步骤一：启动程序

手动启动或者使用QProcess启动，此步略。

## 步骤二：获取窗口类名或者窗口句柄

![](D:\Work\Code\luxiang\BasicSytax\Qt\源码解析\image\spy++使用.gif)

## 步骤三：嵌入到QT窗口 

头文件

```C++
	WId calcID;
	QWindow * window;
	QWidget * childWdg;
```

源文件

```c++
QtEmbeded::QtEmbeded(QWidget *parent)
    : QWidget(parent),calcID(-1)
{
    ui.setupUi(this);
    /***********************************
    可以单独通过类名或者标题名获取，遇到有些第三方程序类名每次获取都是变化的，可只使用标题名参数
    CalcFrame是win7的计算器名称
    在win10上是ApplicationFrameWindow
    ***********************************/
	//calcID = (WId)FindWindow(L"ApplicationFrameWindow", NULL);
	//calcID = (WId)FindWindow(NULL, L"计算器");
	//calcID = (WId)FindWindow(L"Chrome_WidgetWin_1", NULL);//浏览器
}

void QtEmbeded::initQt()
{
	if (NULL == calcID)
	{
		return;
	}
	windowWnd = QWindow::fromWinId(calcID);
	//windowWnd->setWidth(this->width());
	//windowWnd->setHeight(this->height());无效
	
	childWdg = QWidget::createWindowContainer(windowWnd,this);
	 /*******
	 必须设置大小，否则可能因为嵌入后窗口太小看不到效果.设置一个黄色QWidget，为了排查为啥看不到嵌入窗口		***********************************/
    childWdg->setMinimumSize(QSize(500, 600));
   
}
```

嵌入未设置窗口大小时：

![](D:\Work\Code\luxiang\BasicSytax\Qt\源码解析\image\嵌入太小.png)

正常情况

![](D:\Work\Code\luxiang\CSDN\嵌入正常.png)

# 方法二（Win32接口）

现在目前还经常回遇到Qt4开发的情况，window平台没有很好的接口，这个时候需要用到。

```c++
void QtEmbeded::initWin()
{
	HWND hWnd = (HWND)this->winId();
	HWND calcHwnd = (HWND)calcID;
	// Change the parent so the calc window belongs to our apps main window 
	SetParent(calcHwnd, hWnd);

	// Update the style so the calc window is embedded in our main window
	SetWindowLong(calcHwnd, GWL_STYLE, GetWindowLong(calcHwnd, GWL_STYLE) | WS_CHILD);

	// We need to update the position as well since changing the parent does not
	// adjust it automatically.
	SetWindowPos(calcHwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
```

博主测试了浏览器程序，当本程序关闭时，会把嵌入的程序也关闭掉。

### FindWindow句法

​	检索其类名和窗口名与指定字符串匹配的顶级窗口的句柄。此函数不搜索子窗口。此函数不执行区分大小写的搜索。

要从指定的子窗口开始搜索子窗口，请使用FindWindowEx函数。

C++

```cpp
HWND FindWindowA(
  [in, optional] LPCSTR lpClassName,
  [in, optional] LPCSTR lpWindowName
);
```

### 参数

```c++
[in, optional] lpClassName
```

类型： **LPCTSTR** 

应用程序当前实例的句柄。

```
[in] hPrevInstance
```

类型：**HINSTANCE**

 如果*lpClassName*指向一个字符串，它指定窗口类名 。

 如果*lpClassName*为**NULL ，它会查找标题与***lpWindowName*参数匹配的任何窗口。 

```
[in, optional] lpWindowName
```

类型：**LPSTR**

 窗口名称（窗口的标题）。如果此参数为**NULL**，则所有窗口名称都匹配 。

### 返回值

类型：**HWND**

如果函数成功，则返回值是具有指定类名和窗口名的窗口句柄。

如果函数失败，则返回值为**NULL**。要获取扩展的错误信息,使用GetLastError。

