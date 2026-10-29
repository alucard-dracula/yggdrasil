# `time.hpp` 中文说明

## 用途

本头文件定义 `yggr::time::time`，用于表示秒和纳秒组成的时间值。类公开继承 `boost::xtime`，可与该基类互通，并提供时间构造、算术运算、比较、格式化、日历时间转换及哈希支持。

```cpp
#include <yggr/time/time.hpp>
```

`time` 的 `sec` 和 `nsec` 分别来自 `boost::xtime`。纳秒比例为每秒 `1,000,000,000`；类中对应的秒/毫秒、秒/微秒、秒/纳秒等比例常量与 `time_ratio_def` 一致。

## 类型别名

| 别名 | 含义 |
| --- | --- |
| `time_ratio_def_type` | 时间单位换算比例定义，即 `time_ratio_def`。 |
| `time_ratio_type` | 比例常量类型，即 `time_ratio_def_type::value_type`。 |
| `system_type` | `boost::system_time`。 |
| `base_type`、`bridge_time_type` | `boost::xtime`。 |
| `xtime_sec_t`、`xtime_nsec_t` | `boost::xtime` 中秒和纳秒字段的类型。 |

`E_S_AND_MS_RATIO`、`E_S_AND_MCS_RATIO`、`E_S_AND_NS_RATIO` 及相应的 `HALF_RATIO` 常量，还有毫秒/微秒、毫秒/纳秒、微秒/纳秒比例常量，均可通过 `time::` 访问。

## 构造与赋值

```cpp
yggr::time::time();                         // 当前 UTC 时间
explicit time(T seconds);                   // 由秒数构造
time(xtime_sec_t seconds, xtime_nsec_t ns); // 由秒和纳秒构造
time(const base_type& value);
time(const time& other);
```

- 默认构造函数调用 `boost::xtime_get()` 获取当前 UTC 时间。
- 单参数构造和对应的赋值运算符接受整数或浮点秒数；浮点数的小数部分转换为纳秒。
- 双参数构造以及从 `boost::xtime` 构造时会执行 `rebuild()`，整理秒/纳秒字段。
- 支持复制、移动构造和赋值，也支持以 `boost::xtime` 为来源的构造、赋值和交换。

```cpp
yggr::time::time now;
yggr::time::time timeout(2.5); // 2.5 秒
timeout += 1;                 // 增加 1 秒
```

## 值状态与基类互通

- `bridge_time()` 返回对底层 `boost::xtime` 的可变或常量引用。
- `operator bool()` 在秒或纳秒字段任一非零时返回 `true`。
- `reset()` 将秒和纳秒均置零；`update()` 将对象更新为当前 UTC 时间。
- `decomposable()`、`decompose()`、`formatable()`、`format()` 和 `rebuild()` 用于检查或整理秒/纳秒表示。`rebuild()` 依次执行格式整理和分解。
- `validate()` 检查字段是否处于有效的规范形式。

## 算术与比较

成员函数提供 `plus` / `plus_set`、`minus` / `minus_set`、`multiplies` / `multiplies_set`、`divides` / `divides_set`、`modulus` / `modulus_set`。不带 `_set` 的版本返回计算结果而不修改原对象；带 `_set` 的版本更新当前对象并返回其引用。相关运算支持秒数、`boost::xtime` 和 `time` 类型的右操作数。

非成员运算符提供一元正负号、`+`、`-`、`*`、`/`、`%` 及其复合赋值形式；关系运算符支持 `time` 与时间秒数或基类值的比较。`compare()` 返回负数、零或正数表示小于、相等或大于；`compare_eq()` 只判断相等。

要取得两个时间值相除的浮点比例，应使用 `time_float_divides_ref()` 或 `time_float_divides_cref()` 包装操作数。未包装的 `time / time` 运算返回 `time` 类型，并非数值比例。

```cpp
yggr::time::time elapsed(3.0);
yggr::time::time half = elapsed / 2;

double ratio =
    yggr::time::time_float_divides_cref(elapsed)
    / yggr::time::time_float_divides_cref(half);
```

## 单位转换

```cpp
f64 ex_to_second() const;
template<typename T> T ex_to_second() const;

f64 ex_to_millisecond() const;
template<typename T> T ex_to_millisecond() const;

template<typename T> static T time_second();
template<typename T> static T time_millisecond();
```

- `ex_to_second()` 和 `ex_to_millisecond()` 将当前对象转换为秒或毫秒。
- 模板版本的结果精度和截断行为取决于 `T`；需要保留小数时使用浮点类型。
- 静态 `time_second<T>()` 与 `time_millisecond<T>()` 先构造当前时间，再返回相应单位的数值。

毫秒转换通过纳秒半比例补偿进行舍入。时间数值的乘除及转换应留意目标类型范围和中间计算溢出。

## 日历转换与格式化

- `to_tm()` 将时间转换为 UTC 的 `std::tm`。
- `to_local_tm()` 将 UTC 时间调整为本地时间后转换为 `std::tm`。
- `to_string()` 按 UTC 格式生成字符串；`to_local_string()` 按本地时间生成字符串。实例方法支持默认格式、显式格式、Yggdrasil 字符串类型和分配器等重载。
- 静态 `time_string()` 与 `local_time_string()` 对当前时间执行相应格式化。默认格式来自 `time_config::default_time_format`。

格式字符串由 `time_formatting.hpp` 中的 Boost.DateTime 格式化逻辑处理；指定格式和输出字符串时，字符集名称应与实际字符串类型匹配。

```cpp
yggr::time::time now;
yggr::utf8_string utc_text = now.to_string();
yggr::utf8_string local_text =
    now.to_local_string("%Y-%m-%d %H:%M:%S");
```

## 特殊值、哈希与辅助函数

| 接口 | 含义 |
| --- | --- |
| `min_time_step()` | 最小时间步长，即 1 纳秒。 |
| `max_time_step()` | 返回 `max_value()`。 |
| `zero_value()` | 零时间。 |
| `min_value()`、`max_value()` | 由秒字段类型边界和纳秒范围构成的最小、最大值。 |
| `default_precision<T>()` | 按 `sizeof(T)` 选择默认精度：小于 8 字节时为 10,000 纳秒，否则为 10 纳秒。 |
| `hash()` | 根据秒和纳秒字段计算哈希值。 |
| `abs(time)` | 返回时间值的绝对值。 |

本头文件还提供 `swap()`、`hash_value()`，并在启用 `YGGR_HAS_CXX11_STD_HASH` 时为 `std::hash<yggr::time::time>` 提供特化。

## 注意事项

- `time` 的数值算术按秒解释标量；它不是带单位的强类型时长，调用方需自行保证单位一致。
- 小数秒精度受纳秒表示和目标类型限制。对结果使用整数类型可能丢失小数部分。
- 格式化的时区语义不同：`to_string()` / `time_string()` 使用 UTC，`to_local_string()` / `local_time_string()` 使用本地时间。
- 本地时间转换依赖平台时区设置；如果需要稳定的跨时区表示，应优先使用 UTC 接口。
