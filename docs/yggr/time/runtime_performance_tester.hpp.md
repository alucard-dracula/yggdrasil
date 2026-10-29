# `runtime_performance_tester.hpp` 中文说明

## 用途

`yggr::time::runtime_performance_tester<Time, Out, OStream>` 是基于作用域生命周期的运行时间测试辅助类。构造时记录起始时间，析构时计算经过的秒数并输出“总耗时 / count”。

```cpp
#include <yggr/time/runtime_performance_tester.hpp>
```

该类也通过 `yggr::runtime_performance_tester` 名称导出。

## 模板参数

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| `Time` | 无 | 时间类型，需支持默认构造、时间相减及 `ex_to_second<Out>()` 转换。 |
| `Out` | `f64` | 输出数值类型，也是 `count` 参数的类型。 |
| `OStream` | `std::ostream` | 输出流类型，需支持写入数值和换行符。 |

该类私有继承 `nonable::noncopyable`，不能复制。

## 构造

```cpp
runtime_performance_tester(
    Out count = 1.0,
    OStream& os = std::cout);
```

- `count` 是用于归一化总耗时的除数，默认值为 `1.0`。
- `os` 是输出流引用，默认写入 `std::cout`。
- `_s_tm` 在对象构造时默认构造，用作计时起点。

## 析构时输出

对象析构时执行等价于以下计算并写入流：

```cpp
(elapsed_time_in_seconds / count) << "\n";
```

其中经过时间由 `time_type` 的当前时间与起始时间之差计算。因此输出结果的含义取决于 `Time` 默认构造和时间差运算的实现。

该类不提供显式停止或读取接口，计时范围由对象的作用域决定；离开作用域时自动输出。

## 使用示例

```cpp
#include <yggr/time/runtime_performance_tester.hpp>
#include <yggr/time/time.hpp>

void measure()
{
    yggr::runtime_performance_tester<yggr::time::time> timer;

    // 执行待测操作
}
```

示例中离开 `measure()` 作用域时，计时器会向默认输出流输出耗时秒数。

若测试多次操作的平均耗时，可将操作次数作为 `count`：

```cpp
yggr::runtime_performance_tester<yggr::time::time> timer(
    100.0,
    std::cout);
```

此时析构输出总耗时除以 `100` 的值。调用方应保证 `count` 非零且输出流的生命周期覆盖计时器。

## 注意事项

- 计时器析构函数会执行流输出；若输出流已失效或其生命周期早于计时器，行为不符合预期。
- `count` 为零时会发生除零，使用者应传入非零值。
- 该类只负责测量一个作用域的总时间并按给定次数归一化，不会自动循环执行被测操作。
