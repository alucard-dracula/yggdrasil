# c_mongo_host_info.hpp 文档

## 文件概述

`c_mongo_host_info.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB 主机信息的 C++ 类。该类提供了对 MongoDB 主机列表结构的包装，支持主机名、端口、地址族等信息的访问和修改。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 结构体定义

### yggr_mongoc_host_info

该结构体模拟 MongoDB C 驱动的 `mongoc_host_list_t` 结构，用于存储主机信息。

#### 成员变量

- `yggr_mongoc_host_info* next`：指向下一个主机信息的指针。
- `char host[YGGR_BSON_HOST_NAME_MAX + 1]`：主机名字符串。
- `char host_and_port[YGGR_BSON_HOST_NAME_MAX + 7]`：主机名和端口的组合字符串。
- `uint16_t port`：端口号。
- `int family`：地址族（例如 AF_INET）。
- `void* padding[4]`：填充字节，用于内存对齐。

## 类定义

### c_mongo_host_info

该类继承自 `yggr_mongoc_host_info`，提供了对主机信息的封装和管理。它支持复制和移动操作。

#### 类型定义

- `base_type`：基类 `yggr_mongoc_host_info`。
- `org_type`：原始类型 `mongoc_host_list_t`。
- `inner_string_type`：内部字符串类型 `utf8_string`。
- `inner_string_view_type`：内部字符串视图类型 `utf8_string_view`。

#### 构造函数

- `c_mongo_host_info(void)`：默认构造函数。
- `c_mongo_host_info(const StrHost& str_host, uint16_t nport, int nfamily)`：使用主机名、端口和地址族构造对象。
- `c_mongo_host_info(BOOST_RV_REF(org_type) right)`：从原始类型移动构造。
- `c_mongo_host_info(const org_type& right)`：从原始类型复制构造。
- `c_mongo_host_info(const org_type* right)`：从原始类型指针构造。
- `c_mongo_host_info(BOOST_RV_REF(base_type) right)`：从基类移动构造。
- `c_mongo_host_info(const base_type& right)`：从基类复制构造。
- `c_mongo_host_info(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `c_mongo_host_info(const this_type& right)`：复制构造函数。

#### 析构函数

- `~c_mongo_host_info(void)`：析构函数。

#### 赋值运算符

- `operator=(BOOST_RV_REF(org_type) right)`：从原始类型移动赋值。
- `operator=(const org_type& right)`：从原始类型赋值。
- `operator=(BOOST_RV_REF(base_type) right)`：从基类移动赋值。
- `operator=(const base_type& right)`：从基类赋值。
- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值。
- `operator=(const this_type& right)`：复制赋值。

#### 交换函数

- `swap(BOOST_RV_REF(org_type) right)`：与原始类型交换。
- `swap(org_type& right)`：与原始类型交换。
- `swap(BOOST_RV_REF(base_type) right)`：与基类交换。
- `swap(base_type& right)`：与基类交换。
- `swap(BOOST_RV_REF(this_type) right)`：与同类型交换。
- `swap(this_type& right)`：与同类型交换。

#### 成员访问函数

- `var_host(void)`：返回主机名的修改助手。
- `var_host(const string& ret_charset_name) const`：获取主机名字符串视图。
- `var_host_and_port(const string& ret_charset_name) const`：获取主机名和端口的组合字符串视图。
- `var_port(void)`：返回端口的修改助手。
- `var_port(void) const`：获取端口号。
- `var_family(void)`：返回地址族的引用。
- `var_family(void) const`：获取地址族。
- `var_next(void)`：返回下一个主机信息的指针引用。
- `var_next(void) const`：获取下一个主机信息的常量指针。
- `var_next<T>(void)`：模板函数，返回指定类型的下一个指针。

#### 保护成员函数

- `pro_set_host_and_port(const StrHost& str_host, uint16_t nport)`：设置主机名和端口。
- `pro_get_copy_begin(void* pobj)`：获取复制开始位置的指针。

## 非成员函数

### 输出运算符

- `operator<<(std::basic_ostream<Char, Traits>& os, const c_mongo_host_info& val)`：将主机信息输出到流中，格式为 JSON 样式。

### 交换支持

在 `swap_support` 命名空间中定义了多个 `swap` 函数，用于支持与不同类型的主机信息交换。

## 依赖

- `yggr/base/yggrdef.h`：基础定义。
- `yggr/base/static_constant.hpp`：静态常量。
- `yggr/utility/member_var_modify_helper.hpp`：成员变量修改助手。
- `yggr/move/move.hpp`：移动语义。
- `yggr/mplex/static_assert.hpp`：静态断言。
- `yggr/mplex/tag_sfinae.hpp`：SFINAE 标签。
- `yggr/charset/utf8_string.hpp`：UTF-8 字符串。
- `yggr/charset/utf8_string_view.hpp`：UTF-8 字符串视图。
- `yggr/nsql_database_system/mongodb_config.hpp`：MongoDB 配置。
- `yggr/nsql_database_system/bson_visit_t.hpp`：BSON 访问器。
- `yggr/nsql_database_system/detail/string_member_var_get.hpp`：字符串成员变量获取。
- `<boost/mpl/bool.hpp>`：Boost MPL 布尔。
- `<ostream>`：标准输出流。

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。