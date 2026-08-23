# `segment_container.hpp`

## 概述

该头文件定义了 `yggr::segment::segment_container<Seg>`，用于以统一接口封装不同类型的 Boost.Interprocess 段对象。

容器根据 `segment_op_proxy<Seg>::E_is_named_object` 自动选择内部实现：

- 命名段：保存 UTF-8 名称和段对象；
- 非命名段：只保存段对象；
- 消息队列：使用专门实现，以共享指针保存队列对象。

底层段的创建、打开、释放、大小查询和命名对象判断均委托给对应的 `segment_op_proxy<Seg>`。

## 头文件

```cpp
#include <yggr/segment/segment_container.hpp>
```

## 命名空间

```cpp
yggr::segment
```

## 类声明

```cpp
template<typename Seg>
class segment_container;
```

### 模板参数

| 参数 | 说明 |
| --- | --- |
| `Seg` | 要封装的段类型。必须存在对应的 `segment_op_proxy<Seg>` 特化。 |

公开类型别名：

```cpp
typedef typename base_type::key_type key_type;
typedef typename base_type::seg_type seg_type;
typedef typename base_type::org_type org_type;
```

其中：

- `key_type` 通常为 `yggr::charset::utf8_string`；
- `seg_type` 为被封装的段类型；
- `org_type` 为内部基础容器类型，通常是 `std::pair<key_type, Seg>` 或非命名段的原始段类型。

## 构造函数

### 转发构造

```cpp
template<typename ...Args>
segment_container(BOOST_FWD_REF(Args) ...args);
```

将参数转发给内部容器和对应的 `segment_op_proxy<Seg>::s_create()`。未启用 C++11 可变参数模板时，项目通过预处理器生成等价的重载。

不同 `Seg` 类型的具体参数由对应代理决定。例如，命名共享内存通常需要名称、创建策略和大小，非命名堆内存通常只需要大小。

### 从基础容器移动构造

```cpp
segment_container(BOOST_RV_REF(org_type) right);
```

从内部基础容器移动构造。

### 从同类容器移动构造

```cpp
segment_container(BOOST_RV_REF(this_type) right);
```

从另一个 `segment_container` 移动构造。

该类不可复制，但支持移动语义。

## 公共接口

### `name`

```cpp
const key_type& name(void) const;
template<typename String>
String name(void) const;
```

返回命名段的 UTF-8 名称，或将名称转换为指定字符串类型。

对于非命名段，`name()` 返回空的 `key_type` 或空的目标字符串。

### `key`

```cpp
const key_type& key(void) const;
template<typename String>
String key(void) const;
```

`key()` 是名称访问接口的同义形式。命名段返回其名称，非命名段返回空字符串。

### `segment`

```cpp
seg_type& segment(void);
const seg_type& segment(void) const;
```

返回底层段对象的可修改或只读引用。

消息队列特化内部使用共享指针；调用该接口时会断言指针有效，然后返回其指向的消息队列对象。

### `size`

```cpp
size_type size(void) const;
```

调用对应代理的 `s_seg_size()` 查询段大小。

具体含义由段类型决定。例如，托管内存通常返回段大小，消息队列返回最大消息数与单条最大消息大小的乘积。

### `empty`

```cpp
bool empty(void) const;
```

调用对应代理的 `s_seg_is_empty()` 判断段是否为空或未初始化。

### `free`

```cpp
void free(void);
```

释放当前段资源：

- 命名段：通过名称删除底层命名资源，清空名称，并重置段对象；
- 非命名段：调用代理释放段对象；
- 消息队列：删除当前命名队列，切换到内部默认名称 `mq_empty`，清空原队列指针并重新创建默认队列。

`free()` 之后，原 `segment()` 引用不应继续使用。

### `is_named_enable_object`

```cpp
static bool is_named_enable_object(void);
```

返回对应代理的命名对象能力。命名段返回 `true`，非命名段返回 `false`。

## 移动赋值

```cpp
this_type& operator=(BOOST_RV_REF(org_type) right);
this_type& operator=(BOOST_RV_REF(this_type) right);
```

从基础容器或同类容器移动赋值。复制赋值不可用。

## 交换操作

### 与基础容器交换

```cpp
void swap(org_type& right);
void swap(BOOST_RV_REF(org_type) right);
```

与内部基础容器交换名称和段对象。

### 与同类容器交换

```cpp
void swap(this_type& right);
void swap(BOOST_RV_REF(this_type) right);
```

与另一个 `segment_container` 交换其内部状态。

### 非成员 `swap`

项目在 `yggr::segment::swap_support` 中提供以下模板重载，并导出到 `yggr::segment`、`std` 和 `boost`：

```cpp
template<typename Seg>
void swap(typename segment_container<Seg>::org_type& left,
          segment_container<Seg>& right);

template<typename Seg>
void swap(segment_container<Seg>& left,
          typename segment_container<Seg>::org_type& right);

template<typename Seg>
void swap(segment_container<Seg>& left,
          segment_container<Seg>& right);
```

## 内部实现分类

### 命名段

当 `segment_op_proxy<Seg>::E_is_named_object` 为 `true` 时，内部容器保存：

```cpp
std::pair<charset::utf8_string, Seg>
```

典型类型包括托管共享内存、托管映射文件和文件映射。构造时先保存名称，再调用代理创建或打开资源。

命名段基础容器的析构函数不会调用 `s_remove()`；销毁容器只关闭当前段对象，命名资源是否删除由显式 `free()` 或对应的销毁操作决定。

### 消息队列

`boost::interprocess::message_queue_t<T>` 使用专门特化，内部保存：

```cpp
std::pair<key_type, yggr::shared_ptr<message_queue_t<T>)>
```

消息队列必须通过共享指针管理。默认构造会使用名称 `mq_empty` 并尝试以 `open_or_create`、容量 `0, 0` 创建默认队列。

### 非命名段

当 `segment_op_proxy<Seg>::E_is_named_object` 为 `false` 时，内部只保存：

```cpp
Seg _seg;
```

典型类型包括匿名共享内存、托管堆内存和托管外部缓冲区。构造参数直接转发给代理，析构时调用代理的 `s_remove(_seg)`。

## 使用示例

### 命名共享内存

```cpp
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

void use_named_segment()
{
    typedef boost::interprocess::managed_shared_memory memory_type;
    typedef yggr::segment::segment_container<memory_type> container_type;

    container_type container(
        yggr::charset::utf8_string("example_memory"),
        yggr::segment::segment_op::open_or_create(),
        static_cast<memory_type::size_type>(1024 * 1024));

    if(!container.empty())
    {
        memory_type& segment = container.segment();
        (void)segment;
    }

    container.free();
}
```

### 非命名段

```cpp
#include <yggr/segment/segment_container.hpp>
#include <boost/interprocess/managed_heap_memory.hpp>

void use_heap_segment()
{
    typedef boost::interprocess::managed_heap_memory memory_type;
    typedef yggr::segment::segment_container<memory_type> container_type;

    container_type container(static_cast<std::size_t>(4096));

    const std::size_t size = container.size();
    (void)size;
}
```

## 生命周期与所有权

- `segment_container` 不支持复制，只支持移动构造和移动赋值。
- 容器构造时通常会调用对应代理创建或打开段。
- `segment()` 返回的引用受容器生命周期及后续 `free()`、移动和交换操作影响。
- 命名资源通常不会因为命名容器析构而自动删除，应按需显式调用 `free()` 或代理的 `s_remove()`。
- 非命名段在容器析构时由对应代理释放。
- 消息队列的底层对象由 `yggr::shared_ptr` 管理，但命名队列资源需要显式删除。

## 注意事项

1. 使用前必须确保 `segment_op_proxy<Seg>` 已提供匹配的特化和 `E_is_named_object` 常量。
2. 构造参数和创建策略完全由对应代理决定，不能把不同段类型的参数格式混用。
3. 命名段的 `name()` 和 `key()` 返回资源标识；非命名段这两个接口始终为空。
4. 消息队列的 `segment()` 在内部指针为空时会触发断言。
5. `free()` 可能删除操作系统级命名资源；调用前应确认没有其他进程或对象仍在使用该资源。
6. 移动或交换后，名称与段对象会整体转移，原对象不应继续假定拥有原资源。
7. 消息队列默认构造使用 `mq_empty`，多个默认构造对象可能共享同一命名队列，使用时应留意资源冲突。

## 相关组件

- `yggr/segment/segment_op_proxy.hpp`
- `yggr/segment/segment_op.hpp`
- `yggr/segment/segment_destroyer.hpp`
- `yggr/segment/segment_config.hpp`
- `yggr/segment/anonymous_segment_container.hpp`
- `boost/interprocess/managed_shared_memory.hpp`
- `boost/interprocess/managed_mapped_file.hpp`
- `boost/interprocess/ipc/message_queue.hpp`
