# mongoc_index_model_native_ex.hpp

## 概述

此头文件定义了 `yggr_mongoc_index_model_t` 结构体和 `mongoc_index_model_native_ex` 类，用于处理 MongoDB 索引模型。这些定义提供了对 MongoDB C 驱动程序索引功能的扩展包装。

## 命名空间

- `yggr::nsql_database_system`

## 结构体描述

### yggr_mongoc_index_model_t

一个结构体，封装了 MongoDB 索引模型的选项参数。

#### 成员变量

##### `bson_t* keys`

索引键 BSON 文档。定义索引的字段和排序方向。

##### `bson_t* opts`

索引选项 BSON 文档。包含索引的附加选项，如唯一性、稀疏性等。

## 类描述

### mongoc_index_model_native_ex

一个工具类，提供索引模型的类型定义。

#### 类型定义

##### `org_index_model_type`

```cpp
typedef mongoc_index_model_t org_index_model_type;
```

MongoDB C 驱动程序的索引模型类型别名。

##### `yggr_index_model_type`

```cpp
typedef yggr_mongoc_index_model_t yggr_index_model_type;
```

自定义索引模型类型别名。

## 依赖项

- `<yggr/nsql_database_system/mongodb_config.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_index_model_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 创建索引模型结构体
yggr_mongoc_index_model_t index_model = {0};

// 设置索引键 (例如: {"field": 1} 表示升序索引)
bson_t keys = BSON_INITIALIZER;
BSON_APPEND_INT32(&keys, "field", 1);
index_model.keys = &keys;

// 设置索引选项 (例如: {"unique": true} 表示唯一索引)
bson_t opts = BSON_INITIALIZER;
BSON_APPEND_BOOL(&opts, "unique", true);
index_model.opts = &opts;

// 使用索引模型...
```

## 注意事项

- `yggr_mongoc_index_model_t` 结构体是对 MongoDB C 驱动程序索引模型的自定义包装
- `keys` BSON 文档定义了索引的字段，值为 1 表示升序，-1 表示降序
- `opts` BSON 文档可以包含各种索引选项，如：
  - `unique`: 是否为唯一索引
  - `sparse`: 是否为稀疏索引
  - `background`: 是否在后台创建索引
  - `name`: 索引名称
- 该类目前主要提供类型定义，将来可能扩展更多功能
- 使用完 BSON 文档后，需要调用 `bson_destroy()` 释放资源</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_index_model_native_ex_cn.md