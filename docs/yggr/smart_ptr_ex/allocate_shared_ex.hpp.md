# `allocate_shared_ex.hpp`

## 概述

该文件为基于 allocator 的共享指针构造提供统一接口，并结合 `detail` 下的两个实现文件支持 Boost 和标准库智能指针：

- `detail/allocate_shared_ex_boost.hpp`：调用 `boost::allocate_shared`；
- `detail/allocate_shared_ex_stl.hpp`：调用 `std::allocate_shared`；
- 外层 `allocate_shared_ex.hpp`：根据编译宏选择公开的 `allocate_shared` 返回类型。

该接口统一处理构造参数转发和异常转换：对象分配或构造失败时，detail 实现捕获异常并返回空共享指针。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/allocate_shared_ex.hpp>
```

## 依赖关系

外层文件包含：

```cpp
#include <yggr/smart_ptr_ex/detail/allocate_shared_ex_boost.hpp>
#include <yggr/smart_ptr_ex/detail/allocate_shared_ex_stl.hpp>
```

当未定义以下宏时还包含标准库头：

```cpp
#ifndef YGGR_NO_CXX11_SMART_PTR
#include <memory>
#endif
```

两个 detail 文件分别依赖 `boost/make_shared.hpp` 或 `<memory>`，并在不支持可变参数模板时依赖 Yggdrasil 的预处理器参数工具。

## Detail 实现

### `allocate_shared_boost`

定义于：

```cpp
namespace yggr::smart_ptr_ex::detail
```

接口形式为：

```cpp
template<typename T, typename A, typename ...Args>
inline boost::shared_ptr<T> allocate_shared_boost(
    const A& alloc,
    BOOST_FWD_REF(Args)... args);
```

实现调用：

```cpp
boost::allocate_shared<T>(
    alloc,
    boost::forward<Args>(args)...);
```

如果分配或构造过程中抛出任意异常，则返回：

```cpp
boost::shared_ptr<T>();
```

### `allocate_shared_stl`

定义于同一 detail 命名空间，只有在未定义 `YGGR_NO_CXX11_SMART_PTR` 时提供：

```cpp
template<typename T, typename A, typename ...Args>
inline std::shared_ptr<T> allocate_shared_stl(
    const A& alloc,
    BOOST_FWD_REF(Args)... args);
```

实现调用：

```cpp
std::allocate_shared<T>(
    alloc,
    boost::forward<Args>(args)...);
```

发生异常时返回空的：

```cpp
std::shared_ptr<T>();
```

## 公开 detail 函数

外层文件将 detail 函数导出到：

```cpp
yggr::smart_ptr_ex
```

```cpp
using detail::allocate_shared_boost;

#ifndef YGGR_NO_CXX11_SMART_PTR
using detail::allocate_shared_stl;
#endif
```

因此，在启用标准库智能指针时可以显式调用两个后端函数；Boost 后端始终可用。

## 统一 `allocate_shared`

外层文件还将统一入口导出到：

```cpp
yggr::smart_ptr_ex
```

并进一步导出到：

```cpp
yggr
```

调用形式为：

```cpp
yggr::smart_ptr_ex::allocate_shared<T>(alloc, args...);
yggr::allocate_shared<T>(alloc, args...);
```

### 标准库选择路径

当同时满足以下条件时，公开入口返回 `std::shared_ptr<T>`：

```cpp
!defined(YGGR_NO_CXX11_SMART_PTR)
&& defined(YGGR_SMART_PTR_USING_STL)
```

接口为：

```cpp
template<typename T, typename A, typename ...Args>
inline std::shared_ptr<T> allocate_shared(
    const A& alloc,
    BOOST_FWD_REF(Args)... args)
{
    return allocate_shared_stl<T>(
        alloc,
        boost::forward<Args>(args)...);
}
```

### Boost 选择路径

如果不满足标准库选择条件，公开入口返回 `boost::shared_ptr<T>`：

```cpp
template<typename T, typename A, typename ...Args>
inline boost::shared_ptr<T> allocate_shared(
    const A& alloc,
    BOOST_FWD_REF(Args)... args)
{
    return allocate_shared_boost<T>(
        alloc,
        boost::forward<Args>(args)...);
}
```

因此 `YGGR_SMART_PTR_USING_STL` 只在 C++11 智能指针可用时生效；否则自动使用 Boost.SharedPtr。

## 宏控制

| 宏 | 作用 |
| --- | --- |
| `YGGR_NO_CXX11_SMART_PTR` | 禁用 `std::shared_ptr` 和 `std::allocate_shared` 路径 |
| `YGGR_SMART_PTR_USING_STL` | 在标准库智能指针可用时选择 STL 作为默认公开实现 |
| `YGGR_NO_CXX11_VARIADIC_TEMPLATES` | 禁用 variadic templates，改用预处理器生成固定参数数量的重载 |
| `YGGR_PP_FOO_DEFAULT_PARAMS_LEN` | 非 variadic templates 模式下的最大参数数量 |

## 旧编译器兼容

当定义 `YGGR_NO_CXX11_VARIADIC_TEMPLATES` 时，两个 detail 文件都通过 Boost.Preprocessor 生成多个重载：

```cpp
BOOST_PP_LOCAL_LIMITS(
    0,
    YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
```

每个重载都会：

1. 接收 allocator 和固定数量的构造参数；
2. 使用 `YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD` 转发参数；
3. 调用对应的 `boost::allocate_shared` 或 `std::allocate_shared`；
4. 捕获异常并返回空共享指针。

该兼容路径保持与 variadic templates 路径相同的语义，但支持的最大构造参数数量由宏限制。

## 异常行为

两个 detail 后端都使用：

```cpp
try
{
    return allocate_shared(...);
}
catch(...)
{
    return shared_ptr<T>();
}
```

因此：

- allocator 不满足底层库要求时，具体行为取决于底层 `allocate_shared`；
- 对象构造异常不会向调用方传播；
- 调用方必须检查返回的共享指针是否为空。

## 调用流程

标准库路径：

```text
yggr::allocate_shared<T>(alloc, args...)
  -> allocate_shared_stl<T>(alloc, args...)
  -> std::allocate_shared<T>(alloc, args...)
  -> std::shared_ptr<T>
```

Boost 路径：

```text
boost/shared_ptr 公开选择
  -> allocate_shared_boost<T>(alloc, args...)
  -> boost::allocate_shared<T>(alloc, args...)
  -> boost::shared_ptr<T>
```

## 与其他智能指针扩展的关系

`smart_ptr_ex/shared_ptr.hpp` 包含本文件，并根据相同的宏选择公开的 `shared_ptr` 别名：

- 标准库路径使用 `std::shared_ptr`；
- 默认或兼容路径使用 `boost::shared_ptr`。

因此 `allocate_shared_ex.hpp` 的默认返回类型应与 `shared_ptr.hpp` 的公开智能指针选择保持一致。

## 使用限制与注意事项

- `A` 必须是底层 `allocate_shared` 支持的 allocator 类型。
- `T` 必须能使用传入的构造参数构造。
- 标准库路径要求未定义 `YGGR_NO_CXX11_SMART_PTR`，并且公开默认选择要求定义 `YGGR_SMART_PTR_USING_STL`。
- 发生异常时返回空指针，不应依赖异常被调用方捕获。
- 非 variadic templates 编译模式下，构造参数数量不能超过 `YGGR_PP_FOO_DEFAULT_PARAMS_LEN`。
- 修改默认智能指针选择时，应同步检查 `shared_ptr.hpp`、`allocate_interface.hpp` 和其他 shared pointer 扩展接口。
