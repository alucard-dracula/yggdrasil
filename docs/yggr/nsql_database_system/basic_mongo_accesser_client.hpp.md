# basic_mongo_accesser_client.hpp 文档

## 概述

`basic_mongo_accesser_client.hpp` 是 Yggdrasil 项目中的一个 C++ 头文件，提供对 MongoDB 客户端操作的全面封装。该文件定义了 `basic_mongo_accesser_client` 类，该类包含一系列静态方法，用于处理 MongoDB 客户端的各种操作，包括命令执行、配置管理、加密支持等。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

`basic_mongo_accesser_client` 是一个静态方法集合类，用于封装 MongoDB C 驱动程序的客户端功能。它不包含实例变量，所有操作都是通过静态方法进行的。该类提供了对 MongoDB 客户端的完整操作接口，包括命令执行、配置管理、监控和加密功能。

## 辅助类

### client_using_handler_helper
模板类，用于处理客户端使用处理器，支持有返回值和无返回值的情况。

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

### 客户端相关类型
- `uri_type`: URI 类型
- `org_uri_type`: 原始 URI 类型
- `ssl_opt_type`: SSL 选项类型
- `org_ssl_opt_type`: 原始 SSL 选项类型
- `server_description_type`: 服务器描述类型
- `org_server_description_type`: 原始服务器描述类型
- `stream_initiator_type`: 流发起器类型
- `org_stream_initiator_type`: 原始流发起器类型
- `apm_callbacks_type`: APM 回调类型
- `org_apm_callbacks_type`: 原始 APM 回调类型
- `server_api_type`: 服务器 API 类型
- `org_server_api_type`: 原始服务器 API 类型
- `auto_encryption_opts_type`: 自动加密选项类型
- `org_auto_encryption_opts_type`: 原始自动加密选项类型

## 主要方法

### 命令执行
- `s_org_client_command_simple`: 执行简单命令
- `s_org_client_command_simple_with_server_id`: 在指定服务器上执行简单命令
- `s_org_client_command_with_opts`: 执行带选项的命令
- `s_org_client_read_command_with_opts`: 执行读命令（带选项）
- `s_org_client_read_write_command_with_opts`: 执行读写命令（带选项）
- `s_org_client_write_command_with_opts`: 执行写命令（带选项）
- `s_org_client_command`: 通用命令执行（根据标志选择模式）

### 客户端配置
- `s_org_client_get_read_concern`: 获取读关注
- `s_org_client_get_read_prefs`: 获取读偏好
- `s_org_client_get_write_concern`: 获取写关注
- `s_org_client_get_uri`: 获取 URI
- `s_org_client_set_read_concern`: 设置读关注
- `s_org_client_set_read_prefs`: 设置读偏好
- `s_org_client_set_write_concern`: 设置写关注

### SSL 支持（条件编译）
- `s_org_client_get_ssl_opts`: 获取 SSL 选项
- `s_org_client_set_ssl_opts`: 设置 SSL 选项

### 服务器管理
- `s_org_client_get_server_description`: 获取服务器描述
- `s_org_client_get_server_descriptions`: 获取所有服务器描述
- `s_org_client_select_server`: 选择服务器

### 监控和诊断
- `s_org_client_get_crypt_shared_version`: 获取加密共享库版本
- `s_org_client_is_crypt_enabled`: 检查是否启用加密
- `s_org_client_watch`: 监控变更流
- `s_org_client_get_database_names`: 获取数据库名称列表
- `s_org_client_get_handshake_description`: 获取握手描述

### 客户端控制
- `s_org_client_set_stream_initiator`: 设置流发起器
- `s_org_client_reset`: 重置客户端
- `s_org_client_set_appname`: 设置应用名称
- `s_org_client_set_apm_callbacks`: 设置 APM 回调
- `s_org_client_set_error_api`: 设置错误 API
- `s_org_client_set_server_api`: 设置服务器 API

### 自动加密
- `s_org_client_enable_auto_encryption`: 启用自动加密（客户端版本）
- `s_org_client_enable_auto_encryption`: 启用自动加密（连接池版本）

### 处理器使用
- `s_org_client_using_handler`: 使用处理器执行客户端操作

## 依赖项

该文件依赖于以下头文件：
- `<yggr/base/yggrdef.h>`
- `<yggr/bind/bind.hpp>`
- `<yggr/mplex/pointer_to_value_t.hpp>`
- `<yggr/smart_ptr_ex/shared_ptr.hpp>`
- `<yggr/smart_ptr_ex/nonfree_deleter.hpp>`
- `<yggr/container_ex/is_allocator.hpp>`
- `<yggr/nsql_database_system/mongoc_command_mode_def.hpp>`
- `<yggr/nsql_database_system/mongo_accesser_types_decl.hpp>`
- `<yggr/nsql_database_system/c_mongo_uri.hpp>`
- `<yggr/nsql_database_system/c_mongo_ssl_opt.hpp>`
- `<yggr/nsql_database_system/c_mongo_server_description.hpp>`
- `<yggr/nsql_database_system/c_mongo_apm_callbacks.hpp>`
- `<yggr/nsql_database_system/c_mongo_server_api.hpp>`
- `<yggr/nsql_database_system/c_mongo_auto_encryption_opts.hpp>`
- `<yggr/nsql_database_system/detail/get_org_t.hpp>`
- `<boost/ref.hpp>`

## 使用注意事项

1. 所有方法都是静态的，不需要创建类实例
2. 方法使用模板参数来支持不同的连接类型
3. 错误处理通过 `org_bson_error_type*` 参数进行
4. 许多方法需要有效的 MongoDB 客户端连接
5. 该类主要作为 MongoDB C 驱动程序客户端功能的 C++ 包装器
6. 自动加密功能支持客户端和连接池两种模式
7. SSL 支持需要条件编译（`MONGOC_ENABLE_SSL`）

## 错误处理

所有方法都接受一个 `org_bson_error_type*` 参数用于错误报告。错误信息包括错误域、错误代码和描述性消息。

## 线程安全

该类的静态方法设计为可以安全地从多个线程调用，但具体的安全性取决于底层 MongoDB C 驱动程序的行为和使用的连接对象。

## 条件编译

- SSL 相关功能需要 `MONGOC_ENABLE_SSL` 宏定义