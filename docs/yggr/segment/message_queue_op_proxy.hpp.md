# `message_queue_op_proxy.hpp`

## 概述

该头文件为 `boost::interprocess::message_queue_t<Tx1>` 提供
`yggr::segment::segment_op_proxy` 特化，用于统一创建、打开、查询和删除具名消息队列。

代理使用智能指针保存消息队列对象，而不是直接保存对象实例：

```cpp
typedef boost::interprocess::message_queue_t<Tx1> seg_type;
typedef ::yggr::shared_ptr<seg_type> seg_ptr_type;
```

消息队列由名称标识，因此该代理支持命名对象语义。

## 包含方式

该文件要求包含前已经定义 `YGGR_SEGMENT_OPERATOR_INCLUDE`。推荐通过统一入口包含：

```cpp
#include <yggr/segment/segment_op_proxy.hpp>
```

直接包含本文件可能触发预处理器错误。

## 命名空间

```cpp
yggr::segment
```

## 类声明

```cpp
template<typename Tx1>
class segment_op_proxy<boost::interprocess::message_queue_t<Tx1> >;
```

### 模板参数

| 参数 | 说明 |
| --- | --- |
| `Tx1` | Boost.Interprocess 消息队列使用的底层配置类型。 |

类型别名：

```cpp
typedef charset::utf8_string key_type;
typedef boost::interprocess::message_queue_t<Tx1> seg_type;
typedef ::yggr::shared_ptr<seg_type> seg_ptr_type;
typedef boost::mpl::bool_<true> base_type;
typedef base_type is_named_object_type;
```

`E_is_named_object` 和 `s_is_named_enable_object()` 的值均为 `true`。

## 公共接口

### `s_create`

```cpp
static bool s_create(seg_ptr_type& seg);
```

该无参重载明确不支持直接调用。实现会触发断言并返回 `false`，因为消息队列必须提供名称和容量参数。

```cpp
template<typename EArg, typename ...Args>
static bool s_create(seg_ptr_type& seg,
                     const key_type& name,
                     BOOST_FWD_REF(EArg) earg,
                     BOOST_FWD_REF(Args)... args);
```

将消息队列创建或打开参数转发到内部逻辑。未启用 C++11 可变参数模板时，项目通过预处理器生成等价的多参数重载。

实际常用参数形式为：

```cpp
static bool s_create(seg_ptr_type& seg,
                     const key_type& name,
                     segment_op::create_only_type op,
                     seg_type::size_type max_num_msg,
                     seg_type::size_type max_msg_size);

static bool s_create(seg_ptr_type& seg,
                     const key_type& name,
                     segment_op::create_only_type op,
                     seg_type::size_type max_num_msg,
                     seg_type::size_type max_msg_size,
                     const boost::interprocess::permissions& permissions);

static bool s_create(seg_ptr_type& seg,
                     const key_type& name,
                     segment_op::open_or_create_type op,
                     seg_type::size_type max_num_msg,
                     seg_type::size_type max_msg_size);

static bool s_create(seg_ptr_type& seg,
                     const key_type& name,
                     segment_op::open_only_type op);
```

参数说明：

| 参数 | 说明 |
| --- | --- |
| `seg` | 输出的消息队列智能指针。成功后指向新创建或已打开的队列。 |
| `name` | 消息队列名称，类型为 UTF-8 字符串。不能为空。 |
| `op` | 创建或打开策略标签。 |
| `max_num_msg` | 创建队列时允许保存的最大消息数量。 |
| `max_msg_size` | 创建队列时允许的单条消息最大大小。 |
| `permissions` | 可选的 Boost.Interprocess 权限对象。 |

### `s_remove`

```cpp
static void s_remove(const key_type& name);
```

删除指定名称的消息队列。

- 空名称会直接返回；
- 非空名称通过 `segment_destroyer<seg_type>` 执行删除；
- 删除会影响其他使用相同名称的进程或对象。

### `s_seg_is_empty`

```cpp
static bool s_seg_is_empty(seg_ptr_type seg);
```

通过 `s_seg_size(seg)` 判断队列是否为空。空智能指针或有效容量乘积为零时返回 `true`。

### `s_seg_size`

```cpp
static size_type s_seg_size(seg_ptr_type seg);
```

返回：

```cpp
seg ? seg->get_max_msg() * seg->get_max_msg_size() : 0;
```

因此该值表示消息队列配置的总容量估算值，即最大消息数与单条消息最大大小的乘积；空指针返回 `0`。

### `s_is_named_enable_object`

```cpp
static bool s_is_named_enable_object(void);
```

返回 `true`，表示该代理支持命名对象语义。

## 创建策略

### `create_only`

使用指定名称创建新的消息队列。名称已存在时创建失败。

创建时需要提供：

- 最大消息数量；
- 单条消息最大大小；
- 可选权限对象。

### `open_or_create`

名称不存在时创建消息队列，名称已存在时打开现有队列。

当打开已有队列时，传入的容量参数不会改变已有队列的配置；实际容量由已有队列决定。

### `open_only`

仅打开已有消息队列，不创建新队列。该模式只需要名称和打开策略。

## 创建与错误处理

创建模式的内部流程如下：

1. 检查消息队列名称是否为空；
2. 使用 `new seg_type(...)` 创建消息队列，并通过 `construct_shared` 包装为 `shared_ptr`；
3. 将临时智能指针与输出指针交换；
4. 首次捕获 `segment_error` 时调用创建错误修复器；
5. 清理后重试一次；
6. 重试仍失败时，通过 `exception::exception::throw_error()` 抛出错误。

打开模式采用相同的重试结构，但打开失败的默认错误修复器不会删除消息队列。

创建模式的默认错误修复器会删除对应名称的消息队列，以避免残留对象影响后续重试。

## 使用示例

```cpp
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

void create_message_queue()
{
    typedef boost::interprocess::message_queue
        queue_type;
    typedef yggr::segment::segment_op_proxy<queue_type> proxy_type;
    typedef yggr::segment::segment_op segment_op_type;
    typedef proxy_type::seg_ptr_type queue_ptr_type;

    queue_ptr_type queue;
    yggr::charset::utf8_string name("example_message_queue");

    const bool created = proxy_type::s_create(
        queue,
        name,
        segment_op_type::open_or_create(),
        static_cast<queue_type::size_type>(32),
        static_cast<queue_type::size_type>(1024));

    if(created && !proxy_type::s_seg_is_empty(queue))
    {
        const yggr::size_type capacity = proxy_type::s_seg_size(queue);
        (void)capacity;
    }

    proxy_type::s_remove(name);
}
```

### 打开已有队列

```cpp
void open_message_queue(
    yggr::segment::segment_op_proxy<
        boost::interprocess::message_queue>::seg_ptr_type& queue,
    const yggr::charset::utf8_string& name)
{
    typedef yggr::segment::segment_op_proxy<
        boost::interprocess::message_queue> proxy_type;

    proxy_type::s_create(
        queue,
        name,
        yggr::segment::segment_op::open_only());
}
```

## 生命周期与所有权

- 代理本身不可实例化，所有操作均为静态接口。
- 消息队列对象通过 `seg_ptr_type` 管理，智能指针负责当前进程中的对象生命周期。
- 删除命名队列需要显式调用 `s_remove(name)`；仅释放智能指针不会删除系统中的具名队列。
- 删除前应确认没有其他进程仍依赖该队列。
- 队列的实际消息发送、接收和同步操作由 `boost::interprocess::message_queue_t` 提供。

## 注意事项

1. 无参 `s_create(seg_ptr_type&)` 会触发断言并返回 `false`，必须提供名称和创建参数。
2. `name` 不能为空；空名称会使创建或打开操作返回 `false`。
3. `create_only` 和 `open_or_create` 需要最大消息数和最大消息大小；`open_only` 不需要容量参数。
4. `open_or_create` 打开已有队列时不会调整其容量配置。
5. `s_seg_size()` 返回最大消息数与最大消息大小的乘积，不一定等于底层实现实际占用的字节数。
6. 消息队列的容量、消息大小及权限必须符合当前平台和 Boost.Interprocess 的限制。
7. 源码中带 `permissions` 的 `open_or_create` 内部转发重载存在参数名 `size` 的引用，但该作用域没有定义该变量；使用该重载前应先修正源码中的参数转发问题。
8. 创建失败时默认会删除指定名称的消息队列并重试；打开失败时不会删除原队列。

## 相关组件

- `yggr/segment/segment_op_proxy.hpp`
- `yggr/segment/segment_op.hpp`
- `yggr/segment/segment_destroyer.hpp`
- `yggr/segment/segment_error.hpp`
- `yggr/smart_ptr_ex/shared_ptr.hpp`
- `boost/interprocess/ipc/message_queue.hpp`
