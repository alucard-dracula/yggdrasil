# basic_mongo_accesser_collection.hpp 文档

## 概述

`basic_mongo_accesser_collection.hpp` 是 Yggdrasil 项目中的一个 C++ 头文件，提供对 MongoDB 集合操作的全面封装。该文件定义了 `basic_mongo_accesser_collection` 类，该类包含一系列静态方法，用于处理 MongoDB 集合的各种 CRUD 操作、索引管理、聚合查询等。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

`basic_mongo_accesser_collection` 是一个静态方法集合类，用于封装 MongoDB C 驱动程序的集合功能。它不包含实例变量，所有操作都是通过静态方法进行的。该类提供了对 MongoDB 集合的完整操作接口，包括文档的增删改查、索引管理、聚合查询、变更流监控等。

## 辅助类

### exclude_upsert_checker
用于检查和排除 upsert 选项的辅助类。

## 类型定义

### 基本类型
- `types_decl_type`: MongoDB 访问器类型声明
- `inner_string_type`: 内部字符串类型
- `inner_string_view_type`: 内部字符串视图类型
- `bson_type`: BSON 类型
- `org_bson_type`: 原始 BSON 类型
- `bson_value_type`: BSON 值类型
- `org_bson_value_type`: 原始 BSON 值类型
- `bson_error_type`: BSON 错误类型
- `org_bson_error_type`: 原始 BSON 错误类型
- `write_concern_type`: 写关注类型
- `org_write_concern_type`: 原始写关注类型
- `read_prefs_type`: 读偏好类型
- `org_read_prefs_type`: 原始读偏好类型
- `index_model_type`: 索引模型类型
- `org_index_model_type`: 原始索引模型类型
- `find_and_modify_opts_type`: 查找并修改选项类型
- `org_find_and_modify_opts_type`: 原始查找并修改选项类型
- `read_concern_type`: 读关注类型
- `org_read_concern_type`: 原始读关注类型
- `mongoc_error_domain_type`: MongoC 错误域类型
- `mongoc_error_code_type`: MongoC 错误代码类型

## 主要方法

### 插入操作
- `s_org_collection_insert_one`: 插入单个文档
- `s_org_collection_insert_many`: 插入多个文档

### 删除操作
- `s_org_collection_delete_one`: 删除单个文档
- `s_org_collection_delete_many`: 删除多个文档
- `s_org_collection_remove`: 通用删除操作（根据标志选择模式）

### 查询操作
- `s_org_collection_find`: 查找文档
- `s_org_collection_aggregate`: 聚合查询

### 更新操作
- `s_org_collection_replace_one`: 替换单个文档
- `s_org_collection_update_one`: 更新单个文档
- `s_org_collection_update_many`: 更新多个文档
- `s_org_collection_update`: 通用更新操作（根据标志选择模式）

### 计数操作
- `s_org_collection_count`: 精确文档计数
- `s_org_collection_estimated_count`: 估计文档计数

### 集合管理
- `s_org_collecion_drop`: 删除集合
- `s_org_collection_rename`: 重命名集合
- `s_org_collection_save`: 保存文档（插入或替换）
- `s_org_collection_stats`: 获取集合统计信息
- `s_org_collections_validate`: 验证集合

### 索引管理
- `s_org_collection_create_indexes`: 创建索引
- `s_org_collection_drop_index`: 删除索引
- `s_org_collection_find_indexes`: 查找索引

### 查找并修改
- `s_org_collection_find_and_modify`: 查找并修改文档
- `s_org_collection_find_and_modify_with_opt`: 带选项的查找并修改

### 批量操作
- `s_org_collection_bulk_execute`: 执行批量操作

### 变更流监控
- `s_org_collection_watch`: 监控集合变更流

### 处理器使用
- `s_org_collection_using_handler`: 使用处理器执行集合操作

## 依赖项

该文件依赖于以下头文件：
- `<yggr/nsql_database_system/mongo_accesser_types_decl.hpp>`
- `<yggr/nsql_database_system/mongoc_collection_native_ex.hpp>`
- `<yggr/nsql_database_system/mongoc_command_mode_def.hpp>`
- `<yggr/nsql_database_system/mongo_const_strings_def.hpp>`
- `<yggr/nsql_database_system/mongo_operators_dollar_cmd_def.hpp>`

## 使用注意事项

1. 所有方法都是静态的，不需要创建类实例
2. 方法使用模板参数来支持不同的连接类型
3. 错误处理通过 `org_bson_error_type*` 参数进行
4. 许多方法需要有效的 MongoDB 客户端连接和集合名称
5. 该类主要作为 MongoDB C 驱动程序集合功能的 C++ 包装器
6. 查找操作使用处理器收集器模式来处理结果集
7. 批量操作需要提供批量选项制造器来构建操作

## 错误处理

所有方法都接受一个 `org_bson_error_type*` 参数用于错误报告。错误信息包括错误域、错误代码和描述性消息。

## 线程安全

该类的静态方法设计为可以安全地从多个线程调用，但具体的安全性取决于底层 MongoDB C 驱动程序的行为和使用的连接对象。

## 特殊说明

- `s_org_collection_save` 方法会根据文档是否包含 `_id` 字段来决定是执行插入还是替换操作
- `s_org_collection_update` 方法会根据更新文档的第一个键是否以 `$` 开头来决定是执行更新还是替换
- 索引创建支持多个索引模型的批量创建
- 变更流监控支持实时跟踪集合的变化