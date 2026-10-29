# `bytes_to_types.hpp` 中文说明

## 用途

本头文件提供按字节数选择定宽数值类型的编译期类型特征。它将字节数转换为位数（`ByteSize * 8`），再复用 `bits_to_types.hpp` 中定义的映射。

```cpp
#include <yggr/type_traits/bytes_to_types.hpp>
```

## 类型特征

```cpp
yggr::bytes_to_signed<ByteSize>::type
yggr::bytes_to_unsigned<ByteSize>::type
yggr::bytes_to_float<ByteSize>::type
```

| 字节数 | `bytes_to_signed` | `bytes_to_unsigned` | `bytes_to_float` |
| ---: | --- | --- | --- |
| 1 | `s8` | `u8` | 未定义 |
| 2 | `s16` | `u16` | 未定义 |
| 4 | `s32` | `u32` | `f32` |
| 8 | `s64` | `u64` | `f64` |

类型特征定义于 `yggr::type_traits`，并通过 `yggr` 命名空间导出。映射结果通过嵌套类型 `type` 获取。

## 示例

```cpp
#include <yggr/type_traits/bytes_to_types.hpp>

typedef yggr::bytes_to_signed<4>::type signed_type;
typedef yggr::bytes_to_unsigned<2>::type unsigned_type;
typedef yggr::bytes_to_float<8>::type floating_type;
```

上述类型分别对应 `s32`、`u16` 和 `f64`。

## 注意事项

- `ByteSize` 表示字节数；本实现通过左移 3 位换算为位数。
- 当前仅映射 1、2、4、8 字节对应的有符号、无符号整数，以及 4、8 字节浮点类型。未定义的映射没有嵌套 `type`，使用时会在模板实例化阶段失败。
- 这些映射依赖 `bits_to_types.hpp` 中的位宽特化以及 Yggdrasil 的定宽类型别名。
