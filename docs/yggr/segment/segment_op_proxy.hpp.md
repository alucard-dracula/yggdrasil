# `segment_op_proxy.hpp`

## 概述

该头文件是 yggdrasil 段操作代理的统一入口，负责：

- 前置声明 `yggr::segment::segment_op_proxy<Seg>`；
- 设置内部包含控制宏 `YGGR_SEGMENT_OPERATOR_INCLUDE`；
- 集中包含各类命名和非命名段的代理特化。

具体的创建、删除、大小查询和空状态判断由各段类型对应的特化实现。

## 头文件

```cpp
#include <yggr/segment/segment_op_proxy.hpp>
```

业务代码应优先包含本文件，而不是直接包含某个具体的 `*_op_proxy.hpp` 实现文件。

## 命名空间

```cpp
yggr::segment
```

## 主模板声明

```cpp
template<typename Seg>
class segment_op_proxy;
```

该文件只声明主模板，不直接定义通用实现。具体 `Seg` 类型必须存在匹配的特化。

## 统一操作约定

代理特化通常提供以下静态接口：

| 接口 | 作用 |
| --- | --- |
| `s_create(...)` | 创建、打开、初始化或重置段对象。 |
| `s_remove(...)` | 删除命名资源，或释放非命名段对象。 |
| `s_seg_is_empty(...)` | 判断段对象是否为空或未初始化。 |
| `s_seg_size(...)` | 查询段大小或该类型对应的容量值。 |
| `s_is_named_enable_object()` | 判断该类型是否支持命名对象语义。 |

不同段类型的参数和具体行为由各自的代理特化决定，不能假定所有特化具有完全相同的参数列表。

## 内置代理特化

### 命名对象

以下代理支持名称标识，并将 `E_is_named_object` 设置为 `true`：

```cpp
#include <yggr/segment/shared_memory_object_op_proxy.hpp>
#include <yggr/segment/managed_shared_memory_op_proxy.hpp>
#include <yggr/segment/file_mapping_op_proxy.hpp>
#include <yggr/segment/managed_mapped_file_op_proxy.hpp>
#include <yggr/segment/message_queue_op_proxy.hpp>
```

对应类型包括：

| 代理文件 | 底层类型 |
| --- | --- |
| `shared_memory_object_op_proxy.hpp` | `boost::interprocess::shared_memory_object` |
| `managed_shared_memory_op_proxy.hpp` | `boost::interprocess::basic_managed_shared_memory<...>` |
| `file_mapping_op_proxy.hpp` | `boost::interprocess::file_mapping` |
| `managed_mapped_file_op_proxy.hpp` | `boost::interprocess::basic_managed_mapped_file<...>` |
| `message_queue_op_proxy.hpp` | `boost::interprocess::message_queue_t<...>` |

### 非命名对象

以下代理不支持名称标识，并将 `E_is_named_object` 设置为 `false`：

```cpp
#include <yggr/segment/managed_heap_memory_op_proxy.hpp>
#include <yggr/segment/managed_external_buffer_op_proxy.hpp>
#include <yggr/segment/anonymous_shared_memory_op_proxy.hpp>
```

对应类型包括：

| 代理文件 | 底层类型 |
| --- | --- |
| `managed_heap_memory_op_proxy.hpp` | `boost::interprocess::basic_managed_heap_memory<...>` |
| `managed_external_buffer_op_proxy.hpp` | `boost::interprocess::basic_managed_external_buffer<...>` |
| `anonymous_shared_memory_op_proxy.hpp` | `yggr::segment::anonymous_shared_memory` |

## 包含控制宏

在包含具体代理特化前，该文件设置：

```cpp
#define YGGR_SEGMENT_OPERATOR_INCLUDE
```

各具体代理文件会检查该宏；如果用户绕过统一入口直接包含实现文件，通常会触发预处理器错误。这一约束用于确保代理特化的依赖顺序正确。

## 使用示例

```cpp
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

void create_segment()
{
    typedef boost::interprocess::managed_shared_memory memory_type;
    typedef yggr::segment::segment_op_proxy<memory_type> proxy_type;

    memory_type segment;
    const yggr::charset::utf8_string name("example_memory");

    const bool created = proxy_type::s_create(
        segment,
        name,
        yggr::segment::segment_op::open_or_create(),
        static_cast<memory_type::size_type>(4096));

    if(created && !proxy_type::s_seg_is_empty(segment))
    {
        const yggr::size_type size = proxy_type::s_seg_size(segment);
        (void)size;
    }
}
```

## 与 `segment_container` 的关系

`segment_container<Seg>` 根据：

```cpp
segment_op_proxy<Seg>::E_is_named_object
```

选择命名段或非命名段的内部容器实现，并将构造、释放、大小查询和空状态判断转发给对应代理。

因此，为自定义段类型使用 `segment_container` 前，需要提供相应的
`segment_op_proxy<CustomSeg>` 特化，并至少满足容器所需的静态操作约定。

## 扩展自定义代理

自定义段类型通常需要：

1. 声明或定义 `segment_op_proxy<CustomSeg>` 特化；
2. 提供 `seg_type`、`E_is_named_object` 等类型和常量；
3. 实现 `s_create()`、`s_remove()`、`s_seg_is_empty()`、`s_seg_size()` 和
   `s_is_named_enable_object()`；
4. 在使用 `segment_container<CustomSeg>` 前确保特化已可见。

具体参数设计应与自定义段的创建、打开和生命周期模型保持一致。

## 注意事项

1. 推荐始终包含 `segment_op_proxy.hpp` 作为代理入口。
2. 该文件不提供通用主模板实现，只有被具体特化覆盖的 `Seg` 类型才能使用代理接口。
3. 各代理的 `s_create()` 参数因段类型而异，例如消息队列使用共享指针，而托管共享内存使用段对象引用。
4. `s_remove()` 对命名对象通常删除操作系统级资源，对非命名对象通常只释放对象状态。
5. `E_is_named_object` 会影响 `segment_container` 的内部存储和生命周期处理，应准确设置。
6. 具体代理实现依赖 Boost.Interprocess，并可能进一步依赖项目异常、文件系统和智能指针组件。

## 相关组件

- `yggr/segment/segment_container.hpp`
- `yggr/segment/segment_op.hpp`
- `yggr/segment/segment_destroyer.hpp`
- `yggr/segment/shared_memory_object_op_proxy.hpp`
- `yggr/segment/managed_shared_memory_op_proxy.hpp`
- `yggr/segment/file_mapping_op_proxy.hpp`
- `yggr/segment/managed_mapped_file_op_proxy.hpp`
- `yggr/segment/message_queue_op_proxy.hpp`
- `yggr/segment/managed_heap_memory_op_proxy.hpp`
- `yggr/segment/managed_external_buffer_op_proxy.hpp`
- `yggr/segment/anonymous_shared_memory_op_proxy.hpp`
