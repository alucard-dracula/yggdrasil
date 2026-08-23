# `segment_destroyer.hpp`

## 概述

该头文件定义了 `yggr::segment::segment_destroyer<Seg>`，用于按段类型统一处理命名资源的删除。

它采用类型特化为不同的 Boost.Interprocess 对象提供删除行为：

- 命名对象：构造时立即调用底层类型的 `remove(name)`；
- 非命名对象：构造和析构均为空操作；
- 模板化段类型：通过专用宏生成对应的部分特化。

该类型通常作为临时对象使用，以便在作用域内执行一次命名资源删除操作。

## 头文件

```cpp
#include <yggr/segment/segment_destroyer.hpp>
```

## 命名空间

```cpp
yggr::segment
```

## 类声明

```cpp
template<typename Seg>
struct segment_destroyer;
```

### 模板参数

| 参数 | 说明 |
| --- | --- |
| `Seg` | 需要删除或清理的段对象类型。必须匹配本文件提供的特化，或由项目另外提供特化。 |

## 命名对象特化

以下类型属于命名对象，`segment_destroyer<Seg>` 的构造函数接受文件名或对象名：

```cpp
segment_destroyer(const char* name);
```

当 `name` 非空时，构造函数立即调用对应底层类型的静态 `remove(name)`。

### 支持的命名类型

| `Seg` | 实际删除类型 |
| --- | --- |
| `boost::interprocess::shared_memory_object` | `shared_memory_object` |
| `boost::interprocess::basic_managed_shared_memory<T1, T2, T3>` | `shared_memory_object` |
| `boost::interprocess::file_mapping` | `file_mapping` |
| `boost::interprocess::basic_managed_mapped_file<T1, T2, T3>` | `file_mapping` |
| `boost::interprocess::message_queue_t<T1>` | `message_queue_t<T1>` |

### 类型别名

命名对象特化通常提供：

```cpp
typedef Seg obj_type;
typedef RemoveType type;
```

其中 `type` 是实际执行 `remove()` 的底层类型。

## 非命名对象特化

以下类型属于非命名对象，删除器不执行任何资源删除操作：

```cpp
segment_destroyer(void);
~segment_destroyer(void);
```

构造函数和析构函数均为空。

### 支持的非命名类型

| `Seg` | 行为 |
| --- | --- |
| `yggr::segment::anonymous_shared_memory` | 空操作，不按名称删除。 |
| `boost::interprocess::basic_managed_external_buffer<T1, T2, T3>` | 空操作，由外部缓冲区所有者管理内存。 |
| `boost::interprocess::basic_managed_heap_memory<T1, T2, T3>` | 空操作，由堆内存段对象管理资源。 |

非命名特化中的 `type` 定义为 `void`，用于表达不存在底层命名删除类型。

## 生命周期行为

命名对象删除发生在构造阶段，而不是析构阶段：

```cpp
{
    yggr::segment::segment_destroyer<
        boost::interprocess::managed_shared_memory> destroyer("example");
}
```

在 `destroyer` 构造完成前，`example` 对应的共享内存对象已经被请求删除；离开作用域时析构函数不再执行删除。

这种设计允许通过临时对象触发删除：

```cpp
{
    yggr::segment::segment_destroyer<
        boost::interprocess::file_mapping> destroyer(path);
}
```

## 使用示例

### 删除托管共享内存

```cpp
#include <yggr/segment/segment_destroyer.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

void remove_shared_memory()
{
    typedef boost::interprocess::managed_shared_memory memory_type;

    yggr::segment::segment_destroyer<memory_type> destroyer(
        "example_shared_memory");
}
```

### 删除消息队列

```cpp
#include <yggr/segment/segment_destroyer.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

void remove_message_queue()
{
    typedef boost::interprocess::message_queue queue_type;

    yggr::segment::segment_destroyer<queue_type> destroyer(
        "example_message_queue");
}
```

### 非命名对象

```cpp
#include <yggr/segment/segment_destroyer.hpp>
#include <yggr/segment/anonymous_shared_memory.hpp>

void use_anonymous_destroyer()
{
    typedef yggr::segment::anonymous_shared_memory memory_type;

    yggr::segment::segment_destroyer<memory_type> destroyer;
}
```

非命名对象的删除器不会执行任何操作。

## 宏接口

本文件通过以下宏生成特化：

```cpp
YGGR_SEGMENT_NAMED_ENABLE_OBJECT_DELETER_DEF(__obj__, __del_obj__)
YGGR_SEGMENT_NAMED_ENABLE_WRAP_OBJECT_DELETER_DEF(__obj__, __del_obj__)
YGGR_SEGMENT_NAMED_DISABLE_OBJECT_DELETER_DEF(__obj__)
YGGR_SEGMENT_NAMED_DISABLE_WRAP_OBJECT_DELETER_DEF(__obj__)
```

宏用途：

- `NAMED_ENABLE_OBJECT`：为普通命名类型生成特化；
- `NAMED_ENABLE_WRAP_OBJECT`：为三参数模板命名类型生成部分特化；
- `NAMED_DISABLE_OBJECT`：为普通非命名类型生成空操作特化；
- `NAMED_DISABLE_WRAP_OBJECT`：为三参数模板非命名类型生成空操作部分特化。

这些宏主要用于本头文件内部建立统一映射，通常不需要在业务代码中直接调用。

## 注意事项

1. 命名对象删除在 `segment_destroyer` 构造时立即发生，而不是在析构时发生。
2. 传入空指针名称时，命名对象特化不会调用 `remove()`。
3. 删除操作可能影响其他进程或对象正在使用的共享内存、文件映射或消息队列。
4. 删除命名对象不会自动关闭调用者持有的其他映射或队列句柄。
5. 非命名对象没有名称，使用无参构造；其资源由对象自身或外部缓冲区所有者管理。
6. `segment_destroyer` 仅提供已知类型的特化；使用未覆盖的 `Seg` 类型需要另行提供特化，否则无法实例化。
7. 删除是否成功、是否延迟到最后一个句柄关闭等细节由对应 Boost.Interprocess 类型的 `remove()` 实现决定。

## 相关组件

- `yggr/segment/segment_op_proxy.hpp`
- `yggr/segment/segment_container.hpp`
- `yggr/segment/anonymous_shared_memory.hpp`
- `boost/interprocess/shared_memory_object.hpp`
- `boost/interprocess/file_mapping.hpp`
- `boost/interprocess/managed_shared_memory.hpp`
- `boost/interprocess/managed_mapped_file.hpp`
- `boost/interprocess/ipc/message_queue.hpp`
