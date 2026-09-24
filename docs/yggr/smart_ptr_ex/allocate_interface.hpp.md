# `allocate_interface.hpp`

## 概述

该文件为基于分配器的接口对象构造提供统一入口。它由两部分组成：

- 外层 `allocate_interface.hpp`：包含 detail 实现，并将函数导出到公开命名空间；
- `detail/allocate_interface.hpp`：根据智能指针类型调用对应的 `allocate_shared`，再包装为 `interface_ptr`。

该接口适合在使用自定义 allocator 的情况下，以 `Derived` 实现创建对象，并以 `T` 接口类型返回共享指针包装。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/allocate_interface.hpp>
```

外层文件内部包含：

```cpp
#include <yggr/smart_ptr_ex/detail/allocate_interface.hpp>
```

## 公开名称导出

外层头文件位于：

```cpp
yggr::smart_ptr_ex
```

并将 detail 中的函数引入公开命名空间：

```cpp
using detail::allocate_interface;
```

随后再次导出到 `yggr` 根命名空间：

```cpp
using smart_ptr_ex::allocate_interface;
```

因此以下调用形式都可用：

```cpp
yggr::smart_ptr_ex::allocate_interface(...);
yggr::allocate_interface(...);
```

外层文件自身不定义分配逻辑，实际实现全部来自 `detail/allocate_interface.hpp`。

## 依赖

detail 实现主要依赖：

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/smart_ptr_ex/detail/interface_ptr.hpp>
#include <yggr/container_ex/is_allocator.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/make_shared.hpp>
```

当启用 C++11 智能指针支持时还包含：

```cpp
#include <memory>
```

当不支持 C++11 可变参数模板时，额外使用：

```cpp
#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>
```

## `allocate_shared_helper`

detail 中定义辅助模板：

```cpp
template<typename SPtr>
struct allocate_shared_helper;
```

它根据共享指针模板选择具体的分配函数。

### Boost.SharedPtr 分支

针对：

```cpp
boost::shared_ptr<T>
```

特化提供：

```cpp
typedef T shared_type;
typedef boost::shared_ptr<T> result_type;
```

调用运算符要求 `Alloc` 满足：

```cpp
container_ex::is_allocator<Alloc>
```

满足条件时调用：

```cpp
boost::allocate_shared<shared_type>(
    alloc,
    boost::forward<Args>(args)...);
```

### std::SharedPtr 分支

当未定义以下宏时启用：

```cpp
YGGR_NO_CXX11_SMART_PTR
```

针对：

```cpp
std::shared_ptr<T>
```

特化调用：

```cpp
std::allocate_shared<shared_type>(
    alloc,
    boost::forward<Args>(args)...);
```

因此标准库智能指针支持取决于 `YGGR_NO_CXX11_SMART_PTR` 的定义状态。

## `allocate_interface`

核心函数模板为：

```cpp
template<
    typename T,
    typename Derived,
    template<typename _T> class SPtr,
    typename Alloc,
    typename ...Args>
inline typename boost::enable_if<
    container_ex::is_allocator<Alloc>,
    interface_ptr<T, SPtr>>::type
allocate_interface(
    const Alloc& alloc,
    BOOST_FWD_REF(Args)... args);
```

模板参数含义如下：

| 参数 | 含义 |
| --- | --- |
| `T` | 对外暴露的接口或基类类型 |
| `Derived` | 实际创建的派生实现类型 |
| `SPtr` | 共享指针模板，例如 `boost::shared_ptr` 或 `std::shared_ptr` |
| `Alloc` | 用于对象分配和控制块构造的 allocator |
| `Args...` | 转发给 `Derived` 构造函数的参数 |

返回类型为：

```cpp
interface_ptr<T, SPtr>
```

只有当 `Alloc` 被 `container_ex::is_allocator` 识别为 allocator 时，该函数才参与重载解析。

## 构造流程

函数内部的处理流程如下：

1. 将返回类型定义为 `interface_ptr<T, SPtr>`；
2. 将智能指针类型确定为 `SPtr<Derived>`；
3. 创建对应的 `allocate_shared_helper`；
4. 使用 allocator 和构造参数创建 `SPtr<Derived>`；
5. 使用共享指针和 `dp.get()` 构造 `interface_ptr<T, SPtr>`；
6. 返回接口指针。

核心逻辑为：

```cpp
typedef interface_ptr<T, SPtr> ret_type;
typedef SPtr<Derived> derived_shared_ptr_type;
typedef allocate_shared_helper<
    derived_shared_ptr_type> helper_type;

helper_type helper;
derived_shared_ptr_type dp =
    helper(alloc, boost::forward<Args>(args)...);
return ret_type(dp, dp.get());
```

`interface_ptr` 继承或包装相应的共享指针，因此对象生命周期由共享控制块管理，而接口视图使用 `T` 类型访问。

## 异常行为

对象分配和构造位于 `try` 块中：

```cpp
try
{
    ...
}
catch(...)
{
    return ret_type();
}
```

任何异常都会被捕获，并返回默认构造的空 `interface_ptr`。该函数不会向调用者重新抛出异常，因此调用方必须检查返回指针是否为空。

## 可变参数模板兼容

### 支持 variadic templates

当未定义：

```cpp
YGGR_NO_CXX11_VARIADIC_TEMPLATES
```

使用 `typename ...Args` 和完美转发实现任意参数数量的构造。

### 不支持 variadic templates

当定义该宏时，detail 文件通过预处理器迭代生成多组重载：

```cpp
BOOST_PP_LOCAL_LIMITS(
    0,
    YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
```

每个重载都会将参数转发给对应的 `allocate_shared`。支持的最大参数数量由 `YGGR_PP_FOO_DEFAULT_PARAMS_LEN` 决定。

## 调用示意

以 Boost.SharedPtr 为例，调用关系为：

```text
allocate_interface<T, Derived, boost::shared_ptr>(alloc, args...)
  -> allocate_shared_helper<boost::shared_ptr<Derived>>
  -> boost::allocate_shared<Derived>(alloc, args...)
  -> interface_ptr<T, boost::shared_ptr>
```

使用标准库智能指针时，最后的共享指针模板替换为 `std::shared_ptr`，并调用 `std::allocate_shared`。

## 使用限制与注意事项

- `Alloc` 必须满足 `container_ex::is_allocator`，否则函数不会参与重载解析。
- `Derived` 必须能够使用传入参数构造，并且应能作为 `T` 的实现类型使用。
- `SPtr` 必须是接受一个类型模板参数的共享指针模板。
- `YGGR_NO_CXX11_SMART_PTR` 定义时不会提供 `std::shared_ptr` 版本。
- 分配、构造或控制块创建异常会被吞掉并转换为空指针，调用方必须检查返回值。
- 该函数返回 `interface_ptr`，具体复制、移动、别名指针和生命周期行为由 `detail/interface_ptr.hpp` 及所选共享指针实现决定。
