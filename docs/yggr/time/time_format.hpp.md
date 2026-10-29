# `time_format.hpp` 中文说明

## 用途

本头文件定义时间格式字符串的默认值和查询工具，位于 `yggr::time` 命名空间。格式转义符遵循 Boost.DateTime `time_facet` 的格式规则。

```cpp
#include <yggr/time/time_format.hpp>
```

## 默认格式

### 默认紧凑格式

```cpp
time_format::tm_fmt_default()
```

返回格式字符串：

```text
%Y%m%d%H%M%S
```

即年、月、日、时、分、秒连续输出，不含日期或时间分隔符。

### ISO 8601 UTC 格式

```cpp
time_format::tm_fmt_iso8601()
```

返回格式字符串：

```text
%Y-%m-%dT%H:%M:%S%F%Q
```

包含日期和时间分隔符，并使用 Boost.DateTime 格式标记 `%F`、`%Q` 表示相应的时区/偏移信息。具体输出细节遵循 Boost.DateTime `time_facet` 的定义。

上述默认格式均提供不带模板参数的 `char` 重载，以及 `template<typename Char>` 重载，可取得 `wchar_t`、`c8`、`c16`、`c32` 等字符类型对应的字符串形式。相关字符类型受 `YGGR_NO_CWCHAR`、`YGGR_NO_CHAR8_T`、`YGGR_NO_CHAR16_T` 和 `YGGR_NO_CHAR32_T` 配置影响。

## 自定义格式类型

`time_format` 提供两个 Boost.MPL 字符串类型别名：

| 别名 | 格式 |
| --- | --- |
| `tm_utc_fmt_iso8601_type` | `%Y-%m-%dT%H:%M:%S%F%Q` |
| `tm_fmt_default_type` | `%Y%m%d%H%M%S` |

通过 `tm_fmt<T>()` 可将 Boost.MPL 字符串类型转换为 `const char*`：

```cpp
template<typename T>
static const char* tm_fmt();
```

其中 `T` 应为适用的 `boost::mpl::string` 格式类型。

## 格式字符串包装宏

`YGGR_PP_TIME_FORMAT_STRING(FormatType)` 展开为：

```cpp
boost::mpl::c_str<FormatType>::value
```

用于从 Boost.MPL 字符串类型取得其 `const char*` 格式内容。

## 使用示例

```cpp
const char* compact =
    yggr::time::time_format::tm_fmt_default();

const char* iso8601 =
    yggr::time::time_format::tm_fmt_iso8601();

const wchar_t* wide_compact =
    yggr::time::time_format::tm_fmt_default<wchar_t>();
```

## 注意事项

- 格式标记不是 `strftime` 语法的完整替代；应按 Boost.DateTime `time_facet` 的规则解释。
- ISO 8601 默认格式中的 `%F` 和 `%Q` 具体表现依赖 Boost.DateTime 实现及时间对象所含的时区信息。
- `detail` 命名空间内的字符串包装结构是实现细节；应用代码优先使用 `time_format` 的公开静态函数。
