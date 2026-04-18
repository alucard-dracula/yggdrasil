# mongo_accesser.hpp

## 概述

`mongo_accesser.hpp` 是 Yggdrasil 数据库系统中 MongoDB 访问器的主头文件。该文件定义了 `mongo_accesser` 类，这是一个综合性的 MongoDB 访问器，通过多重继承整合了所有 MongoDB 相关的功能模块。

该类是 MongoDB 访问器的统一入口点，提供了客户端连接、数据库操作、集合操作、GridFS 文件存储、加密功能和线程安全支持的完整解决方案。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    class mongo_accesser;
}
}
```

## 类定义

### mongo_accesser

```cpp
class mongo_accesser
    : public mongo_accesser_client,
      public mongo_accesser_client_encryption,
      public mongo_accesser_database,
      public mongo_accesser_collection,
      public mongo_accesser_gridfs,
      public mongo_accesser_thread
{
    // 类型定义和方法...
};
```

## 继承架构

`mongo_accesser` 通过多重继承整合了以下专门的访问器类：

### 基础访问器类型
```cpp
typedef mongo_accesser_client base_accesser_client_type;
typedef mongo_accesser_client_encryption base_accesser_encryption_type;
typedef mongo_accesser_database base_accesser_database_type;
typedef mongo_accesser_collection base_accesser_collection_type;
typedef mongo_accesser_gridfs base_accesser_gridfs_type;
typedef mongo_accesser_thread base_accesser_thread_type;
```

### 继承的功能模块

1. **mongo_accesser_client**: MongoDB 客户端连接和管理
2. **mongo_accesser_client_encryption**: 客户端字段级加密功能
3. **mongo_accesser_database**: 数据库级别的操作
4. **mongo_accesser_collection**: 集合级别的 CRUD 操作
5. **mongo_accesser_gridfs**: GridFS 文件存储系统
6. **mongo_accesser_thread**: 线程安全的访问支持

## 类型定义

### 通用类型（继承自 types_decl_type）
```cpp
typedef mongo_accesser_types_decl types_decl_type;

typedef types_decl_type::inner_string_type inner_string_type;
typedef types_decl_type::inner_string_view_type inner_string_view_type;

typedef types_decl_type::bson_type bson_type;
typedef types_decl_type::org_bson_type org_bson_type;

typedef types_decl_type::bson_value_type bson_value_type;
typedef types_decl_type::org_bson_value_type org_bson_value_type;

typedef types_decl_type::bson_error_type bson_error_type;
typedef types_decl_type::org_bson_error_type org_bson_error_type;

typedef types_decl_type::write_concern_type write_concern_type;
typedef types_decl_type::org_write_concern_type org_write_concern_type;

typedef types_decl_type::read_prefs_type read_prefs_type;
typedef types_decl_type::org_read_prefs_type org_read_prefs_type;

typedef types_decl_type::index_model_type index_model_type;
typedef types_decl_type::org_index_model_type org_index_model_type;

typedef types_decl_type::find_and_modify_opts_type find_and_modify_opts_type;
typedef types_decl_type::org_find_and_modify_opts_type org_find_and_modify_opts_type;

typedef types_decl_type::read_concern_type read_concern_type;
typedef types_decl_type::org_read_concern_type org_read_concern_type;

typedef types_decl_type::mongoc_error_domain_type mongoc_error_domain_type;
typedef types_decl_type::mongoc_error_code_type mongoc_error_code_type;
```

### 客户端专用类型
```cpp
typedef base_accesser_client_type::uri_type uri_type;
typedef base_accesser_client_type::org_uri_type org_uri_type;

typedef base_accesser_client_type::ssl_opt_type ssl_opt_type;
typedef base_accesser_client_type::org_ssl_opt_type org_ssl_opt_type;

typedef base_accesser_client_type::server_description_type server_description_type;
typedef base_accesser_client_type::org_server_description_type org_server_description_type;

typedef base_accesser_client_type::stream_initiator_type stream_initiator_type;
typedef base_accesser_client_type::org_stream_initiator_type org_stream_initiator_type;

typedef base_accesser_client_type::apm_callbacks_type apm_callbacks_type;
typedef base_accesser_client_type::org_apm_callbacks_type org_apm_callbacks_type;

typedef base_accesser_client_type::server_api_type server_api_type;
typedef base_accesser_client_type::org_server_api_type org_server_api_type;

typedef base_accesser_client_type::auto_encryption_opts_type auto_encryption_opts_type;
typedef base_accesser_client_type::org_auto_encryption_opts_type org_auto_encryption_opts_type;
```

### 加密专用类型
```cpp
typedef base_accesser_encryption_type::client_encryption_opts_type client_encryption_opts_type;
typedef base_accesser_encryption_type::org_client_encryption_opts_type org_client_encryption_opts_type;

typedef base_accesser_encryption_type::client_encryption_datakey_opts_type client_encryption_datakey_opts_type;
typedef base_accesser_encryption_type::org_client_encryption_datakey_opts_type org_client_encryption_datakey_opts_type;

typedef base_accesser_encryption_type::client_encryption_rewrap_many_datakey_result_type client_encryption_rewrap_many_datakey_result_type;
typedef base_accesser_encryption_type::org_client_encryption_rewrap_many_datakey_result_type org_client_encryption_rewrap_many_datakey_result_type;

typedef base_accesser_encryption_type::client_encryption_encrypt_opts_type client_encryption_encrypt_opts_type;
typedef base_accesser_encryption_type::org_client_encryption_encrypt_opts_type org_client_encryption_encrypt_opts_type;
```

### GridFS 专用类型
```cpp
typedef base_accesser_gridfs_type::gridfs_file_opt_type gridfs_file_opt_type;
typedef base_accesser_gridfs_type::org_gridfs_file_opt_type org_gridfs_file_opt_type;

typedef base_accesser_gridfs_type::gridfs_file_type gridfs_file_type;
typedef base_accesser_gridfs_type::org_gridfs_file_type org_gridfs_file_type;
```

## 组合方法

### 监听功能组合
```cpp
using base_accesser_client_type::s_watch;
using base_accesser_database_type::s_watch;
using base_accesser_collection_type::s_watch;
```

提供统一的监听接口，支持客户端、数据库和集合级别的变更监听。

### 处理程序使用组合
```cpp
using base_accesser_database_type::s_using_handler;
using base_accesser_collection_type::s_using_handler;
```

提供统一的原生操作处理接口，支持数据库和集合级别的自定义操作。

## 功能特性

### 1. 统一接口
- **单一入口**: 通过一个类访问所有 MongoDB 功能
- **一致命名**: 统一的类型命名和方法命名约定
- **无缝集成**: 各模块功能有机整合，无接口冲突

### 2. 完整功能覆盖
- **连接管理**: 客户端连接、SSL、服务器发现
- **数据操作**: CRUD 操作、聚合、索引管理
- **文件存储**: GridFS 大文件存储和检索
- **安全特性**: 字段级加密和数据密钥管理
- **监控能力**: 变更流监听和性能监控
- **并发支持**: 线程安全的操作支持

### 3. 类型安全
- **强类型化**: 所有操作都有明确的类型定义
- **编译时检查**: 模板元编程确保类型正确性
- **封装抽象**: 隐藏底层 MongoDB C 驱动的复杂性

## 依赖项

该类依赖于所有继承的访问器类：

- `mongo_accesser_client.hpp`
- `mongo_accesser_client_encryption.hpp`
- `mongo_accesser_database.hpp`
- `mongo_accesser_collection.hpp`
- `mongo_accesser_gridfs.hpp`
- `mongo_accesser_thread.hpp`
- `mongo_accesser_types_decl.hpp`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongo_accesser.hpp>

// 创建统一的访问器实例
mongo_accesser accesser;

// 使用客户端功能连接数据库
accesser.s_client_connect("mongodb://localhost:27017");

// 执行数据库操作
mongo_accesser::bson_type doc = BSON_INITIALIZER;
// ... 填充文档 ...

// 插入文档到集合
accesser.s_collection_insert_one(connection, "mydb", "mycollection", doc);

// 使用 GridFS 保存文件
accesser.s_gridfs_save_file(connection, "mydb", "mygridfs", "/path/to/file.txt");

// 监听集合变更
accesser.s_watch(connection, "mydb", "mycollection",
                 [](const bson_type& doc) {
                     // 处理变更文档
                 });

// 线程清理
accesser.s_thread_clear();
```

## 设计理念

### 组合模式
- **多重继承**: 将专门的功能模块组合成完整的解决方案
- **接口统一**: 通过 using 声明提供一致的外部接口
- **职责分离**: 每个基类负责特定的功能领域

### 类型系统
- **类型继承**: 继承所有必要的类型定义
- **命名空间**: 避免类型名冲突
- **向后兼容**: 保持与各模块独立使用的兼容性

## 注意事项

1. **菱形继承问题**: 通过虚拟继承或组合模式避免多重继承的二义性
2. **接口一致性**: 所有同名方法在不同基类中具有相同语义
3. **性能考虑**: 单一类可能比独立使用各模块有轻微的额外开销
4. **内存布局**: 多重继承可能影响对象的内存布局
5. **初始化顺序**: 确保基类的正确初始化顺序

## 许可证

该文件遵循项目的许可证协议，基于 MIT 许可证。