# Qt源码解析 [索引](https://blog.csdn.net/xinqingwuji/article/details/118365888)

# LoadLibraryExA函数说明

## 语法

```c++
HMODULE LoadLibraryExA(
  [in] LPCSTR lpLibFileName,
       HANDLE hFile,
  [in] DWORD  dwFlags
);
```

## 参数

 [in] lpLibFileName 。参考[LoadLibrary](https://blog.csdn.net/xinqingwuji/article/details/78597439) 

```
hFile
```

此参数保留为将来使用. It must be **NULL**.

```
[in] dwFlags
```

加载模块时要执行的操作。如果未指定任何标志，则此函数的行为与 [LoadLibrary](https://blog.csdn.net/xinqingwuji/article/details/78597439)  函数的行为相同。此参数可以是以下值之一。

- DONT_RESOLVE_DLL_REFERENCES

   0x00000001

- LOAD_IGNORE_CODE_AUTHZ_LEVEL
  0x00000010

- LOAD_LIBRARY_AS_DATAFILE
  0x00000002

- LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE
  0x00000040

- LOAD_LIBRARY_AS_IMAGE_RESOURCE
  0x00000020

- LOAD_LIBRARY_SEARCH_APPLICATION_DIR
  0x00000200

- LOAD_LIBRARY_SEARCH_DEFAULT_DIRS
  0x00001000

- LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR
  0x00000100

- LOAD_LIBRARY_SEARCH_SYSTEM32
  0x00000800

- LOAD_LIBRARY_SEARCH_USER_DIRS
  0x00000400

- LOAD_WITH_ALTERED_SEARCH_PATH
  0x00000008

- LOAD_LIBRARY_REQUIRE_SIGNED_TARGET
  0x00000080

- LOAD_LIBRARY_SAFE_CURRENT_DIRS
  0x00002000





