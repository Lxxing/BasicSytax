# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# Qt源码解析-QCefView编译

## 环境配置

### 1. CMake：

最低要求版本为3.19.1，推荐使用最新版本 

### 2. Qt

QCefView是基于Qt框架的，同时支持Qt 5.x和6.x版本。安装Qt之后请确保设置了**QTDIR**环境变量，使其指向Qt工具链目录，例如：

Windows平台：

```bat
set QTDIR=C:\Qt\6.2.2\msvc2019_64
```

macOS平台：

```bash
export QTDIR=/usr/local/Cellar/qt5/5.4.1/clang_64
```

Linux平台：

```bash
export QTDIR=/usr/share/Qt/6.2.2/gcc_64
```

## 编译构建

从https://github.com/CefView/QCefView检出最新代码，不要忘记初始化submodule。该项目依赖子模块：https://github.com/CefView/CefViewCore。

```bat
 git submodule update --init --recursive   
```

### Windows

```bash
# Generate VS projects
generate-win-proj.bat
# Build from cmake
cmake --build .build/Windows
```

生成项目之后也可以在目录`.build/Windows`中找到Visual Studio项目文件，使用Visual Studio打开和编译。

### macOS

```bash
# Generate Xcode project
./generate-mac-proj.sh
# Build from cmake 
cmake --build ./build/macOS
```

生成项目之后也可以在目录`.build/macOS`中找到Xcode项目文件，使用Xcode打开和编译。

### Linux

```bash
# Generate Unix Make file project
./generate-linux-proj.sh
# Build from cmake 
cmake --build ./build/Linux
```

Linux平台推荐使用Qt Creator作为IDE。

 **CEF：**Chromium Embedded Framework 

## 构建过程问题

### 编译64位版本

编译64位时，如果自己去配置，过程会遇到很多需要修改的东西。库的版本，程序运行初始化需要的东西，很多都会出错。

建议还是直接修改generate-win-proj.bat文件内容，使用工具生成64位

```bash
cmake -S . -B .build/Windows -DBUILD_DEMO=ON -G "Visual Studio 15 2017 Win64"
```

### 运行初始化崩溃

  int _retval = cef_initialize(
      &args, &settings, CefAppCppToC::Wrap(application), windows_sandbox_info);

崩溃。查找好久原因，特别是再cef不熟悉的情况下

解决方法：把CEFDIR/Resources目录下的内容拷贝到CEF/Release（或Debug）下即可。CEF需要里面的icudtl.dat、语言文件等 

### CMake编译失败(VS 2015)

[stackoverflow相似问题](https://stackoverflow.com/questions/42701019/problems-generating-solution-for-vs-2017-with-cmake)

用cmake编译时，显示No CMAKE_C_COMPLIER could be found。

在cmakerror.log文件中:

 Compiling the C compiler identification source file "CMakeCCompilerId.c" failed 

显示"链接：错误，无法打开文件“ucrtd.lib”"

类似的问题导致的现象还有（可以手动重定向Windows SDK来解决）

- MSB8036 找不到 Windows SDK 版本 8.1
- 无法打开包括文件stdio.h ctype.h ucrtd.lib等标准库文件

原因分析：
这个报错可能原因是很多的。网络搜索好多方法是重新安装VS或者安装全套的Windows  SDK 。有些时候可以解决问题，并没有把根本原因描述出来。

**根本原因**是VS2015中默认设置Windows kits版本与实际安装的Windows SDK版本号不匹配。查看剧本版本可通过网络搜索查找。

**修改方法：**查找Microsoft.Cpp.Common.props文件中的TargetUniersalCRTVersion标签项

详细查看[MSBuild](https://docs.microsoft.com/en-us/visualstudio/msbuild/build-process-overview?view=vs-2022)主题