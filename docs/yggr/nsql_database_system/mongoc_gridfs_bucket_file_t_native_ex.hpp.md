# mongoc_gridfs_bucket_file_t_native_ex.hpp

## 概述

此头文件定义了 `mongoc_gridfs_bucket_file_t_native_ex` 类，该类提供静态方法来访问 MongoDB GridFS 存储桶文件对象的成员变量。这些方法是对 MongoDB C 驱动程序 `mongoc_gridfs_bucket_file_t` 结构体的安全访问接口。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

### mongoc_gridfs_bucket_file_t_native_ex

一个工具类，提供对 GridFS 存储桶文件结构体成员的静态访问方法。

#### 静态方法

##### 存储桶相关

###### `var_bucket`

```cpp
static mongoc_gridfs_bucket_t* var_bucket(void* pbucket_file);
```

获取对应的 GridFS 存储桶对象。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- 指向存储桶对象的指针

##### 文件数据相关

###### `var_filename`

```cpp
static char* var_filename(void* pbucket_file);
```

获取文件名。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- 文件名字符串指针

###### `var_file_id`

```cpp
static bson_value_t* var_file_id(void* pbucket_file);
```

获取文件 ID。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- 指向文件 ID 的 BSON 值指针

###### `var_metadata`

```cpp
static bson_t* var_metadata(void* pbucket_file);
```

获取文件元数据。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- 指向元数据 BSON 文档的指针

###### `var_chunk_size`

```cpp
static int32_t var_chunk_size(void* pbucket_file);
```

获取块大小。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- 块大小（字节）

###### `var_length`

```cpp
static int64_t var_length(void* pbucket_file);
```

获取文件总长度。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- 文件长度（字节）

##### 读写缓冲区相关

###### `var_buffer`

```cpp
static uint8_t* var_buffer(void* pbucket_file);
```

获取缓冲区指针。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- 缓冲区指针

###### `var_in_buffer`

```cpp
static size_t var_in_buffer(void* pbucket_file);
```

获取缓冲区中的数据大小。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- 缓冲区中数据的字节数

###### `var_curr_chunk`

```cpp
static int32_t var_curr_chunk(void* pbucket_file);
```

获取当前块索引。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- 当前块的索引号

##### 写入相关

###### `var_saved`

```cpp
static bool var_saved(void* pbucket_file);
```

检查文件是否已保存。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- `true` 如果文件已保存，`false` 否则

##### 读取相关

###### `var_cursor`

```cpp
static mongoc_cursor_t* var_cursor(void* pbucket_file);
```

获取读取游标。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- 指向读取游标的指针

###### `var_bytes_read`

```cpp
static size_t var_bytes_read(void* pbucket_file);
```

获取已读取的字节数。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- 已读取的总字节数

###### `var_finished`

```cpp
static bool var_finished(void* pbucket_file);
```

检查读取是否完成。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- `true` 如果读取完成，`false` 否则

##### 错误处理

###### `var_err`

```cpp
static bson_error_t* var_err(void* pbucket_file);
```

获取错误信息。

**参数：**
- `pbucket_file`：指向 `mongoc_gridfs_bucket_file_t` 的指针

**返回值：**
- 指向错误结构的指针

## 依赖项

- `<yggr/nsql_database_system/mongodb_config.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_gridfs_bucket_file_t_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 假设有一个有效的 mongoc_gridfs_bucket_file_t* file
char* filename = mongoc_gridfs_bucket_file_t_native_ex::var_filename(file);
int64_t file_size = mongoc_gridfs_bucket_file_t_native_ex::var_length(file);
bool is_finished = mongoc_gridfs_bucket_file_t_native_ex::var_finished(file);
```

## 注意事项

- 所有方法都是静态的，不需要创建类的实例
- 这些方法提供对 `mongoc_gridfs_bucket_file_t` 结构体成员的直接访问
- 使用这些方法时需要确保传入的指针有效
- 该类是对 MongoDB C 驱动程序 GridFS 文件结构的包装，提供类型安全的访问接口
- 文件中注释掉了原始的结构体定义，现在通过这些访问器方法进行操作</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_gridfs_bucket_file_t_native_ex_cn.md