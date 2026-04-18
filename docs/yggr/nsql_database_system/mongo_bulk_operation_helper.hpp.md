# mongo_bulk_operation_helper.hpp

## 概述

`mongo_bulk_operation_helper.hpp` 是 Yggdrasil 数据库系统中 MongoDB 批量操作辅助器的头文件。该文件定义了 `mongo_bulk_operation_helper` 类，提供了一套类型安全的静态方法来执行 MongoDB 的批量操作。

批量操作允许在单个请求中执行多个插入、更新、删除操作，提高了数据库操作的效率。该辅助器封装了 MongoDB C 驱动的批量操作 API，提供了更方便和类型安全的接口。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    class mongo_bulk_operation_helper;
}
}
```

## 类定义

### mongo_bulk_operation_helper

```cpp
class mongo_bulk_operation_helper
{
public:
    typedef c_bson bson_type;
    typedef bson_type::org_type org_bson_type;

    typedef c_bson_value bson_value_type;
    typedef bson_value_type::org_type org_bson_value_type;

    typedef c_bson_error bosn_error_type;
    typedef bosn_error_type::org_type org_bson_error_type;

    typedef c_mongo_write_concern write_concern_type;
    typedef write_concern_type::org_type org_write_concern_type;

    typedef mongoc_bulk_operation_t org_bulk_opt_type;
};
```

## 主要功能

### 1. 插入操作

#### s_op_insert
向批量操作中添加插入文档。

```cpp
template<typename BsonDoc> inline
static void s_op_insert(org_bulk_opt_type* pbulk, const BsonDoc& doc);
```

**参数：**
- `pbulk`: 批量操作对象指针
- `doc`: 要插入的 BSON 文档

#### s_op_insert (带选项)
带选项的插入操作。

```cpp
template<typename BsonDoc, typename BsonOpts> inline
static bool s_op_insert(org_bulk_opt_type* pbulk, const BsonDoc& doc,
                       const BsonOpts& opts, org_bson_error_type* perr);
```

**参数：**
- `pbulk`: 批量操作对象指针
- `doc`: 要插入的 BSON 文档
- `opts`: 操作选项
- `perr`: 错误对象指针

**返回值：** `bool` - 操作是否成功

### 2. 删除操作

#### s_op_remove
删除匹配选择器的所有文档。

```cpp
template<typename Bson> inline
static void s_op_remove(org_bulk_opt_type* pbulk, const Bson& selector);
```

#### s_op_remove_one
删除匹配选择器的第一个文档。

```cpp
template<typename Bson> inline
static void s_op_remove_one(org_bulk_opt_type* pbulk, const Bson& selector);
```

**参数：**
- `pbulk`: 批量操作对象指针
- `selector`: 删除选择器 BSON 文档

### 3. 更新操作

#### s_op_update
更新匹配选择器的所有文档。

```cpp
template<typename BsonSel, typename BsonDoc> inline
static void s_op_update(org_bulk_opt_type* pbulk,
                       const BsonSel& selector,
                       const BsonDoc& doc,
                       bool upsert);
```

#### s_op_update_one
更新匹配选择器的第一个文档。

```cpp
template<typename BsonSel, typename BsonDoc> inline
static void s_op_update_one(org_bulk_opt_type* pbulk,
                           const BsonSel& selector,
                           const BsonDoc& doc,
                           bool upsert);
```

**参数：**
- `pbulk`: 批量操作对象指针
- `selector`: 更新选择器 BSON 文档
- `doc`: 更新文档
- `upsert`: 是否在未找到匹配文档时插入新文档

### 4. 替换操作

#### s_op_replace_one
替换匹配选择器的第一个文档。

```cpp
template<typename BsonSel, typename BsonDoc> inline
static void s_op_replace_one(org_bulk_opt_type* pbulk,
                            const BsonSel& selector,
                            const BsonDoc& doc,
                            bool upsert);
```

**参数：**
- `pbulk`: 批量操作对象指针
- `selector`: 替换选择器 BSON 文档
- `doc`: 替换文档
- `upsert`: 是否在未找到匹配文档时插入新文档

### 5. 配置和查询方法

#### s_op_get_hint
获取批量操作的服务器提示。

```cpp
inline static u32 s_op_get_hint(const org_bulk_opt_type* pbulk);
```

**返回值：** `u32` - 服务器ID

#### s_op_get_write_concern
获取批量操作的写关注配置。

```cpp
inline static const write_concern_type* s_op_get_write_concern(const org_bulk_opt_type* pbulk);
```

**返回值：** 写关注对象指针

### 6. 设置方法

#### s_op_set_bypass_document_validation
设置是否绕过文档验证。

```cpp
inline static void s_op_set_bypass_document_validation(org_bulk_opt_type* pbulk, bool bypass);
```

**参数：**
- `pbulk`: 批量操作对象指针
- `bypass`: 是否绕过文档验证

#### s_op_set_client_session
设置客户端会话。

```cpp
inline static void s_op_set_client_session(org_bulk_opt_type* pbulk,
                                          mongoc_client_session_t* pclient_session);
```

#### s_op_set_comment
设置操作注释。

```cpp
template<typename BsonValue> inline
static void s_op_set_comment(org_bulk_opt_type* pbulk, const BsonValue& comment);
```

#### s_op_set_hint
设置服务器提示。

```cpp
inline static void s_op_set_hint(org_bulk_opt_type* pbulk, uint32_t server_id);
```

#### s_op_set_let
设置 let 变量（用于聚合表达式）。

```cpp
template<typename BsonLet> inline
static void s_op_set_let(org_bulk_opt_type* pbulk, const BsonLet& let);
```

## 设计理念

### 类型安全封装
- **模板参数**: 所有 BSON 相关参数都使用模板，支持不同类型的 BSON 对象
- **自动转换**: 使用 `detail::to_const_pointer` 自动转换为原始 BSON 指针
- **编译时检查**: 模板实例化时进行类型检查

### 操作分类
- **基础操作**: 插入、删除、更新、替换的基本版本
- **选项版本**: 支持额外的操作选项和错误处理
- **配置方法**: 设置批量操作的各种属性

### 错误处理
- **可选错误报告**: 某些方法接受错误对象指针进行错误报告
- **引用和指针版本**: 支持错误对象的引用和指针两种传递方式
- **断言检查**: 在调试版本中检查参数有效性

## 依赖项

- `yggr/base/yggrdef.h` - 基础定义
- `yggr/nsql_database_system/mongodb_config.hpp` - MongoDB 配置
- `yggr/nsql_database_system/c_bson.hpp` - BSON 封装
- `yggr/nsql_database_system/c_bson_value.hpp` - BSON 值封装
- `yggr/nsql_database_system/c_bson_error.hpp` - BSON 错误封装
- `yggr/nsql_database_system/c_mongo_write_concern.hpp` - 写关注封装
- `yggr/nsql_database_system/detail/to_pointer.hpp` - 指针转换工具

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongo_bulk_operation_helper.hpp>

// 假设已有批量操作对象
mongoc_bulk_operation_t* bulk_op = mongoc_collection_create_bulk_operation(collection);

// 插入文档
bson_t doc = BSON_INITIALIZER;
// ... 填充文档 ...
mongo_bulk_operation_helper::s_op_insert(bulk_op, doc);

// 更新文档
bson_t selector = BSON_INITIALIZER;
bson_t update = BSON_INITIALIZER;
// ... 填充选择器和更新文档 ...
mongo_bulk_operation_helper::s_op_update_one(bulk_op, selector, update, false);

// 删除文档
bson_t delete_selector = BSON_INITIALIZER;
// ... 填充删除选择器 ...
mongo_bulk_operation_helper::s_op_remove_one(bulk_op, delete_selector);

// 设置选项
mongo_bulk_operation_helper::s_op_set_bypass_document_validation(bulk_op, true);

// 执行批量操作
bson_error_t error;
mongoc_bulk_operation_execute(bulk_op, NULL, &error);
mongoc_bulk_operation_destroy(bulk_op);
```

## 注意事项

1. **批量操作生命周期**: 批量操作对象需要手动创建和销毁
2. **错误处理**: 带选项的方法返回布尔值表示成功或失败
3. **参数验证**: 调试版本中会断言检查参数有效性
4. **类型转换**: 自动处理不同 BSON 类型到原始类型的转换
5. **性能考虑**: 批量操作可以显著提高多个操作的性能

## 许可证

该文件遵循项目的许可证协议，基于 MIT 许可证。