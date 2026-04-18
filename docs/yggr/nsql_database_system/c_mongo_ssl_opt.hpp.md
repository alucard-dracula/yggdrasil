# c_mongo_ssl_opt.hpp 文档

## 文件概述

`c_mongo_ssl_opt.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB SSL/TLS 连接选项的 C++ 类。该类包装了 MongoDB 原始 `mongoc_ssl_opt_t` 结构，并提供对证书文件、CA 路径、CRL 文件、证书验证以及内部 TLS 选项的访问和修改。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 结构体模拟

`mongoc_ssl_opt_t` 的注释定义如下：

- `const char *pem_file`：PEM 文件路径。
- `const char *pem_pwd`：PEM 密码。
- `const char *ca_file`：CA 文件路径。
- `const char *ca_dir`：CA 目录路径。
- `const char *crl_file`：证书撤销列表(CRL)文件路径。
- `bool weak_cert_validation`：是否允许弱证书验证。
- `bool allow_invalid_hostname`：是否允许无效主机名。
- `void *internal`：内部 TLS 选项指针。
- `void *padding[6]`：填充。

同样定义了 `yggr_mongoc_ssl_opt_t` 作为同样结构的替代类型。

## 类定义

### c_mongo_ssl_opt

该类继承自 `yggr_mongoc_ssl_opt_t`，用于管理 MongoDB SSL/TLS 选项。它将字符串成员保存在内部 `utf8_string` 对象中，并将原始 `mongoc_ssl_opt_t` 结构字段指向这些字符串的底层字符数据。

#### 类型定义

- `base_type`：基类 `yggr_mongoc_ssl_opt_t`。
- `org_type`：原始类型 `mongoc_ssl_opt_t`。
- `inner_string_type`：内部字符串类型 `utf8_string`。
- `inner_string_view_type`：内部字符串视图类型 `utf8_string_view`。
- `path_string_type`：路径字符串类型，在 Windows 上使用 `yggr::string`，否则使用 `utf8_string`。
- `pwd_string_type`：密码字符串类型 `utf8_string`。
- `internal_tls_opts_type`：内部 TLS 选项类型 `c_mongo_internal_tls_opts`。

#### 常量

- `E_compare_size`：比较结构是否为空时使用的字节数。

#### 构造函数

- `c_mongo_ssl_opt(void)`：默认构造函数。
- `c_mongo_ssl_opt(const String1& pem_file_name, const String2& pem_passwd, const String3& ca_file_name, const String4& ca_dir_name, const String5& crl_file_name, bool bweak_cert_validation = false, bool ballow_invalid_hostname = false, const internal_tls_opts_type& internal_tls_opts = internal_tls_opts_type(false, false))`：从各个字符串参数构造。
- `c_mongo_ssl_opt(const org_type* right)`：从原始类型指针构造。
- `c_mongo_ssl_opt(const org_type& right)`：从原始类型引用构造。
- `c_mongo_ssl_opt(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `c_mongo_ssl_opt(const this_type& right)`：复制构造函数。

#### 析构函数

- `~c_mongo_ssl_opt(void)`：析构函数。

#### 赋值运算符

- `operator=(const org_type& right)`：从原始类型赋值。
- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值。
- `operator=(const this_type& right)`：复制赋值。

#### 交换函数

- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：交换函数。

#### 原始指针访问

- `org_pointer(void)`：返回原始 `mongoc_ssl_opt_t*`。
- `org_pointer(void) const`：返回 `const mongoc_ssl_opt_t*`。
- `operator org_type&(void)`：转换为原始类型引用。
- `operator const org_type&(void) const`：转换为原始类型常量引用。
- `operator org_type*(void)`：转换为原始类型指针。
- `operator const org_type*(void) const`：转换为原始类型常量指针。

#### 比较与状态

- `compare_eq(const org_type& right) const`：与原始类型比较是否相等。
- `compare_eq(const this_type& right) const`：与同类型比较是否相等。
- `empty(void) const`：判断当前 SSL 选项是否为空。

#### 字符串成员访问

- `var_pem_file(void)` / `var_pem_file(void) const`：PEM 文件路径。
- `var_pem_pwd(void)` / `var_pem_pwd(void) const`：PEM 密码。
- `var_ca_file(void)` / `var_ca_file(void) const`：CA 文件路径。
- `var_ca_dir(void)` / `var_ca_dir(void) const`：CA 目录路径。
- `var_crl_file(void)` / `var_crl_file(void) const`：CRL 文件路径。

#### 布尔成员访问

- `var_weak_cert_validation(void)` / `var_weak_cert_validation(void) const`：弱证书验证标志。
- `var_allow_invalid_hostname(void)` / `var_allow_invalid_hostname(void) const`：允许无效主机名标志。

#### 内部 TLS 选项访问

- `var_internal_tls_opts(void)` / `var_internal_tls_opts(void) const`：内部 TLS 选项对象。
- `var_tls_disable_certificate_revocation_check(void)` / `var_tls_disable_certificate_revocation_check(void) const`：是否禁用证书撤销检查。
- `var_tls_disable_ocsp_endpoint_check(void)` / `var_tls_disable_ocsp_endpoint_check(void) const`：是否禁用 OCSP 端点检查。

#### 默认值

- `s_get_default(void)`：获取默认的 `c_mongo_ssl_opt` 全局静态对象。

## 内部实现函数

- `pro_init(void)`：初始化成员。
- `pro_init(const String1&, const String2&, const String3&, const String4&, const String5&, bool, bool, const internal_tls_opts_type&)`：从字符串和选项初始化。
- `pro_move(this_type& right)`：移动实现。
- `pro_copy(const base_type& right)`：从原始类型复制实现。
- `pro_copy(const this_type& right)`：从同类型复制实现。
- `pro_swap(this_type& right)`：交换实现。

## 非成员比较运算符

- `operator==(const org_type& l, const c_mongo_ssl_opt& r)`
- `operator==(const c_mongo_ssl_opt& l, const org_type& r)`
- `operator==(const c_mongo_ssl_opt& l, const c_mongo_ssl_opt& r)`
- `operator!=(const org_type& l, const c_mongo_ssl_opt& r)`
- `operator!=(const c_mongo_ssl_opt& l, const org_type& r)`
- `operator!=(const c_mongo_ssl_opt& l, const c_mongo_ssl_opt& r)`

## 输出支持

- `operator<<`：支持将 `c_mongo_ssl_opt` 以 JSON 风格输出。
- `operator<<`：支持将 `mongoc_ssl_opt_t` 以 JSON 风格输出。

## 交换支持

在 `swap_support` 命名空间中定义了 `YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_ssl_opt)`，并将其导出到 `yggr::nsql_database_system`、`std` 和 `boost`。

## 依赖

- `yggr/base/yggrdef.h`
- `yggr/base/static_constant.hpp`
- `yggr/utility/copy_or_move_or_swap.hpp`
- `yggr/type_traits/native_t.hpp`
- `yggr/charset/utf8_string.hpp`
- `yggr/charset/utf8_string_view.hpp`
- `yggr/charset/string_converter.hpp`
- `yggr/container_ex/is_allocator.hpp`
- `yggr/ppex/swap_this_def.hpp`
- `yggr/ppex/foo_params.hpp`
- `yggr/mplex/tag_sfinae.hpp`
- `yggr/utility/member_var_modify_helper.hpp`
- `yggr/nsql_database_system/mongodb_config.hpp`
- `yggr/nsql_database_system/c_mongo_internal_tls_opts.hpp`
- `yggr/nsql_database_system/bson_visit_t.hpp`
- `<boost/ref.hpp>`
- `<boost/utility/enable_if.hpp>`
- `<boost/mpl/and.hpp>`
- `<boost/mpl/not.hpp>`

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。