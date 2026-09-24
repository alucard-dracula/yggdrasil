# `interface_ptr.hpp`

## 概述

该文件提供面向接口或基类的共享指针包装 `interface_ptr<T, SPtr>`。它以所选共享指针模板为基类，保留共享所有权能力，同时提供：

- 从原始指针、共享指针和其他 `interface_ptr` 构造；
- 基类/接口指针到子类指针的安全动态转换；
- 可选的断言式快速子类转换；
- 指针比较和 ADL `swap`；
- Boost.Serialization 序列化支持。

文件由两层组成：

- 外层 `interface_ptr.hpp`：导出 detail 类型，并包含相关构造辅助接口；
- `detail/interface_ptr.hpp`：定义完整的 `interface_ptr` 类及其非成员操作。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/interface_ptr.hpp>
```

外层文件还包含：

```cpp
#include <yggr/smart_ptr_ex/detail/interface_ptr.hpp>
#include <yggr/smart_ptr_ex/construct_interface.hpp>
#include <yggr/smart_ptr_ex/make_interface.hpp>
#include <yggr/smart_ptr_ex/allocate_interface.hpp>
```

## 公开导出

外层文件将 detail 类型导出到：

```cpp
yggr::smart_ptr_ex
```

```cpp
using detail::interface_ptr;
```

随后再次导出到 `yggr` 根命名空间：

```cpp
using smart_ptr_ex::interface_ptr;
```

因此以下写法均可用：

```cpp
yggr::smart_ptr_ex::interface_ptr<T>;
yggr::interface_ptr<T>;
```

## 类模板

```cpp
template<
    typename T,
    template<typename _T> class SPtr = boost::shared_ptr>
class interface_ptr : public SPtr<T>;
```

模板参数含义如下：

| 参数 | 含义 |
| --- | --- |
| `T` | 对外暴露的接口、基类或视图类型 |
| `SPtr` | 共享指针模板，默认是 `boost::shared_ptr` |

该类公开继承 `SPtr<T>`，因此保留共享指针的基础操作，例如 `get()`、`operator->` 和布尔状态检查。

## 类型别名

类提供：

```cpp
typedef SPtr<T> base_type;
typedef obj_type value_type;
typedef obj_type element_type;
typedef obj_type* pointer;
```

其中 `obj_type` 通过 `shared_ptr_element_t<base_type>` 推导得到。对于常规共享指针，它通常对应共享指针管理的元素类型。

## 构造函数

### 默认构造

```cpp
interface_ptr();
```

创建空接口指针。

### 原始指针构造

```cpp
template<typename Y>
explicit interface_ptr(Y* value);
```

内部使用：

```cpp
construct_shared<obj_type>(value)
```

构造共享所有权对象。

还支持自定义删除器和 allocator：

```cpp
template<typename Y, typename D>
interface_ptr(Y* value, D deleter);

template<typename Y, typename D, typename A>
interface_ptr(Y* value, D deleter, A allocator);
```

这些构造函数分别转发到 `construct_shared` 的对应重载。

### 其他共享指针构造

支持从其他共享指针模板构造：

```cpp
template<typename Y, template<typename _T> class OthSPtr>
explicit interface_ptr(OthSPtr<Y>&& other);

template<typename Y, template<typename _T> class OthSPtr>
interface_ptr(const OthSPtr<Y>& other);
```

也支持带删除器模板的共享指针移动构造，但对应的 const 左值版本被有意排除，因为基础共享指针类型不支持该转换路径。

对于当前 `SPtr` 模板，还提供：

```cpp
template<typename Y>
interface_ptr(SPtr<Y>&& other);

template<typename Y>
interface_ptr(const SPtr<Y>& other);
```

### 别名指针构造

```cpp
template<typename Y>
interface_ptr(
    const SPtr<Y>& right,
    pointer value);
```

该形式使用共享控制块 `right`，但将 `get()` 视图设置为传入的 `value`。它可用于共享一个对象的生命周期，同时暴露其中的接口或子对象指针。

### interface_ptr 转换构造

支持不同元素类型之间的共享指针转换：

```cpp
template<typename Y>
interface_ptr(interface_ptr<Y>&& right);

template<typename Y>
interface_ptr(const interface_ptr<Y>& right);

template<typename Y>
interface_ptr(const interface_ptr<Y>& right, pointer value);
```

转换实际基于对应 `base_type` 的共享指针转换规则。

## 赋值与交换

类提供从以下对象赋值的重载：

- 其他智能指针模板；
- 当前 `SPtr` 的不同元素类型；
- `base_type`；
- 其他 `interface_ptr`。

复制或移动赋值会处理自赋值，并在移动场景中使用 `copy_or_move_or_swap`。

交换接口包括：

```cpp
void swap(base_type& right);
void swap(interface_ptr& right);
```

同时支持右值引用版本。

## 子类转换

### 隐式指针转换

```cpp
template<typename Child>
operator Child*();

template<typename Child>
operator const Child*() const;
```

实现使用 `dynamic_cast`：

```cpp
return dynamic_cast<Child*>(base_type::get());
```

转换失败时返回空指针。

### `child_cast`

```cpp
template<typename Child>
Child* child_cast();

template<typename Child>
const Child* child_cast() const;
```

同样使用 `dynamic_cast`，适合需要明确表达向子类型转换的代码。转换失败时返回空指针。

### `unsafe_child_cast`

```cpp
template<typename Child>
Child* unsafe_child_cast();

template<typename Child>
const Child* unsafe_child_cast() const;
```

该函数先使用 `dynamic_cast` 断言转换成功，再使用 `reinterpret_cast` 返回结果：

```cpp
assert(dynamic_cast<Child*>(base_type::get()));
return reinterpret_cast<Child*>(base_type::get());
```

它避免再次执行动态转换，但只适合调用方已经确认实际类型的场景。发布构建中不应把它当作运行时类型检查机制。

## 指针比较

detail 文件提供 `operator==` 和 `operator!=`，支持比较：

- 原始指针与 `interface_ptr`；
- `interface_ptr` 与原始指针；
- 其他共享指针与 `interface_ptr`；
- `interface_ptr` 与其他共享指针；
- 两个不同元素类型或不同共享指针模板的 `interface_ptr`。

比较基于 `get()` 返回的指针值。部分实现通过 `reinterpret_cast` 将指针转换到可比较的 native pointer 类型。

在特定 GCC/C++ 版本组合下，与普通共享指针的比较模板会被条件编译排除，以规避编译器兼容性问题。

## ADL `swap`

detail 中定义 `swap_support` 命名空间，并导出以下交换形式：

```cpp
swap(interface_ptr<T, SPtr>& left,
     interface_ptr<T, SPtr>& right);

swap(interface_ptr<T, SPtr>& left,
     typename interface_ptr<T, SPtr>::base_type& right);

swap(typename interface_ptr<T, SPtr>::base_type& left,
     interface_ptr<T, SPtr>& right);
```

这些函数随后导入：

```cpp
yggr::smart_ptr_ex::detail::swap_support
```

并分别使用 `using` 导出到 `std` 和 `boost` 命名空间，以支持对应生态的 swap 调用约定。

## 序列化支持

`interface_ptr` 将 `yggr::serialization::access` 声明为友元，并提供私有序列化函数：

```cpp
template<typename Archive>
void serialize(
    Archive& ar,
    const u32 version)
{
    ar & YGGR_SERIALIZATION_NAME_NVP(
        "shared_ptr",
        boost::serialization::base_object<base_type>(*this));
}
```

序列化内容以 `"shared_ptr"` 命名，并将 `interface_ptr` 的共享指针基类作为基础对象序列化。具体序列化行为由基础共享指针类型和项目的共享指针序列化支持决定。

## 与构造辅助接口的关系

外层头文件同时包含：

- `construct_interface.hpp`：直接构造 `interface_ptr<T, SPtr>`；
- `make_interface.hpp`：通过构造参数创建接口指针；
- `allocate_interface.hpp`：使用 allocator 创建接口指针。

典型关系如下：

```text
make_interface / construct_interface / allocate_interface
  -> interface_ptr<T, SPtr>
      -> 继承的共享指针控制块
      -> construct_shared 或 allocate_shared
```

## 使用限制与注意事项

- `T` 应是可作为接口或基类使用的类型，进行 `dynamic_cast` 时通常需要多态类型。
- `unsafe_child_cast` 只在动态类型已确认时使用；断言关闭后，错误类型转换可能产生未定义行为。
- 使用别名指针构造时，必须保证暴露的 `pointer` 在共享控制块生命周期内有效。
- 原始指针、自定义删除器和 allocator 的组合必须与实际对象分配方式匹配。
- `interface_ptr` 公开继承所选共享指针，因此基础共享指针的类型转换、所有权和序列化语义会直接影响它。
- 比较函数使用指针值，不表示对象内容相等。
