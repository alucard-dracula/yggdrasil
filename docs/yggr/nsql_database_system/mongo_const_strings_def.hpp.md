# mongo_const_strings_def.hpp

## 概述

`mongo_const_strings_def.hpp` 是 Yggdrasil 数据库系统中 MongoDB 常量字符串定义的头文件。该文件定义了 `mongo_const_strings_def` 结构体，集中声明了 MongoDB 操作中常用的字符串常量。

这些常量字符串在 MongoDB 操作中频繁使用，通过集中定义可以确保一致性和避免硬编码字符串的问题。该文件使用宏来生成类型安全的字符串常量定义。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    struct mongo_const_strings_def;
}
}
```

## 结构体定义

### mongo_const_strings_def

```cpp
struct mongo_const_strings_def
{
    YGGR_TMP_PP_MAKE_FUNCTION(_id, "_id")
    YGGR_TMP_PP_MAKE_FUNCTION(files_id, "files_id")
    YGGR_TMP_PP_MAKE_FUNCTION(upsert, "upsert")
};
```

该结构体使用预处理器宏来定义常量字符串。

## 常量字符串定义

### _id
MongoDB 文档的默认主键字段名。

```cpp
YGGR_TMP_PP_MAKE_FUNCTION(_id, "_id")
```

- **用途**: MongoDB 文档的唯一标识符字段
- **值**: `"_id"`

### files_id
GridFS 文件的 ID 字段名。

```cpp
YGGR_TMP_PP_MAKE_FUNCTION(files_id, "files_id")
```

- **用途**: GridFS 文件集合中的文件 ID 字段
- **值**: `"files_id"`

### upsert
更新操作的 upsert 选项字段名。

```cpp
YGGR_TMP_PP_MAKE_FUNCTION(upsert, "upsert")
```

- **用途**: 指定更新操作是否在未找到匹配文档时插入新文档
- **值**: `"upsert"`

## 类型别名

```cpp
typedef mongo_const_strings_def mongo_const_strings_def_type;
```

提供简化的类型别名用于引用该结构体。

## 宏定义

### YGGR_TMP_PP_MAKE_FUNCTION
该文件使用预处理器宏来生成字符串常量定义：

```cpp
#define YGGR_TMP_PP_MAKE_FUNCTION YGGR_TMP_PP_CONST_STRING_MAKE_FUNCTION
```

该宏在 `detail/const_string_make_function_macro.hpp` 中定义，用于生成类型安全的字符串常量。

## 设计理念

### 常量集中管理
- **避免硬编码**: 将常用的字符串常量集中定义，避免在代码中硬编码
- **一致性保证**: 确保所有地方使用相同的字符串值
- **维护便利**: 修改常量值时只需在一处更改

### 类型安全
- **宏生成**: 使用预处理器宏生成类型安全的常量定义
- **编译时检查**: 宏展开后生成编译时可检查的代码
- **命名空间隔离**: 常量定义在特定命名空间中，避免命名冲突

### MongoDB 特定常量
- **文档标识**: `_id` 字段是 MongoDB 文档的核心标识符
- **GridFS 支持**: `files_id` 用于 GridFS 文件存储
- **操作选项**: `upsert` 用于更新操作的条件插入

## 依赖项

- `yggr/nsql_database_system/detail/const_string_make_function_macro.hpp` - 字符串常量生成宏

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongo_const_strings_def.hpp>

// 使用常量字符串
mongo_const_strings_def_type const_strings;

// 在查询中使用 _id 字段
bson_t query = BSON_INITIALIZER;
BSON_APPEND_OID(&query, const_strings._id(), &oid);

// 在更新选项中使用 upsert
bson_t update_opts = BSON_INITIALIZER;
BSON_APPEND_BOOL(&update_opts, const_strings.upsert(), true);

// GridFS 文件查询
bson_t gridfs_query = BSON_INITIALIZER;
BSON_APPEND_OID(&gridfs_query, const_strings.files_id(), &file_id);
```

## 注意事项

1. **宏依赖**: 该文件依赖于 `const_string_make_function_macro.hpp` 中的宏定义
2. **命名空间**: 常量在 `yggr::nsql_database_system` 命名空间中定义
3. **使用方式**: 通过结构体实例访问常量字符串
4. **MongoDB 特定**: 这些常量专为 MongoDB 操作设计
5. **扩展性**: 可以轻松添加新的常量字符串定义

## 许可证

该文件遵循项目的许可证协议，基于 MIT 许可证。