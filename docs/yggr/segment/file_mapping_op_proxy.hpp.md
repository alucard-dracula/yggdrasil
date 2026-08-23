# `file_mapping_op_proxy.hpp`

## 概述

该头文件为 `boost::interprocess::file_mapping` 提供
`yggr::segment::segment_op_proxy` 特化，用于管理基于文件的命名内存映射。

该代理负责：

- 创建或打开映射文件；
- 按指定访问模式构造 `file_mapping`；
- 删除映射文件；
- 查询映射文件大小；
- 判断映射是否为空；
- 转换和报告文件系统及段错误。

## 包含方式

该文件要求在包含前已定义 `YGGR_SEGMENT_OPERATOR_INCLUDE`。通常应包含统一入口：

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
class segment_op_proxy<boost::interprocess::file_mapping>;
```

类型别名：

```cpp
typedef charset::utf8_string key_type;
typedef boost::interprocess::file_mapping seg_type;
typedef boost::mpl::bool_<true> base_type;
typedef base_type is_named_object_type;
```

`E_is_named_object` 和 `s_is_named_enable_object()` 均表示 `true`，因为映射对象由文件名标识。

## 创建接口

### 清空当前映射

```cpp
static bool s_create(seg_type& seg);
```

创建一个临时空映射对象，并与 `seg` 交换。该重载主要用于清空或重置当前映射，成功时返回 `true`。

### 按策略创建或打开

```cpp
template<typename EArg, typename ...Args>
static bool s_create(seg_type& seg,
                     BOOST_FWD_REF(EArg) earg,
                     BOOST_FWD_REF(Args)... args);
```

该接口将参数转发到内部创建逻辑。未启用 C++11 可变参数模板时，项目通过预处理器生成等价的多参数重载。

实际使用时，参数通常为以下形式之一：

```cpp
static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::create_only_type op,
                     size_type size,
                     u32 mode);

static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_or_create_type op,
                     size_type size,
                     u32 mode);

static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_only_type op,
                     u32 mode);
```

其中：

| 参数 | 说明 |
| --- | --- |
| `seg` | 输出的 `boost::interprocess::file_mapping` 对象。 |
| `name` | 映射文件路径，类型为 UTF-8 字符串。 |
| `op` | 创建/打开策略标签。 |
| `size` | `create_only` 或 `open_or_create` 策略下创建文件时使用的大小。 |
| `mode` | 转换为 `boost::interprocess::mode_t` 的映射访问模式。 |

## 打开策略

### `create_only`

对应 `segment_op::create_only()`。

1. 创建指定大小的二进制文件；
2. 若文件创建成功，按指定模式打开并映射文件；
3. 若映射创建失败，执行错误修复器；
4. 重试仍失败时抛出 `segment_error` 对应的项目异常。

文件已存在或文件创建失败时，操作不会成功。

### `open_or_create`

对应 `segment_op::open_or_create()`。

创建文件时使用 `file_success_if_exists` 选项：文件不存在则创建，已存在则继续使用现有文件，然后按指定模式打开映射。

需要注意，传入的 `size` 主要用于文件不存在时的创建；文件已经存在时，实际大小由已有文件决定。

### `open_only`

对应 `segment_op::open_only()`。

只打开已有的普通文件，不创建文件。文件不存在、不是普通文件或文件系统检查失败时，操作失败或按项目异常策略报告错误。

## 删除与查询接口

### `s_remove`

```cpp
static void s_remove(const key_type& name);
```

删除指定路径的映射文件。

- 空文件名会直接返回；
- 非空文件名通过 `segment_destroyer<seg_type>` 执行删除；
- 删除器对象离开作用域后完成对应清理。

### `s_seg_is_empty`

```cpp
static bool s_seg_is_empty(const seg_type& seg);
```

当 `s_seg_size(seg)` 返回零时返回 `true`，否则返回 `false`。

### `s_seg_size`

```cpp
static size_type s_seg_size(const seg_type& seg);
```

通过 `seg.get_name()` 获取映射文件路径，再查询文件系统中的文件大小。

以下情况返回 `0`：

- `seg.get_name()` 返回空指针；
- 文件系统查询抛出本地文件系统异常；
- 文件系统查询抛出 `yggr::stl_exception`；
- 查询结果为 `npos`。

### `s_is_named_enable_object`

```cpp
static bool s_is_named_enable_object(void);
```

返回 `true`，表示该代理支持命名对象语义。

## 创建与错误处理

内部流程分为文件准备和映射构造两步：

1. 使用本地文件系统操作创建文件、确认文件存在，或检查文件是否为普通文件；
2. 使用 `boost::interprocess::file_mapping(name.data(), mode_t(mode))` 构造临时映射；
3. 将临时映射与输出对象 `seg` 交换；
4. 若首次映射构造失败，调用传入的错误修复器并重试一次；
5. 重试失败时通过 `exception::exception::throw_error()` 抛出段错误。

创建阶段的默认错误修复器会删除相关文件；打开阶段的默认错误修复器不删除文件。最终错误处理器会直接抛出异常。

文件系统异常会被转换为项目统一异常；相应操作随后返回 `false` 的兜底路径。

## 使用示例

```cpp
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_op_proxy.hpp>

void create_file_mapping()
{
    typedef boost::interprocess::file_mapping mapping_type;
    typedef yggr::segment::segment_op_proxy<mapping_type> proxy_type;
    typedef yggr::segment::segment_op segment_op_type;

    mapping_type mapping;
    yggr::charset::utf8_string name("data/example.mapping");

    const bool created = proxy_type::s_create(
        mapping,
        name,
        segment_op_type::open_or_create(),
        static_cast<yggr::size_type>(4096),
        static_cast<yggr::u32>(boost::interprocess::read_write));

    if(created && !proxy_type::s_seg_is_empty(mapping))
    {
        const yggr::size_type bytes = proxy_type::s_seg_size(mapping);
        (void)bytes;
    }

    proxy_type::s_remove(name);
}
```

## 注意事项

1. `name` 是文件系统路径，也是该命名映射对象的标识；请确保路径编码与 `key_type` 的 UTF-8 约定一致。
2. `create_only` 和 `open_or_create` 需要提供文件大小，`open_only` 不需要大小参数。
3. `open_or_create` 打开已有文件时不会自动将其调整为传入的 `size`。
4. `s_remove` 删除的是文件系统中的映射文件；调用前应确认没有其他映射或进程仍依赖该文件。
5. `s_seg_size` 查询的是文件大小，不是当前映射视图的长度；异常或无法查询时以零表示。
6. 访问模式 `mode` 会转换为 Boost.Interprocess 的 `mode_t`，应传入兼容的模式值。
7. 本代理继承 `nonable::noncreateable`，不能直接实例化代理对象，所有操作均为静态接口。

## 相关组件

- `yggr/segment/segment_op_proxy.hpp`
- `yggr/segment/segment_op.hpp`
- `yggr/segment/segment_container.hpp`
- `yggr/segment/segment_destroyer.hpp`
- `yggr/segment/segment_error.hpp`
- `yggr/file_system/local_fsys_operators.hpp`
- `boost/interprocess/file_mapping.hpp`
