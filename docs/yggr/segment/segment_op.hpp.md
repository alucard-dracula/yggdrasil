# `segment_op.hpp`

## 概述

该头文件定义了 `yggr::segment::segment_op`，用于统一提供 Boost.Interprocess 的段创建和打开策略标签。

该类本身不执行创建或打开操作，只负责生成可传递给各类
`segment_op_proxy<Seg>::s_create()` 的策略对象。

## 头文件

```cpp
#include <yggr/segment/segment_op.hpp>
```

## 命名空间

```cpp
yggr::segment
```

## 类声明

```cpp
class segment_op;
```

`segment_op` 私有继承 `nonable::noncreateable`，不能实例化，应直接调用其静态函数。

## 类型别名

| yggdrasil 类型 | Boost.Interprocess 类型 | 用途 |
| --- | --- | --- |
| `create_only_type` | `create_only_t` | 仅创建新对象。 |
| `open_only_type` | `open_only_t` | 仅打开已有对象。 |
| `open_read_only_type` | `open_read_only_t` | 以只读方式打开对象。 |
| `open_read_private_type` | `open_read_private_t` | 以私有读取方式打开对象。 |
| `open_copy_on_write_type` | `open_copy_on_write_t` | 以写时复制方式打开对象。 |
| `open_or_create_type` | `open_or_create_t` | 已存在则打开，否则创建。 |

对应声明：

```cpp
typedef boost::interprocess::create_only_t create_only_type;
typedef boost::interprocess::open_only_t open_only_type;
typedef boost::interprocess::open_read_only_t open_read_only_type;
typedef boost::interprocess::open_read_private_t open_read_private_type;
typedef boost::interprocess::open_copy_on_write_t open_copy_on_write_type;
typedef boost::interprocess::open_or_create_t open_or_create_type;
```

## 静态工厂函数

### `create_only`

```cpp
static const create_only_type create_only(void);
```

返回 `create_only_type` 标签，要求底层对象不存在，并创建新的对象。

### `open_only`

```cpp
static const open_only_type open_only(void);
```

返回 `open_only_type` 标签，仅打开已经存在的对象。

### `open_read_only`

```cpp
static const open_read_only_type open_read_only(void);
```

返回 `open_read_only_type` 标签，以只读模式打开对象。

### `open_or_create`

```cpp
static const open_or_create_type open_or_create(void);
```

返回 `open_or_create_type` 标签。对象不存在时创建，存在时打开已有对象。

### `open_copy_on_write`

```cpp
static const open_copy_on_write_type open_copy_on_write(void);
```

返回 `open_copy_on_write_type` 标签，以写时复制模式打开对象。

## 未提供工厂函数的类型别名

虽然类中定义了：

```cpp
typedef boost::interprocess::open_read_private_t open_read_private_type;
```

但当前文件没有提供 `open_read_private()` 静态函数。如需使用该策略，应直接构造或使用底层 Boost.Interprocess 提供的对应标签方式，并确认目标代理支持该标签。

## 使用示例

### 与托管共享内存代理配合

```cpp
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

void create_or_open_memory()
{
    typedef boost::interprocess::managed_shared_memory memory_type;
    typedef yggr::segment::segment_op_proxy<memory_type> proxy_type;

    memory_type segment;
    const yggr::charset::utf8_string name("example_memory");

    proxy_type::s_create(
        segment,
        name,
        yggr::segment::segment_op::open_or_create(),
        static_cast<memory_type::size_type>(4096));
}
```

### 仅打开对象

```cpp
void open_existing_memory(
    boost::interprocess::managed_shared_memory& segment,
    const yggr::charset::utf8_string& name)
{
    typedef yggr::segment::segment_op_proxy<
        boost::interprocess::managed_shared_memory> proxy_type;

    proxy_type::s_create(
        segment,
        name,
        yggr::segment::segment_op::open_only());
}
```

## 策略选择建议

- 新建且不允许同名对象存在时使用 `create_only()`；
- 只允许打开已有对象时使用 `open_only()`；
- 需要只读访问时使用 `open_read_only()`；
- 允许对象不存在时自动创建时使用 `open_or_create()`；
- 需要进程本地修改视图时使用 `open_copy_on_write()`。

实际可用的策略取决于目标段类型对应的代理特化。例如，非命名的堆内存代理不接受这些命名打开标签。

## 注意事项

1. `segment_op` 只是策略标签的封装，不负责资源创建、打开或删除。
2. `segment_op` 不能实例化，应通过类名调用静态工厂函数。
3. 各策略的具体行为由 Boost.Interprocess 和目标 `segment_op_proxy<Seg>` 决定。
4. `open_or_create()` 打开已有对象时，传入的大小等创建参数通常不会重设已有对象。
5. 当前文件定义了 `open_read_private_type`，但没有对应的 `open_read_private()` 工厂函数。
6. 使用策略标签时，参数顺序和额外参数必须符合对应代理特化的接口约定。
7. 该文件依赖 `segment_config.hpp` 及 Boost.Interprocess 的 `creation_tags.hpp`。

## 相关组件

- `yggr/segment/segment_op_proxy.hpp`
- `yggr/segment/segment_container.hpp`
- `yggr/segment/segment_mode.hpp`
- `boost/interprocess/creation_tags.hpp`
