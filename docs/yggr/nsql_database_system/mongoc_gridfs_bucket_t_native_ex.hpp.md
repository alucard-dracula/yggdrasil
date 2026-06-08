# mongoc_gridfs_bucket_t_native_ex.hpp

## 概述

此头文件定义了 `mongoc_gridfs_bucket_t_native_ex` 类，该类提供静态方法来访问 MongoDB GridFS 存储桶对象的成员变量。这些方法是对 MongoDB C 驱动程序 `mongoc_gridfs_bucket_t` 结构体的安全访问接口。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

### mongoc_gridfs_bucket_t_native_ex

一个工具类，提供对 GridFS 存储桶结构体成员的静态访问方法。

#### 静态方法

##### `var_chunks`

```cpp
static mongoc_collection_t* var_chunks(mongoc_gridfs_bucket_t* pbucket);
```

获取存储块的集合对象。

**参数：**
- `pbucket`：指向 `mongoc_gridfs_bucket_t` 的指针

**返回值：**
- 指向块集合的指针

##### `var_files`

```cpp
static mongoc_collection_t* var_files(mongoc_gridfs_bucket_t* pbucket);
```

获取文件信息的集合对象。

**参数：**
- `pbucket`：指向 `mongoc_gridfs_bucket_t` 的指针

**返回值：**
- 指向文件集合的指针

##### `var_chunk_size`

```cpp
static int32_t var_chunk_size(mongoc_gridfs_bucket_t* pbucket);
```

获取块大小。

**参数：**
- `pbucket`：指向 `mongoc_gridfs_bucket_t` 的指针

**返回值：**
- 块大小（字节）

##### `var_bucket_name`

```cpp
static char* var_bucket_name(mongoc_gridfs_bucket_t* pbucket);
```

获取存储桶名称。

**参数：**
- `pbucket`：指向 `mongoc_gridfs_bucket_t` 的指针

**返回值：**
- 存储桶名称字符串指针

##### `var_indexed`

```cpp
static bool var_indexed(mongoc_gridfs_bucket_t* pbucket);
```

检查存储桶是否已建立索引。

**参数：**
- `pbucket`：指向 `mongoc_gridfs_bucket_t` 的指针

**返回值：**
- `true` 如果已建立索引，`false` 否则

## 依赖项

- `<yggr/nsql_database_system/mongodb_config.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_gridfs_bucket_t_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 假设有一个有效的 mongoc_gridfs_bucket_t* bucket
mongoc_collection_t* chunks_collection = mongoc_gridfs_bucket_t_native_ex::var_chunks(bucket);
mongoc_collection_t* files_collection = mongoc_gridfs_bucket_t_native_ex::var_files(bucket);
int32_t chunk_size = mongoc_gridfs_bucket_t_native_ex::var_chunk_size(bucket);
char* bucket_name = mongoc_gridfs_bucket_t_native_ex::var_bucket_name(bucket);
bool is_indexed = mongoc_gridfs_bucket_t_native_ex::var_indexed(bucket);
```

## 注意事项

- 所有方法都是静态的，不需要创建类的实例
- 这些方法提供对 `mongoc_gridfs_bucket_t` 结构体成员的直接访问
- 使用这些方法时需要确保传入的指针有效
- 该类是对 MongoDB C 驱动程序 GridFS 存储桶结构的包装，提供类型安全的访问接口
- 文件中注释掉了原始的结构体定义，现在通过这些访问器方法进行操作
- GridFS 使用两个集合：一个用于存储文件块（chunks），另一个用于存储文件元数据（files）</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_gridfs_bucket_t_native_ex_cn.md