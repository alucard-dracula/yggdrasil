# `scoped_ptr.hpp`

## 概述

该头文件将 Boost 的单对象作用域智能指针 `boost::scoped_ptr` 导出到 Yggdrasil 的智能指针扩展命名空间。文件本身不重新实现智能指针，也没有对应的 `detail/scoped_ptr.hpp`；实际行为由 Boost.ScopedPtr 提供。

`scoped_ptr<T>` 适合管理通过 `new T(...)` 分配的单个动态对象，并在作用域结束时使用 `delete` 自动释放对象。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/scoped_ptr.hpp>
```

## 依赖

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/detail/shared_count_patch.hpp>
#include <boost/scoped_ptr.hpp>
```

`boost/scoped_ptr.hpp` 提供实际实现；`shared_count_patch.hpp` 根据 Boost 版本补充兼容性头文件。

## 公开别名

类型被导出到：

```cpp
yggr::smart_ptr_ex
```

```cpp
using boost::scoped_ptr;
```

并进一步导出到 `yggr` 根命名空间：

```cpp
using smart_ptr_ex::scoped_ptr;
```

因此以下名称等价：

```cpp
boost::scoped_ptr<T>
yggr::smart_ptr_ex::scoped_ptr<T>
yggr::scoped_ptr<T>
```

## 基本语义

`scoped_ptr<T>` 是独占所有权的作用域智能指针：

- 保存一个 `T*`；
- 默认构造为空指针；
- 从动态对象指针构造；
- 析构时执行 `delete`；
- 支持 `*`、`->` 和 `get()` 访问；
- 不允许复制，避免重复释放；
- 支持通过 `reset` 替换所管理对象。

典型用法：

```cpp
yggr::smart_ptr_ex::scoped_ptr<Widget> value(
    new Widget());

value->run();
```

离开作用域后，`value` 自动释放 `Widget`。

## 与 `scoped_array` 的区别

| 类型 | 管理对象 | 释放方式 | 访问方式 |
| --- | --- | --- | --- |
| `scoped_ptr<T>` | 单个 `T` 对象 | `delete` | `*`、`->` |
| `scoped_array<T>` | `T` 数组 | `delete[]` | `operator[]` |

不能使用 `scoped_ptr<T>` 管理 `new T[count]` 返回的数组，也不能使用 `scoped_array<T>` 管理单个对象。

## 与共享指针的区别

| 特性 | `scoped_ptr<T>` | `shared_ptr<T>` |
| --- | --- | --- |
| 所有权 | 独占 | 共享 |
| 引用计数 | 无 | 有 |
| 复制 | 不允许 | 通常允许 |
| 生命周期 | 绑定单一作用域 | 绑定共享控制块 |

## Boost 版本兼容

`detail/shared_count_patch.hpp` 在 Boost 1.56 及以上版本包含：

```cpp
#include <boost/detail/lightweight_mutex.hpp>
```

该兼容文件不改变 `scoped_ptr` 的所有权和删除语义。

## 使用限制与注意事项

- 必须使用与 `delete` 匹配的单对象动态指针。
- 不要将数组指针交给 `scoped_ptr<T>`。
- 不要手动删除已经交给它管理的对象。
- 不要复制或让多个独立作用域指针管理同一个原始指针。
- 需要共享所有权时，应使用共享指针类型。
