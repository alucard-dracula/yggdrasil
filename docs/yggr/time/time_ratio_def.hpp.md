# `time_ratio_def.hpp` 中文说明

## 用途

本头文件定义 `yggr::time::time_ratio_def`，集中提供秒、毫秒、微秒和纳秒之间的换算比例及其一半比例。比例同时以 Boost.MPL 整数常量类型和静态常量值的形式公开，供时间计算和模板元编程使用。

```cpp
#include <yggr/time/time_ratio_def.hpp>
```

## 数值类型

```cpp
typedef yggr::s32 value_type;
```

所有比例常量均以 `s32` 表示。

## 比例类型别名

| 类型别名 | 值 | 含义 |
| --- | ---: | --- |
| `s_and_ms_ratio_type` | `1000` | 1 秒对应的毫秒数。 |
| `s_and_ms_half_ratio_type` | `500` | 上述比例的一半。 |
| `s_and_mcs_ratio_type` | `1,000,000` | 1 秒对应的微秒数。 |
| `s_and_mcs_half_ratio_type` | `500,000` | 上述比例的一半。 |
| `s_and_ns_ratio_type` | `1,000,000,000` | 1 秒对应的纳秒数。 |
| `s_and_ns_half_ratio_type` | `500,000,000` | 上述比例的一半。 |
| `ms_and_mcs_ratio_type` | `1000` | 1 毫秒对应的微秒数。 |
| `ms_and_mcs_half_ratio_type` | `500` | 上述比例的一半。 |
| `ms_and_ns_ratio_type` | `1,000,000` | 1 毫秒对应的纳秒数。 |
| `ms_and_ns_half_ratio_type` | `500,000` | 上述比例的一半。 |
| `mcs_and_ns_ratio_type` | `1000` | 1 微秒对应的纳秒数。 |
| `mcs_and_ns_half_ratio_type` | `500` | 上述比例的一半。 |

这些类型均为 `boost::mpl::integral_c<value_type, value>`，可用于编译期计算。

## 静态常量

每个比例类型都有对应的静态常量：

| 静态常量 | 对应类型别名 |
| --- | --- |
| `E_S_AND_MS_RATIO`、`E_S_AND_MS_HALF_RATIO` | `s_and_ms_ratio_type`、`s_and_ms_half_ratio_type` |
| `E_S_AND_MCS_RATIO`、`E_S_AND_MCS_HALF_RATIO` | `s_and_mcs_ratio_type`、`s_and_mcs_half_ratio_type` |
| `E_S_AND_NS_RATIO`、`E_S_AND_NS_HALF_RATIO` | `s_and_ns_ratio_type`、`s_and_ns_half_ratio_type` |
| `E_MS_AND_MCS_RATIO`、`E_MS_AND_MCS_HALF_RATIO` | `ms_and_mcs_ratio_type`、`ms_and_mcs_half_ratio_type` |
| `E_MS_AND_NS_RATIO`、`E_MS_AND_NS_HALF_RATIO` | `ms_and_ns_ratio_type`、`ms_and_ns_half_ratio_type` |
| `E_MCS_AND_NS_RATIO`、`E_MCS_AND_NS_HALF_RATIO` | `mcs_and_ns_ratio_type`、`mcs_and_ns_half_ratio_type` |

其中 `MCS` 表示微秒，`NS` 表示纳秒。命名中的 `HALF_RATIO` 表示对应比例的一半，便于实现舍入或边界计算。

## 使用示例

```cpp
yggr::s32 nanoseconds_per_second =
    yggr::time::time_ratio_def::E_S_AND_NS_RATIO;

typedef yggr::time::time_ratio_def::s_and_ms_ratio_type
    milliseconds_per_second_constant;

static_assert(milliseconds_per_second_constant::value == 1000,
              "unexpected time ratio");
```

## 注意事项

- 本头文件只定义比例常量，不执行时间转换。
- 纳秒每秒的比例为 `1,000,000,000`，虽可由 `s32` 表示，但后续乘法运算仍需留意中间结果溢出。
- 时间单位换算时应结合源/目标单位选择对应比例，避免混淆秒、毫秒、微秒和纳秒。
