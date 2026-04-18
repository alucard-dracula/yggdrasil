# mongoc_client_native_ex.hpp

## 概述

`mongoc_client_native_ex.hpp` 是 Yggdrasil 数据库系统中 MongoDB 客户端原生扩展的头文件。该文件定义了 `mongoc_client_native_ex` 类，提供对 MongoDB 客户端连接配置的扩展支持。

该类封装了 MongoDB C 驱动的客户端配置方法，提供了类型安全的接口来设置客户端的读取偏好、读取关注和写入关注等重要配置选项。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    class mongoc_client_native_ex;
}
}
```

## 类定义

### mongoc_client_native_ex

```cpp
class mongoc_client_native_ex
{
public:
    static mongoc_read_prefs_t* s_set_read_prefs_pointer(mongoc_client_t* pclt, mongoc_read_prefs_t* pread_prefs);
    static mongoc_read_concern_t* s_set_read_concern_pointer(mongoc_client_t* pclt, mongoc_read_concern_t* pread_concern);
    static mongoc_write_concern_t* s_set_write_concern_pointer(mongoc_client_t* pclt, mongoc_write_concern_t* pwrite_concern);
};
```

该类提供静态方法来配置 MongoDB 客户端的各种关注点和偏好设置。

## 配置方法

### s_set_read_prefs_pointer

```cpp
static mongoc_read_prefs_t* s_set_read_prefs_pointer(mongoc_client_t* pclt, mongoc_read_prefs_t* pread_prefs)
```

设置客户端的读取偏好。

**参数：**
- `pclt`: MongoDB 客户端指针
- `pread_prefs`: 读取偏好对象指针

**返回值：** `mongoc_read_prefs_t*` - 设置的读取偏好对象指针

**读取偏好类型：**
- `MONGOC_READ_PRIMARY`: 只从主节点读取
- `MONGOC_READ_SECONDARY`: 只从从节点读取
- `MONGOC_READ_PRIMARY_PREFERRED`: 优先主节点，可读从节点
- `MONGOC_READ_SECONDARY_PREFERRED`: 优先从节点，可读主节点
- `MONGOC_READ_NEAREST`: 从最近的节点读取

### s_set_read_concern_pointer

```cpp
static mongoc_read_concern_t* s_set_read_concern_pointer(mongoc_client_t* pclt, mongoc_read_concern_t* pread_concern)
```

设置客户端的读取关注。

**参数：**
- `pclt`: MongoDB 客户端指针
- `pread_concern`: 读取关注对象指针

**返回值：** `mongoc_read_concern_t*` - 设置的读取关注对象指针

**读取关注级别：**
- `MONGOC_READ_CONCERN_LEVEL_LOCAL`: 读取本地数据（默认）
- `MONGOC_READ_CONCERN_LEVEL_MAJORITY`: 读取大多数节点确认的数据
- `MONGOC_READ_CONCERN_LEVEL_LINEARIZABLE`: 线性化读取
- `MONGOC_READ_CONCERN_LEVEL_AVAILABLE`: 读取可用数据

### s_set_write_concern_pointer

```cpp
static mongoc_write_concern_t* s_set_write_concern_pointer(mongoc_client_t* pclt, mongoc_write_concern_t* pwrite_concern)
```

设置客户端的写入关注。

**参数：**
- `pclt`: MongoDB 客户端指针
- `pwrite_concern`: 写入关注对象指针

**返回值：** `mongoc_write_concern_t*` - 设置的写入关注对象指针

**写入关注参数：**
- `w`: 写入确认数量（0=不等待，1=主节点，>1=指定节点数，"majority"=大多数）
- `wtimeout`: 写入超时时间（毫秒）
- `journal`: 是否等待 journal 写入
- `fsync`: 是否等待 fsync

## MongoDB 客户端配置详解

### 读取偏好 (Read Preferences)
读取偏好控制客户端从哪个节点读取数据：

- **PRIMARY**: 保证读取最新数据，适合需要强一致性的操作
- **SECONDARY**: 减少主节点负载，适合只读操作
- **PRIMARY_PREFERRED**: 平衡一致性和性能
- **SECONDARY_PREFERRED**: 最大化读取性能
- **NEAREST**: 最小化网络延迟

### 读取关注 (Read Concern)
读取关注控制读取操作的一致性级别：

- **LOCAL**: 读取本地节点的数据，可能不是最新的
- **MAJORITY**: 读取已被大多数节点确认的数据
- **LINEARIZABLE**: 提供最强的一致性保证
- **AVAILABLE**: 读取任何可用数据，性能最优

### 写入关注 (Write Concern)
写入关注控制写入操作的持久性保证：

- **w=0**: 异步写入，不等待确认
- **w=1**: 等待主节点确认
- **w=majority**: 等待大多数节点确认
- **journal=true**: 等待 journal 写入
- **wtimeout**: 设置写入超时

## 设计理念

### 类型安全
- **指针参数**: 使用明确的指针类型避免类型错误
- **静态方法**: 提供无状态的配置接口
- **返回值验证**: 返回设置的对象指针便于验证

### 配置管理
- **集中设置**: 在客户端级别统一管理各种关注点
- **灵活配置**: 支持运行时动态修改配置
- **继承行为**: 子操作继承客户端的默认配置

### 性能优化
- **连接复用**: 客户端连接可以被多个操作复用
- **配置缓存**: 避免重复设置相同的配置
- **延迟加载**: 配置在需要时才应用

## 依赖项

- `yggr/base/yggrdef.h` - 基础定义
- `yggr/base/static_constant.hpp` - 静态常量
- `yggr/type_traits/number_t.hpp` - 数字类型特征
- `yggr/nsql_database_system/mongodb_config.hpp` - MongoDB 配置

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_client_native_ex.hpp>

// 创建 MongoDB 客户端
mongoc_client_t* client = mongoc_client_new("mongodb://localhost:27017");

// 创建并配置读取偏好
mongoc_read_prefs_t* read_prefs = mongoc_read_prefs_new(MONGOC_READ_SECONDARY_PREFERRED);
mongoc_read_prefs_add_tag(read_prefs, tmp_bson_new_from_json("{\"region\": \"us-east\"}"));
mongoc_client_native_ex::s_set_read_prefs_pointer(client, read_prefs);

// 创建并配置读取关注
mongoc_read_concern_t* read_concern = mongoc_read_concern_new();
mongoc_read_concern_set_level(read_concern, MONGOC_READ_CONCERN_LEVEL_MAJORITY);
mongoc_client_native_ex::s_set_read_concern_pointer(client, read_concern);

// 创建并配置写入关注
mongoc_write_concern_t* write_concern = mongoc_write_concern_new();
mongoc_write_concern_set_w(write_concern, MONGOC_WRITE_CONCERN_W_MAJORITY);
mongoc_write_concern_set_wtimeout(write_concern, 1000); // 1秒超时
mongoc_write_concern_set_journal(write_concern, true);
mongoc_client_native_ex::s_set_write_concern_pointer(client, write_concern);

// 使用配置的客户端执行操作
mongoc_collection_t* collection = mongoc_client_get_collection(client, "testdb", "testcoll");

// 执行查询（将使用配置的读取偏好和关注）
// mongoc_cursor_t* cursor = mongoc_collection_find_with_opts(collection, query, opts, read_prefs);

// 执行插入（将使用配置的写入关注）
// bool success = mongoc_collection_insert_one(collection, document, NULL, NULL, &error);

// 清理资源
mongoc_collection_destroy(collection);
mongoc_write_concern_destroy(write_concern);
mongoc_read_concern_destroy(read_concern);
mongoc_read_prefs_destroy(read_prefs);
mongoc_client_destroy(client);
```

## 注意事项

1. **配置顺序**: 建议在创建集合或执行操作前设置客户端配置
2. **资源管理**: 正确管理读取偏好、读取关注和写入关注对象的生命周期
3. **性能影响**: 不同的配置对性能和一致性有显著影响
4. **集群兼容性**: 某些配置选项在单节点部署中可能无效
5. **默认行为**: 未设置时使用 MongoDB 的默认配置

## 许可证

该文件遵循项目的许可证协议，基于 MIT 许可证。