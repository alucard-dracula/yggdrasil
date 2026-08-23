# `pfto.hpp`

## 概述

该头文件提供与 Boost.Serialization PFTO（可能为函数参数类型修饰兼容机制）相关的预处理器宏，用于兼容不同 Boost 版本的函数声明和参数列表写法。

文件不提供运行时类、函数或数据结构，主要作用是编译期宏适配。

## 头文件

```cpp
#include <yggr/serialization/pfto.hpp>
```

## Boost 版本条件

当：

```cpp
BOOST_VERSION < 105900
```

时，文件会包含：

```cpp
#include <boost/serialization/pfto.hpp>
```

并生成带 `BOOST_PFTO` 的宏展开。

当 Boost 版本不低于 1.59 时，不包含 Boost 的 `pfto.hpp`，相关 yggdrasil 宏全部展开为空。

## 宏接口

### `YGGR_PFTO_TYPE`

```cpp
YGGR_PFTO_TYPE(__type__)
```

旧版 Boost 下展开为带逗号和 `BOOST_PFTO` 的类型片段：

```cpp
YGGR_PP_SYMBOL_COMMA() BOOST_PFTO __type__
```

新版 Boost 下展开为空。

### `YGGR_NON_PFTO_TYPE`

```cpp
YGGR_NON_PFTO_TYPE(__type__)
```

旧版 Boost 下展开为普通类型参数片段：

```cpp
YGGR_PP_SYMBOL_COMMA() __type__
```

新版 Boost 下展开为空。

### `YGGR_PFTO_TYPE_VAR`

```cpp
YGGR_PFTO_TYPE_VAR(__type__, __var__)
```

旧版 Boost 下展开为带 `BOOST_PFTO` 的类型和变量声明：

```cpp
YGGR_PP_SYMBOL_COMMA() BOOST_PFTO __type__ __var__
```

新版 Boost 下展开为空。

### `YGGR_NON_PFTO_TYPE_VAR`

```cpp
YGGR_NON_PFTO_TYPE_VAR(__type__, __var__)
```

旧版 Boost 下展开为普通类型和变量声明：

```cpp
YGGR_PP_SYMBOL_COMMA() __type__ __var__
```

新版 Boost 下展开为空。

### `YGGR_PFTO_VAR`

```cpp
YGGR_PFTO_VAR(__var__)
```

旧版 Boost 下展开为带逗号的变量参数片段：

```cpp
YGGR_PP_SYMBOL_COMMA() __var__
```

新版 Boost 下展开为空。

## 宏展开对比

| 宏 | Boost < 1.59 | Boost >= 1.59 |
| --- | --- | --- |
| `YGGR_PFTO_TYPE(T)` | `, BOOST_PFTO T` | 空 |
| `YGGR_NON_PFTO_TYPE(T)` | `, T` | 空 |
| `YGGR_PFTO_TYPE_VAR(T, v)` | `, BOOST_PFTO T v` | 空 |
| `YGGR_NON_PFTO_TYPE_VAR(T, v)` | `, T v` | 空 |
| `YGGR_PFTO_VAR(v)` | `, v` | 空 |

实际展开还包含 `YGGR_PP_SYMBOL_COMMA()` 生成的逗号，表格展示的是逻辑形式。

## 使用场景

这些宏适用于项目中需要同时兼容旧版和新版 Boost.Serialization 的模板声明、函数参数声明或宏生成代码。

示例形式：

```cpp
#define DECLARE_VALUE(__type__) \
    void process(YGGR_PFTO_TYPE_VAR(__type__, value))
```

在旧版 Boost 下，宏可以补充历史版本所需的参数修饰；在新版 Boost 下则不产生额外代码。

实际使用时应结合项目中调用这些宏的上下文，确保逗号和参数位置正确。

## 命名空间

文件声明了空命名空间：

```cpp
yggr::serialization
```

该命名空间当前没有新增成员。

## 注意事项

1. 该文件只提供预处理器宏，不提供运行时功能。
2. Boost 版本判断使用 `BOOST_VERSION < 105900`，版本号 `105900` 对应 Boost 1.59.0。
3. 旧版宏展开包含自动生成的逗号，使用时不要额外重复添加逗号。
4. 新版 Boost 下这些宏展开为空，调用宏的上下文必须允许这种空展开。
5. `YGGR_PFTO_TYPE` 和 `YGGR_NON_PFTO_TYPE` 的差异只在旧版 Boost 中体现。
6. 具体 `BOOST_PFTO` 语义由对应 Boost.Serialization 版本定义。
7. 使用这些宏生成声明时，应同时测试目标项目支持的旧版和新版 Boost 配置。
8. 文件依赖 `yggr/base/yggrdef.h` 和 `yggr/ppex/symbols.hpp` 提供的基础类型及预处理器工具。

## 相关组件

- `yggr/base/yggrdef.h`
- `yggr/ppex/symbols.hpp`
- `boost/serialization/pfto.hpp`
- `boost/version.hpp`
