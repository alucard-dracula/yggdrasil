# mongoc_gridfs_bucket_native_ex.hpp

## 概述

此头文件定义了 `mongoc_gridfs_bucket_native_ex` 类，该类提供静态方法来处理 MongoDB GridFS 存储桶的上传和下载操作。这些方法是对 MongoDB C 驱动程序 GridFS 功能的扩展包装。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

### mongoc_gridfs_bucket_native_ex

一个工具类，提供 GridFS 存储桶上传和下载操作的静态方法。

#### 静态方法

##### 上传相关方法

###### `s_mongoc_gridfs_bucket_upload_get_error`

```cpp
static bool s_mongoc_gridfs_bucket_upload_get_error(mongoc_stream_t* chk_stream, bson_error_t* error);
```

从上传流中获取错误信息。

**参数：**
- `chk_stream`：指向上传流的指针
- `error`：指向错误结构的指针，用于存储错误信息

**返回值：**
- `true` 如果获取到错误，`false` 否则

###### `s_mongoc_gridfs_bucket_upload_get_file`

```cpp
static void* s_mongoc_gridfs_bucket_upload_get_file(mongoc_stream_t* chk_stream);
```

从上传流中获取文件对象。

**参数：**
- `chk_stream`：指向上传流的指针

**返回值：**
- 指向文件对象的指针

###### `s_mongoc_gridfs_bucket_upload_from_stream_with_id`

```cpp
static bool s_mongoc_gridfs_bucket_upload_from_stream_with_id(mongoc_gridfs_bucket_t* bucket,
                                                               const bson_value_t* file_id,
                                                               const char* filename,
                                                               mongoc_stream_t* source,
                                                               const bson_t* opts,
                                                               bson_error_t* error);
```

使用指定的文件 ID 从流上传文件到 GridFS 存储桶。

**参数：**
- `bucket`：指向 GridFS 存储桶对象的指针
- `file_id`：文件 ID 的 BSON 值
- `filename`：文件名
- `source`：指向源流的指针
- `opts`：可选的上传选项 BSON 文档
- `error`：指向错误结构的指针，用于报告失败

**返回值：**
- `true` 如果上传成功，`false` 否则

##### 下载相关方法

###### `s_mongoc_gridfs_bucket_download_get_error`

```cpp
static bool s_mongoc_gridfs_bucket_download_get_error(mongoc_stream_t* chk_stream, bson_error_t* error);
```

从下载流中获取错误信息。

**参数：**
- `chk_stream`：指向下载流的指针
- `error`：指向错误结构的指针，用于存储错误信息

**返回值：**
- `true` 如果获取到错误，`false` 否则

###### `s_mongoc_gridfs_bucket_download_get_file`

```cpp
static void* s_mongoc_gridfs_bucket_download_get_file(mongoc_stream_t* chk_stream);
```

从下载流中获取文件对象。

**参数：**
- `chk_stream`：指向下载流的指针

**返回值：**
- 指向文件对象的指针

## 依赖项

- `<yggr/nsql_database_system/mongodb_config.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_gridfs_bucket_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 上传文件示例
bson_value_t file_id = {/* 初始化文件 ID */};
bson_error_t error;

bool success = mongoc_gridfs_bucket_native_ex::s_mongoc_gridfs_bucket_upload_from_stream_with_id(
    bucket, &file_id, "example.txt", source_stream, nullptr, &error);

if (!success) {
    // 处理错误
}

// 从流获取错误示例
bson_error_t stream_error;
if (mongoc_gridfs_bucket_native_ex::s_mongoc_gridfs_bucket_upload_get_error(upload_stream, &stream_error)) {
    // 处理流错误
}
```

## 注意事项

- 所有方法都是静态的，不需要创建类的实例
- 这些方法提供对 MongoDB C 驱动程序 GridFS 上传/下载功能的直接访问
- 使用这些方法时需要确保传入的指针有效
- 错误处理通过 `bson_error_t` 结构体进行
- `s_mongoc_gridfs_bucket_upload_from_stream_with_id` 方法允许指定自定义文件 ID，这在某些场景下很有用
- 流操作需要正确管理流的生命周期</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_gridfs_bucket_native_ex_cn.md