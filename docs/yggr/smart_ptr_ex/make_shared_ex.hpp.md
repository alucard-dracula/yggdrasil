# `make_shared_ex.hpp`

## 概述

该文件为对象创建提供统一的 `make_shared` 接口，并结合 `detail` 下的 Boost/STL 实现支持两种共享指针体系：

- `detail/make_shared_ex_boost.hpp`：调用 `boost::make_shared`；
- `detail/make_shared_ex_stl.hpp`：调用 `std::make_shared`；
- 外层 `make_shared_ex.hpp`：根据编译宏选择公开入口和返回类型。

该接口将构造参数转发给 `T` 的构造函数，并在对象分配或构造失败时返回空共享指针。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/make_shared_ex.hpp>
```

外层文件包含：

```cpp
#include <yggr/smart_ptr_ex/detail/make_shared_ex_boost.hpp>
#include <yggr/smart_ptr_ex/detail/make_shared_ex_stl.hpp>
```

## Detail 后端

### `make_shared_boost`

定义于：

```cpp
yggr::smart_ptr_ex::detail
```

接口为：

```cpp
template<typename T, typename ...Args>
inline boost::shared_ptr<T>
make_shared_boost(BOOST_FWD_REF(Args)... args);
```

实现调用：

```cpp
boost::make_shared<T>(
    boost::forward<Args>(args)...);
```

异常处理为：

```cpp
catch(...)
{
    return boost::shared_ptr<T>();
}
```

### `make_shared_stl`

当未定义以下宏时提供：

```cpp
YGGR_NO_CXX11_SMART_PTR
```

接口为：

```cpp
template<typename T, typename ...Args>
inline std::shared_ptr<T>
make_shared_stl(BOOST_FWD_REF(Args)... args);
```

实现调用：

```cpp
std::make_shared<T>(
    boost::forward<Args>(args)...);
```

异常时返回空的 `std::shared_ptr<T>`。

## 公开后端函数

外层文件将 detail 函数导出到：

```cpp
yggr::smart_ptr_ex
```

```cpp
using detail::make_shared_boost;

#ifndef YGGR_NO_CXX11_SMART_PTR
using detail::make_shared_stl;
#endif
```

因此可以显式选择后端：

```cpp
yggr::smart_ptr_ex::make_shared_boost<T>(args...);
yggr::smart_ptr_ex::make_shared_stl<T>(args...);
```

STL 后端只在标准库智能指针未被禁用时存在。

## 统一 `make_shared`

外层文件将统一接口导出到：

```cpp
yggr::smart_ptr_ex
```

并进一步导出到：

```cpp
yggr
```

调用形式为：

```cpp
yggr::smart_ptr_ex::make_shared<T>(args...);
yggr::make_shared<T>(args...);
```

### STL 默认路径

当同时满足：

```cpp
!defined(YGGR_NO_CXX11_SMART_PTR)
&& defined(YGGR_SMART_PTR_USING_STL)
```

统一入口返回 `std::shared_ptr<T>`，并转发到：

```cpp
make_shared_stl<T>(
    boost::forward<Args>(args)...);
```

### Boost 默认路径

不满足 STL 默认路径时，统一入口返回 `boost::shared_ptr<T>`，并转发到：

```cpp
make_shared_boost<T>(
    boost::forward<Args>(args)...);
```

## 模板参数

统一入口的核心形式为：

```cpp
template<typename T, typename ...Args>
inline shared_ptr<T>
make_shared(BOOST_FWD_REF(Args)... args);
```

| 参数 | 含义 |
| --- | --- |
| `T` | 要创建的对象类型 |
| `Args...` | 转发给 `T` 构造函数的参数 |

返回的共享指针类型由宏配置决定。

## `make_shared` 的分配特性

两个 detail 后端都调用对应标准库的 `make_shared` 实现，而不是先单独 `new T` 再构造共享指针。这样由底层库负责对象和共享控制块的创建，通常可以采用合并分配等实现策略。

具体分配和控制块行为取决于 Boost 或标准库实现，本文件只负责统一接口和返回类型选择。

## 旧编译器兼容

当定义：

```cpp
YGGR_NO_CXX11_VARIADIC_TEMPLATES
```

外层和 detail 文件通过 Boost.Preprocessor 生成固定参数数量的重载：

```cpp
BOOST_PP_LOCAL_LIMITS(
    0,
    YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
```

每个重载都会：

1. 接收固定数量的构造参数；
2. 使用 Yggdrasil 的参数宏和 `boost::forward` 转发参数；
3. 调用 `boost::make_shared` 或 `std::make_shared`；
4. 捕获异常并返回空共享指针。

## 宏控制

| 宏 | 作用 |
| --- | --- |
| `YGGR_NO_CXX11_SMART_PTR` | 禁用 STL 后端和 `std::shared_ptr` 返回路径 |
| `YGGR_SMART_PTR_USING_STL` | 在 STL 智能指针可用时选择 `std::shared_ptr` 作为默认类型 |
| `YGGR_NO_CXX11_VARIADIC_TEMPLATES` | 使用预处理器生成固定参数数量的重载 |
| `YGGR_PP_FOO_DEFAULT_PARAMS_LEN` | 旧编译器模式下支持的最大构造参数数量 |

## 异常行为

两个 detail 后端都捕获所有异常：

```cpp
try
{
    return make_shared<T>(forwarded_args...);
}
catch(...)
{
    return shared_ptr<T>();
}
```

因此：

- `T` 构造函数抛出的异常不会传递给调用方；
- 分配或控制块创建失败时返回空指针；
- 调用方必须检查返回值是否为空。

## 调用流程

标准库路径：

```text
yggr::make_shared<T>(args...)
  -> make_shared_stl<T>(args...)
  -> std::make_shared<T>(args...)
  -> std::shared_ptr<T>
```

Boost 路径：

```text
yggr::make_shared<T>(args...)
  -> make_shared_boost<T>(args...)
  -> boost::make_shared<T>(args...)
  -> boost::shared_ptr<T>
```

## 与相关接口的关系

- `construct_shared.hpp` 使用直接共享指针构造，接口形式相近但不调用 `make_shared`；
- `allocate_shared_ex.hpp` 增加 allocator 参数，并调用 `allocate_shared`；
- `make_interface.hpp` 使用原始 `new` 创建派生对象后包装为 `interface_ptr`，不直接调用本文件；
- `shared_ptr.hpp` 根据相同宏选择公开的 `shared_ptr` 别名，并包含本文件。

## 使用限制与注意事项

- `T` 必须能够使用传入参数构造。
- STL 后端要求未定义 `YGGR_NO_CXX11_SMART_PTR`。
- 只有在定义 `YGGR_SMART_PTR_USING_STL` 且 STL 后端可用时，统一入口才返回 `std::shared_ptr<T>`。
- 构造失败返回空指针，不应依赖异常捕获处理失败。
- 非 variadic templates 模式下，参数数量不能超过 `YGGR_PP_FOO_DEFAULT_PARAMS_LEN`。
- 修改默认智能指针选择时，应同步检查 `shared_ptr.hpp`、`construct_shared.hpp` 和 `allocate_shared_ex.hpp`。
