# mongo_accesser_collection.hpp

## 文件概述

`mongo_accesser_collection.hpp` 是 Yggdrasil 数据库系统中的 MongoDB 集合访问器头文件。该文件定义了 `mongo_accesser_collection` 类，提供了一套完整的 MongoDB 集合操作接口，包括文档的插入、删除、查找、更新、聚合、索引管理等功能。

该类继承自 `basic_mongo_accesser_collection`，并通过静态模板方法提供类型安全的 MongoDB 集合操作。所有方法都支持多种字符串类型和 BSON 类型参数，并使用 Boost MPL 进行编译时类型检查。

## 许可证

该文件使用 MIT 许可证。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    // mongo_accesser_collection 类定义
} // namespace nsql_database_system
} // namespace yggr
```

## 类定义

### mongo_accesser_collection

```cpp
class mongo_accesser_collection
    : public basic_mongo_accesser_collection
{
public:
    // 类型定义和静态方法
};
```

#### 继承关系
- **基类**: `basic_mongo_accesser_collection`

#### 模板参数说明
该类主要通过静态模板方法提供功能，不需要实例化。主要使用模板参数进行类型推导和编译时检查。

## 主要功能

### 1. 文档插入操作

#### s_collection_insert_many
批量插入多个文档到集合中。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2, typename BsonDocs, typename BsonOpts>
static typename boost::enable_if<...>::type
s_collection_insert_many(Connection_T& conn, const String1& str_db, const String2& str_coln,
                        const BsonDocs& docs, const BsonOpts& opts,
                        org_bson_type* preply, org_bson_error_type* perr = 0)
```

**参数**:
- `conn`: 数据库连接对象
- `str_db`: 数据库名称
- `str_coln`: 集合名称
- `docs`: 要插入的文档集合
- `opts`: 插入选项
- `preply`: 回复 BSON 对象指针
- `perr`: 错误对象指针

**返回值**: `bool` - 操作是否成功

#### s_collection_insert_one
插入单个文档到集合中。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2, typename BsonDoc, typename BsonOpts>
static typename boost::enable_if<...>::type
s_collection_insert_one(Connection_T& conn, const String1& str_db, const String2& str_coln,
                       const BsonDoc& doc, const BsonOpts& opts,
                       org_bson_type* preply, org_bson_error_type* perr = 0)
```

### 2. 文档删除操作

#### s_collection_delete_many
删除匹配筛选条件的多个文档。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2, typename BsonSel, typename BsonOpts>
static typename boost::enable_if<...>::type
s_collection_delete_many(Connection_T& conn, const String1& str_db, const String2& str_coln,
                        u32 flags, const BsonSel& sel, const BsonOpts& opts,
                        org_bson_type* preply, org_bson_error_type* perr = 0)
```

#### s_collection_delete_one
删除匹配筛选条件的单个文档。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2, typename BsonSel, typename BsonOpts>
static typename boost::enable_if<...>::type
s_collection_delete_one(Connection_T& conn, const String1& str_db, const String2& str_coln,
                       u32 flags, const BsonSel& sel, const BsonOpts& opts,
                       org_bson_type* preply, org_bson_error_type* perr = 0)
```

### 3. 文档查找操作

#### s_collection_find
查找集合中的文档。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2,
         typename BsonFilter, typename BsonOpts, typename ReadPrefs, typename HandlerCollector>
static typename boost::enable_if<...>::type
s_collection_find(Connection_T& conn, const String1& str_db, const String2& str_coln,
                 const BsonFilter& filter, const BsonOpts& opts, const ReadPrefs& read_prefs,
                 org_bson_error_type* perr, const HandlerCollector& handler_collector)
```

**参数**:
- `filter`: 查询筛选条件
- `opts`: 查询选项
- `read_prefs`: 读取偏好设置
- `handler_collector`: 结果处理器

### 4. 聚合操作

#### s_collection_aggregate
执行聚合管道操作。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2,
         typename Bson1, typename Bson2, typename ReadPrefs, typename HandlerCollector>
static typename boost::enable_if<...>::type
s_collection_aggregate(Connection_T& conn, const String1& str_db, const String2& str_coln,
                      u32 flags, const Bson1& pipeline, const Bson2& opts,
                      const ReadPrefs& read_prefs, org_bson_error_type* err,
                      const HandlerCollector& handler_collector)
```

### 5. 文档更新操作

#### s_collection_replace_one
替换单个文档。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2,
         typename BsonSel, typename BsonReplace, typename BsonOpts>
static typename boost::enable_if<...>::type
s_collection_replace_one(Connection_T& conn, const String1& str_db, const String2& str_coln,
                        const BsonSel& selector, const BsonReplace& replacement,
                        const BsonOpts& opts, org_bson_type* preply, org_bson_error_type* perr = 0)
```

#### s_collection_update_one
更新单个文档。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2,
         typename BsonSel, typename BsonUpdate, typename BsonOpts>
static typename boost::enable_if<...>::type
s_collection_update_one(Connection_T& conn, const String1& str_db, const String2& str_coln,
                       const BsonSel& selector, const BsonUpdate& update,
                       const BsonOpts& opts, org_bson_type* preply, org_bson_error_type* perr = 0)
```

#### s_collection_update_many
更新多个文档。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2,
         typename BsonSel, typename BsonUpdate, typename BsonOpts>
static typename boost::enable_if<...>::type
s_collection_update_many(Connection_T& conn, const String1& str_db, const String2& str_coln,
                        const BsonSel& selector, const BsonUpdate& update,
                        const BsonOpts& opts, org_bson_type* preply, org_bson_error_type* perr = 0)
```

### 6. 计数操作

#### s_collection_count
统计集合中文档数量。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2,
         typename BsonFilter, typename BsonOpts, typename ReadPrefs>
static typename boost::enable_if<...>::type
s_collection_count(Connection_T& conn, const String1& str_db, const String2& str_coln,
                  const BsonFilter& filter, const BsonOpts& opts, const ReadPrefs& read_prefs,
                  org_bson_type* preply, org_bson_error_type* perr = 0)
```

**返回值**: `s64` - 文档数量

#### s_collection_estimated_count
估算集合中文档数量（不使用筛选条件）。

### 7. 集合管理操作

#### s_collection_drop
删除集合。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2, typename BsonOpts>
static typename boost::enable_if<...>::type
s_collection_drop(Connection_T& conn, const String1& str_db, const String2& str_coln,
                 const BsonOpts& opts, org_bson_error_type* perr = 0)
```

#### s_collection_rename
重命名集合。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2,
         typename String3, typename String4, typename BsonOpts>
static typename boost::enable_if<...>::type
s_collection_rename(Connection_T& conn, const String1& str_db, const String2& str_coln,
                   const String3& new_str_db, const String4& new_str_coln,
                   bool drop_target_before_rename, const BsonOpts& opts,
                   org_bson_error_type* perr = 0)
```

### 8. 索引管理操作

#### s_collection_create_indexes
创建索引。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2, typename BsonOpts>
static typename boost::enable_if<...>::type
s_collection_create_indexes(Connection_T& conn, const String1& str_db, const String2& str_coln,
                           org_index_model_type* const* pmodels, size_t n_models,
                           const BsonOpts& opts, org_bson_type* preply, org_bson_error_type* perr = 0)
```

#### s_collection_drop_index
删除指定名称的索引。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2, typename String3, typename BsonOpts>
static typename boost::enable_if<...>::type
s_collection_drop_index(Connection_T& conn, const String1& str_db, const String2& str_coln,
                       const String3& str_idx_name, const BsonOpts& opts, org_bson_error_type* perr = 0)
```

#### s_collection_find_indexes
查找集合中的所有索引。

### 9. 高级操作

#### s_collection_find_and_modify
查找并修改文档（原子操作）。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2,
         typename BsonQuery, typename BsonSort, typename BsonUpdate, typename BsonFields>
static typename boost::enable_if<...>::type
s_collection_find_and_modify(Connection_T& conn, const String1& str_db, const String2& str_coln,
                            const BsonQuery& query, const BsonSort& sort,
                            const BsonUpdate& update, const BsonFields& fields,
                            bool remove, bool upsert, bool new, org_bson_type* preply,
                            org_bson_error_type* perr = 0)
```

#### s_collection_bulk_execute
批量执行多个操作。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2,
         typename BsonOpts, typename HandlerBulkOptMaker>
static typename boost::enable_if<...>::type
s_collection_bulk_execute(Connection_T& conn, const String1& str_db, const String2& str_coln,
                         const BsonOpts& opts, const HandlerBulkOptMaker& handler_bulk_opt_maker,
                         org_bson_type* preply, org_bson_error_type* perr = 0)
```

**返回值**: `u32` - 执行的操作数量

### 10. 监听操作

#### s_collection_watch / s_watch
监听集合的变化。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2,
         typename BsonPipeline, typename BsonOpts, typename HandlerCollector>
static bool s_collection_watch(Connection_T& conn, const String1& str_db, const String2& str_coln,
                              const BsonPipeline& pipeline, const BsonOpts& opts,
                              org_bson_type* preply, org_bson_error_type* perr,
                              const HandlerCollector& handler_collector)
```

### 11. 集合统计操作

#### s_collection_stats
获取集合统计信息。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2, typename BsonOpts>
static typename boost::enable_if<...>::type
s_collection_stats(Connection_T& conn, const String1& str_db, const String2& str_coln,
                  const BsonOpts& opts, org_bson_type* preply, org_bson_error_type* perr = 0)
```

#### s_collection_validate
验证集合的完整性。

## 类型要求

所有模板方法都要求字符串参数满足 `charset::utf8_string_constructable` 特性，这意味着参数可以转换为 UTF-8 编码的字符串。

## 错误处理

所有操作都接受 `org_bson_error_type*` 或 `org_bson_error_type&` 参数用于错误报告。当操作失败时，会在错误对象中设置相应的错误信息。

## 依赖关系

该文件依赖于以下头文件和组件：

1. **Yggdrasil 组件**:
   - `yggr/nsql_database_system/basic_mongo_accesser_collection.hpp` - 基类定义
   - `yggr/charset/charset.hpp` - 字符集处理
   - `yggr/container_ex/container_ex.hpp` - 容器扩展
   - `yggr/iterator_ex/iterator_ex.hpp` - 迭代器扩展
   - `yggr/range_ex/range_ex.hpp` - 范围扩展
   - `yggr/func/foo_t.hpp` - 函数对象工具

2. **Boost 库**:
   - `boost/mpl/and.hpp` - MPL 逻辑与
   - `boost/mpl/or.hpp` - MPL 逻辑或
   - `boost/enable_if.hpp` - SFINAE 启用条件
   - `boost/is_base_of.hpp` - 类型检查
   - `boost/is_integral.hpp` - 整数类型检查
   - `boost/addressof.hpp` - 地址获取

3. **MongoDB C 驱动**:
   - `mongoc.h` - MongoDB C 驱动头文件

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongo_accesser_collection.hpp>

// 假设已有连接对象 conn
using namespace yggr::nsql_database_system;

// 插入文档
mongo_accesser_collection::s_collection_insert_one(
    conn, "testdb", "testcol", 
    bson_doc, bson_opts, &reply, &error);

// 查询文档
mongo_accesser_collection::s_collection_find(
    conn, "testdb", "testcol",
    bson_filter, bson_opts, read_prefs, &error, handler);

// 更新文档
mongo_accesser_collection::s_collection_update_one(
    conn, "testdb", "testcol",
    bson_selector, bson_update, bson_opts, &reply, &error);
```

## 设计特点

1. **类型安全**: 使用模板和 SFINAE 确保编译时类型检查
2. **灵活性**: 支持多种字符串类型和 BSON 类型参数
3. **错误处理**: 统一的错误报告机制
4. **性能优化**: 直接调用底层 MongoDB C 驱动，避免额外开销
5. **编码转换**: 自动处理 UTF-8 字符串编码转换
6. **资源管理**: 正确管理 BSON 对象和错误对象的生命周期