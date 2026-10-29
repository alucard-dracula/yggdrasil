# `bits_to_types.hpp` 中文说明

## 用途

本头文件根据指定的位数，在编译期将位宽映射到 Yggdrasil 的定宽整数或浮点类型。提供有符号整数、无符号整数和浮点数三类类型特征。

```cpp
#include <yggr/type_traits/bits_to_types.hpp>
```

## 类型特征

```cpp
yggr::bits_to_signed<BitSize>::type
yggr::bits_to_unsigned<BitSize>::type
yggr::bits_to_float<BitSize>::type
```

可用映射如下：

| 位数 | `bits_to_signed` | `bits_to_unsigned` | `bits_to_float` |
| ---: | --- | --- | --- |
| 8 | `s8` | `u8` | 未定义 |
| 16 | `s16` | `u16` | 未定义 |
| 32 | `s32` | `u32` | `f32` |
| 64 | `s64` | `u64` | `f64` |

每种特征都在 `yggr::type_traits` 中定义，并通过 `yggr` 命名空间导出。查询结果类型使用嵌套类型 `type`。

## 示例

```cpp
#include <yggr/type_traits/bits_to_types.hpp>

typedef yggr::bits_to_signed<32>::type signed_type;
typedef yggr::bits_to_unsigned<16>::type unsigned_type;
typedef yggr::bits_to_float<64>::type floating_type;
```

上述别名分别对应 `s32`、`u16` 和 `f64`。

## 扩展映射

头文件提供以下宏，用于为对应位数特化内部映射：

- `YGGR_PP_BITS_TO_SIGNED_DEF(bits, type)`
- `YGGR_PP_BITS_TO_UNSIGNED_DEF(bits, type)`
- `YGGR_PP_BITS_TO_FLOAT_DEF(bits, type)`

这些宏面向库配置与实现扩展，会在 `yggr::type_traits::detail` 中生成模板特化。扩展时应确保位数表达式与查询时的 `BitSize` 完全一致，并避免重复定义同一位宽的特化。

## 注意事项

- 当前未定义的位宽没有嵌套 `type`；尝试使用该类型会在模板实例化时失败。
- `BitSize` 是以“位”为单位的数值，例如 32，而不是字节数 4。
- 这些映射依赖 Yggdrasil 在 `yggrdef.h` 中定义的定宽类型别名。
