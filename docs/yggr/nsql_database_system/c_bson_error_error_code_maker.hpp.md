# c_bson_error_error_code_maker.hpp 文档

## 概述

`c_bson_error_error_code_maker.hpp` 是 Yggdrasil 项目中的一个 C++ 头文件，提供 BSON 错误的错误码生成器。该文件定义了模板特化，用于将 BSON 错误转换为 Yggdrasil 框架的标准错误码格式。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::exception`: 异常处理命名空间

## 文件描述

`c_bson_error_error_code_maker.hpp` 提供了 BSON 错误到 Yggdrasil 错误码的转换功能。通过模板特化 `error_code_maker`，可以将原始的 `bson_error_t` 和包装的 `c_bson_error` 类型转换为统一的错误码表示，便于在框架中进行错误处理和日志记录。

## 模板特化

### `error_code_maker<bson_error_t>` 特化

```cpp
template<>
struct error_code_maker<bson_error_t>
{
private:
    typedef yggr::exception::error_code error_code_type;

public:
    typedef bson_error_t type;

    inline error_code_type operator()(const type& err) const;
};
```

#### 方法

##### `operator()`
```cpp
inline error_code_type operator()(const type& err) const
```
- **参数**: `err` - BSON 错误对象
- **返回值**: `error_code_type` - Yggdrasil 错误码
- **说明**: 将 `bson_error_t` 转换为错误码
- **格式化**: `"domain = {domain} {message}"`

### `error_code_maker<c_bson_error>` 特化

```cpp
template<>
struct error_code_maker<yggr::nsql_database_system::c_bson_error>
{
private:
    typedef yggr::exception::error_code error_code_type;

public:
    typedef nsql_database_system::c_bson_error type;

    inline error_code_type operator()(const type& err) const;
};
```

#### 方法

##### `operator()`
```cpp
inline error_code_type operator()(const type& err) const
```
- **参数**: `err` - 包装的 BSON 错误对象
- **返回值**: `error_code_type` - Yggdrasil 错误码
- **说明**: 将 `c_bson_error` 转换为错误码
- **格式化**: `"domain = {domain()} {what()}"`

## 依赖项

该文件依赖于以下头文件：
- `<yggr/exception/error_code_maker.hpp>`: 错误码生成器基类
- `<sstream>`: 字符串流，用于错误信息格式化

## 类型定义

### `bson_error_t`
原始 BSON 错误类型，包含：
- `domain`: 错误域
- `code`: 错误码
- `message`: 错误消息

### `c_bson_error`
Yggdrasil 包装的 BSON 错误类型，提供方法：
- `domain()`: 获取错误域
- `code()`: 获取错误码
- `what()`: 获取错误消息

### `error_code_type`
Yggdrasil 标准错误码类型 `yggr::exception::error_code`

## 使用示例

```cpp
#include <yggr/nsql_database_system/c_bson_error_error_code_maker.hpp>

// 使用原始 BSON 错误
bson_error_t bson_err;
bson_err.domain = BSON_ERROR_JSON;
bson_err.code = 123;
strcpy(bson_err.message, "Invalid JSON format");

yggr::exception::error_code_maker<bson_error_t> maker1;
yggr::exception::error_code ec1 = maker1(bson_err);

// 使用包装的 BSON 错误
yggr::nsql_database_system::c_bson_error wrapped_err(bson_err);

yggr::exception::error_code_maker<yggr::nsql_database_system::c_bson_error> maker2;
yggr::exception::error_code ec2 = maker2(wrapped_err);

// 错误码现在可以用于日志记录或异常处理
std::cout << "Error: " << ec1.message() << std::endl;
```

## 错误信息格式

### 原始 BSON 错误格式
```
"domain = {domain_value} {error_message}"
```

### 包装 BSON 错误格式
```
"domain = {domain_value} {error_description}"
```

## 设计考虑

1. **模板特化**: 使用模板特化为不同 BSON 错误类型提供转换
2. **统一接口**: 所有错误类型转换为相同的 `error_code` 格式
3. **信息保留**: 保留原始错误的所有关键信息 (域、码、消息)
4. **字符串流格式化**: 使用 `stringstream` 进行安全的字符串格式化
5. **内联实现**: 方法内联以提高性能

## 错误域常量

常见的 BSON 错误域包括：
- `BSON_ERROR_JSON`: JSON 解析错误
- `BSON_ERROR_READER`: 读取器错误
- `BSON_ERROR_INVALID`: 无效参数错误
- 其他 MongoDB C 驱动定义的错误域

## 兼容性

- C++11/14/17/20 兼容
- 支持所有 BSON 错误类型
- 与 Yggdrasil 异常处理框架完全集成

## 相关组件

- `error_code_maker<>`: 通用错误码生成器模板
- `c_bson_error`: BSON 错误包装类
- `bson_error_t`: 原始 BSON 错误类型