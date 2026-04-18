# c_mongo_server_description.hpp 文档

## 文件概述

`c_mongo_server_description.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB 服务器描述信息的 C++ 类。该类基于 MongoDB C 驱动的 `mongoc_server_description_t`，并提供智能指针管理、属性访问和类型信息检索。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 结构体与自定义删除器

### detail::mongoc_server_description_t_deleter

用于在 `shared_ptr` 中销毁 `mongoc_server_description_t` 对象：

- `operator()(mongoc_server_description_t* x) const`：如果指针非空，则调用 `mongoc_server_description_destroy(x)`。

## 类定义

### c_mongo_server_description

该类封装 `mongoc_server_description_t`，并使用 `yggr::shared_ptr` 管理底层对象生命周期。

#### 类型定义

- `org_type`：原始类型 `mongoc_server_description_t`。
- `mongoc_server_description_t_ptr_type`：原始类型的 `shared_ptr` 智能指针。
- `typeid_type`：服务器类型 ID 类型，来自 `mongoc_server_description_native_ex::typeid_type`。
- `bson_type`：BSON 类型 `c_bson`。
- `org_bson_type`：BSON 原始类型。
- `host_info_type`：主机信息类型 `c_mongo_host_info`。
- `inner_string_type`：内部字符串类型 `charset::utf8_string`。
- `inner_string_view_type`：内部字符串视图类型 `charset::utf8_string_view`。

#### 构造函数

- `c_mongo_server_description(void)`：默认构造函数。
- `c_mongo_server_description(mongoc_server_description_t* pdesc)`：使用原始描述对象指针构造。
- `c_mongo_server_description(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `c_mongo_server_description(const this_type& right)`：复制构造函数。

#### 析构函数

- `~c_mongo_server_description(void)`：析构函数。

#### 赋值运算符

- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值。
- `operator=(const this_type& right)`：复制赋值。

#### 交换函数

- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：交换函数。

#### 原始指针访问

- `org_pointer(void)`：返回底层 `mongoc_server_description_t*`。
- `org_pointer(void) const`：返回底层 `const mongoc_server_description_t*`。
- `operator org_type*(void)`：转换为原始类型指针。
- `operator const org_type*(void) const`：转换为原始类型常量指针。

#### 状态检查

- `empty(void) const`：检查是否没有底层描述对象。

#### 基本属性访问

- `id(void) const`：返回服务器描述 ID，若为空对象则返回 0。
- `last_update_time(void) const`：返回最后更新时间（微秒），若为空对象则返回 0。
- `round_trip_time(void) const`：返回往返时间（毫秒），若为空对象则返回 0。

#### 主机信息访问

- `host(void) const`：返回 `c_mongo_host_info` 类型的主机信息。

#### Hello 响应访问

- `hello_response(void) const`：返回引用形式的 `c_bson` Hello 响应对象。
- `hello_response<Bson>(void) const`：返回指定 BSON 类型的 Hello 响应对象。

#### 类型信息访问

- `typeid_value(void) const`：返回服务器类型 ID。
- `typeid_name(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const`：返回服务器类型名称字符串视图。

## 交换支持

在 `swap_support` 命名空间中定义了：

- `swap(c_mongo_server_description& l, c_mongo_server_description& r)`：调用 `l.swap(r)`。

并将其导出到 `yggr::nsql_database_system`、`std` 和 `boost` 命名空间。

## 依赖

- `yggr/base/yggrdef.h`
- `yggr/base/static_constant.hpp`
- `yggr/move/move.hpp`
- `yggr/charset/utf8_string.hpp`
- `yggr/charset/utf8_string_view.hpp`
- `yggr/smart_ptr_ex/shared_ptr.hpp`
- `yggr/nsql_database_system/c_bson.hpp`
- `yggr/nsql_database_system/mongoc_server_description_native_ex.hpp`
- `yggr/nsql_database_system/c_mongo_host_info.hpp`
- `yggr/nsql_database_system/detail/string_member_var_get.hpp`

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。