# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# 源码解析6-系统封装-QLibrary插件加载

## 调用示例

```c++
 QLibrary myLib("mylib");
  typedef void (*MyPrototype)();
  MyPrototype myFunction = (MyPrototype) myLib.resolve("mysymbol");
  if (myFunction)
      myFunction();
```

## 调用堆栈

以主要的resolve为例

1. QFunctionPointer QLibrary::resolve(const char *symbol)
2. QFunctionPointer QLibraryPrivate::resolve(const char *symbol)
3. QFunctionPointer QLibraryPrivate::resolve_sys(const char* symbol)
4. 系统调用FARPROC address = GetProcAddress(pHnd, symbol);

## 源码路径

$(QTDIR)\qtbase\src\corelib\plugin

1. qlibrary.cpp:QLibrary类的封装实现，以及QLibraryPrivate的部分封装实现

2. qlibrary_p.h：QLibraryPrivate类定义与接口声明,系统相关的都声明为私有函数

   ```c++
   bool load_sys();//加载库
   bool unload_sys();//卸载库
   QFunctionPointer resolve_sys(const char *);//查找函数符号
   ```

3. QLibraryPrivate的操作系统相关封装实现：

   qlibrary_win.cpp  window系统实现 

   qlibrary_unix.cpp 类Linux系统实现

## 源码编译选择

$(QTDIR)\qtbase\src\corelib\plugin\plugin.pri文件控制了编译对不同操作系统的编译选择

    unix: SOURCES += plugin/qlibrary_unix.cpp
    else: SOURCES += plugin/qlibrary_win.cpp
## 操作系统函数解析

### windows平台

1. 加载库[**LoadLibrary**](https://blog.csdn.net/xinqingwuji/article/details/78597439)
2. 卸载库**FreeLibrary**
3. 查找函数符号**GetProcAddress**

### 类Linux平台

1. 加载库**dlopen**
2. 卸载库**dlclose**
3. 查找函数符号**dlsym**

详细的代码里面还有更详细的区分，此处列了常用的主要的接口。