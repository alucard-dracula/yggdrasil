# `xtime_basic_foo.hpp` 中文说明

## 用途

本头文件为 `boost::xtime` 提供时间值规范化、秒数转换、比较和算术运算的基础函数。函数位于 `yggr::time` 命名空间，是 `yggr::time::time` 等上层时间类型的底层实现。

```cpp
#include <yggr/time/xtime_basic_foo.hpp>
```

`boost::xtime` 由 `sec` 和 `nsec` 两个字段表示时间。本文件以每秒 `1,000,000,000` 纳秒为单位进行转换和进位处理。

## 时间类型筛选

`yggr::time::detail` 中定义了三个用于 SFINAE 的类型特征：

- `is_floating_point_time_second<T>`：判断 `T` 是否为浮点类型。
- `is_integer_time_second<T>`：判断 `T` 是否不是浮点类型，且不是从 `boost::xtime` 派生的类型。
- `is_time_second<T>`：上述浮点或整数秒类型的组合，用于选择按秒数处理的模板重载。

这些特征用于限制接受秒数的模板函数；直接处理两个 `boost::xtime` 值的重载单独提供。

## 秒/纳秒表示规范

以下函数用于检查和整理 `sec`、`nsec`：

| 函数 | 说明 |
| --- | --- |
| `xtime_decomposable(t)` | 检查秒和纳秒符号相反、且纳秒绝对值小于 1 秒的可分解状态。 |
| `xtime_decompose(t)` | 将异号的秒/纳秒调整为一致的表示；例如秒为负而纳秒为正时，秒加 1、纳秒减去 1 秒对应的纳秒数。 |
| `xtime_formatable(t)` | 检查纳秒是否超出 `(-1 秒, 1 秒)` 范围。 |
| `xtime_format(t)` | 将超出范围的纳秒进位到秒字段，并将纳秒归一化到秒范围内。 |
| `xtime_validate(t)` | 检查纳秒范围以及秒/纳秒符号组合是否符合本文件的规范。 |

`xtime_decompose()` 和 `xtime_format()` 都会原地修改输入，并返回该对象的引用；调用方应在需要时通过 `xtime_validate()` 检查结果。

## 秒数构造与转换

```cpp
template<typename T>
boost::xtime& xtime_from_second(boost::xtime& out, const T& seconds);

template<typename T>
T xtime_to_second(const boost::xtime& value);
```

- 整数秒构造会把秒写入 `sec`，并将 `nsec` 设为零；赋值窄化到 `xtime_sec_t` 后通过断言检查是否保持原值。
- 浮点秒构造将整数部分写入 `sec`，并把小数部分换算为纳秒。
- `xtime_to_second<T>()` 将秒与纳秒合并转换为 `T`。转换前通过断言要求输入满足 `xtime_validate()`。

```cpp
boost::xtime value = {0, 0};
yggr::time::xtime_from_second(value, 1.25);
double seconds = yggr::time::xtime_to_second<double>(value);
```

## 比较

```cpp
s32 xtime_compare(const boost::xtime& left, const boost::xtime& right);
bool xtime_compare_eq(const boost::xtime& left, const boost::xtime& right);
```

比较先比较 `sec`，秒相同时再比较 `nsec`。`xtime_compare()` 返回负数、零或正数；`xtime_compare_eq()` 判断两个字段是否都相等。另有模板重载，可将整数或浮点秒数转换为临时时间值后与 `boost::xtime` 比较。

## 算术函数

本文件提供加、减、乘、除和取模操作。以 `_set` 结尾的函数原地修改左操作数并返回其引用；操作数可为整数秒、浮点秒或另一个 `boost::xtime`，具体支持形式由各函数重载决定。

| 函数族 | 行为 |
| --- | --- |
| `xtime_plus_set(left, right)` | 将秒数或时间值加到 `left`。 |
| `xtime_minus_set(left, right)` | 从 `left` 减去秒数或时间值。 |
| `xtime_multiplies_set(left, right)` | 将时间值乘以秒数或另一个时间值。 |
| `xtime_divides_set(left, right)` | 将时间值除以秒数或另一个时间值，并将商写回 `left`。 |
| `xtime_modulus_set(left, right)` | 对时间值执行取模，并将余数写回 `left`。 |
| `xtime_divides<T>(left, right)` | 将两个 `boost::xtime` 的比值转换为指定数值类型 `T`；不同于 `_set` 形式，它返回数值商。 |

整数加减会调整秒字段，并通过 `xtime_decompose()` 处理异号的秒/纳秒。时间值之间的加减会对纳秒字段进行进位或借位。乘法、除法和取模在部分非整数或含纳秒的路径中会先转换为 `f64` 秒数，再转换回 `boost::xtime`。

## 前置条件与精度

- 算术实现使用断言检查时间值有效性；除法和取模还要求除数非零。发布构建中若断言被禁用，调用方仍必须自行保证这些前置条件。
- 输入应保持有效的规范化表示；可先调用 `xtime_validate()` 检查。
- 浮点秒和包含纳秒的复杂乘除/取模路径可能经过 `f64`，结果会受浮点精度限制。
- `xtime_sec_t`、`xtime_nsec_t` 及中间计算类型存在范围限制。处理极大时间值或乘法结果时，应防止溢出。
- 本文件定义的函数修改传入的 `boost::xtime` 对象；需要保留原始值时，应先复制再执行 `_set` 运算。
