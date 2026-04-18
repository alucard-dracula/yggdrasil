# json_escape_string.hpp 文档

## 文件概述

`json_escape_string.hpp` 定义了 `json_escape_string` 类型，用于将 C 字符串转义为适合 JSON 输出的格式，并提供简单的内存管理和字符串访问接口。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关类型和工具。

## 类定义

### json_escape_string

`json_escape_string` 是一个不可拷贝、不可移动的辅助类型，用于在构造时将输入字符串转义为 JSON 安全字符串，并在析构时释放该字符串所占内存。

#### 类型定义

- `string_view_type`：内部返回类型为 `utf8_string_view`。

#### 构造函数

- `json_escape_string(void)`：默认构造，表示空字符串。
- `json_escape_string(const char* str, std::size_t len = -1)`：构造时调用 `bson_utf8_escape_for_json` 将 `str` 转义为 JSON 安全字符串，`len` 默认为 `-1` 表示自动计算长度。

#### 析构函数

- `~json_escape_string(void)`：释放已分配的转义字符串内存。

#### 成员函数

- `void release(void)`：释放内部字符串并将指针置为 `0`。
- `char* data(void)`：返回可变的内部 C 字符串指针。
- `const char* data(void) const`：返回只读内部 C 字符串指针。
- `const char* c_str(void) const`：返回只读 C 字符串指针。
- `bool empty(void) const`：如果内部字符串指针为空则返回 `true`。
- `std::size_t size(void) const`：返回字符串长度（字节数），使用 `charset::charset_base_foo::strlen` 计算。
- `std::size_t length(void) const`：返回 UTF-8 字符长度，使用 `charset::utf8_foo::strlen` 计算。
- `string_view_type view(void) const`：返回当前字符串的 `utf8_string_view`。
- `string_view_type str(void) const`：同 `view()`。

#### 模板字符串转换

类提供多种模板 `str` 方法，用于将内部 UTF-8 字符串转换为指定字符串类型：

- `template<typename String> String str(void) const`
- `template<typename String> String str(const string& ret_charset_name) const`
- `template<typename String, typename Alloc> String str(const Alloc& alloc) const`
- `template<typename String, typename Alloc> String str(const Alloc& alloc, const string& ret_charset_name) const`

其中带 `Alloc` 的版本仅在 `Alloc` 满足 `container_ex::is_allocator` 时启用。

## 输出支持

定义了 `operator<<` 重载，将 `json_escape_string` 以 JSON 字符串形式输出，结果包裹在双引号内，如果字符串为空则输出 `""`。

## 依赖

- `yggr/base/yggrdef.h`
- `yggr/container_ex/is_allocator.hpp`
- `yggr/nonable/nonable.hpp`
- `yggr/charset/charset_base_foo.hpp`
- `yggr/charset/utf8_foo.hpp`
- `yggr/charset/utf8_string_view.hpp`
- `yggr/nsql_database_system/bson_config.hpp`
- `yggr/nsql_database_system/bson_visit_t.hpp`
- `boost/utility/enable_if.hpp`

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。
