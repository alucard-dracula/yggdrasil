# mongo_accesser_client_encryption.hpp 文档

## 文件概述

`mongo_accesser_client_encryption.hpp` 定义了 `mongo_accesser_client_encryption` 类，它继承自 `basic_mongo_accesser_client_encryption`，用于封装 MongoDB 客户端加密操作的静态辅助接口。该类提供了创建加密连接、创建数据密钥、创建加密集合、重新包装数据密钥、密钥管理和加密/解密功能的高级封装。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关类和函数。

## 类定义

### mongo_accesser_client_encryption

继承自 `basic_mongo_accesser_client_encryption`，通过静态成员函数公开客户端加密操作，并对字符串参数做 UTF-8 转换以及对 `BSON`/错误结构做统一封装。

#### 类型定义

- `base_type`：`basic_mongo_accesser_client_encryption`。
- `bson_type` / `org_bson_type`：BSON 类型别名。
- `bson_value_type` / `org_bson_value_type`：BSON 值类型别名。
- `bson_error_type` / `org_bson_error_type`：BSON 错误类型别名。
- `write_concern_type` / `org_write_concern_type`：写关注类型别名。
- `read_prefs_type` / `org_read_prefs_type`：读偏好类型别名。
- `index_model_type` / `org_index_model_type`：索引模型类型别名。
- `find_and_modify_opts_type` / `org_find_and_modify_opts_type`：查找并修改选项类型别名。
- `read_concern_type` / `org_read_concern_type`：读一致性类型别名。
- `mongoc_error_domain_type` / `mongoc_error_code_type`：Mongo C 驱动错误码类型别名。
- `client_encryption_opts_type` / `org_client_encryption_opts_type`：客户端加密选项类型别名。
- `client_encryption_datakey_opts_type` / `org_client_encryption_datakey_opts_type`：数据密钥选项类型别名。
- `client_encryption_rewrap_many_datakey_result_type` / `org_client_encryption_rewrap_many_datakey_result_type`：重新包装多个数据密钥结果类型别名。
- `client_encryption_encrypt_opts_type` / `org_client_encryption_encrypt_opts_type`：加密选项类型别名。

## 设计说明

类内部使用多个 `detail::to_const_pointer` 辅助函数，将输入对象适配为原始 MongoDB C 类型指针，并使用 `charset::utf8_string_constructable` 检查字符串类型，同时对 UTF-8 字符串参数进行转换。

类注释中提醒不要实现 “get_param” 或 “set_params” 这类运行时专用接口；如需自定义原生操作，应使用 `using_handler` 自定义执行流程。

## 主要功能

### 创建客户端加密连接

静态函数 `s_client_encryption_create_connection_encryption` 提供两类重载：

- 使用 `shared_ptr<Connection_T>` 的版本。
- 使用 `Connection_T&` 引用的版本。

两者都支持传入可构造为原始 MongoDB 客户端类型的连接对象、客户端加密选项和可选错误输出参数。

### 创建数据密钥

静态函数 `s_client_encryption_create_datakey` 接受：

- `ConnEnc& conn_enc`：客户端加密对象。
- `const String1& str_kms_provides_mode_name`：KMS 提供程序名称。
- `const DatakeyOpts& popts`：数据密钥选项。
- `org_bson_value_type* pkeyid` 或 `org_bson_value_type& keyid`：返回数据密钥 ID。
- 可选错误输出参数 `org_bson_error_type* perr` 或 `org_bson_error_type& err`。

函数内部将字符串参数转换为 UTF-8 后调用基础类型实现。

### 创建加密集合

提供多组重载的 `s_client_encryption_create_encrypted_collection`：

- 可直接传入 `conn_enc`，或同时传入 `Connection_T& conn`。
- 支持 `opts` 和 `opt_master_key` 参数的不同输出方式：无输出、`org_bson_type*`、`org_bson_type&`、错误指针、错误引用，或组合输出。
- 自动将集合数据库名、集合名和 KMS 提供程序名称转换为 UTF-8。

### 重新包装多个数据密钥

函数 `s_client_encryption_rewrap_many_datakey` 支持：

- 可选 `kms_provider` 字符串或整数类型（当值为 0 时表示未指定）。
- 输入过滤器 `BsonFilter`。
- 可选结果返回 `org_client_encryption_rewrap_many_datakey_result_type*` 或用户自定义结果类型引用。
- 可选错误返回 `org_bson_error_type*` 或 `org_bson_error_type&`。

### 删除密钥

函数 `s_client_encryption_delete_key` 提供多种重载：

- 仅删除密钥。
- 删除密钥并返回 `org_bson_type*` 或 `org_bson_type&`。
- 支持错误指针或错误引用输出。
- 支持同时返回 `reply` 和错误信息。

### 获取密钥

函数 `s_client_encryption_get_key` 支持两种方式：

- 使用 BSON 值 `keyid` 获取密钥。
- 使用密钥别名字符串 `keyaltname` 获取密钥。

两种方式都提供多组重载：

- 无输出参数。
- 输出 BSON 文档指针或引用。
- 输出错误指针或引用。
- 键别名输入支持任意 UTF-8 可构造字符串类型。

### 获取所有密钥

函数 `s_client_encryption_get_keys` 接受：

- `ConnEnc& conn_enc`
- `HandlerCollector` 回调处理对象
- 可选错误输出参数

### 获取加密共享版本

函数 `s_client_encryption_get_crypt_shared_version` 提供多种重载，返回 `inner_string_type` 或模板字符串：

- 默认返回 UTF-8 内部字符串。
- 支持错误输出参数。
- 支持自定义分配器 `Alloc`。
- 支持目标字符串类型 `String`。

该函数用于查询客户端加密共享库版本。

### 添加密钥别名

函数 `s_client_encryption_add_key_alt_name` 支持：

- 通过 `BsonValKeyID` 指定密钥 ID。
- 使用字符串 `keyaltname` 指定别名。
- 可选择返回 BSON 文档或错误信息。

### 删除密钥别名

函数 `s_client_encryption_remove_key_alt_name` 支持：

- 通过 `BsonValKeyID` 指定密钥 ID。
- 使用字符串 `keyaltname` 指定别名。
- 可选择返回 BSON 文档或错误信息。

### 加密和解密

#### 加密

- `s_client_encryption_encrypt`：对 BSON 值进行加密，返回 `ciphertext`。
- `s_client_encryption_encrypt`（表达式版）：对 BSON 表达式进行加密，返回加密表达式文档。

函数支持返回指针或引用输出，并可返回错误信息。

#### 解密

- `s_client_encryption_decrypt`：对加密 BSON 值进行解密，返回明文值。

函数同样支持错误指针或引用输出。

## 依赖

- `yggr/base/yggrdef.h`
- `yggr/type_traits/native_t.hpp`
- `yggr/nsql_database_system/basic_mongo_accesser_client_encryption.hpp`
- `yggr/nsql_database_system/detail/to_pointer.hpp`
- `yggr/container_ex/is_allocator.hpp`
- `boost/mpl/not.hpp`
- `boost/type_traits/is_integral.hpp`

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。
