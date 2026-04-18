# c_mongo_server_api.hpp 文档

## 文件概述

`c_mongo_server_api.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB Server API 配置的 C++ 类。该类包装了 MongoDB 原始 `mongoc_server_api_t` 结构，并提供对版本、严格模式和废弃错误选项的访问与修改。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 结构体模拟

原始结构体 `mongoc_server_api_t` 的注释定义如下：

- `mongoc_server_api_version_t version`：Server API 版本。
- `mongoc_optional_t strict`：严格模式可选值。
- `mongoc_optional_t deprecation_errors`：废弃错误可选值。

同样，库内定义了 `yggr_mongoc_server_api_t` 作为相同结构的替代类型。

## 类定义

### c_mongo_server_api

该类继承自 `yggr_mongoc_server_api_t`，用于管理 MongoDB Server API 的设置。它支持复制和移动语义，并提供对原始对象指针和内部成员的安全访问。

#### 类型定义

- `base_type`：基类 `yggr_mongoc_server_api_t`。
- `org_type`：原始类型 `mongoc_server_api_t`。
- `version_type`：Server API 版本类型，等价于 `YGGR_PP_UINTEGER_T_TYPE(sizeof(mongoc_server_api_version_t))`。
- `org_version_type`：原始版本类型 `mongoc_server_api_version_t`。
- `org_mongoc_optional_type`：MongoDB 可选类型 `mongoc_optional_t`。
- `bool_member_modify_helper_type`：布尔成员修改助手类型。

#### 常量

- `E_MONGOC_SERVER_API_V1`：Server API 版本 V1。

#### 构造函数

- `c_mongo_server_api(void)`：默认构造函数。
- `c_mongo_server_api(version_type ver, bool bstrict = false, bool berrors = false)`：使用版本、严格模式和废弃错误选项构造。
- `c_mongo_server_api(const org_type* right)`：从原始指针构造。
- `c_mongo_server_api(const org_type& right)`：从原始对象构造。
- `c_mongo_server_api(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `c_mongo_server_api(const this_type& right)`：复制构造函数。

#### 析构函数

- `~c_mongo_server_api(void)`：析构函数。

#### 赋值运算符

- `operator=(const org_type& right)`：从原始对象赋值。
- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值。
- `operator=(const this_type& right)`：复制赋值。

#### 交换函数

- `swap(org_type& right)`：与原始对象交换。
- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：与同类型交换。

#### 原始指针访问

- `org_pointer(void)`：返回原始类型指针。
- `org_pointer(void) const`：返回原始类型常量指针。
- `operator org_type&(void)`：转换为原始类型引用。
- `operator const org_type&(void) const`：转换为原始类型常量引用。
- `operator org_type*(void)`：转换为原始类型指针。
- `operator const org_type*(void) const`：转换为原始类型常量指针。

#### 成员访问和修改

- `var_version(void)`：返回版本号引用。
- `var_version(void) const`：获取版本号。

- `var_strict(void)`：返回 `strict` 可选值引用。
- `var_strict(void) const`：获取 `strict` 可选值。
- `strict_value(void)`：获取 `strict` 布尔值。
- `strict_value(void) const`：获取 `strict` 布尔值。
- `strict_is_set(void) const`：检查是否显式设置了 `strict`。

- `var_deprecation_errors(void)`：返回 `deprecation_errors` 可选值引用。
- `var_deprecation_errors(void) const`：获取 `deprecation_errors` 可选值。
- `deprecation_errors_value(void)`：获取 `deprecation_errors` 布尔值。
- `deprecation_errors_value(void) const`：获取 `deprecation_errors` 布尔值。
- `deprecation_errors_is_set(void) const`：检查是否显式设置了 `deprecation_errors`。

#### 内部实现函数

- `prv_init(void)`：私有初始化函数。
- `pro_copy(const base_type& right)`：从基类复制。
- `pro_copy(const this_type& right)`：从同类型复制。
- `pro_swap(base_type& right)`：与基类交换。
- `pro_swap(this_type& right)`：与同类型交换。

## 交换支持

在 `swap_support` 命名空间中定义了多个 `swap` 函数，支持与原始类型和类自身之间的交换，并将这些函数导出到 `yggr::nsql_database_system`、`std` 和 `boost` 命名空间。

## 依赖

- `yggr/base/yggrdef.h`
- `yggr/base/static_constant.hpp`
- `yggr/move/move.hpp`
- `yggr/type_traits/number_t.hpp`
- `yggr/mplex/static_assert.hpp`
- `yggr/utility/member_var_modify_helper.hpp`
- `yggr/nsql_database_system/mongodb_config.hpp`
- `yggr/nsql_database_system/detail/incomplete t_move_patch_macro.hpp`
- `<boost/ref.hpp>`

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。