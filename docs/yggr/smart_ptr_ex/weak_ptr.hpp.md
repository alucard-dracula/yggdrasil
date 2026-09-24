# `weak_ptr.hpp`

## 概述

该头文件提供统一的弱引用智能指针类型。它根据编译宏选择 `std::weak_ptr` 或 `boost::weak_ptr`，并将结果导出到 Yggdrasil 的 smart pointer 扩展命名空间和根命名空间。

弱指针不拥有对象，不会延长共享对象生命周期，访问对象前必须通过 `lock()` 获取有效的共享指针。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/weak_ptr.hpp>
```

## 依赖

```cpp
#include <yggr/base/yggrdef.h>
#include <boost/weak_ptr.hpp>
```

当未定义以下宏时还包含：

```cpp
#include <memory>
```

其中：

- `boost/weak_ptr.hpp` 提供 Boost 弱指针实现；
- `<memory>` 提供标准库 `std::weak_ptr`。

## 默认类型选择

公开类型根据以下条件选择：

```cpp
#if !defined(YGGR_NO_CXX11_SMART_PTR) \
    && defined(YGGR_SMART_PTR_USING_STL)
using std::weak_ptr;
#else
using boost::weak_ptr;
#endif
```

选择规则如下：

| 条件 | 公开 `weak_ptr` 类型 |
| --- | --- |
| 未禁用 C++11 智能指针，且定义 `YGGR_SMART_PTR_USING_STL` | `std::weak_ptr` |
| 定义 `YGGR_NO_CXX11_SMART_PTR` | `boost::weak_ptr` |
| 未定义 `YGGR_SMART_PTR_USING_STL` | `boost::weak_ptr` |

弱指针的默认选择与 `shared_ptr.hpp` 中的共享指针选择保持一致。

## 公开别名

类型首先位于：

```cpp
yggr::smart_ptr_ex
```

随后导出到：

```cpp
yggr
```

因此以下名称等价：

```cpp
std::weak_ptr<T>
yggr::smart_ptr_ex::weak_ptr<T>
yggr::weak_ptr<T>
```

或在 Boost 默认路径下：

```cpp
boost::weak_ptr<T>
yggr::smart_ptr_ex::weak_ptr<T>
yggr::weak_ptr<T>
```

## 基本语义

`weak_ptr<T>` 保存对共享控制块的弱引用，但不拥有对象：

- 不增加共享所有权计数；
- 不阻止对象被最后一个 `shared_ptr` 释放；
- 可以通过 `expired()` 查询对象是否已经销毁；
- 可以通过 `lock()` 临时获取共享所有权；
- 共享对象销毁后，弱指针本身仍可存在，但 `lock()` 会返回空共享指针。

## 典型用法

```cpp
yggr::shared_ptr<Object> owner(
    new Object());

yggr::weak_ptr<Object> observer(owner);

if(!observer.expired())
{
    yggr::shared_ptr<Object> value = observer.lock();
    if(value)
    {
        value->run();
    }
}
```

更常见的写法是直接检查 `lock()` 返回值，以避免检查和获取之间的生命周期竞争：

```cpp
yggr::shared_ptr<Object> value = observer.lock();
if(value)
{
    value->run();
}
```

## 常用操作

具体成员函数由当前选择的标准库或 Boost 弱指针实现提供，常见接口包括：

```cpp
weak_ptr();
weak_ptr(const shared_ptr<T>& value);

bool expired() const;
shared_ptr<T> lock() const;
long use_count() const;

void reset();
void swap(weak_ptr& other);
```

不同 Boost 和标准库版本的构造转换及返回类型细节可能略有差异，应以当前实现定义为准。

## 生命周期关系

```text
shared_ptr 持有对象和共享控制块
  -> weak_ptr 观察同一控制块
  -> 最后一个 shared_ptr 销毁对象
  -> weak_ptr 仍可存在
  -> weak_ptr.lock() 返回空共享指针
```

弱指针不会让对象存活，但通常会让共享控制块在弱引用仍存在时继续保留，以便安全判断对象是否已销毁。

## 与 `shared_ptr` 的区别

| 特性 | `weak_ptr<T>` | `shared_ptr<T>` |
| --- | --- | --- |
| 拥有对象 | 否 | 是 |
| 延长对象生命周期 | 否 | 是 |
| 直接解引用 | 不支持 | 支持 |
| 获取对象 | `lock()` | 直接访问或 `get()` |
| 循环引用 | 可用于打破循环 | 可能形成循环 |
| 对象已销毁后的状态 | `expired()` 为真 | 最后所有者已为空/销毁 |

## 与默认 `shared_ptr` 的配合

为了保证 `lock()` 返回类型和项目共享指针类型一致，`weak_ptr.hpp` 使用与 `shared_ptr.hpp` 相同的宏选择条件：

```cpp
!defined(YGGR_NO_CXX11_SMART_PTR)
&& defined(YGGR_SMART_PTR_USING_STL)
```

因此在同一编译配置中，建议成对使用：

```cpp
yggr::shared_ptr<T>
yggr::weak_ptr<T>
```

不要在同一接口边界中无意混用 STL 和 Boost 的共享/弱指针类型。

## 使用限制与注意事项

- 不要直接解引用弱指针；必须先调用 `lock()` 获得有效共享指针。
- `expired()` 只适合查询状态，实际访问对象时应优先使用 `lock()` 的返回值。
- 弱指针不会延长对象生命周期，外部必须存在有效的共享所有者才能保证对象存活。
- `YGGR_NO_CXX11_SMART_PTR` 定义时不能使用 STL 弱指针后端。
- `YGGR_SMART_PTR_USING_STL` 应与 `shared_ptr.hpp` 使用一致，否则共享指针和弱指针接口可能不匹配。
