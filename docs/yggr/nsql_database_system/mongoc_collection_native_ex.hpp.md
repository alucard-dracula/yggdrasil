# mongoc_collection_native_ex.hpp

## 概述

此头文件定义了 `mongoc_collection_native_ex` 类，该类使用原生 MongoDB C 驱动程序 (mongoc) 为 MongoDB 集合操作提供扩展的包装函数。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

### mongoc_collection_native_ex

一个扩展类，用于包装原生 MongoDB 集合操作，并提供额外的错误处理和便利方法。

#### 公共静态方法

##### `s_mongoc_collection_stats`

```cpp
static bool s_mongoc_collection_stats(mongoc_collection_t *collection,
                                      const bson_t *options,
                                      bson_t *stats,
                                      bson_error_t *error);
```

检索 MongoDB 集合的统计信息。

**参数：**
- `collection`：指向 MongoDB 集合对象的指针
- `options`：可选的 BSON 文档，指定统计命令的选项
- `stats`：输出 BSON 文档，用于存储集合统计信息
- `error`：指向错误结构的指针，用于报告失败

**返回值：**
- `true` 如果操作成功
- `false` 如果发生错误（检查 `error` 参数）

##### `s_mongoc_collection_validate`

```cpp
static bool s_mongoc_collection_validate(mongoc_collection_t* collection,
                                         const bson_t* options,
                                         bson_t* reply,
                                         bson_error_t* error);
```

验证 MongoDB 集合的结构和完整性。

**参数：**
- `collection`：指向要验证的 MongoDB 集合对象的指针
- `options`：可选的 BSON 文档，指定验证选项
- `reply`：输出 BSON 文档，包含验证结果
- `error`：指向错误结构的指针，用于报告失败

**返回值：**
- `true` 如果验证成功
- `false` 如果验证失败或发生错误

## 依赖项

- `<yggr/nsql_database_system/mongodb_config.hpp>`
- MongoDB C 驱动程序 (mongoc) 头文件

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_collection_native_ex.hpp>

// 假设您有一个有效的 mongoc_collection_t* collection
bson_t stats = BSON_INITIALIZER;
bson_error_t error;

if (yggr::nsql_database_system::mongoc_collection_native_ex::s_mongoc_collection_stats(
        collection, nullptr, &stats, &error)) {
    // 处理统计信息
} else {
    // 处理错误
}
```

## 注意事项

- 所有方法都是静态的，因此不需要创建类的实例
- 错误处理在所有方法中保持一致，使用 `bson_error_t`
- 该类通过额外的安全检查扩展了原生 mongoc 功能</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_collection_native_ex_cn.md