# `time_basic_foo.hpp` 中文说明

## 用途

本头文件提供 `std::time_t` 与 `std::tm` 之间的基础时间换算函数，覆盖 UTC、calendar 和 local 三种时间表示之间的转换。函数位于 `yggr::time` 命名空间。

```cpp
#include <yggr/time/time_basic_foo.hpp>
```

此处的 `calendar` 是库中单独定义的一种时间表示；具体相对 UTC 和 local 的差值由对应的时间差函数计算，不应仅凭名称将其理解为某个平台特定时区。

## 时间差函数

头文件导出以下底层差值生成函数，并提供缓存包装：

| 函数 | 含义 |
| --- | --- |
| `gen_utc_calendar_time_diff()` | 生成 UTC 与 calendar 表示之间的差值。 |
| `gen_utc_local_time_diff()` | 生成 UTC 与 local 表示之间的差值。 |
| `gen_local_calendar_time_diff()` | 生成 local 与 calendar 表示之间的差值。 |

对应的 `utc_calendar_time_diff()`、`utc_local_time_diff()` 和 `local_calendar_time_diff()` 默认将计算结果缓存为函数内静态常量。定义 `YGGR_TIME_CONSTANT_RECALCULATE_EVERY_TIME` 后，每次调用都会重新计算。

## `time_t` 表示之间的换算

以下函数直接对 `std::time_t` 值进行表示转换：

| 函数 | 转换 |
| --- | --- |
| `utc_time_to_calendar_time(t)` | UTC → calendar |
| `utc_time_to_local_time(t)` | UTC → local |
| `calendar_time_to_utc_time(t)` | calendar → UTC |
| `calendar_time_to_local_time(t)` | calendar → local |
| `local_time_to_utc_time(t)` | local → UTC |
| `local_time_to_calendar_time(t)` | local → calendar |

实现通过对应时间差进行加减。发生上溢或下溢时，调试构建会触发断言；断言关闭时返回 `static_cast<std::time_t>(-1)`。

## `time_t` 转换为 `std::tm`

提供 UTC、calendar 和 local 三种表示之间的 `time_t` 到日历分解时间转换，例如：

```cpp
std::tm* utc_time_to_utc_tm(std::time_t value, std::tm* result);
std::tm* utc_time_to_utc_tm(std::time_t value, std::tm& result);
```

同类接口还包括：

- `utc_time_to_calendar_tm`、`utc_time_to_local_tm`
- `calendar_time_to_utc_tm`、`calendar_time_to_calendar_tm`、`calendar_time_to_local_tm`
- `local_time_to_utc_tm`、`local_time_to_calendar_tm`、`local_time_to_local_tm`

每组通常提供指针和引用两种输出参数形式。指针形式在 `result` 为空时返回空指针，并在调试构建中触发断言；引用形式将其地址传给指针版本。成功时返回写入结果的 `std::tm*`。

## `std::tm` 转换为 `time_t`

下列接口接受 `const std::tm*` 或 `const std::tm&`，并返回目标表示下的 `std::time_t`：

- `utc_tm_to_utc_time`、`utc_tm_to_calendar_time`、`utc_tm_to_local_time`
- `calendar_tm_to_utc_time`、`calendar_tm_to_calendar_time`、`calendar_tm_to_local_time`
- `local_tm_to_utc_time`、`local_tm_to_calendar_time`、`local_tm_to_local_time`

指针参数为空时返回 `static_cast<std::time_t>(-1)`；引用重载将引用地址转交给指针重载。非空输入先由 `posix_time_converter::tm_to_time_t()` 转换，再按目标表示应用相应时间差。

## `std::tm` 表示之间的转换

以下接口将源 `std::tm` 转换为目标表示的 `std::tm`，并提供指针输入/输出和引用输入/输出形式：

- `utc_tm_to_calendar_tm`
- `utc_tm_to_local_tm`
- `calendar_tm_to_utc_tm`
- `calendar_tm_to_local_tm`
- `local_tm_to_utc_tm`
- `local_tm_to_calendar_tm`

指针形式在源指针或结果指针为空时返回空指针；结果指针为空也会触发断言。引用重载将参数地址转发给指针重载。

**实现核对提示：**当前 `local_tm_to_utc_tm()` 的实现将源 `std::tm` 转为 `time_t` 后调用 `calendar_time_to_utc_time()`，而不是 `local_time_to_utc_time()`。若该接口用于 local→UTC 转换，应核对这是否为预期行为。

## 使用示例

```cpp
std::time_t utc_value = /* UTC time_t */;
std::tm local_parts;

std::tm* result =
    yggr::time::utc_time_to_local_tm(utc_value, local_parts);

if(result)
{
    // 使用 local_parts 中的分解时间字段
}
```

转换函数会写入调用方提供的 `std::tm` 对象，不负责分配输出对象。

## 注意事项

- 头文件使用 `std::time_t` 和 `std::tm`；具体历法、时区及平台语义受 C 运行库和本库差值计算实现影响。
- 指针输出接口的结果指针必须有效；输入 `std::tm*` 为空的 `tm_to_time` 接口以 `-1` 表示失败。
- `std::time_t(-1)` 同时作为错误哨兵返回；调用方应根据使用场景检查它。
- 在 MSVC 下，本头文件临时禁用警告 4996，并在文件末尾恢复警告状态。
