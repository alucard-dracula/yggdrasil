# mongoc_auto_encryption_opts_native_ex.hpp

## 概述

`mongoc_auto_encryption_opts_native_ex.hpp` 是 Yggdrasil 数据库系统中 MongoDB 自动加密选项原生扩展的头文件。该文件定义了 `mongoc_auto_encryption_opts_native_ex` 类，提供对 MongoDB 客户端字段级加密（Client-Side Field Level Encryption）自动加密选项的扩展支持。

该类是对 MongoDB C 驱动自动加密选项的封装和扩展，提供了更方便和安全的接口来配置字段级加密功能。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    class mongoc_auto_encryption_opts_native_ex;
}
}
```

## 结构体定义

### yggr_mongoc_auto_encryption_opts

```cpp
struct yggr_mongoc_auto_encryption_opts
{
    /* keyvault_client and keyvault_client_pool are not owned and must outlive
     * auto encrypted client/pool. */
    mongoc_client_t *keyvault_client;
    mongoc_client_pool_t *keyvault_client_pool;
    char *keyvault_db;
    char *keyvault_coll;
    bson_t *kms_providers;
    bson_t *tls_opts;
    bson_t *schema_map;
    bson_t *encrypted_fields_map;
    bool bypass_auto_encryption;
    bool bypass_query_analysis;
    yggr_mc_kms_credentials_callback creds_cb;
    bson_t *extra;
};
```

该结构体扩展了 MongoDB 的自动加密选项，包含以下字段：

- **keyvault_client**: Key Vault 客户端（不拥有所有权）
- **keyvault_client_pool**: Key Vault 客户端池（不拥有所有权）
- **keyvault_db**: Key Vault 数据库名称
- **keyvault_coll**: Key Vault 集合名称
- **kms_providers**: KMS 提供者配置
- **tls_opts**: TLS 选项
- **schema_map**: 模式映射
- **encrypted_fields_map**: 加密字段映射
- **bypass_auto_encryption**: 是否绕过自动加密
- **bypass_query_analysis**: 是否绕过查询分析
- **creds_cb**: KMS 凭据回调
- **extra**: 额外选项

## 类定义

### mongoc_auto_encryption_opts_native_ex

```cpp
class mongoc_auto_encryption_opts_native_ex
{
public:
    typedef mongoc_auto_encryption_opts_t org_auto_encryption_opts_type;
    typedef yggr_mongoc_auto_encryption_opts yggr_auto_encryption_opts_type;
    typedef mongoc_kms_credentials_provider_callback_fn kms_credentials_provider_callback_fn_type;
};
```

该类提供静态方法来操作自动加密选项。

## 主要功能

### 初始化和清理

#### s_mongoc_auto_encryption_opts_init

```cpp
inline static bool s_mongoc_auto_encryption_opts_init(void* popts);
```

初始化自动加密选项结构体。

**参数：**
- `popts`: 指向选项结构体的指针

**返回值：** `bool` - 初始化是否成功

该方法将结构体清零初始化。

#### s_mongoc_auto_encryption_opts_clear

```cpp
inline static void s_mongoc_auto_encryption_opts_clear(void* popts);
```

清理自动加密选项结构体。

**参数：**
- `popts`: 指向选项结构体的指针

实际上调用 `s_mongoc_auto_encryption_opts_init` 进行清理。

#### s_mongoc_auto_encryption_opts_org_clear

```cpp
static void s_mongoc_auto_encryption_opts_org_clear(void* popts);
```

清理原始的 MongoDB 自动加密选项。

**参数：**
- `popts`: 指向原始选项结构体的指针

### KMS 凭据提供者回调设置

#### s_mongoc_auto_encryption_opts_set_kms_credential_provider_callback

```cpp
inline static bool s_mongoc_auto_encryption_opts_set_kms_credential_provider_callback(void* popts,
                                                                                      kms_credentials_provider_callback_fn_type fn,
                                                                                      void* userdata);
```

设置 KMS 凭据提供者回调函数。

**参数：**
- `popts`: 指向选项结构体的指针
- `fn`: 回调函数指针
- `userdata`: 用户数据指针

**返回值：** `bool` - 设置是否成功

该方法调用 MongoDB C 驱动的相应函数来设置回调。

#### s_mongoc_auto_encryption_opts_set_kms_credential_provider_callback (模板版本)

```cpp
template<typename UserData> inline
static bool s_mongoc_auto_encryption_opts_set_kms_credential_provider_callback(void* popts,
                                                                                kms_credentials_provider_callback_fn_type fn,
                                                                                UserData& userdata);
```

模板版本的 KMS 凭据提供者回调设置。

**模板参数：**
- `UserData`: 用户数据类型

**参数：**
- `popts`: 指向选项结构体的指针
- `fn`: 回调函数指针
- `userdata`: 用户数据引用

**返回值：** `bool` - 设置是否成功

该版本自动获取用户数据的地址，避免手动转换。

## 设计理念

### 扩展原生功能
- **结构体扩展**: 在原生 `mongoc_auto_encryption_opts_t` 基础上提供扩展结构体
- **兼容性**: 保持与 MongoDB C 驱动的兼容性
- **便利性**: 提供更方便的接口和模板方法

### 内存管理
- **非拥有指针**: Key Vault 客户端和客户端池不拥有所有权，必须保证生命周期
- **自动清理**: 提供清理方法确保资源正确释放
- **零初始化**: 使用 memset 进行安全的结构体初始化

### 类型安全
- **模板方法**: 为用户数据提供类型安全的模板版本
- **地址获取**: 自动处理引用到指针的转换
- **静态方法**: 所有操作都是静态的，无需实例化

## 依赖项

- `yggr/base/yggrdef.h` - 基础定义
- `yggr/mplex/static_assert.hpp` - 静态断言
- `yggr/nsql_database_system/mongodb_config.hpp` - MongoDB 配置
- `yggr/nsql_database_system/detail/to_pointer.hpp` - 指针转换工具
- `yggr/nsql_database_system/mongoc_mc_kms_credentials_callback_native_ex.hpp` - KMS 凭据回调扩展
- `boost/mpl/bool.hpp` - MPL 布尔类型
- `<memory>` - 内存管理

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_auto_encryption_opts_native_ex.hpp>

// 创建自动加密选项
yggr_mongoc_auto_encryption_opts opts;

// 初始化选项
mongoc_auto_encryption_opts_native_ex::s_mongoc_auto_encryption_opts_init(&opts);

// 配置 Key Vault
opts.keyvault_db = "admin";
opts.keyvault_coll = "datakeys";

// 设置 KMS 提供者
bson_t kms_providers = BSON_INITIALIZER;
BSON_APPEND_DOCUMENT_BEGIN(&kms_providers, "local", &local_kms);
// ... 配置 KMS 提供者 ...
bson_append_document_end(&kms_providers, &local_kms);

opts.kms_providers = &kms_providers;

// 设置 KMS 凭据提供者回调
auto creds_callback = [](mongoc_kms_credentials_provider_callback_fn fn,
                        void* userdata) -> bool {
    // 实现凭据提供逻辑
    return true;
};

mongoc_auto_encryption_opts_native_ex::s_mongoc_auto_encryption_opts_set_kms_credential_provider_callback(
    &opts, creds_callback, nullptr);

// 使用选项创建加密客户端
// mongoc_client_t* client = mongoc_client_new_with_auto_encryption_opts(uri, &opts, nullptr);

// 清理选项
mongoc_auto_encryption_opts_native_ex::s_mongoc_auto_encryption_opts_clear(&opts);
bson_destroy(&kms_providers);
```

## 注意事项

1. **生命周期管理**: `keyvault_client` 和 `keyvault_client_pool` 不拥有所有权，必须保证它们比自动加密客户端生存时间更长
2. **内存管理**: 需要手动管理 BSON 对象的生命周期
3. **回调函数**: KMS 凭据提供者回调必须正确实现以提供加密密钥
4. **初始化**: 总是使用提供的初始化方法来初始化结构体
5. **清理**: 使用相应的清理方法来释放资源

## 许可证

该文件遵循项目的许可证协议，基于 MIT 许可证。