# bson_date_time_native_ex.hpp 文档

## 概述

`bson_date_time_native_ex.hpp` 是 Yggdrasil 项目中的一个 C++ 头文件，提供 BSON 日期时间处理的本地扩展功能。该文件定义了 `bson_date_time_native_ex` 类，用于处理不同时间格式之间的转换、格式化和比较操作。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`: 主要接口

## 文件描述

`bson_date_time_native_ex.hpp` 提供了一套完整的 BSON 日期时间处理工具，支持多种时间格式之间的相互转换和比较。这些功能是 BSON 数据库操作中时间处理的基础设施。

## 类定义

### `bson_date_time_native_ex` 类

这是一个静态方法类，提供 BSON 日期时间的本地扩展功能。

#### 主要功能

1. **日期格式化**: 将时间戳格式化为字符串
2. **时间转换**: 不同时间格式转换为毫秒时间戳
3. **时间比较**: 各种时间格式之间的比较操作

#### 静态方法

##### 日期格式化方法

###### `s_bson_date_format()`
```cpp
static char* s_bson_date_format(const char* format, s64 msec_since_epoch, char* str, std::size_t len);
static bson_string_t* s_bson_date_format(const char* format, s64 msec_since_epoch, bson_string_t* str);
```
- **参数**:
  - `format`: 时间格式字符串
  - `msec_since_epoch`: 毫秒时间戳
  - `str`: 输出字符串缓冲区
  - `len`: 缓冲区长度 (char* 版本)
- **返回值**: 格式化后的字符串指针
- **说明**: 使用指定格式将毫秒时间戳格式化为字符串

###### `s_bson_date_format_iso8601()`
```cpp
inline static char* s_bson_date_format_iso8601(s64 msec_since_epoch, char* str, std::size_t len);
inline static bson_string_t* s_bson_date_format_iso8601(s64 msec_since_epoch, bson_string_t* str);
```
- **参数**:
  - `msec_since_epoch`: 毫秒时间戳
  - `str`: 输出字符串缓冲区
  - `len`: 缓冲区长度 (char* 版本)
- **返回值**: ISO8601 格式的字符串指针
- **说明**: 使用 ISO8601 UTC 格式格式化时间戳

##### 时间转换方法

###### `date_to_msec_since_epoch()`
```cpp
inline static s64 date_to_msec_since_epoch(s64 date_val);
```
- **参数**: `date_val` - 日期值 (毫秒)
- **返回值**: 毫秒时间戳
- **说明**: 日期值直接转换为毫秒时间戳

###### `time_to_msec_since_epoch()`
```cpp
inline static s64 time_to_msec_since_epoch(s64 time_val);
```
- **参数**: `time_val` - 时间值 (秒)
- **返回值**: 毫秒时间戳
- **说明**: 秒级时间转换为毫秒时间戳

###### `timeval_to_msec_since_epoch()`
```cpp
inline static s64 timeval_to_msec_since_epoch(s64 tv_sec, s64 tv_usec);
```
- **参数**:
  - `tv_sec`: 秒数
  - `tv_usec`: 微秒数
- **返回值**: 毫秒时间戳
- **说明**: struct timeval 格式转换为毫秒时间戳

##### 时间比较方法

该类提供了全面的时间比较功能，支持以下时间格式之间的比较：
- `date`: 毫秒时间戳
- `time`: 秒级时间戳
- `timeval`: 秒+微秒格式
- `xtime`: 秒+纳秒格式

每种比较都包含两个方法：
- `compare_eq`: 相等比较，返回 `bool`
- `compare`: 大小比较，返回 `s32` (-1, 0, 1)

###### 日期-日期比较
```cpp
inline static bool date_date_compare_eq(s64 date1, s64 date2);
inline static s32 date_date_compare(s64 date1, s64 date2);
```

###### 日期-时间比较
```cpp
inline static bool date_time_compare_eq(s64 date, s64 tm);
inline static s32 date_time_compare(s64 date, s64 tm);
```

###### 日期-timeval比较
```cpp
inline static bool date_timeval_compare_eq(s64 date, s64 tv_sec, s64 tv_usec);
inline static s32 date_timeval_compare(s64 date, s64 tv_sec, s64 tv_usec);
```

###### 日期-xtime比较
```cpp
inline static bool date_xtime_compare_eq(s64 date, s64 nsec, s64 nnssec);
inline static s32 date_xtime_compare(s64 date, s64 nsec, s64 nnssec);
```

###### 时间-日期比较
```cpp
inline static bool time_date_compare_eq(s64 tm, s64 date);
inline static s32 time_date_compare(s64 tm, s64 date);
```

###### 时间-timeval比较
```cpp
inline static bool time_timeval_compare_eq(s64 tm, s64 tv_sec, s64 tv_usec);
inline static s32 time_timeval_compare(s64 tm, s64 tv_sec, s64 tv_usec);
```

###### 时间-xtime比较
```cpp
inline static bool time_xtime_compare_eq(s64 tm, s64 nsec, s64 nnssec);
inline static s32 time_xtime_compare(s64 tm, s64 nsec, s64 nnssec);
```

###### timeval-日期比较
```cpp
inline static bool timeval_date_compare_eq(s64 tv_sec, s64 tv_usec, s64 date);
inline static s32 timeval_date_compare(s64 tv_sec, s64 tv_usec, s64 date);
```

###### timeval-时间比较
```cpp
inline static bool timeval_time_compare_eq(s64 tv_sec, s64 tv_usec, s64 tm);
inline static s32 timeval_time_compare(s64 tv_sec, s64 tv_usec, s64 tm);
```

###### timeval-xtime比较
```cpp
inline static bool timeval_xtime_compare_eq(s64 tv_sec, s64 tv_usec, s64 nsec, s64 nnssec);
inline static s32 timeval_xtime_compare(s64 tv_sec, s64 tv_usec, s64 nsec, s64 nnssec);
```

###### xtime-日期比较
```cpp
inline static bool xtime_date_compare_eq(s64 nsec, s64 nnssec, s64 date);
inline static s32 xtime_date_compare(s64 nsec, s64 nnssec, s64 date);
```

###### xtime-时间比较
```cpp
inline static bool xtime_time_compare_eq(s64 nsec, s64 nnssec, s64 tm);
inline static s32 xtime_time_compare(s64 nsec, s64 nnssec, s64 tm);
```

###### xtime-timeval比较
```cpp
inline static bool xtime_timeval_compare_eq(s64 nsec, s64 nnssec, s64 tv_sec, s64 tv_usec);
inline static s32 xtime_timeval_compare(s64 nsec, s64 nnssec, s64 tv_sec, s64 tv_usec);
```

## 依赖项

该文件依赖于以下头文件：
- `<yggr/base/yggrdef.h>`: Yggdrasil 基础定义
- `<yggr/time/time_config.hpp>`: 时间配置
- `<yggr/time/time_ratio_def.hpp>`: 时间比例定义
- `<yggr/nsql_database_system/bson_config.hpp>`: BSON 配置
- `<yggr/nsql_database_system/bson_date_time_format.hpp>`: BSON 日期时间格式
- `<yggr/nsql_database_system/native_ex_decl_bson.hpp>`: BSON 本地扩展声明
- `<boost/thread/xtime.hpp>`: Boost xtime 支持

## 时间格式说明

### 支持的时间格式
- **date**: 毫秒时间戳 (s64)
- **time**: 秒级时间戳 (s64)
- **timeval**: 秒 + 微秒 (s64 tv_sec, s64 tv_usec)
- **xtime**: 秒 + 纳秒 (s64 nsec, s64 nnssec)

### 时间比例常量
- `time::time_ratio_def::E_S_AND_MS_RATIO`: 秒到毫秒的比例 (1000)
- `time::time_ratio_def::E_MS_AND_MCS_RATIO`: 毫秒到微秒的比例 (1000)
- `time::time_ratio_def::E_MS_AND_NS_RATIO`: 毫秒到纳秒的比例 (1000000)

## 使用示例

```cpp
#include <yggr/nsql_database_system/bson_date_time_native_ex.hpp>

// 格式化时间戳为 ISO8601 字符串
char buffer[64];
s64 timestamp = 1640995200000LL; // 2022-01-01 00:00:00 UTC
bson_date_time_native_ex::s_bson_date_format_iso8601(timestamp, buffer, sizeof(buffer));

// 转换不同时间格式到毫秒
s64 msec1 = bson_date_time_native_ex::date_to_msec_since_epoch(1640995200000LL);
s64 msec2 = bson_date_time_native_ex::time_to_msec_since_epoch(1640995200LL);
s64 msec3 = bson_date_time_native_ex::timeval_to_msec_since_epoch(1640995200LL, 0);

// 比较不同时间格式
bool equal = bson_date_time_native_ex::date_time_compare_eq(msec1, 1640995200LL);
s32 cmp = bson_date_time_native_ex::timeval_xtime_compare(1640995200LL, 0, 1640995200LL, 0);
```

## 设计考虑

1. **静态方法设计**: 所有方法都是静态的，无需实例化
2. **类型安全**: 使用强类型 s64 避免隐式转换错误
3. **性能优化**: 内联函数减少函数调用开销
4. **完整性**: 支持所有常见时间格式之间的转换和比较
5. **对称性**: 比较操作支持正反两个方向

## 异常安全

所有方法都设计为异常安全的，不会抛出异常。比较操作使用算术运算确保确定性行为。

## 兼容性

- C++11/14/17/20 兼容
- 支持 Boost.Thread xtime
- 跨平台时间处理