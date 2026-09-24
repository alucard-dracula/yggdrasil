# `default_deleter.hpp`

## 概述

该文件提供一个简单的默认标量删除器 `default_deleter<T>`，用于在智能指针或其他资源管理对象中释放单个动态对象。

文件由两层组成：

- 外层 `default_deleter.hpp`：包含 detail 实现并导出类型别名；
- `detail/default_deleter.hpp`：定义实际的删除器函数对象。

注意：外层源文件顶部注释使用了 `native_array_deleter.hpp` 文件名，但实际 include guard、包含路径和实现都属于 `default_deleter.hpp`。同时，该删除器使用标量 `delete`，并不是数组删除器。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/default_deleter.hpp>
```

外层文件包含：

```cpp
#include <yggr/smart_ptr_ex/detail/default_deleter.hpp>
```

## 公开导出

外层文件在以下命名空间中导出 detail 类型：

```cpp
yggr::smart_ptr_ex
```

```cpp
using detail::default_deleter;
```

因此可以使用：

```cpp
yggr::smart_ptr_ex::default_deleter<T>
```

外层文件没有将该类型再次导出到 `yggr` 根命名空间，和部分其他 smart pointer 扩展接口的导出方式不同。

## `default_deleter<T>`

实现位于：

```cpp
yggr::smart_ptr_ex::detail
```

类型定义为：

```cpp
template<typename T>
struct default_deleter
{
    void operator()(T* value) const;
};
```

它是无状态函数对象，不保存指针，也不保存 allocator 或其他资源信息。

## 调用行为

调用运算符实现为：

```cpp
inline void operator()(T* value) const
{
    if(value)
    {
        delete value;
    }
}
```

行为如下：

| 输入 | 行为 |
| --- | --- |
| 非空 `T*` | 执行 `delete value`，销毁单个对象并释放内存 |
| 空指针 | 不执行任何操作 |

该实现使用标量删除表达式：

```cpp
delete value;
```

因此只适合释放通过单对象 `new T(...)` 获得的指针，不适合释放通过 `new T[n]` 获得的数组指针。数组资源应使用专用的数组删除器。

## 典型用法

作为智能指针删除策略：

```cpp
typedef yggr::smart_ptr_ex::default_deleter<MyType>
    deleter_type;

MyType* raw = new MyType();
deleter_type()(raw);
```

也可以将其作为自定义 deleter 传给支持删除器参数的智能指针类型：

```cpp
MyType* raw = new MyType();
boost::shared_ptr<MyType> ptr(
    raw,
    yggr::smart_ptr_ex::default_deleter<MyType>());
```

实际使用时，应确保指针的分配方式和删除器匹配。

## 所有权与异常

该删除器执行 `delete` 后不保留指针，也不返回删除结果。它不捕获析构函数或删除表达式产生的异常；此类异常行为由 C++ 对象析构和智能指针调用环境决定。

空指针检查使其满足常见删除器的幂等调用习惯：对空指针调用不会产生额外效果。

## 与其他删除器的区别

- `default_deleter<T>`：使用 `delete` 释放单个对象；
- 数组删除器：应使用 `delete[]` 释放动态数组；
- `derived_deleter`：用于通过派生类型或接口类型管理对象时的专用删除关系；
- `nonfree_deleter`：用于不应由删除器释放的非所有权场景。

选择删除器时必须使分配方式、对象类型和删除表达式匹配。

## 使用限制与注意事项

- `T` 应是通过 `new T(...)` 创建的对象类型，或至少能够安全地用 `delete T*` 销毁。
- 不要用该删除器释放 `new T[n]` 返回的数组指针。
- 不要将栈对象地址、静态对象地址或已由其他所有者管理的指针交给该删除器。
- 删除器不承担共享所有权；共享生命周期由使用它的智能指针或资源管理器负责。
- 修改外层导出方式时，应注意当前类型只导出到 `yggr::smart_ptr_ex`，没有额外的 `yggr::default_deleter` 别名。
