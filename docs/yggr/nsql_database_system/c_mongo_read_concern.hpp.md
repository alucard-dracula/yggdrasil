# c_mongo_read_concern.hpp 文档

## 文件概述

`c_mongo_read_concern.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB 读取一致性等级（Read Concern）的 C++ 类。该类包装了 MongoDB 原始 `mongoc_read_concern_t` 结构，并提供了级别设置、冻结/解冻、追加到命令和输出等功能。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 结构体模拟

`mongoc_read_concern_t` 的注释定义如下：

- `char *level`：读取级别字符串。
- `bool frozen`：是否冻结。
- `bson_t compiled`：编译后的 BSON 对象。

## 类定义

### c_mongo_read_concern

该类继承自 `yggr_mongoc_read_concern`，用于在 C++ 中管理 MongoDB 读取一致性等级。它支持复制和移动语义，并提供对原始对象的安全访问。

#### 类型定义

- `base_type`：基类 `yggr_mongoc_read_concern`。
- `org_type`：原始类型 `mongoc_read_concern_t`。
- `inner_string_type`：内部字符串类型 `utf8_string`。
- `inner_string_view_type`：内部字符串视图类型 `utf8_string_view`。
- `bson_type`：BSON 类型 `c_bson`。
- `member_level_modify_helper_type`：读取级别成员修改助手类型。

#### 静态函数

- `s_level_local(void)`：返回 `local` 读取级别字符串。
- `s_level_majority(void)`：返回 `majority` 读取级别字符串。
- `s_level_available(void)`：返回 `available` 读取级别字符串。
- `s_level_linearizable(void)`：返回 `linearizable` 读取级别字符串。
- `s_level_snapshot(void)`：返回 `snapshot` 读取级别字符串。
- `level_check(const char* str_lv)`：检查读取级别字符串是否有效。
- `level_check(const String& str_lv)`：模板版本，检查任意可构造为 UTF-8 字符串的读取级别。

#### 构造函数

- `c_mongo_read_concern(void)`：默认构造函数。
- `c_mongo_read_concern(const String& str_lv)`：使用读取级别字符串构造。
- `explicit c_mongo_read_concern(const org_type* ptr)`：从原始指针构造。
- `c_mongo_read_concern(const org_type& right)`：从原始对象构造。
- `c_mongo_read_concern(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `c_mongo_read_concern(const this_type& right)`：复制构造函数。

#### 析构函数

- `~c_mongo_read_concern(void)`：析构函数。

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
- `org_pointer(void) const`：返回常量原始类型指针。
- `operator org_type&(void)`：转换为原始类型引用。
- `operator const org_type&(void) const`：转换为常量原始类型引用。
- `operator org_type*(void)`：转换为原始类型指针。
- `operator const org_type*(void) const`：转换为常量原始类型指针。

#### 成员访问和修改

- `pro_get_var_level(void) const`：获取读取级别字符串视图。
- `pro_set_var_level(const inner_string_view_type& str_lv)`：设置读取级别字符串。
- `var_level(void)`：返回读取级别成员修改助手。
- `var_level(const string& ret_charset_name) const`：获取读取级别字符串视图。
- `var_compiled(void) const`：获取编译后的 BSON 对象。
- `var_compiled<Bson>(void) const`：获取指定 BSON 类型的编译结果。

#### 追加与冻结控制

- `append_to(Bson& cmd) const`：将读取关心得到的 BSON 追加到命令对象中。
- `unfreeze_copy(void) const`：返回一个解冻后的副本。
- `freeze(void)`：冻结当前对象。
- `unfreeze(void)`：解冻当前对象。
- `is_frozen(void) const`：检查对象是否已冻结。
- `is_default(void) const`：检查是否为默认读取关怀（即 `level` 为空）。

## 输出支持

提供 `operator<<` 输出重载，将读取关怀对象以 JSON 风格输出。

## 交换支持

在 `swap_support` 命名空间中定义了 `swap(c_mongo_read_concern&, c_mongo_read_concern&)`，并导出到 `yggr::nsql_database_system`、`std` 和 `boost`。

## 依赖

- `yggr/base/yggrdef.h`
- `yggr/move/move.hpp`
- `yggr/charset/utf8_string.hpp`
- `yggr/charset/utf8_string_view.hpp`
- `yggr/utility/member_var_modify_helper.hpp`
- `yggr/nsql_database_system/c_bson.hpp`
- `yggr/nsql_database_system/mongoc_read_concern_native_ex.hpp`
- `yggr/nsql_database_system/bson_visit_t.hpp`
- `yggr/nsql_database_system/detail/string_member_var_get.hpp`
- `yggr/nsql_database_system/detail/incomplete t_move_patch_macro.hpp`
- `boost/utility/enable_if.hpp`
- `boost/mpl/bool.hpp`
- `<ostream>`

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。