# c_mongo_index_model.hpp 文档

## 文件概述

`c_mongo_index_model.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB 索引模型的 C++ 类。该类提供了对 MongoDB 索引键和选项的包装，支持创建和管理索引模型。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 类定义

### c_mongo_index_model

该类继承自 `yggr_mongoc_index_model_t`，用于管理 MongoDB 索引的键和选项。它支持从各种 BSON 类型构造，并提供对索引模型的访问和修改。

#### 类型定义

- `base_type`：基类 `yggr_mongoc_index_model_t`。
- `org_type`：原始类型 `mongoc_index_model_t`。
- `bson_type`：BSON 类型 `c_bson`。
- `bson_member_modify_helper_type`：BSON 成员修改助手类型。

#### 构造函数

- `c_mongo_index_model(void)`：默认构造函数。
- `c_mongo_index_model(BOOST_RV_REF(bson_t) k)`：从 BSON 键移动构造。
- `c_mongo_index_model(const bson_t& k)`：从 BSON 键构造。
- `c_mongo_index_model(BOOST_RV_REF(c_bson) k)`：从 c_bson 键移动构造。
- `c_mongo_index_model(const c_bson& k)`：从 c_bson 键构造。
- `c_mongo_index_model(BOOST_RV_REF(Bson1) k, BOOST_RV_REF(Bson2) o)`：从键和选项移动构造（模板）。
- `c_mongo_index_model(BOOST_RV_REF(Bson1) k, const Bson2& o)`：从键移动和选项构造（模板）。
- `c_mongo_index_model(const Bson1& k, BOOST_RV_REF(Bson2) o)`：从键和选项移动构造（模板）。
- `c_mongo_index_model(const Bson1& k, const Bson2& o)`：从键和选项构造（模板）。
- `c_mongo_index_model(const org_type* right)`：从原始类型指针构造。
- `c_mongo_index_model(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `c_mongo_index_model(const this_type& right)`：复制构造函数。

#### 析构函数

- `~c_mongo_index_model(void)`：析构函数。

#### 赋值运算符

- `operator=(const org_type* right)`：从原始类型指针赋值。
- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值。
- `operator=(const this_type& right)`：复制赋值。

#### 交换函数

- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：交换函数。

#### 类型转换运算符

- `operator org_type*(void)`：转换为原始类型指针。
- `operator const org_type*(void) const`：转换为常量原始类型指针。

#### 赋值函数

- `assign(BOOST_RV_REF(Bson) k)`：移动赋值键（模板）。
- `assign(const Bson& k)`：赋值键（模板）。
- `assign(BOOST_RV_REF(Bson1) k, BOOST_RV_REF(Bson2) o)`：移动赋值键和选项（模板）。
- `assign(BOOST_RV_REF(Bson1) k, const Bson2& o)`：移动赋值键，赋值选项（模板）。
- `assign(const Bson1& k, BOOST_RV_REF(Bson2) o)`：赋值键，移动赋值选项（模板）。
- `assign(const Bson1& k, const Bson2& o)`：赋值键和选项（模板）。
- `assign(const org_type* right)`：从原始类型指针赋值。
- `assign(BOOST_RV_REF(this_type) right)`：移动赋值。
- `assign(const this_type& right)`：复制赋值。

#### 成员访问函数

- `var_keys(void)`：返回键的修改助手。
- `var_keys<Bson>(void)`：返回指定类型的键修改助手（模板）。
- `var_keys(void) const`：获取键的常量引用。
- `var_keys<Bson>(void) const`：获取指定类型的键常量引用（模板）。
- `var_opts(void)`：返回选项的修改助手。
- `var_opts<Bson>(void)`：返回指定类型的选项修改助手（模板）。
- `var_opts(void) const`：获取选项的常量引用。
- `var_opts<Bson>(void) const`：获取指定类型的选项常量引用（模板）。

#### 其他函数

- `org_pointer(void)`：返回原始类型指针。
- `org_pointer(void) const`：返回常量原始类型指针。
- `empty(void) const`：检查索引模型是否为空。

#### 保护成员函数

- `pro_rebuild_base(void)`：重建基类指针。
- `pro_get_var_keys(void) const`：获取键（模板）。
- `pro_set_var_keys(const bson_t& k)`：设置键。
- `pro_get_var_opts(void) const`：获取选项（模板）。
- `pro_set_var_opts(const bson_t& o)`：设置选项。

#### 私有成员变量

- `bson_type _keys`：索引键。
- `bson_type _opts`：索引选项。

## 交换支持

在 `swap_support` 命名空间中定义了交换函数，支持与标准库和 Boost 库的交换操作。

## 依赖

- `yggr/base/yggrdef.h`：基础定义。
- `yggr/move/move.hpp`：移动语义。
- `yggr/mplex/tag_sfinae.hpp`：SFINAE 标签。
- `yggr/utility/swap.hpp`：交换工具。
- `yggr/utility/member_var_modify_helper.hpp`：成员变量修改助手。
- `yggr/nsql_database_system/c_bson.hpp`：BSON 类。
- `yggr/nsql_database_system/mongoc_index_model_native_ex.hpp`：MongoDB 索引模型扩展。
- `<boost/ref.hpp>`：Boost 引用。
- `<boost/utility/enable_if.hpp>`：Boost 启用条件。
- `<boost/type_traits/is_base_of.hpp>`：Boost 类型特征。
- `<boost/mpl/and.hpp>`：Boost MPL 与操作。

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。