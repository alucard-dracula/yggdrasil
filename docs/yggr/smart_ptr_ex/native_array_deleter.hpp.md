# `native_array_deleter.hpp`

## 概述

该文件提供用于动态数组的默认删除器 `native_array_deleter<T>`。它适合释放通过数组形式分配的对象：

```cpp
new T[count]
```

删除器对非空指针执行 `delete[]`，对空指针不执行任何操作。

文件由两层组成：

- 外层 `native_array_deleter.hpp`：包含 detail 实现并导出删除器类型；
- `detail/native_array_deleter.hpp`：定义实际的数组删除函数对象。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/native_array_deleter.hpp>
```

外层文件包含：

```cpp
#include <yggr/smart_ptr_ex/detail/native_array_deleter.hpp>
```

## 公开导出

外层文件在以下命名空间导出 detail 类型：

```cpp
yggr::smart_ptr_ex
```

```cpp
using detail::native_array_deleter;
```

公开使用方式为：

```cpp
yggr::smart_ptr_ex::native_array_deleter<T>
```

外层文件没有将该类型进一步导出到 `yggr` 根命名空间。

## `native_array_deleter<T>`

实现位于：

```cpp
yggr::smart_ptr_ex::detail
```

类型定义为：

```cpp
template<typename T>
struct native_array_deleter
{
    void operator()(T* value) const;
};
```

该删除器是无状态函数对象，不保存数组长度、指针或 allocator 信息。

## 调用行为

调用运算符实现为：

```cpp
inline void operator()(T* value) const
{
    if(value)
    {
        delete [] value;
    }
}
```

行为如下：

| 输入 | 行为 |
| --- | --- |
| 非空 `T*` | 执行 `delete[] value`，销毁数组元素并释放数组内存 |
| 空指针 | 不执行任何操作 |

## 典型用法

手动调用：

```cpp
int* values = new int[10];

yggr::smart_ptr_ex::native_array_deleter<int>()(values);
```

作为共享指针删除器：

```cpp
int* values = new int[10];
boost::shared_ptr<int> ptr(
    values,
    yggr::smart_ptr_ex::native_array_deleter<int>());
```

在智能指针场景中，应确保指针类型和删除器的元素类型匹配，并且数组只由一个所有权体系负责释放。

## 与 `default_deleter` 的区别

| 删除器 | 删除表达式 | 分配方式 |
| --- | --- | --- |
| `default_deleter<T>` | `delete value` | `new T(...)` |
| `native_array_deleter<T>` | `delete[] value` | `new T[count]` |

不能使用 `default_deleter` 释放数组，也不能使用 `native_array_deleter` 释放单个对象。分配表达式和删除表达式必须成对匹配。

## 所有权与异常

删除器不负责记录数组长度，`delete[]` 会依赖分配时的数组信息完成元素销毁。它不捕获析构函数或删除表达式产生的异常，相关行为由调用环境和元素类型决定。

空指针检查使其可以安全地对空指针调用，但不会防止重复释放同一个非空数组指针。

## 使用限制与注意事项

- `value` 必须指向通过 `new T[count]` 分配的动态数组。
- 不要将栈数组、静态数组或已经由其他所有者管理的指针交给该删除器。
- 不要使用它释放单个 `new T(...)` 对象。
- 如果数组元素类型需要虚析构或特殊销毁逻辑，应确认 `delete[]` 对该类型是合法的。
- 外层公开名称位于 `yggr::smart_ptr_ex`，不是 `yggr` 根命名空间。
