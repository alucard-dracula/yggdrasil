# `segment_file_pos.hpp`

## 概述

该头文件定义了 `yggr::segment::segment_file_pos`，用于统一提供文件定位操作所需的常量。

这些常量直接映射自 Boost.Interprocess 的文件位置常量，通常作为文件读写或映射相关接口的定位基准使用。

## 头文件

```cpp
#include <yggr/segment/segment_file_pos.hpp>
```

## 命名空间

```cpp
yggr::segment
```

## 类声明

```cpp
class segment_file_pos;
```

该类没有构造函数、数据成员或非静态成员函数，主要作为常量命名空间使用。

## 常量

### `E_file_begin`

```cpp
YGGR_STATIC_CONSTANT(u32, E_file_begin = boost::interprocess::file_begin);
```

表示文件开头位置。

### `E_file_end`

```cpp
YGGR_STATIC_CONSTANT(u32, E_file_end = boost::interprocess::file_end);
```

表示文件结尾位置。

### `E_file_current`

```cpp
YGGR_STATIC_CONSTANT(u32, E_file_current = boost::interprocess::file_current);
```

表示当前位置。

## 使用示例

```cpp
#include <yggr/segment/segment_file_pos.hpp>

void use_file_positions()
{
    const yggr::u32 begin =
        yggr::segment::segment_file_pos::E_file_begin;
    const yggr::u32 end =
        yggr::segment::segment_file_pos::E_file_end;
    const yggr::u32 current =
        yggr::segment::segment_file_pos::E_file_current;

    (void)begin;
    (void)end;
    (void)current;
}
```

## 与 Boost.Interprocess 的关系

| yggdrasil 常量 | Boost.Interprocess 常量 | 含义 |
| --- | --- | --- |
| `E_file_begin` | `file_begin` | 文件开头。 |
| `E_file_end` | `file_end` | 文件结尾。 |
| `E_file_current` | `file_current` | 当前文件位置。 |

该类不重新定义文件定位逻辑；常量值和底层平台行为由 Boost.Interprocess 决定。

## 注意事项

1. `segment_file_pos` 是常量适配类，不需要也不应实例化。
2. 常量类型为 yggdrasil 的 `u32`，使用时应确保目标 API 接受兼容的定位值类型。
3. 文件位置常量的具体使用方式取决于接收它们的 Boost.Interprocess 或项目接口。
4. 该文件依赖 `segment_config.hpp`，应保持项目既有的包含顺序约定。

## 相关组件

- `yggr/segment/segment_config.hpp`
- `boost/interprocess/detail/os_file_functions.hpp`
