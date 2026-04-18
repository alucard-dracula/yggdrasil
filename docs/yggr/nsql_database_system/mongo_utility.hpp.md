# mongo_utility.hpp

## 概述

`mongo_utility.hpp` 是 Yggdrasil 数据库系统中 MongoDB 工具类的头文件。该文件定义了 `mongo_utility` 类，提供了一系列实用的 MongoDB 操作辅助函数。

该类主要提供 BSON 键到索引字符串的转换功能，这是 MongoDB 索引操作中常用的工具函数。通过模板化和类型安全的接口，为不同类型的字符串和分配器提供支持。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    class mongo_utility;
}
}
```

## 类定义

### mongo_utility

```cpp
class mongo_utility
{
public:
    typedef c_bson bson_type;

    // 静态方法...
};
```

该类主要提供静态工具方法，不需要实例化。

## 主要功能

### BSON 键到索引字符串转换

#### keys_to_index_string (返回 UTF-8 字符串)

```cpp
template<typename Bson> inline
static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, utf8_string>::type
keys_to_index_string(const Bson& keys);
```

将 BSON 文档的键转换为索引字符串格式。

**参数：**
- `keys`: BSON 文档，包含索引键定义

**返回值：** `utf8_string` - 索引字符串

```cpp
template<typename Bson> inline
static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, utf8_string>::type
keys_to_index_string(const Bson& keys, const string& /*ret_charset_name*/);
```

带字符集名称参数的版本（实际未使用字符集参数）。

**参数：**
- `keys`: BSON 文档，包含索引键定义
- `ret_charset_name`: 返回字符串的字符集名称（未使用）

**返回值：** `utf8_string` - 索引字符串

#### keys_to_index_string (带分配器)

```cpp
template<typename Bson, typename Alloc> inline
static
typename
    boost::enable_if
    <
        boost::is_base_of<bson_t, Bson>,
        charset::utf8_string_impl<charset::utf8_string::value_type, charset::utf8_string::traits_type, Alloc>
    >::type
keys_to_index_string(const Bson& keys, const Alloc& alloc);
```

使用自定义分配器的版本。

**参数：**
- `keys`: BSON 文档，包含索引键定义
- `alloc`: 自定义分配器

**返回值：** 使用指定分配器的 UTF-8 字符串

```cpp
template<typename Bson, typename Alloc> inline
static
typename
    boost::enable_if
    <
        boost::is_base_of<bson_t, Bson>,
        charset::utf8_string_impl<charset::utf8_string::value_type, charset::utf8_string::traits_type, Alloc>
    >::type
keys_to_index_string(const Bson& keys, const Alloc& alloc, const string& /*ret_charset_name*/);
```

带分配器和字符集名称的版本。

#### keys_to_index_string (通用字符串类型)

```cpp
template<typename String, typename Bson> inline
static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, String>::type
keys_to_index_string(const Bson& keys);
```

转换为任意字符串类型的版本。

**模板参数：**
- `String`: 目标字符串类型

**参数：**
- `keys`: BSON 文档，包含索引键定义

**返回值：** 指定类型的字符串

```cpp
template<typename String, typename Bson> inline
static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, String>::type
keys_to_index_string(const Bson& keys, const string& ret_charset_name);
```

带字符集转换的版本。

**参数：**
- `keys`: BSON 文档，包含索引键定义
- `ret_charset_name`: 返回字符串的字符集名称

**返回值：** 指定类型和字符集的字符串

```cpp
template<typename String, typename Bson, typename Alloc> inline
static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, String>::type
keys_to_index_string(const Bson& keys, const Alloc& alloc);
```

带分配器的通用字符串版本。

```cpp
template<typename String, typename Bson, typename Alloc> inline
static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, String>::type
keys_to_index_string(const Bson& keys, const Alloc& alloc, const string& ret_charset_name);
```

完整的通用字符串版本，支持分配器和字符集转换。

## 内部实现

### 保护方法

#### pro_s_key_to_index_string

```cpp
template<typename Utf8String> inline
static Utf8String& pro_s_key_to_index_string(Utf8String& ret, const bson_t& keys);
```

内部辅助方法，执行实际的键到索引字符串转换。

**参数：**
- `ret`: 输出字符串（引用）
- `keys`: BSON 文档

**返回值：** 输出字符串的引用

该方法调用 MongoDB C 驱动的 `mongoc_collection_keys_to_index_string` 函数，并使用 `bson_free` 释放内存。

## 设计理念

### 类型安全
- **SFINAE**: 使用 `boost::enable_if` 和 `boost::is_base_of` 确保参数是有效的 BSON 类型
- **模板化**: 支持多种字符串类型和分配器
- **编译时检查**: 模板实例化时进行类型验证

### 内存管理
- **自动释放**: 使用 `bson_free` 自动释放 MongoDB 分配的内存
- **分配器支持**: 支持自定义内存分配器
- **RAII**: 确保资源正确释放

### 字符集处理
- **UTF-8优先**: 默认使用 UTF-8 字符串
- **字符集转换**: 支持转换为其他字符集
- **灵活配置**: 可通过参数指定字符集

## 依赖项

- `yggr/base/yggrdef.h` - 基础定义
- `yggr/nsql_database_system/c_bson.hpp` - BSON 封装
- `yggr/nsql_database_system/mongodb_config.hpp` - MongoDB 配置
- `yggr/charset/utf8_string.hpp` - UTF-8 字符串
- `boost/utility/enable_if.hpp` - SFINAE 支持
- `boost/type_traits/is_base_of.hpp` - 类型特征

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongo_utility.hpp>

// 创建索引键定义
bson_t keys = BSON_INITIALIZER;
BSON_APPEND_INT32(&keys, "name", 1);
BSON_APPEND_INT32(&keys, "age", -1);

// 转换为 UTF-8 索引字符串
utf8_string index_str = mongo_utility::keys_to_index_string(keys);
std::cout << "Index string: " << index_str << std::endl;
// 输出: name_1_age_-1

// 使用自定义分配器
std::allocator<char> alloc;
auto index_str_alloc = mongo_utility::keys_to_index_string(keys, alloc);

// 转换为 std::string
std::string std_index_str = mongo_utility::keys_to_index_string<std::string>(keys);

// 带字符集转换
std::string gb2312_str = mongo_utility::keys_to_index_string<std::string>(keys, "gb2312");

// 使用自定义分配器的 std::string
std::allocator<char> str_alloc;
std::string custom_alloc_str = mongo_utility::keys_to_index_string<std::string>(keys, str_alloc);

bson_destroy(&keys);
```

## 注意事项

1. **BSON参数**: 所有方法都要求参数是 `bson_t` 或其派生类型
2. **内存管理**: MongoDB 分配的内存会自动释放，无需手动管理
3. **字符集**: 默认使用 UTF-8，可通过参数指定其他字符集
4. **分配器**: 支持自定义分配器，用于特殊内存管理需求
5. **类型安全**: 模板约束确保只接受有效的 BSON 类型

## 许可证

该文件遵循项目的许可证协议，基于 MIT 许可证。