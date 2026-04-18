# mongoc_client_encryption_encrypt_range_opts_native_ex.hpp

## 概述

`mongoc_client_encryption_encrypt_range_opts_native_ex.hpp` 是 Yggdrasil 数据库系统中 MongoDB 客户端加密范围选项原生扩展的头文件。该文件定义了 `mongoc_client_encryption_encrypt_range_opts_native_ex` 类，提供对 MongoDB 客户端字段级范围加密选项的扩展支持。

该类封装了 MongoDB C 驱动的范围加密选项，提供了类型安全的接口来配置范围查询加密的参数，包括最小值、最大值、稀疏度、精度和修剪因子等。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    class mongoc_client_encryption_encrypt_range_opts_native_ex;
}
}
```

## 版本兼容性

该文件根据 MongoDB C 驱动版本的不同提供不同的结构体定义：

### MongoDB C Driver < 1.28.0 (0x011C0000)

```cpp
struct yggr_mongoc_client_encryption_encrypt_range_opts
{
    typedef struct
    {
        bson_value_t value;
        bool set;
    } min_max_type;

    typedef struct
    {
        int32_t value;
        bool set;
    } precision_type;

    min_max_type min;        // 最小值
    min_max_type max;        // 最大值
    int64_t sparsity;        // 稀疏度
    precision_type precision; // 精度
};
```

### MongoDB C Driver >= 1.28.0

```cpp
struct yggr_mongoc_client_encryption_encrypt_range_opts
{
    typedef struct
    {
        bson_value_t value;
        bool set;
    } min_max_type;

    typedef struct
    {
        int32_t value;
        bool set;
    } int32_val_type;

    typedef int32_val_type trim_factor_type;
    typedef int32_val_type precision_type;

    typedef struct
    {
        int64_t value;
        bool set;
    } sparsity_type;

    min_max_type min;         // 最小值
    min_max_type max;         // 最大值
    trim_factor_type trim_factor; // 修剪因子
    sparsity_type sparsity;   // 稀疏度
    precision_type precision; // 精度
};
```

## 结构体字段说明

### min_max_type
```cpp
typedef struct
{
    bson_value_t value; // BSON 值
    bool set;          // 是否设置了该值
} min_max_type;
```

用于表示范围的最小值和最大值。

- **value**: BSON 值，包含实际的数值
- **set**: 布尔标志，表示该值是否已被设置

### sparsity_type (仅限新版本)
```cpp
typedef struct
{
    int64_t value; // 稀疏度值
    bool set;     // 是否设置了稀疏度
} sparsity_type;
```

控制索引的稀疏程度，影响查询性能和存储效率。

### trim_factor_type (仅限新版本)
```cpp
typedef struct
{
    int32_t value; // 修剪因子值
    bool set;     // 是否设置了修剪因子
} trim_factor_type;
```

用于优化范围查询的索引修剪。

### precision_type
```cpp
typedef struct
{
    int32_t value; // 精度值
    bool set;     // 是否设置了精度
} precision_type;
```

定义数值精度的位数，用于浮点数或高精度数值的加密。

## 类定义

### mongoc_client_encryption_encrypt_range_opts_native_ex

```cpp
class mongoc_client_encryption_encrypt_range_opts_native_ex
{
public:
    typedef mongoc_client_encryption_encrypt_range_opts_t org_client_encryption_encrypt_range_opts_type;
    typedef yggr_mongoc_client_encryption_encrypt_range_opts yggr_client_encryption_encrypt_range_opts_type;
};
```

该类提供静态方法来操作范围加密选项。

## 选项管理方法

### s_mongoc_client_encryption_encrypt_range_opts_init

```cpp
inline static bool s_mongoc_client_encryption_encrypt_range_opts_init(void* popts)
```

初始化范围加密选项结构体。

**参数：**
- `popts`: 指向选项结构体的指针

**返回值：** `bool` - 初始化是否成功

**实现：**
```cpp
return popts && (memset(popts, 0, sizeof(yggr_client_encryption_encrypt_range_opts_type)), true);
```

将结构体内存清零，确保所有字段初始化为默认值。

### s_mongoc_client_encryption_encrypt_range_opts_clear

```cpp
static void s_mongoc_client_encryption_encrypt_range_opts_clear(void* popts);
```

清理范围加密选项结构体。

**参数：**
- `popts`: 指向选项结构体的指针

释放结构体中分配的资源，如 BSON 值。

### s_mongoc_client_encryption_encrypt_range_opts_copy

```cpp
static bool s_mongoc_client_encryption_encrypt_range_opts_copy(void* pdst_opts, const void* psrc_opts);
```

复制范围加密选项。

**参数：**
- `pdst_opts`: 目标选项结构体指针
- `psrc_opts`: 源选项结构体指针

**返回值：** `bool` - 复制是否成功

深度复制所有字段，包括 BSON 值。

### s_mongoc_client_encryption_encrypt_range_opts_swap

```cpp
static void s_mongoc_client_encryption_encrypt_range_opts_swap(void* pdst_opts, void* psrc_opts);
```

交换两个范围加密选项结构体。

**参数：**
- `pdst_opts`: 第一个选项结构体指针
- `psrc_opts`: 第二个选项结构体指针

高效的内存交换操作。

### s_mongoc_client_encryption_encrypt_range_opts_move

```cpp
static void s_mongoc_client_encryption_encrypt_range_opts_move(void* pdst_opts, void* psrc_opts);
```

移动范围加密选项。

**参数：**
- `pdst_opts`: 目标选项结构体指针
- `psrc_opts`: 源选项结构体指针

将资源所有权从源移动到目标。

## 范围加密参数详解

### 最小值和最大值 (min/max)
- **用途**: 定义加密字段的取值范围
- **类型**: BSON 值，支持各种数值类型
- **可选性**: 可以不设置，表示无界范围

### 稀疏度 (sparsity)
- **用途**: 控制索引的稀疏程度
- **类型**: 64位整数
- **影响**: 更高的稀疏度减少存储空间但可能降低查询性能
- **默认值**: 未设置时使用 MongoDB 默认值

### 修剪因子 (trim_factor) - 新版本特性
- **用途**: 优化范围查询的索引修剪
- **类型**: 32位整数
- **影响**: 影响索引的构建和查询效率

### 精度 (precision)
- **用途**: 定义数值精度的位数
- **类型**: 32位整数
- **适用**: 浮点数或需要高精度控制的数值字段

## 设计理念

### 版本兼容性
- **条件编译**: 根据 MongoDB C 驱动版本自动选择合适的结构体定义
- **向后兼容**: 确保代码在不同版本的 MongoDB C 驱动下正常工作
- **特性渐进**: 新版本添加的功能不会破坏旧版本的兼容性

### 内存安全
- **资源管理**: 提供完整的初始化、清理、复制和移动操作
- **零初始化**: 使用 memset 确保结构体安全初始化
- **深度复制**: 正确处理 BSON 值等复杂类型的复制

### 类型安全
- **类型别名**: 使用 typedef 定义清晰的类型名称
- **结构体封装**: 将相关字段组织在结构体中
- **布尔标志**: 使用 set 字段明确表示值是否有效

## 依赖项

- `yggr/base/yggrdef.h` - 基础定义
- `yggr/nsql_database_system/mongodb_config.hpp` - MongoDB 配置
- `yggr/nsql_database_system/bson_value_native_ex.hpp` - BSON 值扩展
- `boost/ref.hpp` - Boost 引用包装器

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_client_encryption_encrypt_range_opts_native_ex.hpp>

// 创建范围选项
yggr_mongoc_client_encryption_encrypt_range_opts opts;

// 初始化选项
mongoc_client_encryption_encrypt_range_opts_native_ex::s_mongoc_client_encryption_encrypt_range_opts_init(&opts);

// 设置最小值
bson_value_t min_val = {0};
min_val.value_type = BSON_TYPE_INT64;
min_val.value.v_int64 = 0;
opts.min.value = min_val;
opts.min.set = true;

// 设置最大值
bson_value_t max_val = {0};
max_val.value_type = BSON_TYPE_INT64;
max_val.value.v_int64 = 1000;
opts.max.value = max_val;
opts.max.set = true;

// 设置稀疏度（如果支持）
opts.sparsity.value = 2;
opts.sparsity.set = true;

// 设置精度
opts.precision.value = 2; // 两位小数精度
opts.precision.set = true;

// 使用选项进行范围加密
// mongoc_client_encryption_encrypt_range(client_encryption, value, &opts, &encrypted_value, &error);

// 清理选项
mongoc_client_encryption_encrypt_range_opts_native_ex::s_mongoc_client_encryption_encrypt_range_opts_clear(&opts);
```

## 注意事项

1. **版本兼容性**: 注意 MongoDB C 驱动版本对可用字段的影响
2. **内存管理**: 正确初始化和清理选项结构体
3. **BSON 值管理**: 确保 BSON 值的生命周期正确管理
4. **参数验证**: 在设置参数前验证值的有效性
5. **性能考虑**: 合理设置稀疏度和精度以平衡性能和准确性

## 许可证

该文件遵循项目的许可证协议，基于 MIT 许可证。