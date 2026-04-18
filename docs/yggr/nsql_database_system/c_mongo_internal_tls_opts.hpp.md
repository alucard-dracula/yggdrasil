# c_mongo_internal_tls_opts.hpp 文档

## 文件概述

`c_mongo_internal_tls_opts.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB 内部 TLS 选项的 C++ 类。该类提供了对 MongoDB TLS 配置的包装，包括证书撤销检查和 OCSP 端点检查的禁用选项。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 结构体定义

### _mongoc_internal_tls_opts_t

该结构体是 MongoDB C 驱动的内部 TLS 选项结构体，包含以下成员：

- `bool tls_disable_certificate_revocation_check`：是否禁用证书撤销检查。
- `bool tls_disable_ocsp_endpoint_check`：是否禁用 OCSP 端点检查。

## 类定义

### c_mongo_internal_tls_opts

该类继承自 `_mongoc_internal_tls_opts_t`，提供了对内部 TLS 选项的封装和管理。它支持复制和移动操作。

#### 类型定义

- `base_type`：基类 `_mongoc_internal_tls_opts_t`。
- `org_type`：原始类型 `_mongoc_internal_tls_opts_t`。

#### 构造函数

- `c_mongo_internal_tls_opts(void)`：默认构造函数，初始化为 false。
- `c_mongo_internal_tls_opts(bool tdcrc, bool tdoec)`：使用指定值构造。
- `c_mongo_internal_tls_opts(BOOST_RV_REF(base_type) right)`：从基类移动构造。
- `c_mongo_internal_tls_opts(const base_type& right)`：从基类复制构造。
- `c_mongo_internal_tls_opts(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `c_mongo_internal_tls_opts(const this_type& right)`：复制构造函数。

#### 析构函数

- `~c_mongo_internal_tls_opts(void)`：析构函数。

#### 赋值运算符

- `operator=(BOOST_RV_REF(base_type) right)`：从基类移动赋值。
- `operator=(const base_type& right)`：从基类赋值。
- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值。
- `operator=(const this_type& right)`：复制赋值。

#### 交换函数

- `swap(BOOST_RV_REF(base_type) right)`：与基类移动交换。
- `swap(base_type& right)`：与基类交换。
- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：交换函数。

#### 类型转换函数

- `org_pointer(void)`：返回原始类型指针。
- `org_pointer(void) const`：返回常量原始类型指针。

#### 比较函数

- `compare_eq(const base_type& right) const`：与基类比较相等。
- `compare_eq(const this_type& right) const`：与同类型比较相等。

#### 成员访问函数

- `var_tls_disable_certificate_revocation_check(void)`：返回禁用证书撤销检查的引用。
- `var_tls_disable_certificate_revocation_check(void) const`：获取禁用证书撤销检查的值。
- `var_tls_disable_ocsp_endpoint_check(void)`：返回禁用 OCSP 端点检查的引用。
- `var_tls_disable_ocsp_endpoint_check(void) const`：获取禁用 OCSP 端点检查的值。

## 非成员函数

### 比较运算符

- `operator==(const base_type& l, const c_mongo_internal_tls_opts& r)`：相等比较。
- `operator==(const c_mongo_internal_tls_opts& l, const base_type& r)`：相等比较。
- `operator==(const c_mongo_internal_tls_opts& l, const c_mongo_internal_tls_opts& r)`：相等比较。
- `operator!=(const base_type& l, const c_mongo_internal_tls_opts& r)`：不相等比较。
- `operator!=(const c_mongo_internal_tls_opts& l, const base_type& r)`：不相等比较。
- `operator!=(const c_mongo_internal_tls_opts& l, const c_mongo_internal_tls_opts& r)`：不相等比较。

### 输出运算符

- `operator<<(std::basic_ostream<Char, Traits>& os, const c_mongo_internal_tls_opts& val)`：将 TLS 选项输出到流中，格式为 JSON 样式。

## 交换支持

在 `swap_support` 命名空间中定义了多个 `swap` 函数，用于支持与不同类型对象的交换操作。

## 依赖

- `yggr/base/yggrdef.h`：基础定义。
- `yggr/move/move.hpp`：移动语义。
- `yggr/nsql_database_system/bson_visit_t.hpp`：BSON 访问器。
- `<ostream>`：标准输出流。
- `<mongoc-ssl-private.h>`：MongoDB SSL 私有头文件（条件包含）。

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。