# `managed_mapped_file_op_proxy.hpp`

## 概述

该头文件为 Boost.Interprocess 的托管映射文件类型提供
`yggr::segment::segment_op_proxy` 特化，用于统一管理基于文件的托管内存段。

该代理负责：

- 创建或打开托管映射文件；
- 支持多种创建和打开模式；
- 自动创建映射文件的父目录；
- 删除映射文件；
- 查询段大小和空状态；
- 处理映射创建失败及相关异常。

该代理管理的是命名对象，因此 `is_named_object_type` 的值为 `true`。

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
    boost::interprocess::basic_managed_mapped_file<Tx1, Tx2, Tx3> >;
```

### 模板参数

| 参数 | 说明 |
| --- | --- |
| `Tx1` | 底层字符或偏移相关配置类型，具体含义由 Boost.Interprocess 类型定义决定。 |
| `Tx2` | 托管映射文件使用的索引或分配配置类型，具体含义由 Boost.Interprocess 类型定义决定。 |
| `Tx3` | 接收一个类型参数的分配器模板。 |

实现中的公开段类型别名为：

```cpp
typedef charset::utf8_string key_type;
typedef boost::interprocess::managed_mapped_file seg_type;
typedef boost::mpl::bool_<true> base_type;
typedef base_type is_named_object_type;
```

`E_is_named_object` 和 `s_is_named_enable_object()` 的值均为 `true`。

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

将创建或打开参数转发到内部逻辑。未启用 C++11 可变参数模板时，项目通过预处理器生成等价的多参数重载。

## 创建模式

创建模式需要文件名、策略标签和段大小，可选映射地址及文件权限。

### `create_only`

```cpp
static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::create_only_type op,
                     seg_type::size_type size);

static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::create_only_type op,
                     seg_type::size_type size,
                     const void* address);

static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::create_only_type op,
                     seg_type::size_type size,
                     const void* address,
                     const boost::interprocess::permissions& permissions);
```

创建新的托管映射文件并建立映射。默认映射地址为 `0`，默认使用 Boost.Interprocess 的默认权限。

文件路径的父目录不存在时，代理会先自动创建父目录。

### `open_or_create`

```cpp
static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_or_create_type op,
                     seg_type::size_type size);

static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_or_create_type op,
                     seg_type::size_type size,
                     const void* address);

static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_or_create_type op,
                     seg_type::size_type size,
                     const void* address,
                     const boost::interprocess::permissions& permissions);
```

文件不存在时创建并映射，文件已存在时打开现有文件。`size` 主要用于创建新文件；打开已有文件时，实际段大小由已有文件决定。

该模式同样会自动创建文件的父目录。

## 打开模式

打开模式只需要文件名和打开策略，可选映射地址；不需要段大小。

### `open_copy_on_write`

```cpp
static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_copy_on_write_type op);

static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_copy_on_write_type op,
                     const void* address);
```

以写时复制方式打开映射文件。对映射的修改不会直接写回共享文件，具体行为遵循 Boost.Interprocess 的映射语义。

### `open_read_only`

```cpp
static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_read_only_type op);

static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_read_only_type op,
                     const void* address);
```

以只读方式打开已有托管映射文件。

### `open_only`

```cpp
static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_only_type op);

static bool s_create(seg_type& seg,
                     const key_type& name,
                     segment_op::open_only_type op,
                     const void* address);
```

仅打开已有托管映射文件，不创建新文件。

## 删除与查询接口

### `s_remove`

```cpp
static void s_remove(const key_type& name);
```

删除指定路径的映射文件。

- 空文件名会直接返回；
- 非空文件名通过 `segment_destroyer<seg_type>` 执行删除；
- 删除操作不会修改调用者手中的字符串对象。

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

返回 `true`，表示该段支持命名对象语义。

## 文件和目录处理

创建模式会将 `name` 转换为本地文件系统路径：

1. 检查路径是否包含父目录；
2. 父目录非空且不存在时，调用本地文件系统操作创建目录；
3. 调用 `basic_managed_mapped_file` 构造函数创建或打开映射文件。

目录创建失败时，错误会转换为项目统一异常，并返回 `false` 的兜底结果。

## 错误处理

内部创建和打开流程均采用一次重试机制：

1. 首次构造临时托管映射文件；
2. 成功后与输出对象 `seg` 交换；
3. 首次捕获 `segment_error` 时调用错误修复器；
4. 再次尝试构造和交换；
5. 重试仍失败时，通过 `exception::exception::throw_error()` 抛出错误。

错误修复行为取决于操作类型：

- 创建模式：默认错误修复器删除相关映射文件，避免残留的无效文件影响重试；
- 打开模式：默认错误修复器不删除文件，因为文件可能由其他进程或对象使用；
- 自定义错误修复器：可通过内部转发接口提供，用于扩展失败处理逻辑。

## 使用示例

```cpp
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>

void create_managed_file()
{
    typedef boost::interprocess::managed_mapped_file file_type;
    typedef yggr::segment::segment_op_proxy<file_type> proxy_type;
    typedef yggr::segment::segment_op segment_op_type;

    file_type segment;
    yggr::charset::utf8_string name("data/example.managed");

    const bool created = proxy_type::s_create(
        segment,
        name,
        segment_op_type::open_or_create(),
        static_cast<file_type::size_type>(1024 * 1024));

    if(created && !proxy_type::s_seg_is_empty(segment))
    {
        const yggr::size_type size = proxy_type::s_seg_size(segment);
        (void)size;
    }

    proxy_type::s_remove(name);
}
```

### 只读打开示例

```cpp
void open_managed_file_read_only(
    boost::interprocess::managed_mapped_file& segment,
    const yggr::charset::utf8_string& name)
{
    typedef yggr::segment::segment_op_proxy<
        boost::interprocess::managed_mapped_file> proxy_type;

    proxy_type::s_create(
        segment,
        name,
        yggr::segment::segment_op::open_read_only());
}
```

## 生命周期与所有权

- 代理本身不可实例化，所有操作均为静态接口。
- 映射文件由文件系统路径标识，`seg` 管理当前进程中的映射状态。
- 调用 `s_remove()` 会删除文件；删除前应确认没有其他映射或进程仍依赖该文件。
- `open_read_only` 和 `open_copy_on_write` 的访问限制由底层映射模式决定。
- `s_seg_size()` 返回有效托管段的大小；空对象返回零。

## 注意事项

1. `name` 不能为空；空文件名会使创建或打开操作返回 `false`。
2. `create_only` 和 `open_or_create` 需要提供段大小；各打开模式不需要大小参数。
3. `open_or_create` 打开已有文件时，不会自动将其调整为传入的大小。
4. 创建模式会自动创建父目录，但目录创建失败会报告项目异常。
5. 创建映射失败时可能删除文件并重试；打开模式失败时不会删除原文件。
6. `address` 只是底层映射地址参数，必须遵守 Boost.Interprocess 对地址和映射布局的要求。
7. `s_seg_size()` 依赖底层段管理器；空段或无效段返回零。
8. 具体模板参数、权限和映射模式语义以当前 Boost.Interprocess 版本中的
   `basic_managed_mapped_file` 定义为准。

## 相关组件

- `yggr/segment/segment_op_proxy.hpp`
- `yggr/segment/segment_op.hpp`
- `yggr/segment/segment_container.hpp`
- `yggr/segment/segment_destroyer.hpp`
- `yggr/segment/segment_error.hpp`
- `yggr/file_system/local_fsys_operators.hpp`
- `boost/interprocess/managed_mapped_file.hpp`
