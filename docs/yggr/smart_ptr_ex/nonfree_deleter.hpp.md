# `nonfree_deleter.hpp`

## 概述

该文件提供非释放删除器 `nonfree_deleter<T>`。它可以作为智能指针的自定义 deleter，使智能指针在销毁控制块时不释放所指向的对象。

文件由两层组成：

- 外层 `nonfree_deleter.hpp`：包含 detail 实现并导出删除器类型；
- `detail/nonfree_deleter.hpp`：定义实际的空操作删除函数对象。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/nonfree_deleter.hpp>
```

外层文件包含：

```cpp
#include <yggr/smart_ptr_ex/detail/nonfree_deleter.hpp>
```

## 公开导出

外层文件在以下命名空间导出 detail 类型：

```cpp
yggr::smart_ptr_ex
```

```cpp
using detail::nonfree_deleter;
```

公开使用方式为：

```cpp
yggr::smart_ptr_ex::nonfree_deleter<T>
```

外层文件没有将该类型进一步导出到 `yggr` 根命名空间。

## `nonfree_deleter<T>`

实现位于：

```cpp
yggr::smart_ptr_ex::detail
```

类型定义为：

```cpp
template<typename T>
struct nonfree_deleter
{
    void operator()(T* value) const;
};
```

该删除器是无状态函数对象，不保存指针，也不拥有对象。

## 调用行为

调用运算符为空操作：

```cpp
inline void operator()(T* value) const
{
}
```

调用时：

- 不检查指针是否为空；
- 不执行 `delete` 或 `delete[]`；
- 不调用析构函数；
- 不释放内存；
- 不改变对象状态。

参数名虽然表示传入指针，但实现不会使用该指针。

## 典型用途

将外部管理的对象包装为共享指针视图：

```cpp
Object object;

boost::shared_ptr<Object> view(
    &object,
    yggr::smart_ptr_ex::nonfree_deleter<Object>());
```

当 `view` 销毁时，控制块会调用 `nonfree_deleter<Object>`，但 `object` 不会被删除。外部对象的生命周期必须长于所有使用该视图的智能指针。

也可以用于由其他资源管理器负责释放的动态对象：

```cpp
Object* object = external_owner.acquire();
boost::shared_ptr<Object> view(
    object,
    yggr::smart_ptr_ex::nonfree_deleter<Object>());
```

此时必须保证外部所有者在所有 view 销毁前仍然有效。

## 与其他删除器的区别

| 删除器 | 调用时行为 | 适用场景 |
| --- | --- | --- |
| `default_deleter<T>` | `delete value` | 单个动态对象由该智能指针拥有 |
| `native_array_deleter<T>` | `delete[] value` | 动态数组由该智能指针拥有 |
| `derived_deleter<Derived>` | 转为派生类型后执行 `delete` | 通过基类指针释放派生对象 |
| `nonfree_deleter<T>` | 不执行任何释放操作 | 对外部拥有对象创建非拥有视图 |

## 所有权语义

使用 `nonfree_deleter` 的智能指针仍然会拥有自己的共享控制块，但不拥有其 `get()` 返回的对象。也就是说：

- 智能指针可以参与引用计数和共享控制块管理；
- 最后一个智能指针销毁时只调用空操作 deleter；
- 对象本身必须由其他机制销毁；
- 智能指针不会延长外部对象的实际生命周期。

## 生命周期风险

`nonfree_deleter` 不会阻止悬空指针。若外部对象先于包装它的智能指针销毁，包装指针仍可能保留一个非空但已经失效的地址：

```text
外部对象销毁
  -> nonfree_deleter 包装指针仍存在
  -> 包装指针成为悬空指针
  -> 解引用产生未定义行为
```

调用方必须建立并维护明确的生命周期顺序。

## 使用限制与注意事项

- 仅在对象由其他所有者或外部生命周期管理器负责释放时使用。
- 不要把 `nonfree_deleter` 当作普通对象的默认删除器，否则会造成内存泄漏。
- 不要用它来解决所有权不明确的问题；它只是不释放，并不会自动跟踪外部所有者。
- 包装栈对象、静态对象或外部管理对象时，必须保证包装智能指针不会超过对象生命周期。
- 该删除器既不执行 `delete` 也不执行 `delete[]`，分配方式与它本身没有配对释放关系。
