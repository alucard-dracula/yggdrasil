# `task_data_id_def.hpp` 中文说明

## 用途

`yggr::task_center::task_data_id_def` 定义任务数据使用的内置类型 ID 常量。每个常量均为 `u32`，以 `E_data_id_start` 为基值，用于区分字符、整数、浮点数及字符类型数据的 ID。

```cpp
#include <yggr/task_center/task_data_id_def.hpp>
```

## 常量列表

| 常量 | 数值 | 表示的数据类型 |
| --- | ---: | --- |
| `E_data_id_start` | `0x00008000` | 内置数据 ID 的起始基值。 |
| `E_data_id_char` | `0x00008001` | `char`。 |
| `E_data_id_s8` | `0x00008002` | 有符号 8 位整数。 |
| `E_data_id_u8` | `0x00008003` | 无符号 8 位整数。 |
| `E_data_id_wchar_t` | `0x00008004` | `wchar_t`。 |
| `E_data_id_s16` | `0x00008005` | 有符号 16 位整数。 |
| `E_data_id_u16` | `0x00008006` | 无符号 16 位整数。 |
| `E_data_id_s32` | `0x00008007` | 有符号 32 位整数。 |
| `E_data_id_u32` | `0x00008008` | 无符号 32 位整数。 |
| `E_data_id_s64` | `0x00008009` | 有符号 64 位整数。 |
| `E_data_id_u64` | `0x0000800A` | 无符号 64 位整数。 |
| `E_data_id_f32` | `0x0000800B` | 32 位浮点数。 |
| `E_data_id_f64` | `0x0000800C` | 64 位浮点数。 |
| `E_data_id_c8` | `0x0000800D` | 8 位字符类型。 |
| `E_data_id_c16` | `0x0000800E` | 16 位字符类型。 |
| `E_data_id_c32` | `0x0000800F` | 32 位字符类型。 |
| `E_data_id_user` | `0x00008064` | 用户自定义数据 ID 的起始值。 |

内置类型 ID 连续占用 `E_data_id_start + 1` 至 `E_data_id_start + 15`；用户自定义 ID 起点为 `E_data_id_start + 100`，与内置值段保持间隔。

## 使用示例

```cpp
const yggr::u32 type_id =
    yggr::task_center::task_data_id_def::E_data_id_s32;
```

## 注意事项

- 该结构只提供静态常量，无需创建 `task_data_id_def` 对象。
- 自定义数据类型应从 `E_data_id_user` 开始规划 ID，避免与保留的内置 ID 重复。
- 此头文件只定义 ID，不包含数据序列化、校验或转换逻辑。
