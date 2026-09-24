# `scoped_array.hpp`

## 概述

该头文件将 Boost 的数组作用域智能指针 `boost::scoped_array` 导出到 Yggdrasil 的智能指针扩展命名空间。文件本身不重新实现数组指针，也没有对应的 `detail` 实现文件。

`scoped_array<T>` 适合管理通过：

```cpp
new T[count]
```

分配的动态数组，并在作用域结束时使用 `delete[]` 自动释放数组。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/scoped_array.hpp>
```

## 依赖

```cpp
#include <yggr/base/yggrdef.h>
#include <boost/scoped_array.hpp>
```

其中 `boost/scoped_array.hpp` 提供实际的作用域数组智能指针实现。

## 公开别名

文件将 Boost 类型导出到：

```cpp
yggr::smart_ptr_ex
```

```cpp
using boost::scoped_array;
```

随后再导出到 `yggr` 根命名空间：

```cpp
using smart_ptr_ex::scoped_array;
```

因此以下类型名称等价：

```cpp
boost::scoped_array<T>
yggr::smart_ptr_ex::scoped_array<T>
yggr::scoped_array<T>
```

## 基本语义

`scoped_array<T>` 是独占所有权的作用域数组指针，主要行为包括：

- 保存一个 `T*` 数组指针；
- 默认构造为空指针；
- 从动态数组指针构造；
- 作用域结束时自动执行 `delete[]`；
- 支持下标访问和指针访问；
- 不允许复制，以避免双重释放；
- 可以通过 `reset` 替换所管理的数组。

## 典型用法

```cpp
yggr::smart_ptr_ex::scoped_array<int> values(
    new int[10]);

values[0] = 42;
int first = values[0];
```

离开作用域后，`values` 自动释放整个数组。

## 常用操作

具体成员接口由 Boost `scoped_array` 提供，常见操作包括：

```cpp
scoped_array();
explicit scoped_array(T* value);
~scoped_array();

T& operator[](std::ptrdiff_t index) const;
T* get() const;
T& operator*() const;
T* operator->() const;
explicit operator bool() const;

void reset(T* value = 0);
void swap(scoped_array& other);
```

不同 Boost 版本的精确接口和显式转换细节可能略有差异，应以当前 Boost 版本定义为准。

## 所有权和生命周期

```text
new T[count]
  -> scoped_array<T> 接管
  -> 作用域内独占访问
  -> scoped_array 析构
  -> delete[] 数组
```

该类型不共享所有权，不提供引用计数，也不应与另一个会释放同一数组的资源管理器同时使用。

## 与其他数组指针工具的区别

| 类型 | 所有权 | 释放方式 | 复制 |
| --- | --- | --- | --- |
| `scoped_array<T>` | 独占、作用域绑定 | `delete[]` | 通常不可复制 |
| `shared_array<T>` | 共享所有权 | 引用计数归零时释放 | 可共享 |
| `native_array_deleter<T>` | 仅删除策略 | 调用时执行 `delete[]` | 取决于外部智能指针 |
| `nonfree_deleter<T>` | 不拥有对象 | 不释放 | 取决于外部智能指针 |

## 使用限制与注意事项

- 只能用于与 `delete[]` 匹配的动态数组指针。
- 不要将单个 `new T(...)` 返回的指针交给 `scoped_array<T>`。
- 不要手动删除已经交给 `scoped_array` 管理的数组。
- 不要复制 `scoped_array`；需要共享所有权时应使用共享数组或共享指针方案。
- 数组元素的析构行为由 `T` 类型决定，数组长度由分配和 Boost 智能指针机制管理。
