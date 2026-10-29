# `time_diff.hpp` 中文说明

## 用途

本头文件在 `yggr::time::detail` 命名空间中提供三个辅助函数，用于根据当前系统时间和 C 时间转换函数计算日历时间、UTC 时间及本地时间之间的秒数差异。

```cpp
#include <yggr/time/detail/time_diff.hpp>
```

这些函数使用 `std::time()` 获取当前时间，并通过 Boost 的线程安全 `gmtime()` / `localtime()` 包装及 `std::mktime()`、`std::difftime()` 计算结果。它们是内部实现辅助函数，不接收指定的时间参数，也不提供公开的时区对象接口。

## 函数说明

### `gen_utc_calendar_time_diff()`

```cpp
std::time_t gen_utc_calendar_time_diff();
```

获取当前时间，将其转换为 UTC 的 `std::tm`，再把该日历值交给 `std::mktime()` 按本地时间解释，最后计算当前时间戳与该结果的差值。可用于取得 UTC 日历值与本地日历解释之间的偏差。

### `gen_utc_local_time_diff()`

```cpp
std::time_t gen_utc_local_time_diff();
```

获取当前时间，分别转换为 UTC 和本地 `std::tm`，并将两者传给 `std::mktime()` 后计算结果差值。源码注释特别指出 `mktime()` 与 `gmtime()` / `localtime()` 的调用顺序以及可能共享静态结果的问题；当前实现使用 Boost 的输出缓冲区版本，并复用同一个 `std::tm` 临时对象，因此结果依赖具体调用及平台行为，使用时应谨慎核对。

### `gen_local_calendar_time_diff()`

```cpp
std::time_t gen_local_calendar_time_diff();
```

获取当前时间，将其转换为本地 `std::tm`，再用 `std::mktime()` 重新解释并与原始时间戳计算差值。

## 错误返回

三个函数遇到下列失败时都会返回 `static_cast<std::time_t>(-1)`：

- `std::time()` 无法取得当前时间；
- UTC 或本地时间转换返回空指针；
- `std::mktime()` 返回错误值。

调用方应检查 `-1`，不要把它当作有效的时差值继续使用。

## 注意事项

- 这些函数每次调用都读取当前系统时钟；计算过程跨越时刻边界时，内部时间采样之间可能存在微小时间差。
- `std::mktime()` 将输入的 `std::tm` 作为本地日历时间解释，不是通用的 UTC 转换函数。结果会受进程时区、夏令时规则和平台 C 运行库影响。
- UTC 与本地时间偏差可能随日期变化（例如夏令时），不应将一次计算结果视为全年固定的时区偏移。
- 第二个函数中对同一 `std::tm` 临时对象先后写入 UTC 与本地时间，且使用 `mktime()` 重新解释日历字段；如需准确、可移植的时区换算，应使用明确支持目标时区的接口并对目标平台进行验证。
