# `time_config.hpp` 中文说明

## 用途

本头文件定义 `yggr::time::time_config`，集中提供时间格式化相关的标签、长度配置和默认时间格式查询接口，供时间类型及格式化工具使用。

```cpp
#include <yggr/time/time_config.hpp>
```

## 标签类型

| 标签 | 用途 |
| --- | --- |
| `tag_format` | 标识格式配置类别。 |
| `tag_charset` | 标识字符集配置类别。 |

这两个空结构体用于类型层面的区分，本身不保存配置数据。

## 长度常量

| 常量 | 数值 | 含义 |
| --- | ---: | --- |
| `limit_format_length` | `128` | 时间格式字符串长度限制配置。 |
| `default_string_buffer_length` | `255` | 默认字符串缓冲区长度配置。 |

常量类型均为 `std::size_t`。

## 默认时间格式

```cpp
static const char* default_time_format() noexcept;

template<typename Char>
static const Char* default_time_format() noexcept;
```

- 无模板参数的重载返回 `time_format::tm_fmt_default()` 提供的 `char` 格式字符串。
- 模板重载返回 `time_format::tm_fmt_default<Char>()` 提供的字符类型对应格式字符串。

具体格式内容由 `yggr::time::time_format` 定义；本配置类只转发查询，不创建或修改格式字符串。

## 使用示例

```cpp
const char* format =
    yggr::time::time_config::default_time_format();

const wchar_t* wide_format =
    yggr::time::time_config::default_time_format<wchar_t>();
```

`Char` 需是 `time_format::tm_fmt_default<Char>()` 支持的字符类型。
