# mongoc_uri_native_ex.hpp

## 概述

此头文件定义了 `yggr_mongoc_uri_t` 结构体和 `mongoc_uri_native_ex` 类，用于处理 MongoDB 连接 URI (Uniform Resource Identifier) 的解析、构建和管理。MongoDB URI 用于指定连接到 MongoDB 集群的详细信息，包括主机、端口、认证信息和连接选项。

## 命名空间

- `yggr::nsql_database_system`

## 结构体描述

### yggr_mongoc_uri_t

一个结构体，封装了 MongoDB URI 的所有组件信息，对应 MongoDB C 驱动的 `_mongoc_uri_t` 结构。

#### 成员变量

##### `char* str`

完整的 URI 字符串。

##### `bool is_srv`

是否为 SRV 记录 URI（用于 DNS 服务发现）。

##### `char srv[BSON_HOST_NAME_MAX + 1]`

SRV 记录的主机名。

##### `mongoc_host_list_t* hosts`

主机列表，包含主机名和端口。

##### `char* username`

用户名。

##### `char* password`

密码。

##### `char* database`

默认数据库名。

##### `bson_t raw`

未解析的原始选项。

##### `bson_t options`

已解析和规范化的选项。

##### `bson_t credentials`

凭据信息。

##### `bson_t compressors`

压缩器配置。

##### `mongoc_read_prefs_t* read_prefs`

读偏好设置。

##### `mongoc_read_concern_t* read_concern`

读关注设置。

##### `mongoc_write_concern_t* write_concern`

写关注设置。

## 类描述

### mongoc_uri_native_ex

一个工具类，提供 MongoDB URI 的完整生命周期管理和操作方法。

#### 类型定义

##### `org_uri_type`

原始的 `mongoc_uri_t` 类型。

##### `yggr_uri_type`

YGGR 封装的 `yggr_mongoc_uri_t` 类型。

##### `inner_string_type`

内部字符串类型，使用 UTF-8 编码。

#### 常量定义

##### URI 方案常量

- `s_mongoc_uri_scheme()`：返回 `"mongodb://"`
- `s_mongoc_uri_scheme_srv_record()`：返回 `"mongodb+srv://"`
- `s_mongoc_default_uri()`：返回 `"mongodb://127.0.0.1/"`
- `s_mongoc_default_uri_srv_record()`：返回 `"mongodb+srv://127.0.0.1/"`

#### 初始化方法

##### `s_mongoc_uri_init`

初始化 URI 对象。有多个重载版本：

- `s_mongoc_uri_init(void* puri, bson_error_t* perr)`：使用默认 URI 初始化
- `s_mongoc_uri_init(void* puri, const char* host_name, u16 port, bson_error_t* perr)`：使用主机名和端口初始化
- `s_mongoc_uri_init(void* puri, const char* uri_scheme, const char* host_name, u16 port, bson_error_t* perr)`：使用自定义方案、主机名和端口初始化
- `s_mongoc_uri_init(void* puri, const char* uri_str, bson_error_t* perr)`：使用 URI 字符串初始化

#### 管理方法

##### `s_mongoc_uri_copy`

复制 URI 对象。

##### `s_mongoc_uri_swap`

交换两个 URI 对象。

##### `s_mongoc_uri_destroy`

销毁 URI 对象，释放所有资源。

##### `s_mongoc_uri_clear`

清除 URI 对象的内容，但不释放对象本身。

##### `s_mongoc_uri_empty`

检查 URI 对象是否为空。

#### 访问器方法

##### 主机相关

- `s_mongoc_uri_get_hosts`：获取主机列表
- `s_mongoc_uri_get_srv_hostname`：获取 SRV 主机名
- `s_mongoc_uri_get_srv_service_name`：获取 SRV 服务名

##### 数据库相关

- `s_mongoc_uri_get_database`：获取数据库名
- `s_mongoc_uri_set_database`：设置数据库名

##### 认证相关

- `s_mongoc_uri_get_username`：获取用户名
- `s_mongoc_uri_set_username`：设置用户名
- `s_mongoc_uri_get_password`：获取密码
- `s_mongoc_uri_set_password`：设置密码
- `s_mongoc_uri_get_auth_source`：获取认证源
- `s_mongoc_uri_set_auth_source`：设置认证源
- `s_mongoc_uri_get_auth_mechanism`：获取认证机制
- `s_mongoc_uri_set_auth_mechanism`：设置认证机制
- `s_mongoc_uri_get_mechanism_properties`：获取机制属性
- `s_mongoc_uri_set_mechanism_properties`：设置机制属性

##### 选项相关

- `s_mongoc_uri_get_options`：获取选项 BSON
- `s_mongoc_uri_has_option`：检查是否存在指定选项
- `s_mongoc_uri_option_is_int32`：检查选项是否为 int32 类型
- `s_mongoc_uri_option_is_int64`：检查选项是否为 int64 类型
- `s_mongoc_uri_option_is_bool`：检查选项是否为 bool 类型
- `s_mongoc_uri_option_is_utf8`：检查选项是否为 UTF-8 字符串类型

##### 选项获取方法

- `s_mongoc_uri_get_option_as_int32`：获取 int32 选项值
- `s_mongoc_uri_get_option_as_int64`：获取 int64 选项值
- `s_mongoc_uri_get_option_as_bool`：获取 bool 选项值
- `s_mongoc_uri_get_option_as_utf8`：获取 UTF-8 字符串选项值

##### 选项设置方法

- `s_mongoc_uri_set_option_as_int32`：设置 int32 选项值
- `s_mongoc_uri_set_option_as_int64`：设置 int64 选项值
- `s_mongoc_uri_set_option_as_bool`：设置 bool 选项值
- `s_mongoc_uri_set_option_as_utf8`：设置 UTF-8 字符串选项值

##### 其他属性

- `s_mongoc_uri_get_replica_set`：获取副本集名称
- `s_mongoc_uri_get_string`：获取完整的 URI 字符串
- `s_mongoc_uri_get_credentials`：获取凭据 BSON
- `s_mongoc_uri_get_appname`：获取应用名称
- `s_mongoc_uri_set_appname`：设置应用名称
- `s_mongoc_uri_set_compressors`：设置压缩器
- `s_mongoc_uri_get_compressors`：获取压缩器配置
- `s_mongoc_uri_get_tls`：获取 TLS 设置

##### 读写关注

- `s_mongoc_uri_get_read_prefs`：获取读偏好 BSON
- `s_mongoc_uri_get_read_prefs_t`：获取读偏好对象
- `s_mongoc_uri_set_read_prefs_t`：设置读偏好对象
- `s_mongoc_uri_get_write_concern`：获取写关注对象
- `s_mongoc_uri_set_write_concern`：设置写关注对象
- `s_mongoc_uri_get_read_concern`：获取读关注对象
- `s_mongoc_uri_set_read_concern`：设置读关注对象

#### 工具方法

##### `s_mongoc_uri_unescape`

对转义的字符串进行解码。

**参数：**
- `escaped_string`：转义的字符串

**返回值：**
- 解码后的字符串

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/charset/utf8_string.hpp>`
- `<yggr/nsql_database_system/mongodb_config.hpp>`
- `<boost/ref.hpp>`
- `<cassert>`
- `<sstream>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_uri_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 使用默认 URI 初始化
yggr_mongoc_uri_t uri;
bson_error_t error;
if (mongoc_uri_native_ex::s_mongoc_uri_init(&uri, &error)) {
    // 设置数据库
    mongoc_uri_native_ex::s_mongoc_uri_set_database(&uri, "testdb");
    
    // 设置认证信息
    mongoc_uri_native_ex::s_mongoc_uri_set_username(&uri, "user");
    mongoc_uri_native_ex::s_mongoc_uri_set_password(&uri, "password");
    
    // 设置选项
    mongoc_uri_native_ex::s_mongoc_uri_set_option_as_int32(&uri, "maxPoolSize", 10);
    
    // 获取完整 URI 字符串
    const char* uri_str = mongoc_uri_native_ex::s_mongoc_uri_get_string(&uri);
    std::cout << "URI: " << uri_str << std::endl;
    
    // 清理资源
    mongoc_uri_native_ex::s_mongoc_uri_destroy(&uri);
}

// 使用主机名和端口初始化
yggr_mongoc_uri_t uri2;
if (mongoc_uri_native_ex::s_mongoc_uri_init(&uri2, "localhost", 27017, &error)) {
    // 使用 URI 对象...
    mongoc_uri_native_ex::s_mongoc_uri_destroy(&uri2);
}

// 使用完整 URI 字符串初始化
yggr_mongoc_uri_t uri3;
const char* uri_string = "mongodb://user:pass@localhost:27017/testdb?maxPoolSize=10";
if (mongoc_uri_native_ex::s_mongoc_uri_init(&uri3, uri_string, &error)) {
    // 使用 URI 对象...
    mongoc_uri_native_ex::s_mongoc_uri_destroy(&uri3);
}
```

## 注意事项

- URI 格式遵循 MongoDB 连接字符串规范
- SRV 记录 URI（`mongodb+srv://`）用于自动发现副本集成员
- 标准 URI（`mongodb://`）需要手动指定所有主机
- 用户名和密码在 URI 中应该进行百分号编码
- 选项可以是连接池、超时、认证等各种设置
- 读偏好、读关注和写关注可以通过 URI 选项或单独对象设置
- 使用完 URI 对象后必须调用 `s_mongoc_uri_destroy` 释放资源
- 某些方法返回的指针可能为 NULL，需要检查
- 字符串编码使用 UTF-8
- 压缩器支持多种算法，如 snappy、zlib、zstd
- TLS 设置控制是否使用加密连接
- 应用名称有助于服务器端日志记录和调试
- 认证机制支持多种方式，如 SCRAM-SHA-1、X.509 等
- 机制属性用于特定认证机制的额外配置</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_uri_native_ex_cn.md