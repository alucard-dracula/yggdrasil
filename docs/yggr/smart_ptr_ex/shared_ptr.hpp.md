# `shared_ptr.hpp`

## 概述

该头文件是 Yggdrasil 智能指针扩展中的共享指针统一入口。它根据编译宏选择公开的 `shared_ptr` 类型，并集中包含对象创建、allocator 创建和直接构造相关接口。

主要职责包括：

- 在 `std::shared_ptr` 和 `boost::shared_ptr` 之间选择默认类型；
- 导出 `yggr::smart_ptr_ex::shared_ptr` 和 `yggr::shared_ptr`；
- 提供跨智能指针实现的元素类型特征 `shared_ptr_element_t`；
- 接入 `make_shared`、`allocate_shared` 和 `construct_shared`；
- 包含 Boost 版本兼容补丁。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
```

## 依赖

主要依赖包括：

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/detail/shared_count_patch.hpp>
#include <yggr/smart_ptr_ex/detail/shared_ptr_has_element_t.hpp>
#include <yggr/smart_ptr_ex/make_shared_ex.hpp>
#include <yggr/smart_ptr_ex/allocate_shared_ex.hpp>
#include <yggr/smart_ptr_ex/construct_shared.hpp>
```

当未定义 `YGGR_NO_CXX11_SMART_PTR` 时还包含：

```cpp
#include <memory>
```

## 默认 `shared_ptr` 选择

公开别名根据两个宏决定：

```cpp
#if !defined(YGGR_NO_CXX11_SMART_PTR) \
    && defined(YGGR_SMART_PTR_USING_STL)
using std::shared_ptr;
#else
using boost::shared_ptr;
#endif
```

选择规则如下：

| 条件 | `yggr::smart_ptr_ex::shared_ptr` |
| --- | --- |
| 未禁用 C++11 智能指针，且定义 `YGGR_SMART_PTR_USING_STL` | `std::shared_ptr` |
| 定义 `YGGR_NO_CXX11_SMART_PTR` | `boost::shared_ptr` |
| 未定义 `YGGR_SMART_PTR_USING_STL` | `boost::shared_ptr` |

因此 Boost.SharedPtr 是默认和兼容路径，STL.SharedPtr 需要显式选择且编译环境必须支持 C++11 智能指针。

## 公开名称

选择完成后，类型位于：

```cpp
yggr::smart_ptr_ex
```

并被导出到根命名空间：

```cpp
yggr::shared_ptr
```

典型写法为：

```cpp
yggr::shared_ptr<Object> value;
```

或：

```cpp
yggr::smart_ptr_ex::shared_ptr<Object> value;
```

## `shared_ptr_element_t`

该特征用于从不同共享指针模板中提取所管理的元素类型：

```cpp
template<typename T>
struct shared_ptr_element_t;
```

它位于：

```cpp
yggr::smart_ptr_ex
```

使用示例：

```cpp
typedef yggr::shared_ptr<Object> pointer_type;
typedef yggr::smart_ptr_ex::shared_ptr_element_t<
    pointer_type>::type element_type;
```

此时 `element_type` 通常为 `Object`。

## 元素类型检测实现

`detail/shared_ptr_has_element_t.hpp` 提供：

```cpp
template<typename T>
struct shared_ptr_has_element_t;
```

该特征通过 SFINAE 检查 `T::element_type` 是否存在：

- 存在 `element_type` 时，选择 `element_type`；
- 不存在时，回退到 `value_type`。

`shared_ptr_element_t_impl` 的两条路径为：

```cpp
template<
    typename T,
    template<typename _T> class SPtr>
struct shared_ptr_element_t_impl<SPtr<T>, true>
{
    typedef typename SPtr<T>::element_type type;
};
```

和：

```cpp
template<
    typename T,
    template<typename _T> class SPtr>
struct shared_ptr_element_t_impl<SPtr<T>, false>
{
    typedef typename SPtr<T>::value_type type;
};
```

该兼容设计允许支持不同成员命名约定的共享指针模板。

## 创建接口

### `make_shared`

来自：

```cpp
#include <yggr/smart_ptr_ex/make_shared_ex.hpp>
```

通过 `boost::make_shared` 或 `std::make_shared` 创建对象，并根据默认智能指针选择返回对应共享指针。

### `allocate_shared`

来自：

```cpp
#include <yggr/smart_ptr_ex/allocate_shared_ex.hpp>
```

使用 allocator 创建对象和共享控制块，同样根据宏选择 Boost 或 STL 返回类型。

### `construct_shared`

来自：

```cpp
#include <yggr/smart_ptr_ex/construct_shared.hpp>
```

直接使用转发参数构造共享指针对象，底层调用对应的 Boost 或 STL 共享指针构造路径。

这些接口的默认智能指针选择应与本文件公开的 `shared_ptr` 保持一致。

## Boost 版本兼容

`detail/shared_count_patch.hpp` 根据 Boost 版本处理兼容依赖：

```cpp
#include <boost/version.hpp>

#if !(BOOST_VERSION < 105600)
#include <boost/detail/lightweight_mutex.hpp>
#endif
```

Boost 1.56 及以上版本会包含 `boost/detail/lightweight_mutex.hpp`。该文件不改变 `shared_ptr` 的公开别名，只补充底层兼容所需的头文件。

## 调用关系

```text
shared_ptr.hpp
  -> 选择 std::shared_ptr 或 boost::shared_ptr
  -> 导出 yggr::shared_ptr
  -> 提供 shared_ptr_element_t
  -> make_shared_ex.hpp
  -> allocate_shared_ex.hpp
  -> construct_shared.hpp
```

其他扩展接口还会依赖本文件提供的共享指针类型和元素类型特征，例如 `interface_ptr`、`allocate_interface` 和 `construct_interface`。

## 宏总结

| 宏 | 作用 |
| --- | --- |
| `YGGR_NO_CXX11_SMART_PTR` | 禁用 STL 智能指针，强制使用 Boost.SharedPtr |
| `YGGR_SMART_PTR_USING_STL` | 在 STL 可用时选择 `std::shared_ptr` |
| `BOOST_VERSION` | 控制共享计数兼容头的包含条件 |

## 使用限制与注意事项

- 同一程序或组件中应保持 `shared_ptr` 选择宏一致，避免 STL 和 Boost 共享指针类型混用造成接口不匹配。
- `shared_ptr_element_t<T>` 的模板参数应是形如 `SPtr<U>` 的共享指针类型。
- 自定义共享指针模板至少应提供 `element_type` 或 `value_type`，才能被元素类型特征识别。
- `make_shared`、`allocate_shared` 和 `construct_shared` 的返回类型由配置宏决定，调用代码不应假定永远是 STL 或 Boost 类型。
- `shared_count_patch.hpp` 属于内部兼容依赖，不应由业务代码直接修改或绕过。
