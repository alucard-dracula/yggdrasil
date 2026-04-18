# c_mongo_index_description.hpp 文档

## 文件概述

`c_mongo_index_description.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB 索引描述信息的 C++ 类。该类提供了对 MongoDB 索引元数据的访问和管理，包括索引版本、键、名称和命名空间等。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 类定义

### c_mongo_index_description

该类用于存储和操作 MongoDB 索引的描述信息。它支持从 BSON 对象构造，并提供对索引各个字段的访问。

#### 类型定义

- `string_type`：字符串类型，定义为 `::yggr::string`。
- `index_key_type`：索引键类型，定义为 `std::pair<string_type, s32>`，表示键名和排序顺序。
- `bson_type`：BSON 类型，定义为 `c_bson`。

#### 构造函数

- `c_mongo_index_description(void)`：默认构造函数。
- `c_mongo_index_description(const bson_type& right)`：从 BSON 对象构造。
- `c_mongo_index_description(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `c_mongo_index_description(const this_type& right)`：复制构造函数。

#### 析构函数

- `~c_mongo_index_description(void)`：析构函数。

#### 赋值运算符

- `operator=(const bson_type& right)`：从 BSON 对象赋值。
- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值。
- `operator=(const this_type& right)`：复制赋值。

#### 交换函数

- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：交换函数。

#### 成员访问函数

- `v(void)`：返回索引版本的引用。
- `v(void) const`：获取索引版本。
- `index_order(void)`：返回索引键和顺序的引用。
- `index_key(void) const`：获取索引键和顺序的常量引用。
- `name(void)`：返回索引名称的引用。
- `name(void) const`：获取索引名称。
- `ns(void)`：返回命名空间的引用。
- `ns(void) const`：获取命名空间。
- `index_key_name(void) const`：获取索引键名。
- `index_key_order(void) const`：获取索引键的排序顺序。

#### 静态函数

- `s_v_name(void)`：返回版本字段名 "v"。
- `s_key_name(void)`：返回键字段名 "key"。
- `s_name_name(void)`：返回名称字段名 "name"。
- `s_ns_name(void)`：返回命名空间字段名 "ns"。

#### 私有成员函数

- `prv_parse_bson(const bson_type& bs)`：解析 BSON 对象以填充成员变量。

#### 私有成员变量

- `s32 _v`：索引版本。
- `index_key_type _idx_key`：索引键和排序顺序。
- `string_type _name`：索引名称。
- `string_type _ns`：命名空间。

## 交换支持

在 `swap_support` 命名空间中定义了交换函数，支持与标准库和 Boost 库的交换操作。

## 依赖

- `yggr/base/yggrdef.h`：基础定义。
- `yggr/utility/copy_or_move_or_swap.hpp`：复制、移动或交换工具。
- `yggr/ppex/swap_this_def.hpp`：交换定义宏。
- `yggr/charset/utf8_string.hpp`：UTF-8 字符串（间接依赖）。
- `yggr/nsql_database_system/c_bson.hpp`：BSON 类。
- `<utility>`：标准库工具。

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。