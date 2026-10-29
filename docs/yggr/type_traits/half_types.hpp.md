# `half_types.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::half_t<T>`，尝试将数值类型 `T` 映射为同类别、约半数大小的 Yggdrasil 数值类型。

```cpp
#include <yggr/type_traits/half_types.hpp>
```

## 类型映射规则

`half_t<T>::type` 根据 `T` 的类别和 `sizeof(T)` 选择目标类型：

- 浮点类型映射到 `bytes_to_float<sizeof(T) / 2>::type`。
- 有符号类型映射到 `bytes_to_signed<sizeof(T) / 2>::type`。
- 无符号类型映射到 `bytes_to_unsigned<sizeof(T) / 2>::type`。

这些映射依赖 `bytes_to_types.hpp` 支持的字节宽度。按当前映射，常见结果包括：

| 源类型宽度 | 类别 | 目标类型 |
| ---: | --- | --- |
| 2 字节 | 有符号整数 | `s8` |
| 2 字节 | 无符号整数 | `u8` |
| 4 字节 | 有符号整数 | `s16` |
| 4 字节 | 无符号整数 | `u16` |
| 8 字节 | 有符号整数 | `s32` |
| 8 字节 | 无符号整数 | `u32` |
| 8 字节 | 浮点数 | `f32` |
| 16 字节 | 浮点数 | `f64` |

## 示例

```cpp
#include <yggr/type_traits/half_types.hpp>

typedef yggr::half_t<int64_t>::type half_integer;
```

如果 `int64_t` 为 8 字节有符号整数，`half_integer` 对应 `s32`。

## 自定义特化

头文件提供 `YGGR_PP_HALF_T_DEF(source_type, target_type)` 宏，用于为特定源类型显式定义 `half_t` 特化及其 `type`。例如，项目可为某个自定义数值类型指定合适的目标类型。

## 注意事项

- “half”表示按 `sizeof(T) / 2` 选择目标存储宽度的类型映射，不会执行数值转换，也不保证数值范围、精度或舍入语义。
- 目标类型必须在 `bytes_to_types.hpp` 中存在对应映射；不支持的类别或宽度不会产生有效的嵌套 `type`。
- 例如，常见的 4 字节浮点类型会查询 2 字节浮点映射，而当前 `bytes_to_types.hpp` 未定义该映射；如需此类支持，应显式提供特化或扩展映射。
- `sizeof(T) / 2` 对奇数字节大小会向下取整；此特征主要面向具有预期固定宽度的数值类型。
