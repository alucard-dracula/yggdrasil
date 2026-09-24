# `intrusive_ptr.hpp`

## 概述

该头文件将 Boost.IntrusivePtr 引入 Yggdrasil 的 smart pointer 扩展命名空间。它不重新实现 intrusive pointer，而是直接导出：

```cpp
boost::intrusive_ptr
```

同时包含 `detail/shared_count_patch.hpp`，用于处理 Boost 版本相关的共享计数依赖。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/intrusive_ptr.hpp>
```

## 依赖

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/detail/shared_count_patch.hpp>
#include <boost/intrusive_ptr.hpp>
```

其中：

- `yggrdef.h` 提供 Yggdrasil 基础定义；
- `boost/intrusive_ptr.hpp` 提供 intrusive pointer 类模板及其操作；
- `detail/shared_count_patch.hpp` 根据 Boost 版本补充轻量级互斥量头文件。

## 公开别名

文件在以下命名空间导出 Boost 类型：

```cpp
namespace yggr::smart_ptr_ex
{
using boost::intrusive_ptr;
}
```

随后再导出到 Yggdrasil 根命名空间：

```cpp
using smart_ptr_ex::intrusive_ptr;
```

因此以下名称等价：

```cpp
boost::intrusive_ptr<T>
yggr::smart_ptr_ex::intrusive_ptr<T>
yggr::intrusive_ptr<T>
```

它们使用相同的 Boost intrusive pointer 实现和引用计数机制。

## intrusive pointer 基本语义

`intrusive_ptr<T>` 不单独分配或保存外部控制块。引用计数通常存储在被管理对象内部，并通过非成员函数完成增减：

```cpp
void intrusive_ptr_add_ref(T* value);
void intrusive_ptr_release(T* value);
```

这两个函数通常由对象所在命名空间提供，以便通过 ADL 查找。使用类型 `T` 前，必须确保其满足 Boost.IntrusivePtr 的引用计数契约。

典型对象需要提供或关联：

```cpp
struct Object
{
    mutable unsigned int ref_count;
};

void intrusive_ptr_add_ref(Object* value);
void intrusive_ptr_release(Object* value);
```

`intrusive_ptr_release` 通常在引用计数归零时销毁对象，但实际销毁策略由对象类型和对应的 release 函数决定。

## 与 shared_ptr 的区别

| 特性 | `intrusive_ptr` | `shared_ptr` |
| --- | --- | --- |
| 控制块 | 通常嵌入对象内部 | 通常由智能指针单独管理 |
| 引用计数函数 | 由对象类型提供 `add_ref`/`release` | 由智能指针实现管理 |
| 对象要求 | 必须支持 intrusive 引用计数契约 | 通常只需可构造和可删除 |
| 多个指针模板共享控制块 | 依赖对象内部计数 | 依赖共享控制块 |
| 内存布局 | 可减少额外控制块，但对象需携带计数 | 控制块与对象布局由实现决定 |

本文件只导出 intrusive pointer，不负责在 intrusive_ptr 和 shared_ptr 之间自动转换。

## `shared_count_patch.hpp`

该 detail 文件的实现为：

```cpp
#include <boost/version.hpp>

#if !(BOOST_VERSION < 105600)
#include <boost/detail/lightweight_mutex.hpp>
#endif
```

也就是说：

- Boost 版本小于 1.56 时，不包含该轻量级互斥量头文件；
- Boost 版本为 1.56 或更高时，补充包含 `boost/detail/lightweight_mutex.hpp`。

该文件名虽然包含 `patch`，当前源码中没有重写共享计数类或 intrusive_ptr 函数，只执行版本条件下的头文件兼容处理。

## 生命周期流程

```text
创建 intrusive_ptr<T>
  -> 对非空对象调用 intrusive_ptr_add_ref

复制 intrusive_ptr<T>
  -> 增加对象内部引用计数

销毁或重置 intrusive_ptr<T>
  -> 调用 intrusive_ptr_release
      -> 引用计数归零时由 release 逻辑销毁对象
```

具体调用时机由 Boost.IntrusivePtr 实现决定，实际引用计数和对象销毁逻辑由 `T` 相关的非成员函数负责。

## 使用示例

```cpp
class Object
{
public:
    Object() : _ref_count(0) {}

private:
    friend void intrusive_ptr_add_ref(Object* value);
    friend void intrusive_ptr_release(Object* value);
    unsigned int _ref_count;
};

void intrusive_ptr_add_ref(Object* value)
{
    ++value->_ref_count;
}

void intrusive_ptr_release(Object* value)
{
    if(--value->_ref_count == 0)
    {
        delete value;
    }
}

yggr::smart_ptr_ex::intrusive_ptr<Object> value(
    new Object());
```

示例中的引用计数函数仅展示基本契约，实际多线程场景应根据项目要求使用合适的原子或同步机制。

## 使用限制与注意事项

- 管理对象必须提供可被 Boost.IntrusivePtr 查找到的 `intrusive_ptr_add_ref` 和 `intrusive_ptr_release`。
- `intrusive_ptr_release` 必须正确处理引用计数归零和对象销毁，否则会造成泄漏或重复释放。
- 不要将同一对象同时交给互不兼容的所有权系统管理，避免重复释放。
- 该指针没有独立控制块，引用计数成员和线程安全策略由对象实现负责。
- `shared_count_patch.hpp` 只是 Boost 版本兼容依赖，不会改变 intrusive pointer 的所有权语义。
