# basic_mongo_accesser_client_encryption.hpp 文档

## 概述

`basic_mongo_accesser_client_encryption.hpp` 是 Yggdrasil 项目中的一个 C++ 头文件，提供对 MongoDB 客户端加密功能的封装。该文件定义了 `basic_mongo_accesser_client_encryption` 类，该类包含一系列静态方法，用于处理 MongoDB 的客户端字段级加密（Client-Side Field Level Encryption, CSFLE）操作。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

`basic_mongo_accesser_client_encryption` 是一个静态方法集合类，用于封装 MongoDB C 驱动程序的客户端加密功能。它不包含实例变量，所有操作都是通过静态方法进行的。

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

### 加密相关类型
- `client_encryption_opts_type`: 客户端加密选项类型
- `org_client_encryption_opts_type`: 原始客户端加密选项类型
- `client_encryption_datakey_opts_type`: 客户端加密数据密钥选项类型
- `org_client_encryption_datakey_opts_type`: 原始客户端加密数据密钥选项类型
- `client_encryption_rewrap_many_datakey_result_type`: 客户端加密重新包装多个数据密钥结果类型
- `org_client_encryption_rewrap_many_datakey_result_type`: 原始客户端加密重新包装多个数据密钥结果类型
- `client_encryption_encrypt_opts_type`: 客户端加密加密选项类型
- `org_client_encryption_encrypt_opts_type`: 原始客户端加密加密选项类型

## 主要方法

### 连接加密创建
- `s_org_client_encryption_create_connection_encryption`: 创建连接加密对象

### 数据密钥管理
- `s_org_client_encryption_create_datakey`: 创建数据密钥
- `s_org_client_encryption_rewrap_many_datakey`: 重新包装多个数据密钥
- `s_org_client_encryption_delete_key`: 删除密钥
- `s_org_client_encryption_get_key`: 通过 ID 获取密钥
- `s_org_client_encryption_get_key_by_alt_name`: 通过别名获取密钥
- `s_org_client_encryption_get_keys`: 获取所有密钥
- `s_org_client_encryption_add_key_alt_name`: 添加密钥别名
- `s_org_client_encryption_remove_key_alt_name`: 移除密钥别名

### 加密集合管理
- `s_org_client_encryption_create_encrypted_collection`: 创建加密集合

### 加密/解密操作
- `s_org_client_encryption_encrypt`: 加密值
- `s_org_client_encryption_encrypt_expression`: 加密表达式
- `s_org_client_encryption_decrypt`: 解密值

### 版本信息
- `s_org_client_encryption_get_crypt_shared_version`: 获取加密共享库版本

## 依赖项

该文件依赖于以下头文件：
- `<yggr/base/yggrdef.h>`
- `<yggr/bind/bind.hpp>`
- `<yggr/mplex/pointer_to_value_t.hpp>`
- `<yggr/smart_ptr_ex/shared_ptr.hpp>`
- `<yggr/smart_ptr_ex/nonfree_deleter.hpp>`
- `<yggr/range_ex/range_value_ex.hpp>`
- `<yggr/nsql_database_system/mongo_accesser_types_decl.hpp>`
- `<yggr/nsql_database_system/c_mongo_client_encryption_opts.hpp>`
- `<yggr/nsql_database_system/c_mongo_client_encryption_datakey_opts.hpp>`
- `<yggr/nsql_database_system/c_mongo_client_encryption_encrypt_opts.hpp>`
- `<yggr/nsql_database_system/c_mongo_client_encryption_rewrap_many_datakey_result.hpp>`
- `<yggr/nsql_database_system/detail/get_org_t.hpp>`
- `<yggr/nsql_database_system/detail/mongoc_client_encryption_deleter.hpp>`
- `<boost/ref.hpp>`

## 使用注意事项

1. 所有方法都是静态的，不需要创建类实例
2. 方法使用模板参数来支持不同的连接和加密类型
3. 错误处理通过 `org_bson_error_type*` 参数进行
4. 许多方法需要有效的 MongoDB 客户端连接和加密上下文
5. 该类主要作为 MongoDB C 驱动程序客户端加密功能的 C++ 包装器

## 错误处理

所有方法都接受一个 `org_bson_error_type*` 参数用于错误报告。错误信息包括错误域、错误代码和描述性消息。

## 线程安全

该类的静态方法设计为可以安全地从多个线程调用，但具体的安全性取决于底层 MongoDB C 驱动程序的行为和使用的连接对象。