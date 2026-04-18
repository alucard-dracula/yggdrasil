# c_mongo_write_concern.hpp 文档

## 文件概述

`c_mongo_write_concern.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB 写关注（Write Concern）的 C++ 类。该类包装了 MongoDB 原始 `mongoc_write_concern_t` 结构，并提供了写关注参数的访问、设置、冻结、追加到命令、以及流式输出等功能。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 结构体模拟

注释中给出了 `mongoc_write_concern_t` 的成员结构：

- `int8_t fsync_`：是否启用 fsync（已弃用）。
- `int8_t journal`：是否启用 journal。
- `int32_t w`：写关注级别或标签。
- `int64_t wtimeout`：写超时时间。
- `char *wtag`：写关注标签字符串。
- `bool frozen`：是否冻结对象。
- `bson_t compiled`：编译后的 BSON 对象。
- `bool is_default_v`：是否为默认写关注。

## 类定义

### c_mongo_write_concern

该类继承自 `yggr_mongoc_write_concern`，用于以 C++ 方式管理 MongoDB 写关注对象。它支持复制和移动语义，并提供对原始 `mongoc_write_concern_t` 的安全访问。

#### 类型定义

- `base_type`：基类 `yggr_mongoc_write_concern`。
- `org_type`：原始 MongoDB 类型 `mongoc_write_concern_t`。
- `bson_type`：BSON 类型 `c_bson`。
- `inner_string_type`：内部字符串类型 `utf8_string`。
- `inner_string_view_type`：内部字符串视图类型 `utf8_string_view`。
- `s8_member_var_modify_helper_type`：用于修改 `s8` 类型成员的助手。
- `s32_member_var_modify_helper_type`：用于修改 `s32` 类型成员的助手。
- `s64_member_var_modify_helper_type`：用于修改 `s64` 类型成员的助手。
- `wtimeout32_member_var_modify_helper_type`：用于 `wmajority` 的超时修改助手。
- `member_tag_modify_helper_type`：用于写标签成员修改的助手。

#### 静态常量

- `E_MONGOC_WRITE_CONCERN_FSYNC_DEFAULT`：默认 fsync 值。
- `E_MONGOC_WRITE_CONCERN_JOURNAL_DEFAULT`：默认 journal 值。
- `E_MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED`：未确认写关注。
- `E_MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED`：忽略错误写关注（已弃用）。
- `E_MONGOC_WRITE_CONCERN_W_DEFAULT`：默认写关注。
- `E_MONGOC_WRITE_CONCERN_W_MAJORITY`：多数写关注。
- `E_MONGOC_WRITE_CONCERN_W_TAG`：标签写关注。

#### 构造函数

- `c_mongo_write_concern(void)`：默认构造函数。
- `explicit c_mongo_write_concern(s32 w_count)`：使用写关注计数构造。
- `template<typename String> explicit c_mongo_write_concern(const String& wtag, ...)`：使用写关注标签构造。
- `explicit c_mongo_write_concern(const org_type* ptr)`：从原始指针构造。
- `c_mongo_write_concern(const org_type& right)`：从原始对象构造。
- `c_mongo_write_concern(BOOST_RV_REF(this_type) right)`：移动构造。
- `c_mongo_write_concern(const this_type& right)`：复制构造。

#### 析构函数

- `~c_mongo_write_concern(void)`：析构函数。

#### 赋值运算符

- `operator=(const org_type& right)`：从原始对象赋值。
- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值。
- `operator=(const this_type& right)`：复制赋值。

#### 交换函数

- `swap(org_type& right)`：与原始 `mongoc_write_concern_t` 对象交换。
- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：与同类型对象交换。

#### 原始指针访问

- `org_pointer(void)`：返回可变原始指针。
- `org_pointer(void) const`：返回只读原始指针。
- `operator org_type&(void)`：转换为原始类型引用。
- `operator const org_type&(void) const`：转换为只读原始类型引用。
- `operator org_type*(void)`：转换为原始类型指针。
- `operator const org_type*(void) const`：转换为只读原始类型指针。

#### 验证

- `validate(void) const`：检查写关注是否有效。

#### 成员访问与设置

- `var_fsync(void)` / `var_fsync(void) const`：获取或修改 `fsync` 标志。
- `var_journal(void)` / `var_journal(void) const`：获取或修改 `journal` 标志。
- `var_w(void)` / `var_w(void) const`：获取或修改写关注 `w`。
- `var_wtimeout(void)` / `var_wtimeout(void) const`：获取或修改写超时时间。
- `var_wmajority(void)` / `var_wmajority(void) const`：获取或修改多数写关注，以及对应超时时间。
- `var_wtag(void)` / `var_wtag(const string& ret_charset_name) const`：获取或修改写标签。

#### 字符串标签支持

类中提供模板方法 `pro_set_var_wtag_tpl`，可以接收任意可构造为 UTF-8 字符串的类型，并将其转换为内部 UTF-8 字符串后设置 `wtag`。

#### BSON 编译对象

- `freeze_compiled(void)`：返回当前冻结后的编译 BSON 对象引用。
- `freeze_compiled<Bson>(void)`：返回指定 BSON 类型的编译对象引用。
- `var_compiled(void) const`：返回内部 `compiled` BSON 对象引用。
- `var_compiled<Bson>(void) const`：返回指定 BSON 类型的内部编译对象引用。

#### 追加到 BSON 命令

- `append_to(Bson& cmd) const`：将写关注内容追加到 BSON 命令对象中。

#### 冻结控制

- `freeze(void)`：冻结写关注对象，使其状态不可修改。
- `unfreeze(void)`：解除冻结，使对象可修改。
- `is_frozen(void) const`：检查对象是否已冻结。

#### 状态判断

- `is_default(void) const`：判断是否为默认写关注对象。
- `journal_is_set(void) const`：判断是否显式设置了 `journal`。
- `is_acknowledged(void) const`：判断写关注是否为已确认模式。

## 输出支持

定义了 `operator<<` 重载，用于将 `c_mongo_write_concern` 对象以 JSON 风格输出，包含以下字段：

- `fsync_`
- `journal`
- `w`
- `wtimeout`
- `frozen`
- `compiled`
- `is_default`

该输出实现位于 `detail::bson_operator_outter<c_mongo_write_concern>` 中。

## 交换支持

在 `yggr::nsql_database_system::swap_support` 命名空间中定义了 `swap(c_mongo_write_concern&, c_mongo_write_concern&)`，并将其导出到 `yggr::nsql_database_system`、`std` 和 `boost`。

## 依赖

- `yggr/utility/copy_or_move_or_swap.hpp`
- `yggr/charset/utf8_string.hpp`
- `yggr/charset/utf8_string_view.hpp`
- `yggr/utility/member_var_modify_helper.hpp`
- `yggr/container/empty.hpp`
- `yggr/nsql_database_system/c_bson.hpp`
- `yggr/nsql_database_system/mongodb_config.hpp`
- `yggr/nsql_database_system/mongoc_write_concern_native_ex.hpp`
- `yggr/nsql_database_system/bson_visit_t.hpp`
- `yggr/nsql_database_system/detail/string_member_var_get.hpp`
- `boost/utility/enable_if.hpp`
- `boost/mpl/not.hpp`
- `boost/mpl/and.hpp`
- `boost/type_traits/is_enum.hpp`

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。
