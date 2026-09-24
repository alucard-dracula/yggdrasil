# `make_interface.hpp`

## 概述

该文件提供面向接口或基类的对象创建入口 `make_interface`。它使用指定的 `Derived` 类型创建对象，再将对象交给 `interface_ptr<T, SPtr>` 管理，从而以接口类型 `T` 对外提供共享指针视图。

文件由两层组成：

- 外层 `make_interface.hpp`：包含 detail 实现并导出函数；
- `detail/make_interface.hpp`：执行派生对象分配、接口指针构造和异常处理。

与 `make_shared` 不同，该实现使用 `yggr_nothrow_new` 创建派生对象，再通过 `interface_ptr` 接管指针；不会直接调用 `boost::make_shared` 或 `std::make_shared`。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/make_interface.hpp>
```

外层文件包含：

```cpp
#include <yggr/smart_ptr_ex/detail/make_interface.hpp>
```

## 公开导出

外层文件将 detail 函数导出到：

```cpp
yggr::smart_ptr_ex
```

```cpp
using detail::make_interface;
```

随后再次导出到 `yggr` 根命名空间：

```cpp
using smart_ptr_ex::make_interface;
```

因此以下调用形式均可使用：

```cpp
yggr::smart_ptr_ex::make_interface<T, Derived, SPtr>(args...);
yggr::make_interface<T, Derived, SPtr>(args...);
```

## 核心接口

支持 variadic templates 时，detail 函数形式为：

```cpp
template<
    typename T,
    typename Derived,
    template<typename _T> class SPtr,
    typename ...Args>
inline interface_ptr<T, SPtr>
make_interface(BOOST_FWD_REF(Args)... args);
```

模板参数含义如下：

| 参数 | 含义 |
| --- | --- |
| `T` | 对外暴露的接口或基类类型 |
| `Derived` | 实际创建的派生实现类型 |
| `SPtr` | 共享指针模板，例如 `boost::shared_ptr` 或 `std::shared_ptr` |
| `Args...` | 转发给 `Derived` 构造函数的参数 |

返回类型为：

```cpp
interface_ptr<T, SPtr>
```

## 创建流程

核心实现为：

```cpp
typedef interface_ptr<T, SPtr> ret_type;
typedef Derived derived_type;

try
{
    return ret_type(
        yggr_nothrow_new derived_type(
            boost::forward<Args>(args)...));
}
catch(...)
{
    return ret_type();
}
```

处理步骤如下：

1. 将构造参数完美转发给 `Derived` 构造函数；
2. 使用 `yggr_nothrow_new` 分配并创建 `Derived` 对象；
3. 将生成的 `Derived*` 传给 `interface_ptr<T, SPtr>`；
4. 由 `interface_ptr` 建立共享所有权并提供 `T` 类型视图；
5. 创建或接管失败时返回空 `interface_ptr`。

## `yggr_nothrow_new` 语义

该接口使用项目的 `yggr_nothrow_new` 分配表达式，因此内存分配失败通常表现为空指针，而不是直接抛出分配异常。随后构造过程或 `interface_ptr` 接管过程仍可能产生异常，detail 实现通过：

```cpp
catch(...)
{
    return ret_type();
}
```

将异常转换为空接口指针。

调用方应检查返回对象是否为空：

```cpp
auto value = yggr::smart_ptr_ex::make_interface<
    Interface,
    Concrete,
    boost::shared_ptr>(constructor_args...);

if(value)
{
    // 使用接口对象
}
```

## 与 `interface_ptr` 的关系

`make_interface` 依赖 `detail/interface_ptr.hpp` 的原始指针构造：

```text
make_interface<T, Derived, SPtr>(args...)
  -> yggr_nothrow_new Derived(args...)
  -> interface_ptr<T, SPtr>(Derived*)
      -> construct_shared<obj_type>(Derived*)
      -> 共享指针控制块
```

`interface_ptr` 负责实际的共享生命周期、接口视图、子类转换和删除器调用。`make_interface` 只负责创建 `Derived` 并把它交给接口指针。

## 智能指针模板

`SPtr` 是模板模板参数，可以显式选择：

```cpp
boost::shared_ptr
std::shared_ptr
```

示例：

```cpp
boost::shared_ptr<Interface> boost_value =
    yggr::smart_ptr_ex::make_interface<
        Interface,
        Concrete,
        boost::shared_ptr>();
```

```cpp
std::shared_ptr<Interface> stl_value =
    yggr::smart_ptr_ex::make_interface<
        Interface,
        Concrete,
        std::shared_ptr>();
```

使用 `std::shared_ptr` 时，项目必须未定义：

```cpp
YGGR_NO_CXX11_SMART_PTR
```

## 旧编译器兼容

当定义以下宏时：

```cpp
YGGR_NO_CXX11_VARIADIC_TEMPLATES
```

detail 文件通过 Boost.Preprocessor 生成固定参数数量的重载：

```cpp
BOOST_PP_LOCAL_LIMITS(
    0,
    YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
```

这些重载仍然将参数转发给 `Derived` 构造函数，并保持相同的返回和异常语义。最大构造参数数量由 `YGGR_PP_FOO_DEFAULT_PARAMS_LEN` 决定。

## 与相关接口的区别

| 接口 | 创建方式 | 返回对象 |
| --- | --- | --- |
| `make_interface` | `yggr_nothrow_new Derived(args...)` 后交给 `interface_ptr` | `interface_ptr<T, SPtr>` |
| `construct_interface` | 直接构造 `interface_ptr` | `interface_ptr<T, SPtr>` |
| `allocate_interface` | 使用 allocator 和 `allocate_shared` 创建 Derived | `interface_ptr<T, SPtr>` |
| `make_shared` | 使用 Boost/STL make_shared 创建具体类型 | 对应 shared_ptr |

`make_interface` 适合需要明确指定接口类型和具体派生类型，并采用普通动态分配的场景。

## 异常和失败行为

以下情况会返回空 `interface_ptr`：

- `yggr_nothrow_new` 分配失败；
- `Derived` 构造函数抛出异常；
- `interface_ptr` 接管指针时发生异常。

detail 实现不会向调用方重新抛出异常。调用方应通过指针的布尔状态检查创建是否成功。

## 使用限制与注意事项

- `Derived` 应与 `T` 存在有效的接口/基类关系，且 `interface_ptr` 的后续转换和删除行为必须安全。
- `SPtr` 必须是接受一个类型参数的共享指针模板。
- `Derived` 必须能使用传入参数构造。
- 返回空指针表示创建失败，不应直接解引用返回值。
- 对象由 `interface_ptr` 接管后，不应再由调用方手动删除原始指针。
- 非 variadic templates 模式下，构造参数数量不能超过 `YGGR_PP_FOO_DEFAULT_PARAMS_LEN`。
