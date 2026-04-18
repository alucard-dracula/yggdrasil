# mongoc_client_encryption_opts_native_ex.hpp

## 概述

`mongoc_client_encryption_opts_native_ex.hpp` 是 Yggdrasil 数据库系统中 MongoDB 客户端加密选项原生扩展的头文件。该文件定义了 `mongoc_client_encryption_opts_native_ex` 类，提供对 MongoDB 客户端字段级加密选项的扩展支持。

该类封装了 MongoDB C 驱动的客户端加密选项，提供了类型安全的接口来配置客户端加密的参数，包括密钥库设置、KMS 提供者、TLS 选项和凭据回调等。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    class mongoc_client_encryption_opts_native_ex;
}
}
```

## 结构体定义

### yggr_mongoc_client_encryption_opts

```cpp
struct yggr_mongoc_client_encryption_opts
{
    mongoc_client_t *keyvault_client;     // 密钥库客户端
    char *keyvault_db;                    // 密钥库数据库名
    char *keyvault_coll;                  // 密钥库集合名
    bson_t *kms_providers;                // KMS 提供者配置
    bson_t *tls_opts;                     // TLS 选项
    yggr_mc_kms_credentials_callback creds_cb; // KMS 凭据回调
};
```

该结构体定义了客户端加密的配置选项：

- **keyvault_client**: MongoDB 客户端，用于访问密钥库
- **keyvault_db**: 密钥库所在的数据库名称
- **keyvault_coll**: 密钥库集合名称
- **kms_providers**: KMS（密钥管理服务）提供者配置，包含各种 KMS 的凭据信息
- **tls_opts**: TLS 连接选项，用于安全通信
- **creds_cb**: KMS 凭据提供者回调函数，用于动态获取凭据

## 类定义

### mongoc_client_encryption_opts_native_ex

```cpp
class mongoc_client_encryption_opts_native_ex
{
public:
    typedef mongoc_client_encryption_opts_t org_client_encryption_opts_type;
    typedef yggr_mongoc_client_encryption_opts yggr_client_encryption_opts_type;
    typedef mongoc_kms_credentials_provider_callback_fn kms_credentials_provider_callback_fn_type;
};
```

该类提供静态方法来操作客户端加密选项。

## 选项管理方法

### s_mongoc_client_encryption_opts_init

```cpp
inline static bool s_mongoc_client_encryption_opts_init(void* popts)
```

初始化客户端加密选项结构体。

**参数：**
- `popts`: 指向选项结构体的指针

**返回值：** `bool` - 初始化是否成功

**实现：**
```cpp
return popts && (memset(popts, 0, sizeof(yggr_client_encryption_opts_type)), true);
```

将结构体内存清零，确保所有指针初始化为 NULL。

### s_mongoc_client_encryption_opts_clear

```cpp
inline static void s_mongoc_client_encryption_opts_clear(void* popts)
```

清理客户端加密选项结构体。

**参数：**
- `popts`: 指向选项结构体的指针

**实现：** 调用初始化方法将结构体清零。

### s_mongoc_client_encryption_opts_org_clear

```cpp
static void s_mongoc_client_encryption_opts_org_clear(void* popts);
```

清理原始的 MongoDB 客户端加密选项。

**参数：**
- `popts`: 指向原始选项结构体的指针

释放原始 MongoDB 结构体中分配的资源。

## KMS 凭据提供者回调设置

### s_mongoc_client_encryption_opts_set_kms_credential_provider_callback (void* 版本)

```cpp
inline static bool s_mongoc_client_encryption_opts_set_kms_credential_provider_callback(void* popts,
                                                                                        kms_credentials_provider_callback_fn_type fn,
                                                                                        void* userdata)
```

设置 KMS 凭据提供者回调函数。

**参数：**
- `popts`: 指向选项结构体的指针
- `fn`: 回调函数指针
- `userdata`: 用户数据指针

**返回值：** `bool` - 设置是否成功

**实现：**
```cpp
return
    popts
    && (mongoc_client_encryption_opts_set_kms_credential_provider_callback (
        reinterpret_cast<org_client_encryption_opts_type*>(popts),
        fn, userdata), true);
```

### s_mongoc_client_encryption_opts_set_kms_credential_provider_callback (模板版本)

```cpp
template<typename UserData> inline
static bool s_mongoc_client_encryption_opts_set_kms_credential_provider_callback(void* popts,
                                                                                kms_credentials_provider_callback_fn_type fn,
                                                                                UserData& userdata)
```

模板版本的 KMS 凭据提供者回调设置方法。

**模板参数：**
- `UserData`: 用户数据类型

**参数：**
- `popts`: 指向选项结构体的指针
- `fn`: 回调函数指针
- `userdata`: 用户数据对象的引用

**返回值：** `bool` - 设置是否成功

**实现：**
```cpp
return
    popts
    && (mongoc_client_encryption_opts_set_kms_credential_provider_callback (
        reinterpret_cast<org_client_encryption_opts_type*>(popts),
        fn, reinterpret_cast<void*>(boost::addressof(userdata)) ), true);
```

使用 Boost 的 addressof 获取用户数据的地址，确保正确传递给回调函数。

## 客户端加密配置详解

### 密钥库 (Key Vault)
- **keyvault_client**: 用于访问存储数据加密密钥的 MongoDB 集合的客户端
- **keyvault_db**: 密钥库数据库名称，通常为 "encryption"
- **keyvault_coll**: 密钥库集合名称，通常为 "__keyVault"

### KMS 提供者 (KMS Providers)
KMS 提供者配置包含各种密钥管理服务的凭据信息：

- **AWS KMS**: Amazon Web Services 密钥管理服务
- **Azure Key Vault**: Microsoft Azure 密钥库
- **GCP KMS**: Google Cloud Platform 密钥管理服务
- **Local KMS**: 本地密钥管理（开发环境使用）

配置格式示例：
```json
{
  "aws": {
    "accessKeyId": "your-access-key-id",
    "secretAccessKey": "your-secret-access-key"
  },
  "local": {
    "key": "your-base64-encoded-key"
  }
}
```

### TLS 选项 (TLS Options)
TLS 选项用于配置与 KMS 提供者的安全通信：

- **证书文件路径**
- **私钥文件路径**
- **CA 证书文件路径**
- **证书密码**

### KMS 凭据回调 (KMS Credentials Callback)
当需要动态获取 KMS 凭据时使用回调函数：

- **回调函数签名**: `bool callback(void *userdata, mongoc_kms_credentials_t *creds)`
- **用途**: 支持凭据轮换、临时凭据等高级场景
- **用户数据**: 可以传递自定义上下文信息

## 设计理念

### 类型安全
- **类型别名**: 使用 typedef 定义清晰的类型名称
- **模板方法**: 提供类型安全的回调设置方法
- **指针验证**: 在所有方法中验证指针有效性

### 内存管理
- **零初始化**: 使用 memset 确保结构体安全初始化
- **资源清理**: 提供清理方法释放分配的资源
- **RAII友好**: 支持 RAII 编程模式

### 兼容性
- **原始类型封装**: 封装原始 MongoDB 结构体类型
- **函数转发**: 将调用转发到原始 MongoDB 函数
- **版本适配**: 适配不同版本的 MongoDB C 驱动

## 依赖项

- `yggr/nsql_database_system/mongodb_config.hpp` - MongoDB 配置
- `yggr/nsql_database_system/mongoc_mc_kms_credentials_callback_native_ex.hpp` - KMS 凭据回调扩展
- `memory` - C++ 标准库内存管理
- `boost/addressof.hpp` - Boost 地址获取工具

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_client_encryption_opts_native_ex.hpp>

// 创建客户端加密选项
yggr_mongoc_client_encryption_opts opts;

// 初始化选项
mongoc_client_encryption_opts_native_ex::s_mongoc_client_encryption_opts_init(&opts);

// 设置密钥库信息
mongoc_client_t* keyvault_client = mongoc_client_new("mongodb://localhost:27017");
opts.keyvault_client = keyvault_client;
opts.keyvault_db = bson_strdup("encryption");
opts.keyvault_coll = bson_strdup("__keyVault");

// 设置 KMS 提供者（使用本地密钥进行演示）
bson_t* kms_providers = BCON_NEW(
    "local", "{",
        "key", BCON_UTF8("your-base64-encoded-96-byte-key")
    "}"
);
opts.kms_providers = kms_providers;

// 可选：设置 TLS 选项
bson_t* tls_opts = bson_new();
// ... 配置 TLS 选项 ...
opts.tls_opts = tls_opts;

// 可选：设置 KMS 凭据提供者回调
bool kms_callback(void* userdata, mongoc_kms_credentials_t* creds) {
    // 实现凭据获取逻辑
    return true;
}

int user_data = 42;
mongoc_client_encryption_opts_native_ex::s_mongoc_client_encryption_opts_set_kms_credential_provider_callback(
    &opts, kms_callback, user_data);

// 创建客户端加密对象
// mongoc_client_encryption_t* client_encryption =
//     mongoc_client_encryption_new(opts, &error);

// 清理选项
mongoc_client_encryption_opts_native_ex::s_mongoc_client_encryption_opts_org_clear(&opts);
```

## 注意事项

1. **内存管理**: 正确管理字符串和 BSON 对象的生命周期
2. **客户端管理**: 确保 keyvault_client 的生命周期覆盖整个加密操作
3. **KMS 配置**: 根据使用的 KMS 提供者正确配置凭据信息
4. **安全考虑**: 妥善保护 KMS 凭据，避免在日志中暴露敏感信息
5. **错误处理**: 检查所有方法的返回值，确保操作成功

## 许可证

该文件遵循项目的许可证协议，基于 MIT 许可证。