# c_mongo_uri.hpp 文档

## 文件概述

`c_mongo_uri.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB URI 的 C++ 类 `c_mongo_uri`。它基于 MongoDB C 驱动的 `mongoc_uri_t` 结构，并提供字符串解析、属性访问、选项读取与设置、认证信息管理、读写关注和压缩器配置等功能。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 结构体模拟

`mongoc_uri_t` 注释定义如下：

- `char *str`：URI 原始字符串。
- `bool is_srv`：是否为 SRV URI。
- `char srv[BSON_HOST_NAME_MAX + 1]`：SRV 主机名。
- `mongoc_host_list_t *hosts`：主机列表。
- `char *username`：用户名。
- `char *password`：密码。
- `char *database`：数据库名称。
- `bson_t raw`：原始选项 BSON。
- `bson_t options`：标准化后的选项 BSON。
- `bson_t credentials`：凭证 BSON。
- `bson_t compressors`：压缩器 BSON。
- `mongoc_read_prefs_t *read_prefs`：读取偏好。
- `mongoc_read_concern_t *read_concern`：读取一致性。
- `mongoc_write_concern_t *write_concern`：写入关注。

## 类定义

### c_mongo_uri

该类继承自 `yggr_mongoc_uri_t` 并封装了 MongoDB URI 的解析与访问逻辑。

#### 类型定义

- `base_type`：基类 `yggr_mongoc_uri_t`。
- `org_type`：原始类型 `mongoc_uri_t`。
- `inner_string_type`：内部字符串类型 `utf8_string`。
- `inner_string_view_type`：内部字符串视图类型 `utf8_string_view`。
- `bson_type`：BSON 类型 `c_bson`。
- `bson_error_type`：BSON 错误类型 `c_bson_error`。
- `read_prefs_type`：读取偏好类型 `c_mongo_read_prefs`。
- `read_concern_type`：读取一致性类型 `c_mongo_read_concern`。
- `write_concern_type`：写入关注类型 `c_mongo_write_concern`。
- `mongo_host_info_type`：主机信息类型 `c_mongo_host_info`。
- `mongo_host_info_list_type`：主机信息列表类型 `container::list<mongo_host_info_type>`。

#### 成员修改助手类型

- `inner_string_member_modify_helper_type`：用于修改内部字符串成员。
- `non_member_string_view_modify_helper_type`：用于无成员字符串视图修改。
- `member_compressors_modify_helper_type`：用于修改压缩器 BSON。

## 构造与初始化

- `c_mongo_uri(void)`：默认构造。
- `c_mongo_uri(bson_error_type& outerr)`：带错误对象的默认构造。
- `c_mongo_uri(const String& str_uri)`：从 URI 字符串构造。
- `c_mongo_uri(const String& str_uri, bson_error_type& outerr)`：解析带错误输出的 URI。
- `c_mongo_uri(const String& str_host, u16 port)`：从主机和端口构造 URI。
- `c_mongo_uri(const String& str_host, u16 port, bson_error_type& outerr)`：解析主机和端口带错误输出的 URI。
- `explicit c_mongo_uri(const org_type* right)`：从原始指针构造。
- `c_mongo_uri(const org_type& right)`：从原始对象构造。
- `c_mongo_uri(BOOST_RV_REF(this_type) right)`：移动构造。
- `c_mongo_uri(const this_type& right)`：复制构造。

## 赋值与交换

- `operator=(const String& right)`：从 URI 字符串赋值。
- `operator=(const org_type& right)`：从原始对象赋值。
- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值。
- `operator=(const this_type& right)`：复制赋值。
- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：交换函数。

## 原始指针访问

- `org_pointer(void)`：返回 `mongoc_uri_t*`。
- `org_pointer(void) const`：返回 `const mongoc_uri_t*`。
- `operator org_type&(void)`：转换为 `mongoc_uri_t&`。
- `operator const org_type&(void) const`：转换为 `const mongoc_uri_t&`。
- `operator org_type*(void)`：转换为 `mongoc_uri_t*`。
- `operator const org_type*(void) const`：转换为 `const mongoc_uri_t*`。

## 状态检查

- `empty(void) const`：判断 URI 是否为空。
- `failed(void) const`：判断解析是否失败。

## URI 组件访问

- `var_scheme(void) const`：返回 URI 协议部分，如 `mongodb://` 或 `mongodb+srv://`。
- `var_hosts(Container& cont) const`：将 URI 中的主机列表填充到容器中。
- `var_hosts(void) const`：返回主机列表容器。
- `var_srv_hostname(void) const`：返回 SRV 主机名。
- `var_srv_service_name(void) const`：返回 SRV 服务名。
- `var_database(void)`：访问数据库名称。
- `var_username(void)`：访问用户名。
- `var_passwd(void)`：访问密码。

## 选项与属性访问

- `var_options(void)`：获取标准化后的 URI 选项 BSON。
- `has_option(const OptString& key) const`：检查是否存在指定选项。
- `option_is_int32(const OptString& key)`：检查选项是否为 int32。
- `option_is_int64(const OptString& key)`：检查选项是否为 int64。
- `option_is_bool(const OptString& key)`：检查选项是否为 bool。
- `option_is_utf8(const OptString& key)`：检查选项是否为 UTF-8 字符串。
- `get_option_as_int32(const OptString& option, int32_t fallback) const`：获取 int32 选项。
- `set_option_as_int32(const OptString& option, int32_t value)`：设置 int32 选项。
- `get_option_as_int64(const OptString& option, int64_t fallback) const`：获取 int64 选项。
- `set_option_as_int64(const OptString& option, int64_t value)`：设置 int64 选项。
- `get_option_as_bool(const OptString& option, bool fallback) const`：获取 bool 选项。
- `set_option_as_bool(const OptString& option, bool value)`：设置 bool 选项。
- `get_option_as_utf8(const OptString& option, const inner_string_view_type& fallback, const string& ret_charset_name) const`：获取 UTF-8 选项值。
- `set_option_as_utf8(const OptString& option, const ValString& val)`：设置 UTF-8 选项值。

## 凭证与认证

- `var_credentials(void)`：访问凭证 BSON。
- `auth_mechanism(void)`：访问或设置认证机制。
- `auth_source(void)`：访问或设置认证源。

## 压缩器与机制属性

- `var_compressors(void)`：访问压缩器 BSON。
- `modify_compressors(void)`：修改压缩器设置。
- `get_compressors(void) const`：获取压缩器设置。
- `set_compressors(const String& val)`：设置压缩器字符串。
- `get_mechanism_properties(Bson& bs) const`：获取认证机制属性。
- `get_mechanism_properties(void) const`：获取认证机制属性并返回 BSON。
- `set_mechanism_properties(const Bson& val)`：设置认证机制属性。

## 读写关注

- `var_read_concern(void)`：访问读取一致性对象。
- `var_read_prefs(void)`：访问读取偏好对象。
- `var_write_concern(void)`：访问写入关注对象。

## 其他属性

- `get_replica_set(void) const`：获取副本集名称。
- `get_tls(void) const`：检测是否启用 TLS。
- `has_tls(void) const`：同 `get_tls()`。

## URI 字符串访问

- `var_uri_string(void)`：访问原始 URI 字符串。
- `clear(void)`：清空对象。

## 内部实现

- `pro_copy(const base_type& right)`：从原始类型复制。
- `pro_copy(const this_type& right)`：从同类型复制。
- `pro_move(this_type& right)`：移动实现。
- `pro_swap(this_type& right)`：交换实现。
- `prv_init_default(void)`：默认初始化。
- `prv_init(base_type& right)`：从底层结构初始化。
- `prv_destroy(void)`：销毁底层资源。

## 输出支持

- `operator<<(std::basic_ostream<Char, Traits>& os, const c_mongo_uri& val)`：支持将 URI 对象以 JSON 风格输出。

## 交换支持

在 `swap_support` 命名空间中定义了 `swap(c_mongo_uri& l, c_mongo_uri& r)`，并导出到 `yggr::nsql_database_system`、`std` 和 `boost`。

## 依赖

- `yggr/base/yggrdef.h`
- `yggr/move/move.hpp`
- `yggr/utility/swap.hpp`
- `yggr/utility/member_var_modify_helper.hpp`
- `yggr/charset/utf8_string.hpp`
- `yggr/charset/utf8_string_view.hpp`
- `yggr/container/list.hpp`
- `yggr/container/empty.hpp`
- `yggr/iterator_ex/iterator.hpp`
- `yggr/type_traits/native_t.hpp`
- `yggr/type_traits/traits.hpp`
- `yggr/mplex/static_assert.hpp`
- `yggr/mplex/tag_sfinae.hpp`
- `yggr/nsql_database_system/mongoc_uri_native_ex.hpp`
- `yggr/nsql_database_system/c_bson.hpp`
- `yggr/nsql_database_system/c_bson_error.hpp`
- `yggr/nsql_database_system/c_mongo_read_prefs.hpp`
- `yggr/nsql_database_system/c_mongo_read_concern.hpp`
- `yggr/nsql_database_system/c_mongo_write_concern.hpp`
- `yggr/nsql_database_system/c_mongo_host_info.hpp`
- `yggr/nsql_database_system/bson_visit_t.hpp`
- `yggr/nsql_database_system/detail/string_member_var_get.hpp`
- `<boost/ref.hpp>`
- `<boost/utility/enable_if.hpp>`
- `<boost/type_traits/is_base_of.hpp>`
- `<boost/type_traits/is_same.hpp>`
- `<boost/range/functions.hpp>`
- `<boost/range/value_type.hpp>`
- `<ostream>`

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。