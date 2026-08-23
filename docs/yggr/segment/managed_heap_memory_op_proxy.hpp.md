# `managed_heap_memory_op_proxy.hpp`

## 概述

该头文件为
`boost::interprocess::basic_managed_heap_memory<Tx1, Tx2, Tx3>` 提供
`yggr::segment::segment_op_proxy` 特化，用于统一管理基于堆内存的 Boost.Interprocess 托管段。

该代理负责：

- 按指定大小创建托管堆内存段；
- 清空或释放当前段；
- 判断段是否为空；
- 查询段大小；
- 统一处理创建失败和 `segment_error`。

该段没有名称，因此不支持命名对象语义。

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
template<typename Tx1, typename Tx2,
         template<typename> class Tx3>
class segment_op_proxy<
    boost::interprocess::basic_managed_heap_memory<Tx1, Tx2, Tx3> >;
```

### 模板参数

| 参数 | 说明 |
| --- | --- |
| `Tx1` | 底层字符或偏移相关配置类型，具体含义由 Boost.Interprocess 类型定义决定。 |
| `Tx2` | 托管堆内存使用的索引或分配配置类型，具体含义由 Boost.Interprocess 类型定义决定。 |
| `Tx3` | 接收一个类型参数的分配器模板。 |

类型别名：

```cpp
typedef boost::interprocess::basic_managed_heap_memory<
    Tx1, Tx2, Tx3> seg_type;
typedef boost::mpl::bool_<false> base_type;
typedef base_type is_named_object_type;
```

`E_is_named_object` 和 `s_is_named_enable_object()` 的值均为 `false`。

## 公共接口

### `s_create`

```cpp
static bool s_create(seg_type& seg);
```

创建一个临时空段，并与 `seg` 交换。该重载主要用于初始化、清空或重置当前对象，成功时返回 `true`。

```cpp
template<typename EArg, typename ...Args>
static bool s_create(seg_type& seg,
                     BOOST_FWD_REF(EArg) earg,
                     BOOST_FWD_REF(Args)... args);
```

将创建参数转发到内部创建逻辑。未启用 C++11 可变参数模板时，项目通过预处理器生成等价的多参数重载。

该代理的实际创建参数为：

```cpp
static bool s_create(seg_type& seg, std::size_t size);
```

参数说明：

| 参数 | 说明 |
| --- | --- |
| `seg` | 输出的托管堆内存对象。 |
| `size` | 要创建的托管堆内存大小，不能为零。 |

### `s_remove`

```cpp
static void s_remove(seg_type& seg);
```

通过重新创建空的 `seg_type` 并与 `seg` 交换，释放当前段管理状态。

### `s_seg_is_empty`

```cpp
static bool s_seg_is_empty(const seg_type& seg);
```

当 `seg.get_segment_manager()` 返回空指针时返回 `true`，否则返回 `false`。

### `s_seg_size`

```cpp
static size_type s_seg_size(const seg_type& seg);
```

当段管理器存在时返回 `seg.get_size()`；当对象为空时返回 `0`。

### `s_is_named_enable_object`

```cpp
static bool s_is_named_enable_object(void);
```

返回 `false`，表示该段不支持命名对象语义。

## 创建流程与错误处理

内部创建逻辑由 `s_prv_create` 完成：

1. `size` 为零时立即返回 `false`；
2. 使用 `size` 构造临时 `seg_type`；
3. 将临时对象与 `seg` 交换；
4. 首次捕获 `segment_error` 时调用默认错误修复器；
5. 清理后重试一次；
6. 重试仍失败时，通过 `exception::exception::throw_error()` 抛出错误。

默认错误修复器调用 `s_remove(seg)`，用于清理输出对象的现有段状态。

## 使用示例

```cpp
#include <yggr/segment/segment_op_proxy.hpp>
#include <boost/interprocess/managed_heap_memory.hpp>

void create_heap_memory()
{
    typedef boost::interprocess::managed_heap_memory memory_type;
    typedef yggr::segment::segment_op_proxy<memory_type> proxy_type;

    memory_type memory;
    const std::size_t memory_size = 4096;

    if(proxy_type::s_create(memory, memory_size))
    {
        const yggr::size_type actual_size = proxy_type::s_seg_size(memory);
        const bool empty = proxy_type::s_seg_is_empty(memory);
        (void)actual_size;
        (void)empty;
    }

    proxy_type::s_remove(memory);
}
```

## 生命周期与所有权

- 代理本身不可实例化，所有操作均为静态接口。
- `managed_heap_memory` 自行管理其堆内存；调用者不需要额外提供外部缓冲区地址。
- 调用 `s_remove()` 后，原托管段状态不应继续使用。
- `s_seg_size()` 返回有效托管段的大小；空对象返回零。
- 段的具体内存分配和对象管理能力由底层 `basic_managed_heap_memory` 提供。

## 注意事项

1. `size == 0` 时 `s_create()` 返回 `false`，不会进行构造。
2. 该代理不支持 `create_only`、`open_only` 或 `open_or_create` 标签；创建只接受内存大小。
3. 这是进程内的堆内存托管段，不应将其当作命名共享内存或文件映射使用。
4. `s_is_named_enable_object()` 始终返回 `false`，不能通过名称查找该段。
5. 首次创建失败会触发清理并重试；第二次失败会抛出项目异常。
6. 具体模板参数、段布局和大小语义以当前 Boost.Interprocess 版本中的 `basic_managed_heap_memory` 定义为准。

## 相关组件

- `yggr/segment/segment_op_proxy.hpp`
- `yggr/segment/segment_error.hpp`
- `boost/interprocess/managed_heap_memory.hpp`
