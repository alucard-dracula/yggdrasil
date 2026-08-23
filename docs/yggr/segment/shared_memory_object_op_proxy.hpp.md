# `shared_memory_object_op_proxy.hpp`

## 概述

该头文件为 `boost::interprocess::shared_memory_object` 提供
`yggr::segment::segment_op_proxy` 特化，用于统一创建、打开、查询和删除具名共享内存对象。

该代理只管理共享内存对象本身，不负责在其中构造托管对象或分配器；这些更高层能力由
`managed_shared_memory` 等类型提供。

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
template<>
class segment_op_proxy<boost::interprocess::shared_memory_object>;
```

类型别名：

```cpp
typedef charset::utf8_string key_type;
typedef boost::interprocess::shared_memory_object seg_type;
typedef boost::mpl::bool_<true> base_type;
typedef base_type is_named_object_type;
```

`E_is_named_object` 和 `s_is_named_enable_object()` 的值均为 `true`。

## 公共接口

### `s_create`

```cpp
static bool s_create(seg_type& seg);
```

创建一个临时空共享内存对象，并与 `seg` 交换。该重载用于重置当前对象，成功时返回 `true`。

```cpp
template<typename EArg, typename ...Args>
static bool s_create(seg_type& seg,
                     const key_type& key,
                     BOOST_FWD_REF(EArg) earg,
                     BOOST_FWD_REF(Args)... args);
```

将名称、创建策略、访问模式和可选权限等参数转发到内部创建逻辑。未启用 C++11 可变参数模板时，项目通过预处理器生成等价的多参数重载。

常用参数形式为：

```cpp
static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::create_only_type op,
                     u32 mode);

static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::create_only_type op,
                     u32 mode,
                     const boost::interprocess::permissions& permissions);

static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_or_create_type op,
                     u32 mode);

static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_or_create_type op,
                     u32 mode,
                     const boost::interprocess::permissions& permissions);

static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_only_type op,
                     u32 mode);
```

参数说明：

| 参数 | 说明 |
| --- | --- |
| `seg` | 输出的共享内存对象。成功后与新对象或已打开对象交换。 |
| `name` | 共享内存对象名称，类型为 UTF-8 字符串，不能为空。 |
| `op` | 创建或打开策略标签。 |
| `mode` | 转换为 `boost::interprocess::mode_t` 的访问模式。 |
| `permissions` | 创建共享内存对象时使用的可选权限。 |

### `s_remove`

```cpp
static void s_remove(const key_type& name);
```

删除指定名称的共享内存对象。

- 空名称会直接返回；
- 非空名称通过 `segment_destroyer<seg_type>` 执行底层 `remove(name)`；
- 删除操作可能影响其他进程中使用同名对象的代码。

### `s_seg_is_empty`

```cpp
static bool s_seg_is_empty(const seg_type& seg);
```

通过 `s_seg_size(seg)` 判断共享内存对象大小是否为零。

### `s_seg_size`

```cpp
static size_type s_seg_size(const seg_type& seg);
```

调用 `seg.get_size(offset)` 查询对象大小。查询成功时返回转换为 `size_type` 的大小；查询失败时返回 `0`。

### `s_is_named_enable_object`

```cpp
static bool s_is_named_enable_object(void);
```

返回 `true`，表示该代理支持命名对象语义。

## 创建策略

### `create_only`

使用指定名称创建新的共享内存对象。对象已存在时创建失败。

### `open_or_create`

名称不存在时创建对象，已存在时打开现有对象。

### `open_only`

仅打开已存在的共享内存对象，不创建新对象。

## 访问模式

`mode` 参数会转换为 Boost.Interprocess 的 `mode_t`。通常可使用
`yggr::segment::segment_mode` 提供的常量：

```cpp
static_cast<yggr::u32>(yggr::segment::segment_mode::E_read_only)
static_cast<yggr::u32>(yggr::segment::segment_mode::E_read_write)
static_cast<yggr::u32>(yggr::segment::segment_mode::E_copy_on_write)
```

具体模式是否被目标平台和底层操作支持，由 Boost.Interprocess 决定。

## 错误处理

内部创建和打开流程均采用一次重试机制：

1. 检查名称是否为空；
2. 使用策略、名称、访问模式和权限构造临时对象；
3. 将临时对象与 `seg` 交换；
4. 首次捕获 `segment_error` 时调用错误修复器；
5. 再次尝试构造和交换；
6. 重试仍失败时，通过 `exception::exception::throw_error()` 抛出错误。

错误修复行为取决于操作类型：

- 创建模式：默认删除指定名称的共享内存对象，再进行重试；
- 打开模式：默认不删除原共享内存对象；
- 名称为空：直接返回 `false`，不执行底层操作。

## 使用示例

```cpp
#include <yggr/segment/segment_mode.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <boost/interprocess/shared_memory_object.hpp>

void create_shared_memory_object()
{
    typedef boost::interprocess::shared_memory_object object_type;
    typedef yggr::segment::segment_op_proxy<object_type> proxy_type;
    typedef yggr::segment::segment_op segment_op_type;

    object_type object;
    yggr::charset::utf8_string name("example_shared_memory");

    const bool created = proxy_type::s_create(
        object,
        name,
        segment_op_type::open_or_create(),
        yggr::segment::segment_mode::E_read_write);

    if(created && !proxy_type::s_seg_is_empty(object))
    {
        const yggr::size_type size = proxy_type::s_seg_size(object);
        (void)size;
    }

    proxy_type::s_remove(name);
}
```

### 只读打开

```cpp
void open_shared_memory_object_read_only(
    boost::interprocess::shared_memory_object& object,
    const yggr::charset::utf8_string& name)
{
    typedef yggr::segment::segment_op_proxy<
        boost::interprocess::shared_memory_object> proxy_type;

    proxy_type::s_create(
        object,
        name,
        yggr::segment::segment_op::open_only(),
        yggr::segment::segment_mode::E_read_only);
}
```

## 生命周期与所有权

- 代理本身不可实例化，所有操作均为静态接口。
- `shared_memory_object` 对象管理当前进程中的句柄；系统级共享内存名称不会因为普通对象离开作用域而自动删除。
- 需要删除系统级共享内存对象时，应显式调用 `s_remove(name)`。
- 删除前应确认没有其他进程或映射仍依赖该名称。
- `s_create(seg)` 的无参数重载只用于重置对象，不会创建具名共享内存。

## 注意事项

1. `name` 不能为空；空名称会使创建和打开操作返回 `false`。
2. `mode` 应传入与 Boost.Interprocess `mode_t` 兼容的访问模式值。
3. `open_or_create` 打开已有对象时，权限参数不会重新设置已有对象的权限。
4. `s_seg_size()` 查询的是共享内存对象大小，查询失败时返回零。
5. 创建失败时默认可能删除指定名称并重试；打开失败时不会删除原对象。
6. `shared_memory_object` 只提供原始共享内存对象能力，不等同于托管共享内存。
7. 删除共享内存对象可能影响其他进程，调用 `s_remove()` 前应协调跨进程生命周期。
8. 具体名称规则、权限和访问模式语义以当前 Boost.Interprocess 版本及目标平台为准。

## 相关组件

- `yggr/segment/segment_op_proxy.hpp`
- `yggr/segment/segment_op.hpp`
- `yggr/segment/segment_mode.hpp`
- `yggr/segment/segment_destroyer.hpp`
- `yggr/segment/segment_error.hpp`
- `boost/interprocess/shared_memory_object.hpp`
