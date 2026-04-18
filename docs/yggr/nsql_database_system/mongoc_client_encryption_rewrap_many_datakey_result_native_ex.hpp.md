# mongoc_client_encryption_rewrap_many_datakey_result_native_ex.hpp

## 概述

`mongoc_client_encryption_rewrap_many_datakey_result_native_ex.hpp` 是 Yggdrasil 数据库系统中 MongoDB 客户端加密重新包装多个数据密钥结果原生扩展的头文件。该文件定义了 `yggr_mongoc_client_encryption_rewrap_many_datakey_result_t` 结构体，提供对 MongoDB 客户端字段级加密批量重新包装数据密钥操作结果的封装。

该结构体封装了 MongoDB C 驱动的重新包装多个数据密钥结果，提供了对批量密钥重新包装操作结果的类型安全访问。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    struct yggr_mongoc_client_encryption_rewrap_many_datakey_result_t;
}
}
```

## 结构体定义

### yggr_mongoc_client_encryption_rewrap_many_datakey_result_t

```cpp
struct yggr_mongoc_client_encryption_rewrap_many_datakey_result_t
{
    bson_t bulk_write_result;
};
```

该结构体表示批量重新包装数据密钥操作的结果：

- **bulk_write_result**: 批量写入操作的结果，以 BSON 文档格式存储

该结果包含了重新包装操作的详细信息，如：
- 成功重新包装的数据密钥数量
- 失败的操作及其错误信息
- 操作的统计信息

## 重新包装操作概述

### 数据密钥重新包装 (Data Key Rewrapping)
数据密钥重新包装是指使用新的主密钥重新加密现有的数据加密密钥 (DEK) 的过程：

- **目的**: 更新密钥层次结构中的主密钥，而无需重新加密所有数据
- **场景**: 主密钥轮换、密钥迁移、合规性要求等
- **优势**: 高效的密钥管理，无需大规模数据重新加密

### 批量重新包装 (Bulk Rewrapping)
批量重新包装允许一次性重新包装多个数据密钥：

- **效率**: 减少网络往返和操作开销
- **原子性**: 整个批量操作要么全部成功，要么全部失败
- **监控**: 通过结果结构体获取详细的操作统计

## 结果字段详解

### bulk_write_result
`bulk_write_result` 字段包含批量写入操作的完整结果信息：

**可能的字段：**
- `nInserted`: 插入的文档数量
- `nUpserted`: 更新插入的文档数量
- `nMatched`: 匹配的文档数量
- `nModified`: 修改的文档数量
- `nRemoved`: 删除的文档数量
- `writeErrors`: 写入错误数组
- `writeConcernErrors`: 写入关注错误数组
- `upserted`: 更新插入的文档 ID 数组

**在重新包装上下文中的含义：**
- `nModified`: 成功重新包装的数据密钥数量
- `writeErrors`: 重新包装失败的数据密钥及其错误信息
- `nMatched`: 找到并尝试重新包装的数据密钥数量

## 使用场景

### 密钥轮换 (Key Rotation)
```cpp
// 定期轮换主密钥
// 1. 创建新的主密钥
// 2. 批量重新包装所有数据密钥
// 3. 验证重新包装结果
// 4. 安全删除旧的主密钥
```

### 密钥迁移 (Key Migration)
```cpp
// 将数据密钥从一个 KMS 迁移到另一个
// 1. 配置新的 KMS 提供者
// 2. 批量重新包装数据密钥
// 3. 更新应用程序配置
// 4. 清理旧的 KMS 配置
```

### 合规性维护 (Compliance Maintenance)
```cpp
// 满足安全合规要求
// 1. 定期重新包装密钥
// 2. 记录重新包装操作
// 3. 生成审计报告
```

## 设计理念

### 简单封装
- **单一字段**: 只封装必要的批量写入结果
- **类型安全**: 使用 BSON 文档确保数据完整性
- **内存效率**: 最小化结构体大小

### 兼容性
- **标准格式**: 使用 MongoDB 标准的批量写入结果格式
- **版本无关**: 不依赖特定版本的 MongoDB C 驱动特性
- **扩展友好**: 可以轻松添加未来需要的字段

### 错误处理
- **完整信息**: 保留所有错误详情和统计信息
- **调试友好**: 便于诊断批量操作中的问题
- **审计就绪**: 支持合规性和审计要求

## 依赖项

- `yggr/nsql_database_system/mongodb_config.hpp` - MongoDB 配置

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_client_encryption_rewrap_many_datakey_result_native_ex.hpp>

// 执行批量重新包装操作
// mongoc_client_encryption_rewrap_many_datakey_result_t* result =
//     mongoc_client_encryption_rewrap_many_datakey(client_encryption,
//                                                  filter, // 选择要重新包装的密钥
//                                                  &new_masterkey, // 新的主密钥
//                                                  &error);

// 检查结果
if (result) {
    // 访问批量写入结果
    const bson_t* bulk_result = &result->bulk_write_result;

    // 解析结果文档
    bson_iter_t iter;
    if (bson_iter_init(&iter, bulk_result)) {
        while (bson_iter_next(&iter)) {
            const char* key = bson_iter_key(&iter);
            if (strcmp(key, "nModified") == 0) {
                int64_t modified = bson_iter_int64(&iter);
                printf("成功重新包装 %lld 个数据密钥\n", modified);
            } else if (strcmp(key, "writeErrors") == 0) {
                // 处理写入错误
                // ...
            }
        }
    }

    // 释放结果
    // mongoc_client_encryption_rewrap_many_datakey_result_destroy(result);
}
```

## 注意事项

1. **结果生命周期**: 正确管理结果结构体的内存生命周期
2. **错误检查**: 始终检查批量操作中的错误和失败情况
3. **性能监控**: 使用结果统计信息监控操作性能
4. **审计记录**: 记录重新包装操作以满足合规要求
5. **回滚考虑**: 了解批量操作的原子性保证

## 许可证

该文件遵循项目的许可证协议，基于 MIT 许可证。