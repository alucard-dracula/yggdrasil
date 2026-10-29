# `number_t.hpp` 中文说明

## 用途

本头文件提供一组编译期类型选择特征和宏，根据数值类型的类别及 `sizeof` 大小选择 Yggdrasil 定宽整数或浮点类型。还提供按最小或最大字节宽度选择类型的版本。

```cpp
#include <yggr/type_traits/number_t.hpp>
```

## 基础宽度映射

头文件定义以下标签模板，结果类型通过嵌套类型 `type` 获取：

| 标签 | 已提供的显式映射 |
| --- | --- |
| `tag_integer_t<Bytes>` | 1、2、4、8 字节分别映射到 `s8`、`s16`、`s32`、`s64` |
| `tag_uinteger_t<Bytes>` | 1、2、4、8 字节分别映射到 `u8`、`u16`、`u32`、`u64` |
| `tag_real_t<Bytes>` | 4、8 字节分别映射到 `f32`、`f64` |

未显式特化时，这三个标签模板分别以 `s64`、`u64`、`f64` 作为默认 `type`。因此，未列出的宽度不会自动报错，而会采用各自的默认类型。

## 按输入类型选择

### `tag_number_t<T>`

按 `T` 的类别和大小选择 `type`：

- 有符号类型或枚举：选择 `tag_integer_t<sizeof(T)>`；
- 无符号类型：选择 `tag_uinteger_t<sizeof(T)>`；
- 其他类型：选择 `tag_real_t<sizeof(T)>`。

### `tag_low_number_t<Low, T>`

按 `T` 的类别选择整数或浮点标签，并使用 `max(Low, sizeof(T))` 作为映射宽度。`Low` 必须为偶数；该特征的静态成员 `value` 等于传入的 `Low`。

### `tag_upper_number_t<Upper, T>`

按 `T` 的类别选择整数或浮点标签，并使用 `min(Upper, sizeof(T))` 作为映射宽度。`Upper` 必须为偶数；静态成员 `value` 等于传入的 `Upper`。

以上特征定义于 `yggr::type_traits` 命名空间，并通过 `yggr` 命名空间导出。

## 预处理器接口

头文件提供以下类型选择宏：

- `YGGR_PP_INTEGER_T_TYPE(Bytes)`、`YGGR_PP_UINTEGER_T_TYPE(Bytes)`、`YGGR_PP_REAL_T_TYPE(Bytes)`：按字节宽度选择有符号整数、无符号整数或浮点类型。
- `YGGR_PP_NUMBER_T_TYPE(T)`：根据 `T` 选择数值类型。
- `YGGR_PP_LOW_NUMBER_T_TYPE(Low, T)`：按最小宽度规则选择类型。
- `YGGR_PP_UPPER_NUMBER_T_TYPE(Upper, T)`：按最大宽度规则选择类型。
- 对应的 `_TPL` 变体会为模板上下文添加 `typename`。

此外，`YGGR_PP_INTEGER_T_DEF`、`YGGR_PP_UINTEGER_T_DEF`、`YGGR_PP_REAL_T_DEF` 和 `YGGR_PP_NUMBER_T_DEF` 用于定义或扩展映射特化。

## 示例

```cpp
#include <yggr/type_traits/number_t.hpp>

typedef yggr::tag_number_t<int>::type signed_number;
typedef yggr::tag_number_t<unsigned int>::type unsigned_number;
typedef yggr::tag_low_number_t<4, int>::type at_least_four_bytes;
typedef yggr::tag_upper_number_t<4, long long>::type at_most_four_bytes;
```

在常见平台上，`int` 会按其大小选择相应的有符号类型；实际结果取决于 `sizeof` 和本头文件中的宽度映射。

## 注意事项

- 宽度参数以字节为单位；类型选择依据 `sizeof(T)`，结果可能受平台 ABI 影响。
- 对未显式映射的宽度，标签模板会采用默认类型（`s64`、`u64` 或 `f64`）；使用者应确认该默认结果符合预期。
- `tag_number_t` 将枚举归入有符号整数标签，但实际枚举底层表示及值域由语言和枚举定义决定。
- `tag_low_number_t` 和 `tag_upper_number_t` 的 `value` 是传入的边界参数，不是最终映射所用的 `max` 或 `min` 宽度。
- 对浮点类型应用低/高宽度选择时，也会按映射后的字节宽度选择 `tag_real_t`；应确认所选浮点宽度有对应特化，否则会落到默认 `f64`。
