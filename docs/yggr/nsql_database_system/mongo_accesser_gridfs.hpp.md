# mongo_accesser_gridfs.hpp

## 概述

`mongo_accesser_gridfs.hpp` 是 Yggdrasil 数据库系统中的 MongoDB GridFS 访问器类，提供对 MongoDB GridFS（Grid File System）功能的类型安全封装。该文件定义了 `mongo_accesser_gridfs` 类，继承自 `basic_mongo_accesser_gridfs`，提供了丰富的静态方法用于文件存储、检索和管理操作。

GridFS 是 MongoDB 用于存储大文件的规范，将大文件分割成多个小块存储在 MongoDB 集合中。该访问器提供了两种主要的 GridFS 操作接口：
- 传统的 GridFS API（gridfs_* 方法）
- 新的 GridFS Bucket API（gridfs_bucket_* 方法）

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    class mongo_accesser_gridfs;
}
}
```

## 类定义

### mongo_accesser_gridfs

```cpp
class mongo_accesser_gridfs
    : public basic_mongo_accesser_gridfs
{
public:
    // 类型定义
    typedef basic_mongo_accesser_gridfs base_type;
    typedef base_type::org_gridfs_type org_gridfs_type;
    typedef base_type::org_gridfs_file_type org_gridfs_file_type;
    typedef base_type::org_gridfs_file_opt_type org_gridfs_file_opt_type;
    typedef base_type::org_gridfs_bucket_type org_gridfs_bucket_type;
    typedef base_type::org_bson_type org_bson_type;
    typedef base_type::org_bson_value_type org_bson_value_type;
    typedef base_type::org_bson_error_type org_bson_error_type;
    typedef base_type::org_read_prefs_type org_read_prefs_type;
    typedef base_type::org_cursor_type org_cursor_type;
    typedef base_type::org_gridfs_file_list_type org_gridfs_file_list_type;
    typedef base_type::org_gridfs_bucket_file_type org_gridfs_bucket_file_type;
    typedef base_type::org_gridfs_bucket_file_opt_type org_gridfs_bucket_file_opt_type;
    typedef base_type::org_stream_type org_stream_type;
    typedef base_type::org_iovec_type org_iovec_type;
    typedef base_type::org_gridfs_bucket_stream_type org_gridfs_bucket_stream_type;
    typedef base_type::org_gridfs_bucket_upload_stream_type org_gridfs_bucket_upload_stream_type;
    typedef base_type::org_gridfs_bucket_download_stream_type org_gridfs_bucket_download_stream_type;
};
```

## 主要功能

### 1. GridFS 文件保存

#### s_gridfs_save_file
保存文件到 GridFS。

```cpp
template<typename Connection_T, typename String1, typename String2, typename String3>
static typename boost::enable_if<...>::type
s_gridfs_save_file(Connection_T& conn,
                   const String1& str_db,
                   const String2& str_gridfs,
                   const String3& str_fpath,
                   org_bson_error_type* err = 0);
```

**参数：**
- `conn`: MongoDB 连接对象
- `str_db`: 数据库名称
- `str_gridfs`: GridFS 名称
- `str_fpath`: 文件路径
- `err`: 错误对象指针

**返回值：** `bool` - 操作是否成功

#### s_gridfs_save_file_of_buffer
保存缓冲区数据到 GridFS。

```cpp
template<typename Connection_T, typename String1, typename String2, typename Buffer>
static typename boost::enable_if<...>::type
s_gridfs_save_file_of_buffer(Connection_T& conn,
                            const String1& str_db,
                            const String2& str_gridfs,
                            const String2& str_fname,
                            const Buffer& buffer,
                            org_bson_error_type* err = 0);
```

### 2. GridFS 文件加载

#### s_gridfs_load_file
从 GridFS 加载文件。

```cpp
template<typename Connection_T, typename String1, typename String2, typename Buffer>
static typename boost::enable_if<...>::type
s_gridfs_load_file(Connection_T& conn,
                   const String1& str_db,
                   const String2& str_gridfs,
                   const String2& str_fname,
                   Buffer& out_buf,
                   org_bson_error_type* err = 0);
```

**参数：**
- `conn`: MongoDB 连接对象
- `str_db`: 数据库名称
- `str_gridfs`: GridFS 名称
- `str_fname`: 文件名
- `out_buf`: 输出缓冲区
- `err`: 错误对象指针

**返回值：** `u64` - 读取的字节数

### 3. GridFS 文件查找

#### s_gridfs_find
查找 GridFS 中的文件。

```cpp
template<typename Connection_T, typename String1, typename String2, typename HandlerCollector>
static typename boost::enable_if<...>::type
s_gridfs_find(Connection_T& conn,
              const String1& str_db,
              const String2& str_gridfs,
              const String2& str_fname,
              org_bson_error_type* err,
              const HandlerCollector& handler_collector);
```

### 4. GridFS 文件删除

#### s_gridfs_remove_file
删除 GridFS 中的文件。

```cpp
template<typename Connection_T, typename String1, typename String2>
static typename boost::enable_if<...>::type
s_gridfs_remove_file(Connection_T& conn,
                     const String1& str_db,
                     const String2& str_gridfs,
                     const String2& str_fname,
                     org_bson_error_type* err = 0);
```

### 5. GridFS 文件列表操作

#### s_gridfs_files_begin
开始遍历 GridFS 文件列表。

```cpp
template<typename Connection_T, typename String1, typename String2, typename HandlerCollector>
static typename boost::enable_if<...>::type
s_gridfs_files_begin(Connection_T& conn,
                     const String1& str_db,
                     const String2& str_gridfs,
                     const Bson& query,
                     org_bson_error_type* err,
                     const HandlerCollector& handler_collector);
```

#### s_gridfs_files_end
结束遍历 GridFS 文件列表。

```cpp
template<typename Connection_T, typename Files_Container>
static bool s_gridfs_files_end(Connection_T& conn,
                               Files_Container& files_cont,
                               org_bson_error_type& err);
```

### 6. GridFS Bucket 操作

GridFS Bucket API 提供了更现代的文件操作接口：

#### s_gridfs_bucket_save_file
使用 Bucket API 保存文件。

```cpp
template<typename Connection_T, typename String1, typename String2, typename String3,
         typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts>
static typename boost::enable_if<...>::type
s_gridfs_bucket_save_file(Connection_T& conn,
                         const String1& str_db,
                         const String2& str_fname,
                         const String3& str_fpath,
                         s32 flag, s32 mode,
                         const BsonBucketOpts& bucket_opts,
                         const ReadPrefs& bucket_read_prefs,
                         const BsonStreamOpts& stream_opts,
                         org_bson_value_type* pfile_id,
                         org_bson_error_type* perr = 0);
```

#### s_gridfs_bucket_load_file
使用 Bucket API 加载文件。

```cpp
template<typename Connection_T, typename String1,
         typename BsonBucketOpts, typename ReadPrefs,
         typename BsonValueFileID, typename Buffer>
static typename boost::enable_if<...>::type
s_gridfs_bucket_load_file(Connection_T& conn,
                         const String1& str_db,
                         const BsonBucketOpts& bucket_opts,
                         const ReadPrefs& bucket_read_prefs,
                         const BsonValueFileID& file_id,
                         s32 tmout_msec,
                         Buffer& out_buf,
                         org_bson_error_type* perr = 0);
```

#### s_gridfs_bucket_find
使用 Bucket API 查找文件。

```cpp
template<typename Connection_T, typename String1,
         typename BsonBucketOpts, typename ReadPrefs,
         typename BsonFilter, typename BsonOpts,
         typename HandlerCollector>
static typename boost::enable_if<...>::type
s_gridfs_bucket_find(Connection_T& conn,
                     const String1& str_db,
                     const BsonBucketOpts& bucket_opts,
                     const ReadPrefs& bucket_read_prefs,
                     const BsonFilter& filter,
                     const BsonOpts& opts,
                     org_bson_error_type* perr,
                     const HandlerCollector& handler_collector);
```

#### s_gridfs_bucket_remove_file_by_id
通过文件ID删除文件。

```cpp
template<typename Connection_T, typename String1,
         typename BsonBucketOpts, typename ReadPrefs,
         typename BsonValueFileID>
static typename boost::enable_if<...>::type
s_gridfs_bucket_remove_file_by_id(Connection_T& conn,
                                 const String1& str_db,
                                 const BsonBucketOpts& bucket_opts,
                                 const ReadPrefs& bucket_read_prefs,
                                 const BsonValueFileID& file_id,
                                 org_bson_error_type* perr = 0);
```

## 模板参数要求

所有字符串参数必须满足 `charset::utf8_string_constructable` 概念，确保可以转换为 UTF-8 字符串。

## 错误处理

所有方法都接受 `org_bson_error_type*` 或 `org_bson_error_type&` 参数用于错误报告。错误信息通过 MongoDB C 驱动的错误结构返回。

## 依赖项

- `basic_mongo_accesser_gridfs` - 基础 GridFS 访问器类
- `boost::mpl` - 用于模板元编程
- `boost::enable_if` - 用于 SFINAE 条件编译
- Yggdrasil charset 库 - 用于字符串编码转换
- Yggdrasil container_ex 库 - 用于容器扩展
- MongoDB C 驱动 GridFS API

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongo_accesser_gridfs.hpp>

// 保存文件到 GridFS
bool result = mongo_accesser_gridfs::s_gridfs_save_file(
    connection, "mydb", "mygridfs", "/path/to/file.txt");

// 从缓冲区保存文件
std::vector<char> buffer = ...;
bool result = mongo_accesser_gridfs::s_gridfs_save_file_of_buffer(
    connection, "mydb", "mygridfs", "filename.txt", buffer);

// 加载文件
std::vector<char> out_buffer;
u64 bytes_read = mongo_accesser_gridfs::s_gridfs_load_file(
    connection, "mydb", "mygridfs", "filename.txt", out_buffer);

// 使用 Bucket API 保存文件
bson_t bucket_opts = BSON_INITIALIZER;
bson_t read_prefs = BSON_INITIALIZER;
bson_t stream_opts = BSON_INITIALIZER;
bson_value_t file_id;

bool result = mongo_accesser_gridfs::s_gridfs_bucket_save_file(
    connection, "mydb", "filename.txt", "/path/to/file.txt",
    O_RDONLY, 0644, bucket_opts, read_prefs, stream_opts, &file_id);
```

## 注意事项

1. 该类只提供静态方法，不需要实例化
2. 所有操作都是线程安全的，由底层 MongoDB 驱动保证
3. 文件路径在 Windows 和非 Windows 系统上有不同的处理方式
4. GridFS Bucket API 提供了更丰富的功能和更好的性能
5. 缓冲区操作支持各种 STL 容器和自定义缓冲区类型
6. 所有字符串参数都会被转换为 UTF-8 编码

## 许可证

该文件遵循项目的许可证协议。