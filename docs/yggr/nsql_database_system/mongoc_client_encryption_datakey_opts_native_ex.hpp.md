# mongoc_client_encryption_datakey_opts_native_ex.hpp

## 概述

`mongoc_client_encryption_datakey_opts_native_ex.hpp` 是 Yggdrasil 数据库系统中 MongoDB 客户端加密数据密钥选项原生扩展的头文件。该文件定义了 `mongoc_client_encryption_datakey_opts_native_ex` 类，提供对 MongoDB 客户端字段级加密数据密钥选项的扩展支持。

该类是对 MongoDB C 驱动客户端加密数据密钥选项的封装，提供了更安全和方便的接口来配置数据加密密钥（Data Encryption Key）的参数。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    class mongoc_client_encryption_datakey_opts_native_ex;
}
}
```

## 结构体定义

### yggr_mongoc_client_encryption_datakey_opts

```cpp
struct yggr_mongoc_client_encryption_datakey_opts
{
    bson_t* masterkey;
    char** keyaltnames;
    uint32_t keyaltnames_count;
    uint8_t* keymaterial;
    uint32_t keymaterial_len;
};
```

该结构体定义了数据密钥的配置选项：

- **masterkey**: 主密钥文档，用于加密数据密钥
- **keyaltnames**: 密钥别名数组，提供额外的密钥标识方式
- **keyaltnames_count**: 密钥别名数组的长度
- **keymaterial**: 自定义密钥材料（可选）
- **keymaterial_len**: 密钥材料的长度

## 类定义

### mongoc_client_encryption_datakey_opts_native_ex

```cpp
class mongoc_client_encryption_datakey_opts_native_ex
{
public:
    typedef mongoc_client_encryption_datakey_opts_t org_client_encryption_datakey_opts_type;
    typedef yggr_mongoc_client_encryption_datakey_opts yggr_client_encryption_datakey_opts_type;
};
```

该类提供静态方法来操作数据密钥选项。

## 主要功能

### 初始化和清理

#### s_mongoc_client_encryption_datakey_opts_init

```cpp
inline static bool s_mongoc_client_encryption_datakey_opts_init(void* popts);
```

初始化数据密钥选项结构体。

**参数：**
- `popts`: 指向选项结构体的指针

**返回值：** `bool` - 初始化是否成功

该方法将结构体清零初始化。

#### s_mongoc_client_encryption_datakey_opts_clear

```cpp
inline static void s_mongoc_client_encryption_datakey_opts_clear(void* popts);
```

清理数据密钥选项结构体。

**参数：**
- `popts`: 指向选项结构体的指针

实际上调用初始化方法进行清理。

#### s_mongoc_client_encryption_datakey_opts_org_clear

```cpp
static void s_mongoc_client_encryption_datakey_opts_org_clear(void* popts);
```

清理原始的 MongoDB 数据密钥选项。

**参数：**
- `popts`: 指向原始选项结构体的指针

### 保护方法

#### pro_s__clear_datakey_keyaltnames

```cpp
static void pro_s__clear_datakey_keyaltnames(void* popts);
```

清理数据密钥的别名数组。

**参数：**
- `popts`: 指向选项结构体的指针

该方法负责释放密钥别名数组占用的内存。

## 设计理念

### 数据密钥管理
- **主密钥**: 支持配置用于加密数据密钥的主密钥
- **密钥别名**: 提供多种方式来引用同一个数据密钥
- **自定义材料**: 支持使用自定义的密钥材料创建数据密钥

### 内存安全
- **结构体初始化**: 提供安全的清零初始化
- **资源清理**: 正确的内存释放和资源管理
- **保护方法**: 内部辅助方法确保正确的清理顺序

### 扩展兼容性
- **原生兼容**: 与 MongoDB C 驱动的原生结构体兼容
- **扩展结构体**: 提供 Yggdrasil 特定的扩展版本
- **类型定义**: 清晰的类型别名便于使用

## 依赖项

- `yggr/nsql_database_system/mongodb_config.hpp` - MongoDB 配置
- `<memory>` - 内存管理

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_client_encryption_datakey_opts_native_ex.hpp>

// 创建数据密钥选项
yggr_mongoc_client_encryption_datakey_opts opts;

// 初始化选项
mongoc_client_encryption_datakey_opts_native_ex::s_mongoc_client_encryption_datakey_opts_init(&opts);

// 配置主密钥 (例如使用 AWS KMS)
bson_t masterkey = BSON_INITIALIZER;
BSON_APPEND_UTF8(&masterkey, "provider", "aws");
BSON_APPEND_UTF8(&masterkey, "key", "arn:aws:kms:us-east-1:123456789012:key/12345678-1234-1234-1234-123456789012");
BSON_APPEND_UTF8(&masterkey, "region", "us-east-1");

opts.masterkey = &masterkey;

// 配置密钥别名
const char* aliases[] = {"myKeyAlias", "anotherAlias"};
opts.keyaltnames = const_cast<char**>(aliases);
opts.keyaltnames_count = 2;

// 可选：提供自定义密钥材料
// uint8_t custom_key[32] = {...};
// opts.keymaterial = custom_key;
// opts.keymaterial_len = 32;

// 使用选项创建数据密钥
// mongoc_client_encryption_datakey_opts_t* datakey_opts = ...;
// // 设置选项到 datakey_opts
// bson_value_t datakey_id = {0};
// mongoc_client_encryption_create_datakey(client_encryption, datakey_opts, &datakey_id, nullptr);

// 清理选项
mongoc_client_encryption_datakey_opts_native_ex::s_mongoc_client_encryption_datakey_opts_clear(&opts);
bson_destroy(&masterkey);
```

## 数据密钥概念

### 数据加密密钥 (DEK)
数据加密密钥是用于加密实际数据的密钥。在字段级加密中：
- DEK 本身由客户主密钥 (CMK) 加密存储
- 每个 DEK 可以有多个别名用于引用
- DEK 可以是自动生成或自定义提供的

### 主密钥 (Master Key)
主密钥用于加密数据加密密钥，支持多种 KMS 提供者：
- **本地**: 本地生成的密钥
- **AWS KMS**: Amazon Web Services 密钥管理服务
- **Azure Key Vault**: Microsoft Azure 密钥保管库
- **GCP KMS**: Google Cloud Platform 密钥管理服务

### 密钥别名 (Key Alt Names)
- 提供额外的密钥标识方式
- 支持更灵活的密钥管理和引用
- 可以有多个别名指向同一个数据密钥

## 注意事项

1. **内存管理**: 需要手动管理 BSON 对象和字符串数组的生命周期
2. **主密钥配置**: 必须正确配置主密钥才能创建有效的数据密钥
3. **别名管理**: 密钥别名数组必须是连续的字符串指针数组
4. **自定义材料**: 提供自定义密钥材料时确保长度正确（通常为32字节）
5. **初始化**: 总是使用提供的初始化方法来初始化结构体

## 许可证

该文件遵循项目的许可证协议，基于 MIT 许可证。