# `segment_mode.hpp`

## 概述

该头文件定义了 `yggr::segment::segment_mode`，用于统一提供段、文件映射和共享内存的访问模式常量。

所有常量均直接映射到 Boost.Interprocess 对应的模式值，类型为 yggdrasil 的 `u32`。

## 头文件

```cpp
#include <yggr/segment/segment_mode.hpp>
```

## 命名空间

```cpp
yggr::segment
```

## 类声明

```cpp
class segment_mode;
```

该类私有继承 `nonable::noncreateable`，不能实例化，主要作为访问模式常量的命名空间使用。

## 常量

| yggdrasil 常量 | Boost.Interprocess 常量 | 含义 |
| --- | --- | --- |
| `E_read_only` | `read_only` | 只读访问。 |
| `E_read_write` | `read_write` | 可读写访问。 |
| `E_copy_on_write` | `copy_on_write` | 写时复制访问，修改不会直接写回共享底层对象。 |
| `E_read_private` | `read_private` | 私有读取模式。 |
| `E_invalid_mode` | `invalid_mode` | 无效访问模式。 |

对应声明：

```cpp
YGGR_STATIC_CONSTANT(u32, E_read_only = boost::interprocess::read_only);
YGGR_STATIC_CONSTANT(u32, E_read_write = boost::interprocess::read_write);
YGGR_STATIC_CONSTANT(u32, E_copy_on_write = boost::interprocess::copy_on_write);
YGGR_STATIC_CONSTANT(u32, E_read_private = boost::interprocess::read_private);
YGGR_STATIC_CONSTANT(u32, E_invalid_mode = boost::interprocess::invalid_mode);
```

## 使用示例

```cpp
#include <yggr/segment/segment_mode.hpp>

void choose_segment_mode()
{
    const yggr::u32 read_mode =
        yggr::segment::segment_mode::E_read_only;
    const yggr::u32 write_mode =
        yggr::segment::segment_mode::E_read_write;

    (void)read_mode;
    (void)write_mode;
}
```

在需要 Boost.Interprocess `mode_t` 的接口中，可根据接口要求进行转换：

```cpp
const boost::interprocess::mode_t mode =
    static_cast<boost::interprocess::mode_t>(
        yggr::segment::segment_mode::E_read_write);
```

## 使用建议

- 使用 `E_read_only` 打开只需要读取的映射或共享内存；
- 使用 `E_read_write` 进行读写访问；
- 使用 `E_copy_on_write` 创建进程私有的修改视图；
- 使用 `E_invalid_mode` 表示或检测无效模式，不应将其传给创建或打开接口。

具体模式限制和平台行为由 Boost.Interprocess 及底层操作系统决定。

## 注意事项

1. `segment_mode` 不能实例化，应直接通过类名访问静态常量。
2. 常量类型为 `u32`，传递给 Boost.Interprocess 接口时应按目标参数类型转换。
3. `E_copy_on_write`、`E_read_private` 的实际语义取决于底层映射对象和平台支持。
4. `E_invalid_mode` 不是有效的访问模式，不能用于正常的段创建或打开操作。
5. 该文件依赖 `segment_config.hpp` 和 Boost.Interprocess 的操作系统文件函数定义。

## 相关组件

- `yggr/segment/segment_config.hpp`
- `boost/interprocess/detail/os_file_functions.hpp`
- `yggr/segment/file_mapping_op_proxy.hpp`
- `yggr/segment/managed_mapped_file_op_proxy.hpp`
- `yggr/segment/managed_shared_memory_op_proxy.hpp`
