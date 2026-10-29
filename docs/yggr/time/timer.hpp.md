# `timer.hpp` 中文说明

## 用途

本头文件定义模板类 `yggr::time::timer<Time>`，用于记录一个起始时间，并计算从该时间点到当前时刻的经过时间。它不负责周期调度或后台线程执行；需要管理和调度回调任务时，可使用 `timer_mgr`。

```cpp
#include <yggr/time/timer.hpp>
```

## 模板参数

| 参数 | 含义 |
| --- | --- |
| `Time` | 时间类型，需支持默认构造、赋值、复制构造以及相减运算；当前实现还要求 `Time()` 表示可用于当前时刻的时间值。 |

`time_type` 是对模板参数 `Time` 的类型别名。

## 构造与赋值

```cpp
yggr::time::timer<Time> timer;
```

默认构造会默认构造 `_start_time`。对于 `yggr::time::time`，默认构造表示当前 UTC 时间，因此计时器从创建时开始计时。

类支持复制和移动构造、复制和移动赋值。复制或移动计时器会一并复制或移动其起始时间。

## 成员接口

| 接口 | 说明 |
| --- | --- |
| `restart()` | 将起始时间重置为新的 `time_type()`，重新开始计时。 |
| `elapsed() const` | 返回 `time_type() - _start_time`，即当前时刻与起始时间的差。 |
| `start_time() const` | 返回起始时间的常量引用。 |
| `swap(timer&)` | 与另一个计时器交换起始时间；同时提供移动引用形式的重载。 |

## 使用示例

```cpp
#include <yggr/time/time.hpp>
#include <yggr/time/timer.hpp>

yggr::time::timer<yggr::time::time> stopwatch;

// 执行需要计时的工作
yggr::time::time duration = stopwatch.elapsed();

stopwatch.restart();
```

此示例使用 `yggr::time::time` 作为时间类型。`elapsed()` 返回的仍是时间类型，可再使用其 `ex_to_second()` 或 `ex_to_millisecond()` 转换为数值。

## 交换支持

头文件为 `timer<Time>` 提供 `yggr::time::swap_support::swap()`，并将其引入 `yggr::time`、`std` 和 `boost` 命名空间，以配合相应的交换调用方式。

## 注意事项

- 计时器只保存一个起始时间，不保存停止时间，也不提供暂停、自动重启或定时回调功能。
- `elapsed()` 每次调用都会重新构造 `time_type()` 并与起始时间相减；因此 `Time` 的默认构造语义会决定“当前时刻”的含义。
- 时间类型必须支持源代码所使用的减法和复制操作。使用自定义 `Time` 时，应保证这些操作具有与预期一致的时间差语义。
