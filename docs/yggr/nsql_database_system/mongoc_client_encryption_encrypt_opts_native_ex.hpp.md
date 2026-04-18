# mongoc_client_encryption_encrypt_opts_native_ex.hpp

## 概述

`mongoc_client_encryption_encrypt_opts_native_ex.hpp` 是 Yggdrasil 数据库系统中 MongoDB 客户端加密选项原生扩展的头文件。该文件定义了 `mongoc_client_encryption_encrypt_opts_native_ex` 类，提供对 MongoDB 客户端字段级加密选项的扩展支持。

该类封装了 MongoDB C 驱动的加密选项，提供了类型安全的接口来配置字段加密参数，包括加密算法、密钥选择、查询类型等。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    class mongoc_client_encryption_encrypt_opts_native_ex;
}
}
```

## 结构体定义

### yggr_mongoc_client_encryption_encrypt_opts

```cpp
struct yggr_mongoc_client_encryption_encrypt_opts
{
    typedef struct
    {
        int64_t value;
        bool set;
    } contention_factor_type;

    bson_value_t keyid;
    char *algorithm;
    char *keyaltname;
    contention_factor_type contention_factor;
    char *query_type;
    mongoc_client_encryption_encrypt_range_opts_t *range_opts;
};
```

该结构体定义了字段加密的配置选项：

- **keyid**: 数据加密密钥的 ID
- **algorithm**: 加密算法名称
- **keyaltname**: 密钥别名（可选）
- **contention_factor**: 争用因子，用于索引加密
- **query_type**: 查询类型（用于索引加密）
- **range_opts**: 范围加密选项

## 类定义

### mongoc_client_encryption_encrypt_opts_native_ex

```cpp
class mongoc_client_encryption_encrypt_opts_native_ex
{
public:
    typedef mongoc_client_encryption_encrypt_opts_t org_client_encryption_encrypt_opts_type;
    typedef yggr_mongoc_client_encryption_encrypt_opts yggr_client_encryption_encrypt_range_type;
    typedef charset::utf8_string inner_string_type;
    typedef charset::utf8_string_view inner_string_view_type;
};
```

该类提供静态方法来操作加密选项和获取标准值。

## 加密算法常量

### AEAD_AES_256_CBC_HMAC_SHA_512 算法

#### s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_random

```cpp
inline static const inner_string_type& s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_random(void);
```

返回随机加密算法名称：`"AEAD_AES_256_CBC_HMAC_SHA_512-Random"`

适用于需要随机加密的字段，每个相同的值会产生不同的密文。

#### s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_deterministic

```cpp
inline static const inner_string_type& s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_deterministic(void);
```

返回确定性加密算法名称：`"AEAD_AES_256_CBC_HMAC_SHA_512-Deterministic"`

适用于需要可查询的字段，相同的值总是产生相同的密文。

### 索引加密算法

#### s_encrypt_algorithm_indexed

```cpp
inline static const inner_string_type& s_encrypt_algorithm_indexed(void);
```

返回索引加密算法名称：`"Indexed"`

用于支持范围查询的字段加密。

#### s_encrypt_algorithm_unindexed

```cpp
inline static const inner_string_type& s_encrypt_algorithm_unindexed(void);
```

返回非索引加密算法名称：`"Unindexed"`

用于不支持范围查询的字段加密。

### 范围加密算法

#### s_encrypt_algorithm_range

```cpp
inline static const inner_string_type& s_encrypt_algorithm_range(void);
```

返回范围加密算法名称。

根据 MongoDB 版本不同：
- MongoDB C Driver < 1.28.0: `"RangePreview"`
- MongoDB C Driver >= 1.28.0: `"Range"`

#### s_encrypt_algorithm_range_preview

```cpp
inline static const inner_string_type& s_encrypt_algorithm_range_preview(void);
```

返回范围预览算法名称（向后兼容）。

## 查询模式常量

### s_encrypt_query_mode_equality

```cpp
inline static const inner_string_type& s_encrypt_query_mode_equality(void);
```

返回等值查询模式：`"equality"`

用于等值比较的索引加密字段。

### s_encrypt_query_mode_range

```cpp
inline static const inner_string_type& s_encrypt_query_mode_range(void);
```

返回范围查询模式。

根据 MongoDB 版本不同：
- MongoDB C Driver < 1.28.0: `"rangePreview"`
- MongoDB C Driver >= 1.28.0: `"range"`

### s_encrypt_query_mode_range_preview

```cpp
inline static const inner_string_type& s_encrypt_query_mode_range_preview(void);
```

返回范围预览查询模式（向后兼容）。

## 选项管理方法

### s_mongoc_client_encryption_encrypt_opts_init

```cpp
static bool s_mongoc_client_encryption_encrypt_opts_init(void* popts);
```

初始化加密选项结构体。

**参数：**
- `popts`: 指向选项结构体的指针

**返回值：** `bool` - 初始化是否成功

### s_mongoc_client_encryption_encrypt_opts_clear

```cpp
static void s_mongoc_client_encryption_encrypt_opts_clear(void* popts);
```

清理加密选项结构体。

**参数：**
- `popts`: 指向选项结构体的指针

### s_mongoc_client_encryption_encrypt_opts_org_clear

```cpp
static void s_mongoc_client_encryption_encrypt_opts_org_clear(void* popts);
```

清理原始的 MongoDB 加密选项。

**参数：**
- `popts`: 指向原始选项结构体的指针

## 加密算法类型

### 随机加密 (Random)
- **适用场景**: 不需要查询的敏感字段
- **特点**: 相同明文产生不同密文，提供最高安全性
- **查询支持**: 不支持任何查询操作

### 确定性加密 (Deterministic)
- **适用场景**: 需要等值查询的字段
- **特点**: 相同明文总是产生相同密文
- **查询支持**: 支持等值查询（$eq）

### 索引加密 (Indexed)
- **适用场景**: 需要范围查询的数值字段
- **特点**: 支持范围和等值查询
- **查询支持**: 支持 $eq, $gt, $gte, $lt, $lte, $in 等操作符

### 范围加密 (Range)
- **适用场景**: 需要范围查询的字段（MongoDB 6.1+）
- **特点**: 提供更好的范围查询性能
- **查询支持**: 支持所有比较操作符

## 设计理念

### 版本兼容性
- **条件编译**: 根据 MongoDB C 驱动版本使用不同的常量
- **向后兼容**: 提供预览版本的别名确保兼容性
- **未来证明**: 支持新版本的特性

### 类型安全
- **字符串类型**: 使用 UTF-8 字符串类型确保编码正确
- **静态常量**: 编译时生成常量，避免运行时开销
- **引用返回**: 返回常量引用避免拷贝

### 内存管理
- **结构体初始化**: 安全的选项结构体初始化
- **资源清理**: 正确的内存释放和资源管理
- **RAII友好**: 支持 RAII 编程模式

## 依赖项

- `yggr/base/yggrdef.h` - 基础定义
- `yggr/charset/utf8_string.hpp` - UTF-8 字符串
- `yggr/charset/utf8_string_view.hpp` - UTF-8 字符串视图
- `yggr/nsql_database_system/mongodb_config.hpp` - MongoDB 配置
- `yggr/nsql_database_system/mongoc_client_encryption_encrypt_range_opts_native_ex.hpp` - 范围加密选项
- `boost/ref.hpp` - Boost 引用包装器

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_client_encryption_encrypt_opts_native_ex.hpp>

// 创建加密选项
yggr_mongoc_client_encryption_encrypt_opts opts;

// 初始化选项
mongoc_client_encryption_encrypt_opts_native_ex::s_mongoc_client_encryption_encrypt_opts_init(&opts);

// 配置随机加密
opts.algorithm = const_cast<char*>(
    mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_random().c_str()
);

// 设置密钥ID
bson_value_t key_id = {0};
// ... 设置密钥ID ...
opts.keyid = key_id;

// 对于索引加密，设置查询类型
opts.algorithm = const_cast<char*>(
    mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_algorithm_indexed().c_str()
);
opts.query_type = const_cast<char*>(
    mongoc_client_encryption_encrypt_opts_native_ex::s_encrypt_query_mode_equality().c_str()
);

// 可选：设置争用因子（用于索引加密）
opts.contention_factor.value = 1;
opts.contention_factor.set = true;

// 使用选项进行加密
// mongoc_client_encryption_encrypt(client_encryption, value, opts, &encrypted_value, &error);

// 清理选项
mongoc_client_encryption_encrypt_opts_native_ex::s_mongoc_client_encryption_encrypt_opts_clear(&opts);
```

## 注意事项

1. **算法选择**: 根据查询需求选择合适的加密算法
2. **版本兼容性**: 注意 MongoDB 和 C 驱动的版本要求
3. **内存管理**: 需要手动管理字符串和结构体的生命周期
4. **密钥管理**: 确保正确设置数据加密密钥 ID 或别名
5. **查询限制**: 随机加密不支持任何查询操作

## 许可证

该文件遵循项目的许可证协议，基于 MIT 许可证。