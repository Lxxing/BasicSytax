# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# Qt源码解析2---GUI入口函数

Qt程序在window下的入口源码

qt-everywhere-src-5.13.2\qtbase\src\winmain\qtmain_win.cpp

```C++
#if defined(QT_NEEDS_QMAIN)
int qMain(int, char **);
#define main qMain
#else
extern "C" int main(int, char **);
#endif

/*
  WinMain() - Initializes Windows and calls user's startup function main().
  NOTE: WinMain() won't be called if the application was linked as a "console"
  application.
*/

extern "C" int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR /*cmdParamarg*/, int /* cmdShow */)
{
	...
    const int exitCode = main(argc, argv);
	...
}
```



window下入口函数参考： 用户提供的基于 Windows 的图形应用程序的入口点。 

# WinMain 函数 (winbase.h)

- 用程序的入口点。

**WinMain**是用于应用程序入口点的常规名称。

## 句法

C++

```cpp
int __clrcall WinMain(
  [in] HINSTANCE hInstance,
  [in] HINSTANCE hPrevInstance,
  [in] LPSTR     lpCmdLine,
  [in] int       nShowCmd
);
```

## 参数

```
[in] hInstance
```

类型：**HINSTANCE**

应用程序当前实例的句柄。

```
[in] hPrevInstance
```

类型：**HINSTANCE**

应用程序前一个实例的句柄。此参数始终为**NULL**。如果您需要检测另一个实例是否已经存在，请使用[CreateMutex](https://docs.microsoft.com/en-us/windows/desktop/api/synchapi/nf-synchapi-createmutexa)函数创建一个唯一命名的互斥锁。即使互斥锁已经存在，**CreateMutex**也会成功，但函数会返回**ERROR_ALREADY_EXISTS**. 这表明您的应用程序的另一个实例存在，因为它首先创建了互斥锁。但是，恶意用户可以在您之前创建此互斥锁并阻止您的应用程序启动。为防止出现这种情况，请创建一个随机命名的互斥锁并存储该名称，以便只能由授权用户获取。或者，您可以为此目的使用文件。要将您的应用程序限制为每个用户一个实例，请在用户的配置文件目录中创建一个锁定文件。

```
[in] lpCmdLine
```

类型：**LPSTR**

应用程序的命令行，不包括程序名称。要检索整个命令行，请使用[GetCommandLine](https://docs.microsoft.com/en-us/windows/desktop/api/processenv/nf-processenv-getcommandlinea)函数。

```
[in] nShowCmd
```

类型：**int**

控制窗口的显示方式。此参数可以是可以在[ShowWindow](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-showwindow)函数的*nCmdShow*参数中指定的任何值。

## 返回值

类型：**int**

如果函数成功，当它接收到[WM_QUIT](https://docs.microsoft.com/en-us/windows/desktop/winmsg/wm-quit)消息时终止，它应该返回包含在该消息的*wParam*参数中的退出值。如果函数在进入消息循环之前终止，它应该返回零。

## 评论

许多编程框架按照惯例使用名称**WinMain**。根据编程框架，可以在调用**WinMain**函数之前和之后执行特定于该框架的附加活动。

您的**WinMain**应该初始化应用程序，显示其主窗口，并进入一个消息检索和分发循环，这是应用程序执行剩余部分的顶级控制结构。当它收到[WM_QUIT](https://docs.microsoft.com/en-us/windows/desktop/winmsg/wm-quit)消息时终止消息循环。此时，您的**WinMain**应该退出应用程序，返回在**WM_QUIT**消息的*wParam*参数中传递的值。如果作为调用[PostQuitMessage](https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-postquitmessage)的结果接收到**WM_QUIT**，则*wParam*的值是**PostQuitMessage**函数的*nExitCode*参数的值。有关更多信息，请参阅[创建消息循环](https://docs.microsoft.com/en-us/windows/desktop/winmsg/using-messages-and-message-queues)。

ANSI 应用程序可以使用**WinMain**函数的*lpCmdLine*参数来访问命令行字符串，不包括程序名称。请注意，*lpCmdLine*使用**LPSTR**数据类型而不是**LPTSTR**数据类型。这意味着**WinMain**不能被 Unicode 程序使用。该[GetCommandLineW](https://docs.microsoft.com/en-us/windows/desktop/api/processenv/nf-processenv-getcommandlinea)函数可以被用来获取命令行作为Unicode字符串。某些编程框架可能会提供一个替代入口点来提供 Unicode 命令行。例如，Microsoft Visual Studio C++ 编译器使用名称**wWinMain**作为 Unicode 入口点。

## 例子

下面的代码示例演示了**WinMain**的使用

```cpp
#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    return MessageBox(NULL, "hello, world", "caption", 0);
}
```

# 入口点含义

### 在 Visual Studio 开发环境中设置此链接器选项

1. 打开项目的“属性页” 对话框。 

2. 选择"**配置属性****链接器****高级"**属性页。

3. 修改 **入口点** 属性。

   ![无标题](C:\Users\xiaox\Desktop\无标题.png)

/ENTRY 选项将入口点函数指定为文件或 DLL .exe起始地址。

必须定义 函数，以使用 **`__stdcall`** 调用约定。 参数和返回值取决于程序是控制台应用程序、Windows 应用程序还是 DLL。 建议让链接器设置入口点，以便正确初始化 C 运行时库，并执行静态对象的 C++ 构造函数。

默认情况下，起始地址是 C 运行时库中的函数名称。 链接器根据程序的属性选择它，如下表所示。

| 函数名称                                           | 默认为                                                       |
| :------------------------------------------------- | :----------------------------------------------------------- |
| **mainCRTStartup** (或**wmainCRTStartup)**         | 使用 /SUBSYSTEM：CONSOLE 的应用程序;调用 `main` (或 `wmain`) |
| **WinMainCRTStartup (** 或 **wWinMainCRTStartup**) | 使用 /SUBSYSTEM 的应用程序**：WINDOWS**;调用 (或 `wWinMain`) ，必须定义为使用 **`__stdcall`** |
| **_DllMainCRTStartup**                             | A DLL;如果存在 `DllMain` ，则调用 ，必须定义该调用以使用 **`__stdcall`** |

如果 [未指定 /DLL](https://docs.microsoft.com/zh-cn/cpp/build/reference/dll-build-a-dll?view=msvc-170) 或 [/SUBSYSTEM](https://docs.microsoft.com/zh-cn/cpp/build/reference/subsystem-specify-subsystem?view=msvc-170) 选项，则链接器将根据是否定义了 或 来选择子系统 和入口点。

函数 `main` 、 `WinMain` 和 `DllMain` 是用户定义的入口点的三种形式。

创建托管映像时，指定给 /ENTRY 的函数必须具有 (LPVOID *var1、DWORD**var2、LPVOID**var3*) 。

若要了解如何定义自己的入口点，请参阅 `DllMain` DLL 和Visual C++ `DllMain` 。