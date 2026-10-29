# `upper_types.hpp` 中文说明

## 用途

本头文件提供编译期“提升类型”特征，根据类型类别选择或转换为适用于更宽数值表示的类型。结果通过每个特征的嵌套类型 `type` 获取。

```cpp
#include <yggr/type_traits/upper_types.hpp>
```

## 提供的特征

| 特征 | 默认行为 |
| --- | --- |
| `upper_float<T>` | 若 `sizeof(T)` 小于 `f32`，选择 `f32`；否则按 `sizeof(T)` 选择对应浮点类型。 |
| `upper_signed<T>` | 浮点类型按 `sizeof(T)` 映射到同宽的 Yggdrasil 浮点类型；非浮点类型使用 `boost::make_signed<T>`。 |
| `upper_unsigned<T>` | 使用 `boost::make_unsigned<T>` 将整数类型转换为对应无符号类型。 |
| `upper_integer<T>` | 浮点类型按其大小映射到对应的有符号整数类型；非浮点类型保持为 `T`。 |

这些特征在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/upper_types.hpp>

typedef yggr::upper_float<float>::type at_least_float;
typedef yggr::upper_signed<unsigned int>::type signed_integer;
typedef yggr::upper_unsigned<int>::type unsigned_integer;
typedef yggr::upper_integer<double>::type integer_for_double;
```

在常见平台上，`at_least_float` 为 `f32`，`signed_integer` 和 `unsigned_integer` 由 Boost 类型特征决定；`integer_for_double` 按 `double` 的大小映射为相应的有符号整数类型。

## 自定义映射

头文件提供以下宏，为特定源类型显式指定目标类型：

- `YGGR_PP_UPPER_FLOAT_DEF(source, target)`
- `YGGR_PP_UPPER_SIGNED_DEF(source, target)`
- `YGGR_PP_UPPER_UNSIGNED_DEF(source, target)`
- `YGGR_PP_UPPER_INTEGER_DEF(source, target)`

这些宏生成对应 `upper_*` 模板的显式特化，适用于库配置或特殊类型适配。请在相关模板实例化前定义，并避免重复特化。

## 注意事项

- “upper”是由各特征实现定义的类型选择规则，并不保证一定比输入类型占用更多字节；例如 `upper_signed` 对非浮点类型仅执行 `make_signed` 转换。
- 浮点到整数的 `upper_integer` 仅选择目标类型，不执行数值转换，也不定义舍入、溢出或精度行为。
- 字节宽度映射依赖 `bytes_to_types.hpp`；未显式支持的宽度可能按其默认映射处理。
- `upper_unsigned` 和 `upper_signed` 对不适用的类型可能无法实例化，具体限制遵循 Boost.TypeTraits 的 `make_unsigned` / `make_signed`。
