# c_mongo_index_opt_wt.hpp 文档

## 文件概述

`c_mongo_index_opt_wt.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB WiredTiger 索引选项的 C++ 类。该类提供了对 MongoDB WiredTiger 存储引擎索引配置的包装，包括基础存储选项和配置字符串。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 结构体定义

### mongoc_index_opt_wt_t

该结构体是 MongoDB C 驱动的 WiredTiger 索引选项结构体，包含以下成员：

- `mongoc_index_opt_storage_t base`：基础存储选项。
- `const char *config_str`：配置字符串。
- `void *padding[8]`：填充字节。

## 类定义

### c_mongo_index_opt_wt

该类继承自 `mongoc_index_opt_wt_t`，提供了对 WiredTiger 索引选项的封装和管理。它支持复制和移动操作。

#### 类型定义

- `base_type`：基类 `mongoc_index_opt_wt_t`。
- `org_type`：原始类型 `mongoc_index_opt_wt_t`。
- `index_opt_storage_type`：索引存储选项类型 `mongoc_index_opt_storage_t`。
- `inner_string_type`：内部字符串类型 `yggr::utf8_string`。
- `string_type`：字符串类型，定义为 `inner_string_type`。
- `string_member_modify_helper_type`：字符串成员修改助手类型。

#### 构造函数

- `c_mongo_index_opt_wt(void)`：默认构造函数。
- `c_mongo_index_opt_wt(const base_type* ptr)`：从基类指针构造。
- `c_mongo_index_opt_wt(const base_type& right)`：从基类引用构造。
- `c_mongo_index_opt_wt(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `c_mongo_index_opt_wt(const this_type& right)`：复制构造函数。

#### 析构函数

- `~c_mongo_index_opt_wt(void)`：析构函数。

#### 赋值运算符

- `operator=(const base_type& right)`：从基类赋值。
- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值。
- `operator=(const this_type& right)`：复制赋值。

#### 交换函数

- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：交换函数。

#### 类型转换函数

- `org_pointer(void)`：返回原始类型指针。
- `org_pointer(void) const`：返回常量原始类型指针。

#### 成员访问函数

- `var_index_opt_storage(void)`：返回基础存储选项的引用。
- `var_index_opt_storage(void) const`：获取基础存储选项的常量引用。
- `var_config_str(void)`：返回配置字符串的修改助手。
- `var_config_str(void) const`：获取配置字符串的常量引用。

#### 私有成员函数

- `get_var_config_str(void) const`：获取配置字符串。
- `set_var_config_str(const inner_string_type& cfg_str)`：设置配置字符串。
- `prv_init(void)`：私有初始化。
- `prv_copy(const base_type* ptr)`：从指针复制。
- `prv_copy(const this_type& right)`：复制。
- `prv_swap(this_type& right)`：私有交换。
- `pro_move(this_type& right)`：移动。

#### 私有成员变量

- `inner_string_type _config_str`：配置字符串。

## 交换支持

在 `swap_support` 命名空间中定义了交换函数，支持与标准库和 Boost 库的交换操作。

## 依赖

- `yggr/charset/utf8_string.hpp`：UTF-8 字符串。
- `yggr/move/move.hpp`：移动语义。
- `yggr/ppex/swap_this_def.hpp`：交换定义宏。
- `yggr/utility/member_var_modify_helper.hpp`：成员变量修改助手。
- `yggr/nsql_database_system/mongoc_index_opt_wt_native_ex.hpp`：MongoDB WiredTiger 索引选项扩展。

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。