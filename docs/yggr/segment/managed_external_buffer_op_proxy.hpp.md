# `managed_external_buffer_op_proxy.hpp`

## 概述

该头文件为
`boost::interprocess::basic_managed_external_buffer<Tx1, Tx2, Tx3>` 提供
`yggr::segment::segment_op_proxy` 特化，用于统一管理外部缓冲区中的 Boost.Interprocess 托管段。

该代理提供：

- 创建和初始化托管外部缓冲区；
- 打开已有的托管外部缓冲区；
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
    boost::interprocess::basic_managed_external_buffer<Tx1, Tx2, Tx3> >;
```

### 模板参数

| 参数 | 说明 |
| --- | --- |
| `Tx1` | 底层字符或偏移相关配置类型，具体含义由 Boost.Interprocess 类型定义决定。 |
| `Tx2` | 托管外部缓冲区使用的索引或分配配置类型，具体含义由 Boost.Interprocess 类型定义决定。 |
| `Tx3` | 接收一个类型参数的分配器模板。 |

类型别名：

```cpp
typedef boost::interprocess::basic_managed_external_buffer<
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

常用调用形式为：

```cpp
static bool s_create(seg_type& seg,
                     segment_op::create_only_type op,
                     void* address,
                     std::size_t size);

static bool s_create(seg_type& seg,
                     segment_op::open_only_type op,
                     void* address,
                     std::size_t size);
```

参数说明：

| 参数 | 说明 |
| --- | --- |
| `seg` | 输出的托管外部缓冲区对象。 |
| `op` | 创建或打开策略标签。 |
| `address` | 外部缓冲区起始地址。 |
| `size` | 外部缓冲区大小，不能为零。 |

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

## 创建策略

### `create_only`

对应 `segment_op::create_only()`。

使用给定的外部地址和大小创建并初始化一个新的托管外部缓冲区。创建成功后，临时段对象会与输出对象交换。

### `open_only`

对应 `segment_op::open_only()`。

使用给定的外部地址和大小打开已有的托管外部缓冲区状态。该策略不会执行命名文件查找；地址和缓冲区布局必须符合底层 Boost.Interprocess 类型的要求。

该代理没有为 `open_or_create` 提供专用重载。

## 错误处理

内部创建流程由 `s_prv_create_detail` 完成：

1. `size` 为零时立即返回 `false`；
2. 使用 `op`、`address` 和 `size` 构造临时 `seg_type`；
3. 将临时对象与 `seg` 交换；
4. 首次捕获 `segment_error` 时调用错误修复器；
5. 清理后重试一次；
6. 重试仍失败时，通过 `exception::exception::throw_error()` 抛出错误。

默认错误修复器调用 `s_remove(seg)`，用于清理输出对象的现有段状态。

## 使用示例

```cpp
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>

void create_external_buffer(void* buffer, std::size_t buffer_size)
{
    typedef boost::interprocess::managed_external_buffer buffer_type;
    typedef yggr::segment::segment_op_proxy<buffer_type> proxy_type;

    buffer_type segment;

    if(proxy_type::s_create(
        segment,
        yggr::segment::segment_op::create_only(),
        buffer,
        buffer_size))
    {
        const yggr::size_type size = proxy_type::s_seg_size(segment);
        (void)size;
    }

    proxy_type::s_remove(segment);
}
```

## 生命周期与所有权

- 代理本身不可实例化，所有操作均为静态接口。
- 该代理不会分配外部原始缓冲区；调用者必须先准备好有效的内存区域，并保证其生命周期覆盖托管段的使用期。
- `address` 指向的缓冲区必须满足底层 `managed_external_buffer` 对地址、大小和对齐的要求。
- 调用 `s_remove()` 后，原托管段状态不应继续使用。
- `s_seg_size()` 返回的是托管段大小；空对象返回零。

## 注意事项

1. `size == 0` 时 `s_create()` 返回 `false`，不会进行构造。
2. `managed_external_buffer` 是基于调用者提供内存的托管段，不负责替调用者取得或释放外部缓冲区本身。
3. 不同进程或不同对象重新打开同一外部缓冲区时，必须保证地址、大小和布局满足 Boost.Interprocess 的约束。
4. `create_only` 与 `open_only` 是本代理支持的策略；不要假定存在 `open_or_create` 行为。
5. 首次创建失败会触发清理并重试；第二次失败会抛出项目异常。
6. `s_is_named_enable_object()` 始终返回 `false`，不能通过名称查找该段。
7. 具体模板参数和底层内存布局规则以当前 Boost.Interprocess 版本中的 `basic_managed_external_buffer` 定义为准。

## 相关组件

- `yggr/segment/segment_op_proxy.hpp`
- `yggr/segment/segment_op.hpp`
- `yggr/segment/segment_error.hpp`
- `boost/interprocess/managed_external_buffer.hpp`
