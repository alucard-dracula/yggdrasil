# `shared_array.hpp`

## 概述

该头文件将 Boost 的共享数组智能指针 `boost::shared_array` 导出到 Yggdrasil 的智能指针扩展命名空间。文件本身不重新实现共享数组，也没有对应的 `detail/shared_array.hpp`；实际行为由 Boost.SharedArray 提供。

`shared_array<T>` 适合管理通过数组形式分配的对象：

```cpp
new T[count]
```

多个 `shared_array` 实例可以共享同一个数组的生命周期，最后一个共享所有者销毁时使用 `delete[]` 释放数组。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/shared_array.hpp>
```

## 依赖

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/detail/shared_count_patch.hpp>
#include <boost/shared_array.hpp>
```

其中：

- `yggrdef.h` 提供 Yggdrasil 基础定义；
- `boost/shared_array.hpp` 提供实际的共享数组智能指针；
- `detail/shared_count_patch.hpp` 根据 Boost 版本补充共享计数相关兼容头文件。

## 公开别名

文件将 Boost 类型导出到：

```cpp
yggr::smart_ptr_ex
```

```cpp
using boost::shared_array;
```

随后再导出到 `yggr` 根命名空间：

```cpp
using smart_ptr_ex::shared_array;
```

因此以下类型名称等价：

```cpp
boost::shared_array<T>
yggr::smart_ptr_ex::shared_array<T>
yggr::shared_array<T>
```

## 基本语义

`shared_array<T>` 是带引用计数的共享数组指针，主要行为包括：

- 保存一个数组首地址；
- 默认构造为空；
- 从动态数组指针构造；
- 复制时共享同一数组和控制信息；
- 最后一个所有者销毁时执行 `delete[]`；
- 支持下标访问、`get()`、`reset` 和 `swap`。

## 典型用法

```cpp
yggr::smart_ptr_ex::shared_array<int> values(
    new int[10]);

values[0] = 42;

// 复制后共享同一个数组
boost::shared_array<int> other = values;
```

当 `values` 和 `other` 都销毁后，数组才会被释放。

## 常用操作

具体接口由 Boost `shared_array` 提供，常见操作包括：

```cpp
shared_array();
explicit shared_array(T* value);

T& operator[](std::ptrdiff_t index) const;
T* get() const;

void reset(T* value = 0);
void swap(shared_array& other);
```

不同 Boost 版本的布尔转换和辅助接口可能略有差异，应以当前 Boost 版本定义为准。

## 所有权和生命周期

```text
new T[count]
  -> shared_array<T> 接管
  -> 复制 shared_array，共享控制信息
  -> 最后一个 shared_array 析构
  -> delete[] 数组
```

共享数组只对控制信息进行共享，数组元素本身仍由同一个共享控制块负责最终释放。

## 与其他指针类型的区别

| 类型 | 所有权 | 管理对象 | 释放方式 |
| --- | --- | --- | --- |
| `scoped_ptr<T>` | 独占 | 单个对象 | `delete` |
| `scoped_array<T>` | 独占 | 数组 | `delete[]` |
| `shared_ptr<T>` | 共享 | 单个对象 | `delete` 或自定义 deleter |
| `shared_array<T>` | 共享 | 数组 | `delete[]` |
| `native_array_deleter<T>` | 仅删除策略 | 数组指针 | 调用时执行 `delete[]` |

不能使用 `shared_array<T>` 管理单个 `new T(...)` 对象，也不能用普通 `shared_ptr<T>` 代替数组删除语义而不配置合适的 deleter。

## `shared_count_patch.hpp`

该 detail 依赖根据 Boost 版本包含：

```cpp
#include <boost/version.hpp>

#if !(BOOST_VERSION < 105600)
#include <boost/detail/lightweight_mutex.hpp>
#endif
```

Boost 1.56 及以上版本会包含 `boost/detail/lightweight_mutex.hpp`。该兼容文件不改变 `shared_array` 的引用计数和数组释放语义。

## 使用限制与注意事项

- 必须使用与 `delete[]` 匹配的动态数组指针初始化。
- 不要将单个 `new T(...)` 返回的指针交给 `shared_array<T>`。
- 不要手动删除已经由 `shared_array` 管理的数组。
- 共享数组实例之间可以复制，但所有实例必须遵守同一数组的生命周期规则。
- 不要让独立的资源管理器同时释放同一个数组，否则会造成重复释放。
- 如果需要自定义数组销毁方式，应确认 Boost.SharedArray 是否支持所需的删除策略，或改用支持自定义 deleter 的共享指针方案。
