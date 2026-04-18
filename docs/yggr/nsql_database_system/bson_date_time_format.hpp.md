# bson_date_time_format.hpp 文档

## 概述

`bson_date_time_format.hpp` 是 Yggdrasil 项目中的一个 C++ 头文件，提供 BSON 日期时间格式的定义和访问接口。该文件定义了标准的 ISO8601 时间格式字符串，用于 BSON 文档中的日期时间表示。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system::detail`: 内部实现细节
- `yggr::nsql_database_system`: 主要接口

## 文件描述

`bson_date_time_format.hpp` 提供了一套标准化的时间格式定义，专门用于 BSON 文档中的日期时间字段格式化。这些格式遵循 ISO8601 标准，确保了跨平台和跨语言的时间表示一致性。

## 时间格式定义

### UTC 时间格式 (无时区后缀)
- `str_bson_tm_utc_fmt_iso8601_org`: `"%Y-%m-%dT%H:%M:%S"`
- 格式: `YYYY-MM-DDTHH:MM:SS`

### UTC 时间格式 (有时区后缀)
- `str_bson_tm_utc_fmt_iso8601`: `"%Y-%m-%dT%H:%M:%S%FZ"`
- 格式: `YYYY-MM-DDTHH:MM:SS.FFFFFFZ`

### 本地时间格式
- `str_bson_tm_local_fmt_iso8601`: `"%Y-%m-%dT%H:%M:%S%F"`
- 格式: `YYYY-MM-DDTHH:MM:SS.FFFFFF`

## 类定义

### `bson_date_time_format` 结构体

这是一个静态方法结构体，提供对时间格式字符串的访问接口。

#### 静态方法

##### `tm_fmt_iso8601_utc_org()`
```cpp
YGGR_CXX14_CONSTEXPR_OR_INLINE
static const char* tm_fmt_iso8601_utc_org(void) YGGR_NOEXCEPT_OR_NOTHROW
```
- **返回值**: `const char*` - UTC 时间格式字符串 (无时区后缀)
- **说明**: 返回基本的 ISO8601 UTC 时间格式

##### `tm_fmt_iso8601_utc_org<Char>()` (模板化)
```cpp
template<typename Char> YGGR_CXX14_CONSTEXPR_OR_INLINE
static const Char* tm_fmt_iso8601_utc_org(void) YGGR_NOEXCEPT_OR_NOTHROW
```
- **模板参数**: `Char` - 字符类型
- **返回值**: `const Char*` - 指定字符类型的 UTC 时间格式字符串
- **说明**: 模板化版本，支持不同的字符类型

##### `tm_fmt_iso8601_utc()`
```cpp
YGGR_CXX14_CONSTEXPR_OR_INLINE
static const char* tm_fmt_iso8601_utc(void) YGGR_NOEXCEPT_OR_NOTHROW
```
- **返回值**: `const char*` - UTC 时间格式字符串 (有时区后缀 Z)
- **说明**: 返回带时区标识的 ISO8601 UTC 时间格式

##### `tm_fmt_iso8601_utc<Char>()` (模板化)
```cpp
template<typename Char> YGGR_CXX14_CONSTEXPR_OR_INLINE
static const Char* tm_fmt_iso8601_utc(void) YGGR_NOEXCEPT_OR_NOTHROW
```
- **模板参数**: `Char` - 字符类型
- **返回值**: `const Char*` - 指定字符类型的 UTC 时间格式字符串
- **说明**: 模板化版本，支持不同的字符类型

##### `tm_fmt_iso8601_local()`
```cpp
YGGR_CXX14_CONSTEXPR_OR_INLINE
static const char* tm_fmt_iso8601_local(void) YGGR_NOEXCEPT_OR_NOTHROW
```
- **返回值**: `const char*` - 本地时间格式字符串
- **说明**: 返回 ISO8601 本地时间格式

##### `tm_fmt_iso8601_local<Char>()` (模板化)
```cpp
template<typename Char> YGGR_CXX14_CONSTEXPR_OR_INLINE
static const Char* tm_fmt_iso8601_local(void) YGGR_NOEXCEPT_OR_NOTHROW
```
- **模板参数**: `Char` - 字符类型
- **返回值**: `const Char*` - 指定字符类型的本地时间格式字符串
- **说明**: 模板化版本，支持不同的字符类型

## 依赖项

该文件依赖于以下头文件：
- `<yggr/base/yggrdef.h>`: Yggdrasil 基础定义
- `<yggr/time/time_format.hpp>`: 时间格式工具

## 宏依赖

- `YGGR_PP_MAKE_STR_TM_FMT`: 用于创建时间格式字符串常量
- `YGGR_CXX14_CONSTEXPR_OR_INLINE`: C++14 constexpr 或 inline 修饰符
- `YGGR_CXX14_CONSTEXPR_OR_STATIC`: C++14 constexpr 或 static 修饰符
- `YGGR_NOEXCEPT_OR_NOTHROW`: noexcept 或不抛出异常修饰符

## 使用示例

```cpp
#include <yggr/nsql_database_system/bson_date_time_format.hpp>

// 获取 UTC 时间格式字符串
const char* utc_fmt = yggr::nsql_database_system::bson_date_time_format::tm_fmt_iso8601_utc();

// 获取本地时间格式字符串
const char* local_fmt = yggr::nsql_database_system::bson_date_time_format::tm_fmt_iso8601_local();

// 使用模板化版本 (例如用于宽字符)
const wchar_t* utc_fmt_w = yggr::nsql_database_system::bson_date_time_format::tm_fmt_iso8601_utc<wchar_t>();
```

## 格式说明

### ISO8601 标准
- **日期格式**: `YYYY-MM-DD`
- **时间格式**: `HH:MM:SS.FFFFFF`
- **分隔符**: `T` (日期时间分隔符)
- **时区标识**: `Z` (UTC 时区)

### 格式字段说明
- `%Y`: 4位年份
- `%m`: 2位月份 (01-12)
- `%d`: 2位日期 (01-31)
- `%H`: 24小时制小时 (00-23)
- `%M`: 分钟 (00-59)
- `%S`: 秒 (00-59)
- `%F`: 微秒 (6位小数)
- `%Z`: 时区标识 (仅 UTC 格式)

## 设计考虑

1. **标准化**: 使用 ISO8601 标准确保时间表示的一致性
2. **模板化**: 支持不同的字符类型 (char, wchar_t 等)
3. **常量表达式**: 在编译时计算格式字符串，提高运行时性能
4. **异常安全**: 所有方法都标记为 noexcept，保证不抛出异常

## 兼容性

- C++11/14/17/20 兼容
- 支持 constexpr (C++14 及以上)
- 跨平台时间格式表示