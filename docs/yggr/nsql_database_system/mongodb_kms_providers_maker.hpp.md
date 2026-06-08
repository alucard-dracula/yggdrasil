# mongodb_kms_providers_maker.hpp

## 概述

此头文件定义了 `mongodb_kms_providers_maker` 类，用于创建 MongoDB 客户端加密的 KMS (Key Management Service) 提供者配置。KMS 提供者用于管理客户端字段级加密的数据加密密钥。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

### mongodb_kms_providers_maker

一个工具类，提供创建各种 KMS 提供者配置的方法。目前主要支持本地 KMS 提供者。

#### 类型定义

##### `bson_type`

BSON 类型，定义为 `c_bson`。

##### `inner_string_type`

内部字符串类型，使用 UTF-8 编码的字符串。

##### `inner_string_view_type`

内部字符串视图类型，使用 UTF-8 字符串视图。

#### KMS 提供者模式名称常量

##### `s_kms_providers_mode_name_local`

返回本地 KMS 提供者模式名称："local"

##### `s_kms_providers_mode_name_aws`

返回 AWS KMS 提供者模式名称："aws"

##### `s_kms_providers_mode_name_azure`

返回 Azure KMS 提供者模式名称："azure"

##### `s_kms_providers_mode_name_gcp`

返回 GCP KMS 提供者模式名称："gcp"

##### `s_kms_providers_mode_name_kmip`

返回 KMIP KMS 提供者模式名称："kmip"

#### KMS 提供者创建方法

##### `s_make_local_kms_providers` (引用版本)

```cpp
template<typename Bson, typename BinaryBuffer>
static typename boost::enable_if<boost::is_base_of<bson_t, Bson>, Bson&>::type
s_make_local_kms_providers(Bson& bs_kms_providers, const BinaryBuffer& binary_buffer)
```

在现有的 BSON 对象中创建本地 KMS 提供者配置。

**参数：**
- `bs_kms_providers`：BSON 对象引用，用于存储 KMS 提供者配置
- `binary_buffer`：二进制缓冲区，包含本地主密钥

**返回值：**
- 修改后的 BSON 对象引用

**模板参数：**
- `Bson`：必须是 `bson_t` 的派生类
- `BinaryBuffer`：二进制缓冲区类型

##### `s_make_local_kms_providers` (新建版本)

```cpp
template<typename BinaryBuffer>
static bson_type s_make_local_kms_providers(const BinaryBuffer& binary_buffer)
```

创建新的 BSON 对象并配置本地 KMS 提供者。

**参数：**
- `binary_buffer`：二进制缓冲区，包含本地主密钥

**返回值：**
- 包含本地 KMS 提供者配置的新 BSON 对象

**模板参数：**
- `BinaryBuffer`：二进制缓冲区类型

## 依赖项

- `<yggr/charset/utf8_string.hpp>`
- `<yggr/charset/utf8_string_view.hpp>`
- `<yggr/nsql_database_system/c_bson.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongodb_kms_providers_maker.hpp>

using namespace yggr::nsql_database_system;

// 获取 KMS 提供者模式名称
const charset::utf8_string& local_mode = mongodb_kms_providers_maker::s_kms_providers_mode_name_local();
const charset::utf8_string& aws_mode = mongodb_kms_providers_maker::s_kms_providers_mode_name_aws();

// 创建本地 KMS 提供者配置
// 假设有一个包含 96 字节主密钥的二进制缓冲区
std::vector<uint8_t> master_key(96, 0x00); // 示例主密钥

// 方法1: 创建新的 BSON 对象
mongodb_kms_providers_maker::bson_type kms_providers =
    mongodb_kms_providers_maker::s_make_local_kms_providers(master_key);

// 方法2: 在现有 BSON 对象中添加配置
bson_t existing_bson = BSON_INITIALIZER;
mongodb_kms_providers_maker::s_make_local_kms_providers(existing_bson, master_key);

// 现在 kms_providers 或 existing_bson 包含了本地 KMS 提供者配置
// 可以用于 MongoDB 客户端加密选项
```

## 注意事项

- 目前只实现了本地 KMS 提供者的创建方法
- 本地 KMS 使用对称密钥进行数据加密
- 主密钥应该是 96 字节（768 位）的二进制数据
- AWS、Azure、GCP 和 KMIP 模式名称已定义，但创建方法尚未实现
- BSON 对象在使用前应该正确初始化
- 返回的 BSON 对象可以直接用于 MongoDB 客户端加密配置
- 字符串编码使用 UTF-8，确保字符集兼容性
- 模板方法使用 SFINAE 确保类型安全
- 本地 KMS 适合开发和测试环境，生产环境建议使用云 KMS 服务
- KMS 提供者配置是客户端字段级加密的必需组件</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongodb_kms_providers_maker_cn.md