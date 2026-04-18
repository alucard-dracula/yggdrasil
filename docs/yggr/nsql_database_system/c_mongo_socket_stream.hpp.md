# c_mongo_socket_stream.hpp 文档

## 文件概述

`c_mongo_socket_stream.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB Socket 流的 C++ 类。该类继承自 `c_mongo_basic_stream`，用于管理与 MongoDB C 驱动 `mongoc_stream_socket_t` 相关的流对象。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 类定义

### c_mongo_socket_stream

该类继承自 `c_mongo_basic_stream`，用于包装 MongoDB Socket 流对象，并提供构造、移动、交换和底层套接字访问。

#### 类型定义

- `base_type`：基类 `c_mongo_basic_stream`。
- `org_type`：底层原始类型 `mongoc_stream_socket_t`。
- `base_org_type`：基类的原始类型。
- `stream_ptr_type`：基类定义的流智能指针类型。

#### 构造函数

- `c_mongo_socket_stream(void)`：默认构造函数。
- `c_mongo_socket_stream(base_org_type* pstream)`：通过基类原始流指针构造。
- `c_mongo_socket_stream(base_org_type* pstream, bool need_del)`：通过基类原始流指针构造，并指定是否需要删除。
- `c_mongo_socket_stream(mongoc_socket_t* psocket)`：通过底层 MongoDB 套接字构造。
- `c_mongo_socket_stream(BOOST_RV_REF(this_type) right)`：移动构造函数。

#### 析构函数

- `~c_mongo_socket_stream(void)`：析构函数。

#### 赋值运算符

- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值运算符。

#### 交换函数

- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：交换函数。

#### 原始指针访问

- `org_pointer(void) const`：返回指向底层 `mongoc_stream_socket_t` 的指针。
- `operator org_type*(void) const`：转换为底层 `mongoc_stream_socket_t*` 指针。

#### 套接字访问

- `socket(void) const`：返回底层 `mongoc_socket_t*` 套接字指针；如果对象为空则返回 `0`。

## 交换支持

在 `swap_support` 命名空间中定义了 `YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_socket_stream)`，并在 `yggr::nsql_database_system`、`std` 和 `boost` 命名空间中导出 `swap` 函数。

## 依赖

- `yggr/ppex/swap_this_def.hpp`
- `yggr/nsql_database_system/c_mongo_basic_stream.hpp`

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。