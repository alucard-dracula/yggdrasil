# `construct_shared.hpp`

## 概述

该文件为对象构造提供统一的共享指针接口，并结合 `detail` 下的两个实现文件支持 Boost 和标准库智能指针：

- `detail/construct_shared_boost.hpp`：构造 `boost::shared_ptr<T>`；
- `detail/construct_shared_stl.hpp`：构造 `std::shared_ptr<T>`；
- 外层 `construct_shared.hpp`：根据编译宏选择公开的 `construct_shared` 返回类型。

该接口将构造参数转发给共享指针构造函数。对象构造发生异常时，detail 实现捕获异常并返回空共享指针。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/construct_shared.hpp>
```

外层文件包含：

```cpp
#include <yggr/smart_ptr_ex/detail/construct_shared_boost.hpp>
#include <yggr/smart_ptr_ex/detail/construct_shared_stl.hpp>
```

## Detail 后端

### `construct_shared_boost`

定义于：

```cpp
yggr::smart_ptr_ex::detail
```

接口形式为：

```cpp
template<typename T, typename ...Args>
inline boost::shared_ptr<T>
construct_shared_boost(BOOST_FWD_REF(Args)... args);
```

实现通过转发参数构造：

```cpp
boost::shared_ptr<T>(
    boost::forward<Args>(args)...)
```

构造失败时返回：

```cpp
boost::shared_ptr<T>();
```

### `construct_shared_stl`

当未定义以下宏时提供：

```cpp
YGGR_NO_CXX11_SMART_PTR
```

接口形式为：

```cpp
template<typename T, typename ...Args>
inline std::shared_ptr<T>
construct_shared_stl(BOOST_FWD_REF(Args)... args);
```

实现通过转发参数构造：

```cpp
std::shared_ptr<T>(
    boost::forward<Args>(args)...)
```

构造失败时返回空的：

```cpp
std::shared_ptr<T>();
```

两个后端都使用 `try/catch(...)`，不会把对象构造异常继续抛给调用方。

## 公开后端函数

外层文件将 detail 后端函数导出到：

```cpp
yggr::smart_ptr_ex
```

```cpp
using detail::construct_shared_boost;

#ifndef YGGR_NO_CXX11_SMART_PTR
using detail::construct_shared_stl;
#endif
```

因此可以显式选择后端：

```cpp
yggr::smart_ptr_ex::construct_shared_boost<T>(args...);
yggr::smart_ptr_ex::construct_shared_stl<T>(args...);
```

其中 STL 后端只有在标准库智能指针未被禁用时可用。

## 统一 `construct_shared`

外层文件将统一入口导出到：

```cpp
yggr::smart_ptr_ex
```

并进一步导出到：

```cpp
yggr
```

因此以下调用均可用：

```cpp
yggr::smart_ptr_ex::construct_shared<T>(args...);
yggr::construct_shared<T>(args...);
```

### STL 默认路径

当同时满足以下条件时，公开入口返回 `std::shared_ptr<T>`：

```cpp
!defined(YGGR_NO_CXX11_SMART_PTR)
&& defined(YGGR_SMART_PTR_USING_STL)
```

调用实现为：

```cpp
return construct_shared_stl<T>(
    boost::forward<Args>(args)...);
```

### Boost 默认路径

不满足 STL 默认路径时，公开入口返回 `boost::shared_ptr<T>`：

```cpp
return construct_shared_boost<T>(
    boost::forward<Args>(args)...);
```

这也是未启用 C++11 智能指针或未选择 STL 智能指针时的默认行为。

## 模板参数

统一接口的核心形式为：

```cpp
template<typename T, typename ...Args>
inline shared_ptr<T>
construct_shared(BOOST_FWD_REF(Args)... args);
```

| 参数 | 含义 |
| --- | --- |
| `T` | 要构造的对象类型 |
| `Args...` | 转发给 `T` 构造函数的参数 |

返回的 `shared_ptr<T>` 类型由智能指针选择宏决定。

## 旧编译器兼容

当定义以下宏时：

```cpp
YGGR_NO_CXX11_VARIADIC_TEMPLATES
```

文件和两个 detail 后端通过 Boost.Preprocessor 生成固定参数数量的重载：

```cpp
BOOST_PP_LOCAL_LIMITS(
    0,
    YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
```

每个重载仍然执行参数转发和异常转空指针处理，但最大构造参数数量由 `YGGR_PP_FOO_DEFAULT_PARAMS_LEN` 决定。

## 宏控制

| 宏 | 作用 |
| --- | --- |
| `YGGR_NO_CXX11_SMART_PTR` | 禁用 `std::shared_ptr` 后端和公开 STL 选择 |
| `YGGR_SMART_PTR_USING_STL` | 在 STL 智能指针可用时选择 `std::shared_ptr` 作为默认返回类型 |
| `YGGR_NO_CXX11_VARIADIC_TEMPLATES` | 使用预处理器生成固定参数数量的重载 |
| `YGGR_PP_FOO_DEFAULT_PARAMS_LEN` | 旧编译器分支支持的最大构造参数数量 |

## 异常行为

两个 detail 后端都采用以下语义：

```cpp
try
{
    return shared_ptr<T>(forwarded_args...);
}
catch(...)
{
    return shared_ptr<T>();
}
```

因此：

- 构造函数抛出的异常不会传播到调用方；
- 返回值可能为空；
- 调用方必须检查返回指针后再访问对象。

## 调用流程

标准库路径：

```text
yggr::construct_shared<T>(args...)
  -> construct_shared_stl<T>(args...)
  -> std::shared_ptr<T>(forwarded_args...)
```

Boost 路径：

```text
yggr::construct_shared<T>(args...)
  -> construct_shared_boost<T>(args...)
  -> boost::shared_ptr<T>(forwarded_args...)
```

## 与其他接口的关系

- `allocate_shared_ex.hpp` 在此基础上增加 allocator 支持，并调用 `allocate_shared`；
- `construct_interface.hpp` 使用 `interface_ptr<T, SPtr>` 包装接口类型，内部可能通过 `construct_shared` 创建对象；
- `shared_ptr.hpp` 使用相同的宏条件选择公开 `shared_ptr` 别名，并包含本文件。

## 使用限制与注意事项

- `T` 必须能够使用传入参数构造。
- 标准库后端要求未定义 `YGGR_NO_CXX11_SMART_PTR`。
- 只有在定义 `YGGR_SMART_PTR_USING_STL` 且 STL 后端可用时，统一入口才返回 `std::shared_ptr<T>`。
- 构造失败返回空指针，不应依赖异常捕获机制处理失败。
- 非 variadic templates 模式下，参数数量不能超过 `YGGR_PP_FOO_DEFAULT_PARAMS_LEN`。
- 修改默认智能指针选择时，应同步检查 `shared_ptr.hpp`、`allocate_shared_ex.hpp` 和 `construct_interface.hpp`。
