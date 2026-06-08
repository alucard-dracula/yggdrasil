# mongoc_mc_kms_credentials_callback_native_ex.hpp

## 概述

此头文件定义了 `yggr_mc_kms_credentials_callback` 结构体，该结构体封装了 MongoDB KMS (Key Management Service) 凭据提供者回调函数。这些定义用于处理 MongoDB 客户端加密功能的密钥管理。

## 命名空间

- `yggr::nsql_database_system`

## 结构体描述

### yggr_mc_kms_credentials_callback

一个结构体，封装 KMS 凭据提供者回调函数和用户数据。

#### 成员变量

##### `mongoc_kms_credentials_provider_callback_fn fn`

KMS 凭据提供者回调函数指针。该函数用于提供加密密钥的凭据。

##### `void *userdata`

用户数据指针。传递给回调函数的用户自定义数据。

## 编译时断言

文件包含一个 Boost MPL 断言，用于确保自定义结构体的大小与原始 MongoDB 结构体的大小相同：

```cpp
BOOST_MPL_ASSERT((
    boost::mpl::bool_<
        (sizeof(yggr_mc_kms_credentials_callback) == sizeof(mongoc_kms_credentials_provider_callback_fn) + sizeof(void*))
    >));
```

这确保了结构体的内存布局兼容性。

## 依赖项

- `<yggr/nsql_database_system/mongodb_config.hpp>`
- Boost MPL 库 (用于编译时断言)

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_mc_kms_credentials_callback_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 定义回调函数
bool kms_callback(void *ctx, mongoc_kms_credentials_t *credentials, bson_error_t *error) {
    // 实现密钥凭据提供逻辑
    return true;
}

// 创建回调结构体
yggr_mc_kms_credentials_callback callback;
callback.fn = kms_callback;
callback.userdata = nullptr; // 或传递自定义数据
```

## 注意事项

- 该结构体是对 MongoDB C 驱动程序 `mc_kms_credentials_callback` 的自定义包装
- 回调函数 `fn` 用于在需要时提供 KMS 凭据
- `userdata` 允许传递上下文信息给回调函数
- 编译时断言确保结构体大小与原始定义匹配，保证内存兼容性
- KMS 凭据提供者用于 MongoDB 客户端加密功能，支持 AWS KMS、Azure Key Vault 等
- 使用此结构体时需要正确实现回调函数，以提供有效的密钥凭据</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_mc_kms_credentials_callback_native_ex_cn.md