# `posix_time_converter.hpp` 中文说明

## 用途

本头文件定义内部辅助结构 `yggr::time::detail::posix_time_converter`，使用 Boost.DateTime 在 `std::time_t`、`std::tm` 与 POSIX 时间表示之间进行转换。

```cpp
#include <yggr/time/detail/posix_time_converter.hpp>
```

该结构位于 `detail` 命名空间，主要供库内部的时间转换实现使用。

## 接口

### `time_t_to_tm`

```cpp
static std::tm& time_t_to_tm(std::tm& out_tm, std::time_t value);
static std::tm time_t_to_tm(std::time_t value);
```

将 `std::time_t` 转换为 `std::tm`。实现通过 `boost::posix_time::from_time_t()` 构造 Boost POSIX 时间，再使用 `boost::posix_time::to_tm()` 转换。

- 带输出参数的重载将结果写入 `out_tm`，并返回该对象的引用。
- 单参数重载内部创建并返回一个 `std::tm` 值。

### `tm_to_time_t`

```cpp
static std::time_t tm_to_time_t(const std::tm& value);
```

将 `std::tm` 转换为自 Unix epoch（1970-01-01）起经过的整秒数。实现先通过 `ptime_from_tm()` 转换为 Boost POSIX 时间，再减去 epoch，并返回 `total_seconds()`。

## 使用示例

```cpp
std::time_t raw_time = /* 已有的时间戳 */;
std::tm calendar_time =
    yggr::time::detail::posix_time_converter::time_t_to_tm(raw_time);

std::time_t restored_time =
    yggr::time::detail::posix_time_converter::tm_to_time_t(calendar_time);
```

## 注意事项

- 该辅助类型不提供时区转换接口；`time_t_to_tm()` 的具体时区语义受 Boost `from_time_t()` 的平台约定影响。
- `tm_to_time_t()` 返回整数秒，亚秒精度不会保留。
- 转换结果还受 `std::tm` 字段有效性及平台 `std::time_t` 范围约束；调用方应传入有效日历时间，并注意超出平台可表示范围的情况。
- 该头文件包含本地时间调整器头文件，但本结构中公开的两个转换函数本身没有调用本地时区调整器。
