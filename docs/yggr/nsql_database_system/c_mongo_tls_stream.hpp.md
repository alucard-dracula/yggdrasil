# c_mongo_tls_stream.hpp 文档

## 文件概述

`c_mongo_tls_stream.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB TLS 流的 C++ 类。该类继承自 `c_mongo_basic_stream`，用于创建和管理 MongoDB TLS 加密流，并提供握手接口。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 类定义

### c_mongo_tls_stream

该类继承自 `c_mongo_basic_stream`，用于包装 MongoDB C 驱动的 TLS 流对象，并提供构造、移动、交换以及 TLS 握手操作。

#### 类型定义

- `base_type`：基类 `c_mongo_basic_stream`。
- `org_type`：原始类型，继承自基类定义。
- `base_org_type`：基类原始类型。
- `stream_ptr_type`：基类的流指针类型。
- `org_bson_type`：原始 BSON 类型 `bson_t`。
- `org_bson_error_type`：原始 BSON 错误类型 `bson_error_t`。

#### 构造函数

- `c_mongo_tls_stream(void)`：默认构造函数。
- `c_mongo_tls_stream(base_org_type* pstream)`：使用基类原始流指针构造。
- `c_mongo_tls_stream(base_org_type* pstream, bool need_del)`：使用基类原始流指针并指定是否需要删除。
- `c_mongo_tls_stream(base_org_type* porg_base_stream, const char* str_host, mongoc_ssl_opt_t* pssl_opt, int client)`：通过原始基类流、主机名、SSL 选项和客户端标志构造。
- `c_mongo_tls_stream(base_org_type* porg_base_stream, const String& str_host, const MongocSSLOpt& ssl_opt, int client)`：模板构造，支持任意可转换为 UTF-8 的主机名字符串类型。
- `c_mongo_tls_stream(BOOST_RV_REF(this_type) right)`：移动构造函数。

#### 析构函数

- `~c_mongo_tls_stream(void)`：析构函数。

#### 赋值运算符

- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值运算符。

#### 交换函数

- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：交换函数。

#### 握手接口

- `handshake(const String& str_host, int32_t timeout_msec, int& events, org_bson_error_type* perr = 0)`：异步握手操作，返回是否成功。
- `handshake(const String& host, int32_t timeout_msec, int& events, org_bson_error_type& err)`：异步握手操作，错误信息通过引用返回。
- `handshake_block(const String& str_host, int32_t timeout_msec, org_bson_error_type* perr = 0)`：阻塞握手操作，返回是否成功。
- `handshake_block(const String& host, int32_t timeout_msec, org_bson_error_type& err)`：阻塞握手操作，错误信息通过引用返回。

#### 显式类型要求

模板接口仅接受可转换为 UTF-8 的字符串类型。

## 交换支持

在 `swap_support` 命名空间中定义了 `YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_tls_stream)`，并导出到 `yggr::nsql_database_system`、`std` 和 `boost` 命名空间。

## 依赖

- `yggr/base/yggrdef.h`
- `yggr/charset/utf8_string.hpp`
- `yggr/mplex/tag_sfinae.hpp`
- `yggr/nsql_database_system/c_mongo_basic_stream.hpp`
- `<boost/utility/enable_if.hpp>`
- `<boost/type_traits/is_base_of.hpp>`

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。