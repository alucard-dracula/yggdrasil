# mongoc_native_err_ex.hpp

## 概述

此头文件定义了 `mongoc_error_domain_ex_t` 和 `mongoc_error_code_ex_t` 两个结构体，这些结构体包含了 MongoDB C 驱动程序中所有错误域和错误代码的静态常量定义。这些定义用于错误处理和诊断 MongoDB 操作中的各种错误情况。

## 命名空间

- `yggr::nsql_database_system`

## 结构体描述

### mongoc_error_domain_ex_t

错误域结构体，定义了 MongoDB 各种组件和操作的错误分类。

#### 主要错误域常量

- `E_MONGOC_ERROR_CLIENT`：客户端相关错误
- `E_MONGOC_ERROR_STREAM`：网络流相关错误
- `E_MONGOC_ERROR_PROTOCOL`：协议相关错误
- `E_MONGOC_ERROR_CURSOR`：游标相关错误
- `E_MONGOC_ERROR_QUERY`：查询相关错误
- `E_MONGOC_ERROR_COMMAND`：命令相关错误
- `E_MONGOC_ERROR_COLLECTION`：集合相关错误
- `E_MONGOC_ERROR_GRIDFS`：GridFS相关错误
- `E_MONGOC_ERROR_TRANSACTION`：事务相关错误
- `E_MONGOC_ERROR_CLIENT_SIDE_ENCRYPTION`：客户端加密相关错误
- 以及其他特定领域的错误域...

### mongoc_error_code_ex_t

错误代码结构体，定义了具体的错误代码值。

#### 主要错误代码分类

##### 流相关错误
- `E_MONGOC_ERROR_STREAM_INVALID_TYPE`：无效的流类型
- `E_MONGOC_ERROR_STREAM_INVALID_STATE`：无效的流状态
- `E_MONGOC_ERROR_STREAM_CONNECT`：连接失败
- 等...

##### 客户端相关错误
- `E_MONGOC_ERROR_CLIENT_NOT_READY`：客户端未就绪
- `E_MONGOC_ERROR_CLIENT_AUTHENTICATE`：认证失败
- 等...

##### 协议和查询错误
- `E_MONGOC_ERROR_PROTOCOL_INVALID_REPLY`：无效的协议回复
- `E_MONGOC_ERROR_QUERY_FAILURE`：查询失败
- 等...

##### 集合操作错误
- `E_MONGOC_ERROR_COLLECTION_INSERT_FAILED`：插入失败
- `E_MONGOC_ERROR_COLLECTION_UPDATE_FAILED`：更新失败
- `E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST`：集合不存在
- 等...

##### GridFS错误
- `E_MONGOC_ERROR_GRIDFS_INVALID_FILENAME`：无效的文件名
- `E_MONGOC_ERROR_GRIDFS_CHUNK_MISSING`：块缺失
- 等...

##### 事务和会话错误
- `E_MONGOC_ERROR_TRANSACTION_INVALID_STATE`：无效的事务状态
- `E_MONGOC_ERROR_CLIENT_SESSION_FAILURE`：客户端会话失败
- 等...

##### 加密相关错误
- `E_MONGOC_ERROR_CLIENT_INVALID_ENCRYPTION_STATE`：无效的加密状态
- 等...

##### 自定义扩展错误
- `E_MONGOC_ERROR_CODE_USER_EX`：用户扩展错误起始值
- `E_MONGOC_ERROR_EMPTY_OBJECT`：空对象错误
- `E_MONGOC_ERROR_DATABASE_DOES_NOT_EXIST`：数据库不存在
- `E_MONGOC_ERROR_THREAD_PANIC`：线程恐慌

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/base/static_constant.hpp>`
- `<yggr/nsql_database_system/mongodb_config.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_native_err_ex.hpp>

using namespace yggr::nsql_database_system;

// 检查错误域
if (error.domain == mongoc_error_domain_ex_t::E_MONGOC_ERROR_COLLECTION) {
    // 处理集合相关错误
}

// 检查错误代码
if (error.code == mongoc_error_code_ex_t::E_MONGOC_ERROR_COLLECTION_DOES_NOT_EXIST) {
    // 处理集合不存在的错误
}
```

## 注意事项

- 所有错误常量都使用 `YGGR_STATIC_CONSTANT` 宏定义，确保编译时常量折叠
- 这些常量直接映射到 MongoDB C 驱动程序的错误定义
- 错误域用于分类错误类型，错误代码用于标识具体的错误情况
- 文件包含了大量的错误常量，涵盖了 MongoDB 操作的各个方面
- 自定义扩展错误 (`E_MONGOC_ERROR_*_USER_EX`) 用于项目特定的错误处理
- 使用这些常量可以进行精确的错误处理和诊断
- 某些错误代码可能依赖于 MongoDB C 驱动程序的版本（如 `MONGOC_ERROR_API_VERSION_2`）</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_native_err_ex_cn.md