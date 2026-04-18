# mongo_accesser_client.hpp 文档

## 文件概述

`mongo_accesser_client.hpp` 定义了 `mongo_accesser_client` 类，它继承自 `basic_mongo_accesser_client`，用于封装 MongoDB 客户端操作的静态辅助接口。该类主要提供对客户端命令、读写选项、版本信息、SSL 设置、加密自动启用和观察功能的统一调用封装。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关类和函数。

## 类定义

### mongo_accesser_client

继承自 `basic_mongo_accesser_client`，用于在 C++ 中调用 MongoDB 客户端的各种操作。类将字符串参数自动转换为 UTF-8，并使用 `detail::to_const_pointer` 将高层对象适配为原始 MongoDB C API 使用的指针。

#### 类型别名

- `base_type`：`basic_mongo_accesser_client`。
- `bson_type` / `org_bson_type`：BSON 类型。
- `bson_value_type` / `org_bson_value_type`：BSON 值类型。
- `bson_error_type` / `org_bson_error_type`：BSON 错误类型。
- `write_concern_type` / `org_write_concern_type`：写关注类型。
- `read_prefs_type` / `org_read_prefs_type`：读偏好类型。
- `index_model_type` / `org_index_model_type`：索引模型类型。
- `find_and_modify_opts_type` / `org_find_and_modify_opts_type`：查找修改选项类型。
- `read_concern_type` / `org_read_concern_type`：读取一致性类型。
- `mongoc_error_domain_type` / `mongoc_error_code_type`：MongoDB 驱动错误码类型。
- `uri_type` / `org_uri_type`：URI 类型。
- `ssl_opt_type` / `org_ssl_opt_type`：SSL 选项类型。
- `server_description_type` / `org_server_description_type`：服务器描述类型。
- `stream_initiator_type` / `org_stream_initiator_type`：流初始化器类型。
- `apm_callbacks_type` / `org_apm_callbacks_type`：APM 回调类型。
- `server_api_type` / `org_server_api_type`：服务器 API 类型。
- `auto_encryption_opts_type` / `org_auto_encryption_opts_type`：自动加密选项类型。

## 主要功能

### 客户端命令执行

提供多个静态方法用于执行客户端命令：

- `s_client_command_simple`：带读偏好和可选回复/错误输出的简单命令执行。
- `s_run_command_simple`：同 `s_client_command_simple`，作为更通用的别名。
- `s_client_command_simple`（带 `server_id`）：允许指定服务器 ID 进行命令执行。
- `s_run_command_simple`（带 `server_id`）：同上。
- `s_client_command`：支持命令选项 `opts`、读偏好和可选回复/错误输出。
- `s_client_command`（无读偏好版本）：在不传入读偏好的情况下执行命令。
- `s_run_command`：作为通用命令执行接口的别名。

所有命令接口都支持：

- 通过 `String1` 类型传入数据库名，并自动转换成 UTF-8。
- 通过 `BsonCmd` 和 `BsonOpts` 类型传入 BSON 命令和选项。
- 通过 `ReadPrefs` 类型传入读偏好。
- 支持 `org_bson_type*` 或 `org_bson_type&` 的回复接收。
- 支持 `org_bson_error_type*` 或 `org_bson_error_type&` 的错误接收。

### 读写命令

- `s_client_read_command`：执行读取命令，支持命令、读偏好、选项、回复和错误输出。
- `s_client_read_write_command`：执行可读可写命令。
- `s_client_write_command`：执行写命令。

### 命令标志版本

- `s_client_command`（带 `flags` 参数）：可附加标志的命令执行接口。
- `s_run_command`（带 `flags` 参数）：命令运行别名。

### 观察与监听

- `s_client_watch`：在客户端上启用观察（watch），接收管道、选项、回复和错误，并使用处理回调收集结果。
- `s_watch`：与 `s_client_watch` 相同，作为别名。

### 数据库与服务器信息

- `s_client_get_database_names`：获取数据库名称列表，可选错误输出。
- `s_client_get_server_description`：获取指定服务器 ID 的描述。
- `s_client_get_server_descriptions`：获取所有服务器描述。
- `s_client_select_server`：根据是否写操作和读偏好选择服务器。

### 读取设置获取与修改

- `s_client_get_read_concern`：获取客户端读取一致性对象。
- `s_client_get_read_prefs`：获取客户端读偏好对象。
- `s_client_set_read_concern`：设置客户端读取一致性。
- `s_client_set_read_prefs`：设置客户端读偏好。

### 写关注设置获取与修改

- `s_client_get_write_concern`：获取客户端写关注对象。
- `s_client_set_write_concern`：设置客户端写关注。

### URI 与 SSL

- `s_client_get_uri`：从连接对象获取 URI。
- `s_client_get_ssl_opts`：获取 SSL 选项（仅在 `MONGOC_ENABLE_SSL` 定义时可用）。
- `s_org_client_set_ssl_opts` / `s_client_set_ssl_opts`：设置 SSL 选项。

### 流与服务器 API 设置

- `s_client_set_stream_initiator`：设置客户端流初始化器，可传入用户数据或 handler。
- `s_client_reset`：重置客户端状态。
- `s_client_set_appname`：设置客户端应用名称。
- `s_client_set_apm_callbacks`：设置 APM 回调及其上下文。
- `s_client_set_error_api`：设置错误 API 版本。
- `s_client_get_handshake_description`：获取握手描述。
- `s_client_set_server_api`：设置服务器 API。

### 自动加密支持

- `s_client_enable_auto_encryption`：启用客户端自动加密，支持多种连接与密钥库组合方式：
  - `EncConnect` 加密连接对象。
  - `Connection_T` 普通客户端连接对象。
  - `pconn` / `pkeyvault_conn`：`shared_ptr` 版本或直接 `Connection_T&` 版本。
  - `AutoEncOpts` 自动加密选项。
  - 可选错误输出参数。

### 通用处理器

- `s_client_using_handler`：在客户端连接上执行自定义 handler，并返回 handler 的执行结果，支持错误输出。

## 实现细节

- 内部私有包装函数负责将字符串参数转换为 UTF-8，然后调用基类的原始方法。
- 通过 `detail::to_const_pointer` 将高层类型转换为原始 MongoDB C API 所需指针。
- 仅在连接原始类型为 `mongoc_client_t` 时，部分方法才启用。
- 所有方法均为静态成员函数，适合无状态调用。

## 依赖

- `yggr/nsql_database_system/basic_mongo_accesser_client.hpp`
- `yggr/nsql_database_system/detail/to_pointer.hpp`
- `yggr/base/yggrdef.h`
- `yggr/type_traits/native_t.hpp`
- `yggr/charset/utf8_string.hpp`
- `yggr/charset/utf8_foo.hpp`
- `yggr/charset/utf8_string_view.hpp`
- `yggr/container_ex/is_allocator.hpp`
- `boost/utility/enable_if.hpp`
- `boost/type_traits/is_same.hpp`
- `boost/type_traits/is_base_of.hpp`
- `boost/mpl/and.hpp`
- `boost/mpl/not.hpp`

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。
