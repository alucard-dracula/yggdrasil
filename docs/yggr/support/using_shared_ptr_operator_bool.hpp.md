# `using_shared_ptr_operator_bool.hpp`

## 概述

该头文件提供智能指针类的布尔运算符兼容宏：

```cpp
USING_OPERATOR_BOOL(__base_type__)
```

宏用于将基类的布尔转换运算符和逻辑非运算符引入派生类，使包装类型能够像基类智能指针一样参与有效性判断。

源码顶部注释使用了 `using_operator_bool.hpp` 名称，但实际文件名、include guard 和公开宏均属于 `using_shared_ptr_operator_bool.hpp`。

## 头文件与依赖

```cpp
#include <yggr/support/using_shared_ptr_operator_bool.hpp>
```

该文件确保包含：

```cpp
#include <boost/config.hpp>
```

它只依赖 Boost 配置宏，不定义运行时类型或函数。

## `USING_OPERATOR_BOOL`

宏接口为：

```cpp
USING_OPERATOR_BOOL(__base_type__)
```

参数 `__base_type__` 应是拥有布尔转换运算符和 `operator!` 的基类类型。

现代 C++ 环境下，若以下两个 Boost 能力宏均未定义：

```cpp
BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
BOOST_NO_CXX11_NULLPTR
```

宏展开为：

```cpp
using __base_type__::operator bool;
using __base_type__::operator!;
```

旧式兼容环境下展开为：

```cpp
using __base_type__::operator unspecified_bool_type;
using __base_type__::operator!;
```

## 编译器分支

### 现代 C++ 路径

支持 C++11 显式转换运算符和 `nullptr` 时引入 `operator bool`。

### Sun Studio 旧版本和 CINT

以下条件下仍引入 `operator bool`：

```cpp
(defined(__SUNPRO_CC) && BOOST_WORKAROUND(__SUNPRO_CC, < 0x570))
|| defined(__CINT__)
```

### Managed C++

定义 `_MANAGED` 时引入：

```cpp
operator unspecified_bool_type
```

### 旧编译器

以下环境使用旧式安全布尔转换：

```cpp
(
    defined(__MWERKS__)
    && BOOST_WORKAROUND(__MWERKS__, < 0x3200)
)
|| (
    defined(__GNUC__)
    && (__GNUC__ * 100 + __GNUC_MINOR__ < 304)
)
|| (
    defined(__SUNPRO_CC)
    && BOOST_WORKAROUND(__SUNPRO_CC, <= 0x590)
)
```

其他未匹配环境也默认使用 `unspecified_bool_type` 路径。

## 使用示例

```cpp
template<typename Base>
class wrapper : public Base
{
public:
    USING_OPERATOR_BOOL(Base)
};
```

这样 `wrapper` 可以复用基类的有效性判断：

```cpp
wrapper<Base> value;

if(value)
{
    // 基类对象有效
}

if(!value)
{
    // 基类对象为空
}
```

## 与 `operator_t.hpp` 的区别

| 文件 | 功能 |
| --- | --- |
| `operator_t.hpp` | 根据编译器能力决定是否生成 `explicit` |
| `using_shared_ptr_operator_bool.hpp` | 将基类的布尔转换和 `operator!` 引入派生类 |

本文件不生成新的转换运算符，只使用 `using` 声明复用基类接口。

## 宏分支总结

| 条件 | 引入的运算符 |
| --- | --- |
| 支持 C++11 显式转换运算符和 `nullptr` | `operator bool`、`operator!` |
| 旧 Sun Studio 或 CINT | `operator bool`、`operator!` |
| `_MANAGED` | `operator unspecified_bool_type`、`operator!` |
| 旧 Metrowerks、旧 GCC、旧 Sun Studio | `operator unspecified_bool_type`、`operator!` |
| 其他情况 | `operator unspecified_bool_type`、`operator!` |

## 注意事项

- `__base_type__` 必须实际提供宏所引用的转换运算符和 `operator!`。
- 使用旧式路径时，基类必须定义可访问的 `unspecified_bool_type`。
- 不要在同一派生类中重复引入冲突的布尔转换运算符。
- Boost 配置宏应由 Boost 配置头提供，不应由业务代码随意覆盖。
