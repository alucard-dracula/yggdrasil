# c_mongo_mc_kms_credentials_callback.hpp 文档

## 文件概述

`c_mongo_mc_kms_credentials_callback.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB 客户端加密 (Client-Side Field Level Encryption, CSFLE) KMS 凭证回调的 C++ 类。该类包装了原始 MongoDB KMS 凭证回调结构，提供便捷的构造、赋值、交换和访问函数。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 结构体模拟

原始结构体 `yggr_mc_kms_credentials_callback` 的注释形式定义如下：

- `mongoc_kms_credentials_provider_callback_fn fn`：回调函数指针。
- `void *userdata`：用户数据指针。

## 类定义

### c_mc_kms_credentials_callback

该类继承自 `yggr_mc_kms_credentials_callback`，用于包装 MongoDB KMS 凭证回调结构。它支持复制和移动语义，并提供对回调函数及用户数据的访问。

#### 类型定义

- `base_type`：基类 `yggr_mc_kms_credentials_callback`。
- `callback_fn_type`：回调函数类型，等价于 `mongoc_kms_credentials_provider_callback_fn`。
- `userdata_type`：用户数据类型，等价于 `void*`。

#### 构造函数

- `c_mc_kms_credentials_callback(void)`：默认构造，初始化为空回调。
- `c_mc_kms_credentials_callback(callback_fn_type pfoo, void* puserdata = 0)`：使用回调函数指针和可选用户数据构造。
- `c_mc_kms_credentials_callback(callback_fn_type pfoo, UserData& arg_userdata)`：模板构造，通过引用参数设置用户数据指针。
- `c_mc_kms_credentials_callback(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `c_mc_kms_credentials_callback(const this_type& right)`：复制构造函数。

#### 析构函数

- `~c_mc_kms_credentials_callback(void)`：析构函数。

#### 赋值运算符

- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值运算符。
- `operator=(const this_type& right)`：复制赋值运算符。

#### 交换函数

- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：交换函数。

#### 成员访问函数

- `var_fn(void)`：返回回调函数指针的引用。
- `var_fn(void) const`：获取回调函数指针。
- `var_userdata(void)`：返回用户数据指针的引用。
- `var_userdata(void) const`：获取用户数据指针。

#### 其他函数

- `empty(void) const`：判断回调是否为空（即 `fn` 是否为 `nullptr`）。

## 交换支持

在 `swap_support` 命名空间中定义了 `YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mc_kms_credentials_callback)`，并将其导出到 `yggr::nsql_database_system`、`std` 和 `boost` 命名空间，支持通用交换操作。

## 依赖

- `yggr/move/move.hpp`：移动语义支持。
- `yggr/ppex/swap_this_def.hpp`：交换定义宏。
- `yggr/nsql_database_system/mongoc_mc_kms_credentials_callback_native_ex.hpp`：MongoDB KMS 凭证回调本机扩展。

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。