# `operator_t.hpp`

## 概述

该头文件提供与 C++11 显式转换运算符能力相关的预处理器宏。它根据 Boost 配置判断编译器是否同时支持：

- C++11 显式转换运算符；
- C++11 `nullptr`。

如果两项能力都可用，宏会生成 `explicit`；否则生成空文本，以兼容旧编译器或旧标准模式。

## 头文件

```cpp
#include <yggr/support/operator_t.hpp>
```

## 依赖

```cpp
#ifndef BOOST_CONFIG_HPP
#include <boost/config.hpp>
#endif
```

该文件通过 Boost 配置宏判断当前编译环境，不定义运行时类型、函数或类。

## `YGGR_OP_TYPE_EXPLICIT`

文件定义函数式宏：

```cpp
YGGR_OP_TYPE_EXPLICIT()
```

当以下两个 Boost 能力宏都未定义时：

```cpp
BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
BOOST_NO_CXX11_NULLPTR
```

宏展开为：

```cpp
explicit
```

对应条件为：

```cpp
#if !defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS) \
    && !defined(BOOST_NO_CXX11_NULLPTR)
#define YGGR_OP_TYPE_EXPLICIT() explicit
#else
#define YGGR_OP_TYPE_EXPLICIT()
#endif
```

### 使用示例

```cpp
struct wrapper
{
    YGGR_OP_TYPE_EXPLICIT() operator bool() const;
};
```

在支持现代显式转换运算符的环境中展开为：

```cpp
explicit operator bool() const;
```

在不支持的环境中展开为：

```cpp
operator bool() const;
```

这样可以让同一份源代码兼容两类编译器能力。

## `YGGR_OP_BOOL_EXPLICIT`

文件还定义：

```cpp
#define YGGR_OP_BOOL_EXPLICIT YGGR_OP_TYPE_EXPLICIT
```

该宏是 `YGGR_OP_TYPE_EXPLICIT` 的语义别名，通常用于标记布尔转换运算符：

```cpp
YGGR_OP_BOOL_EXPLICIT() operator bool() const;
```

它的展开结果与 `YGGR_OP_TYPE_EXPLICIT()` 完全相同。

## 设计目的

显式 `operator bool` 可以避免对象在不期望的上下文中发生隐式整数或指针转换，但旧编译器可能不支持该语法。本文件通过统一宏让库代码能够写成：

```cpp
YGGR_OP_BOOL_EXPLICIT() operator bool() const;
```

并根据编译环境自动选择：

- 现代环境：使用显式布尔转换；
- 兼容环境：使用普通布尔转换。

## 与相关支持文件的关系

同目录的 `using_shared_ptr_operator_bool.hpp` 也处理智能指针的布尔运算符兼容，但它主要负责将基类的 `operator bool`/旧式布尔转换运算符引入派生类。

本文件只负责提供 `explicit` 关键字的条件宏，不直接实现或引入任何转换运算符。

## 宏状态总结

| 条件 | `YGGR_OP_TYPE_EXPLICIT()` 展开结果 |
| --- | --- |
| 支持显式转换运算符且支持 `nullptr` | `explicit` |
| 任一能力宏已定义 | 空文本 |

注意：该判断使用 Boost 配置宏的“能力缺失”语义。宏未定义表示对应能力可用，宏已定义表示对应能力不可用。

## 使用限制与注意事项

- `YGGR_OP_TYPE_EXPLICIT` 是函数式宏，调用时应写成 `YGGR_OP_TYPE_EXPLICIT()`。
- `YGGR_OP_BOOL_EXPLICIT` 是宏别名，通常也应写成 `YGGR_OP_BOOL_EXPLICIT()`。
- 该文件只控制语法展开，不保证旧编译器对 `operator bool` 的所有语义与现代编译器完全一致。
- 不要自行覆盖 Boost 配置宏；它们应由 Boost 配置头根据编译环境提供。
- 修改该宏时，应同步检查使用它的布尔转换和智能指针兼容代码。
