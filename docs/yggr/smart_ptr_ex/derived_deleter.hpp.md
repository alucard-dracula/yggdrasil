# `derived_deleter.hpp`

## 概述

该文件提供面向派生类型的删除器 `derived_deleter<Derived>`。它适用于通过基类或接口指针持有派生对象、但销毁时需要按实际派生类型执行 `delete` 的场景。

文件由两层组成：

- 外层 `derived_deleter.hpp`：包含 detail 实现并导出删除器类型；
- `detail/derived_deleter.hpp`：实现派生类型检查、指针转换和删除操作。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/derived_deleter.hpp>
```

外层文件包含：

```cpp
#include <yggr/smart_ptr_ex/detail/derived_deleter.hpp>
```

## 公开导出

外层文件在以下命名空间导出 detail 类型：

```cpp
yggr::smart_ptr_ex
```

```cpp
using detail::derived_deleter;
```

因此公开使用方式为：

```cpp
yggr::smart_ptr_ex::derived_deleter<Derived>
```

外层文件没有进一步提供 `yggr::derived_deleter` 根命名空间别名。

## `derived_deleter<Derived>`

实现位于：

```cpp
yggr::smart_ptr_ex::detail
```

类型定义为：

```cpp
template<typename Derived>
struct derived_deleter
{
    typedef Derived derived_type;
};
```

公开类型别名：

```cpp
typedef Derived derived_type;
```

删除器本身无状态，不保存对象指针或其他资源。

## 调用运算符

删除操作接口为：

```cpp
template<typename T>
inline typename boost::enable_if<
    boost::is_base_of<
        typename boost::remove_cv<T>::type,
        derived_type>,
    void>::type
operator()(T* value) const;
```

模板约束要求：

```cpp
boost::is_base_of<
    typename boost::remove_cv<T>::type,
    Derived>
```

为真。换言之，传入的 `T` 去除 `const`/`volatile` 后必须是 `Derived` 的基类或 `Derived` 本身。

## 删除流程

当传入指针非空时，detail 实现执行：

```cpp
if(value)
{
    delete reinterpret_cast<derived_type*>(value);
}
```

处理行为如下：

| 输入 | 行为 |
| --- | --- |
| 空指针 | 不执行操作 |
| 满足类型约束且指向有效派生对象的指针 | 转换为 `Derived*` 并执行 `delete` |
| 不满足类型约束的指针 | 函数通过 SFINAE 从重载集合中移除，通常表现为编译错误 |

## 典型用途

假设 `Concrete` 派生自 `Interface`：

```cpp
struct Interface
{
    virtual ~Interface() {}
};

struct Concrete : Interface
{
};
```

可以使用：

```cpp
Interface* raw = new Concrete();

yggr::smart_ptr_ex::derived_deleter<Concrete> deleter;
deleter(raw);
```

也可以将其作为智能指针的自定义删除器：

```cpp
boost::shared_ptr<Interface> ptr(
    new Concrete(),
    yggr::smart_ptr_ex::derived_deleter<Concrete>());
```

实际使用时必须确保传入指针确实指向 `Derived` 对象或符合该删除器预期的派生对象。

## 与 `default_deleter` 的区别

| 删除器 | 删除方式 | 适用场景 |
| --- | --- | --- |
| `default_deleter<T>` | 直接对 `T*` 执行 `delete` | 指针静态类型与实际销毁类型一致 |
| `derived_deleter<Derived>` | 将基类/接口指针转换为 `Derived*` 后执行 `delete` | 通过基类或接口指针管理派生对象 |

`derived_deleter` 不执行数组删除，也不负责验证运行时对象的真实动态类型。

## 类型安全边界

`boost::is_base_of` 只提供编译期继承关系约束，不能证明运行时传入的指针实际指向 `Derived` 对象。调用方必须满足以下前提：

- `value` 指向的对象确实由 `Derived` 类型创建；
- 指针转换结果可用于销毁该对象；
- 对象没有已经被其他所有者释放；
- 指针不是数组指针、栈对象地址或静态对象地址。

如果这些前提不成立，将 `T*` 重解释为 `Derived*` 后删除会产生未定义行为。

## 依赖

detail 实现依赖：

```cpp
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_cv.hpp>
```

这些依赖分别提供 SFINAE 返回类型、继承关系判断和 cv 限定移除。

## 使用限制与注意事项

- `Derived` 应是实际分配对象的类型，或至少是能够安全执行删除的完整对象类型。
- 传入类型 `T` 必须是 `Derived` 的基类、带 cv 限定的基类，或 `Derived` 本身。
- 删除器使用 `reinterpret_cast`，运行时不会检查动态类型。
- 该删除器只使用标量 `delete`，不能用于 `new[]` 分配的数组。
- 外层公开名称位于 `yggr::smart_ptr_ex`，不是 `yggr` 根命名空间。
