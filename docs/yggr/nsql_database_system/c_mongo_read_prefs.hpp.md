# c_mongo_read_prefs.hpp 文档

## 文件概述

`c_mongo_read_prefs.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB 读取偏好（Read Preferences）的 C++ 类。该类包装了 MongoDB 原始 `mongoc_read_prefs_t` 结构，并提供了模式、标签、延迟和 hedge 等读取偏好设置的访问与修改。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 结构体模拟

`mongoc_read_prefs_t` 的注释定义如下：

- `mongoc_read_mode_t mode`：读取模式。
- `bson_t tags`：标签集合。
- `int64_t max_staleness_seconds`：最大陈旧时间（秒）。
- `bson_t hedge`：hedge 选项。

## 类定义

### c_mongo_read_prefs

该类继承自 `yggr_mongoc_read_prefs`，用于在 C++ 中管理 MongoDB 读取偏好。它支持复制和移动语义，并提供对原始结构的安全访问。

#### 类型定义

- `base_type`：基类 `yggr_mongoc_read_prefs`。
- `org_type`：原始类型 `mongoc_read_prefs_t`。
- `bson_type`：BSON 类型 `c_bson`。
- `mode_type`：读取模式类型，来自 `mongoc_read_prefs_native_ex::mode_type`。
- `max_staleness_seconds_type`：最大陈旧时间类型 `s64`。

#### 常量定义

- `E_MONGOC_NO_MAX_STALENESS`：表示不设最大陈旧时间。
- `E_MONGOC_SMALLEST_MAX_STALENESS_SECONDS`：最小可用最大陈旧时间。
- `E_MONGOC_READ_INVALIDATE`：读取模式：Invalidate。
- `E_MONGOC_READ_PRIMARY`：读取模式：Primary。
- `E_MONGOC_READ_SECONDARY`：读取模式：Secondary。
- `E_MONGOC_READ_PRIMARY_PREFERRED`：读取模式：Primary Preferred。
- `E_MONGOC_READ_SECONDARY_PREFERRED`：读取模式：Secondary Preferred。
- `E_MONGOC_READ_NEAREST`：读取模式：Nearest。

#### 构造函数

- `c_mongo_read_prefs(void)`：默认构造函数。
- `explicit c_mongo_read_prefs(mode_type mode)`：使用读取模式构造。
- `explicit c_mongo_read_prefs(const org_type* read_prefs)`：从原始指针构造。
- `c_mongo_read_prefs(const org_type& right)`：从原始对象构造。
- `c_mongo_read_prefs(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `c_mongo_read_prefs(const this_type& right)`：复制构造函数。

#### 析构函数

- `~c_mongo_read_prefs(void)`：析构函数。

#### 赋值运算符

- `operator=(const org_type& right)`：从原始对象赋值。
- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值。
- `operator=(const this_type& right)`：复制赋值。

#### 交换函数

- `swap(org_type&)`：与原始对象交换。
- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：与同类型交换。

#### 原始指针访问

- `org_pointer(void)`：返回原始类型指针。
- `org_pointer(void) const`：返回常量原始类型指针。
- `operator org_type&(void)`：转换为原始类型引用。
- `operator const org_type&(void) const`：转换为常量原始类型引用。
- `operator org_type*(void)`：转换为原始类型指针。
- `operator const org_type*(void) const`：转换为常量原始类型指针。

#### 验证函数

- `validate(void) const`：检查读取偏好对象是否合法。

#### 成员访问和修改

- `var_mode(void)`：返回读取模式的引用。
- `var_mode(void) const`：获取读取模式。
- `var_tags(void)`：返回标签 BSON 的引用。
- `var_tags<Bson>(void)`：返回指定 BSON 类型的标签引用。
- `var_tags(void) const`：获取标签 BSON 的常量引用。
- `var_tags<Bson>(void) const`：返回指定 BSON 类型的标签常量引用。
- `add_tag(const Bson& tag)`：添加标签。
- `var_max_staleness_seconds(void)`：返回最大陈旧时间的引用。
- `var_max_staleness_seconds(void) const`：获取最大陈旧时间。
- `var_hedge(void)`：返回 hedge BSON 的引用。
- `var_hedge<Bson>(void)`：返回指定 BSON 类型的 hedge 引用。
- `var_hedge(void) const`：获取 hedge BSON 的常量引用。
- `var_hedge<Bson>(void) const`：返回指定 BSON 类型的 hedge 常量引用。

## 输出支持

提供 `operator<<` 输出重载，将读取偏好对象以 JSON 风格输出。

## 交换支持

在 `swap_support` 命名空间中定义了 `swap(c_mongo_read_prefs&, c_mongo_read_prefs&)`，并导出到 `yggr::nsql_database_system`、`std` 和 `boost`。

## 依赖

- `yggr/base/yggrdef.h`
- `yggr/mplex/static_assert.hpp`
- `yggr/move/move.hpp`
- `yggr/nsql_database_system/mongoc_read_prefs_native_ex.hpp`
- `yggr/nsql_database_system/c_bson.hpp`
- `yggr/nsql_database_system/bson_visit_t.hpp`
- `boost/type_traits/is_base_of.hpp`

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。