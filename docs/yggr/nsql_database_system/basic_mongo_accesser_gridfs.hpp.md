# basic_mongo_accesser_gridfs.hpp 文档

## 概述

`basic_mongo_accesser_gridfs.hpp` 是 Yggdrasil 项目中的一个 C++ 头文件，提供对 MongoDB GridFS（Grid File System）操作的全面封装。GridFS 是 MongoDB 中用于存储和检索大文件的规范，该文件定义了 `basic_mongo_accesser_gridfs` 类，该类包含一系列静态方法，用于处理 GridFS 文件的存储、检索和管理操作。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

`basic_mongo_accesser_gridfs` 是一个静态方法集合类，用于封装 MongoDB C 驱动程序的 GridFS 功能。它不包含实例变量，所有操作都是通过静态方法进行的。该类提供了对 MongoDB GridFS 的完整操作接口，包括文件的上传、下载、查找、删除等操作，同时支持传统的 GridFS API 和现代的 GridFS Bucket API。

## 类型定义

### 基本类型
- `types_decl_type`: MongoDB 访问器类型声明
- `inner_string_type`: 内部字符串类型
- `inner_string_view_type`: 内部字符串视图类型
- `bson_type`: BSON 类型
- `org_bson_type`: 原始 BSON 类型
- `bson_value_type`: BSON 值类型
- `org_bson_value_type`: 原始 BSON 值类型
- `bson_error_type`: BSON 错误类型
- `org_bson_error_type`: 原始 BSON 错误类型
- `write_concern_type`: 写关注类型
- `org_write_concern_type`: 原始写关注类型
- `read_prefs_type`: 读偏好类型
- `org_read_prefs_type`: 原始读偏好类型
- `index_model_type`: 索引模型类型
- `org_index_model_type`: 原始索引模型类型
- `find_and_modify_opts_type`: 查找并修改选项类型
- `org_find_and_modify_opts_type`: 原始查找并修改选项类型
- `read_concern_type`: 读关注类型
- `org_read_concern_type`: 原始读关注类型
- `mongoc_error_domain_type`: MongoC 错误域类型
- `mongoc_error_code_type`: MongoC 错误代码类型

### GridFS 相关类型
- `gridfs_file_opt_type`: GridFS 文件选项类型
- `org_gridfs_file_opt_type`: 原始 GridFS 文件选项类型
- `gridfs_file_type`: GridFS 文件类型
- `org_gridfs_file_type`: 原始 GridFS 文件类型

## 主要方法

### 传统 GridFS 操作

#### 文件保存
- `s_org_gridfs_save_file`: 从文件路径保存文件到 GridFS
- `s_org_gridfs_save_file_of_buffer`: 从缓冲区保存文件到 GridFS

#### 文件加载
- `s_org_gridfs_load_file`: 从 GridFS 加载文件到缓冲区

#### 文件查找
- `s_org_gridfs_find`: 查找 GridFS 文件
- `s_org_gridfs_find_one`: 查找单个 GridFS 文件
- `s_org_gridfs_find_one_by_filename`: 通过文件名查找单个 GridFS 文件

#### 文件删除
- `s_org_gridfs_remove_file`: 删除 GridFS 文件

#### 分块和文件查询
- `s_org_gridfs_chunks_find`: 在 chunks 集合中查找
- `s_org_gridfs_files_find`: 在 files 集合中查找

#### 文件容器管理
- `s_org_gridfs_files_begin`: 开始文件容器操作
- `s_org_gridfs_files_end`: 结束文件容器操作

### GridFS Bucket 操作（现代 API）

#### 文件保存
- `s_org_gridfs_bucket_save_file`: 从文件路径保存文件到 GridFS Bucket
- `s_org_gridfs_bucket_save_file_with_id`: 从文件路径保存文件到 GridFS Bucket（指定 ID）
- `s_org_gridfs_bucket_save_file_of_buffer`: 从缓冲区保存文件到 GridFS Bucket
- `s_org_gridfs_bucket_save_file_of_buffer_with_id`: 从缓冲区保存文件到 GridFS Bucket（指定 ID）

#### 文件加载
- `s_org_gridfs_bucket_load_file`: 从 GridFS Bucket 加载文件到缓冲区

#### 文件查找
- `s_org_gridfs_bucket_find`: 在 GridFS Bucket 中查找文件

#### 文件删除
- `s_org_gridfs_bucket_remove_file_by_id`: 通过 ID 删除 GridFS Bucket 文件
- `s_org_gridfs_bucket_remove_files`: 批量删除 GridFS Bucket 文件

## 依赖项

该文件依赖于以下头文件：
- `<yggr/nsql_database_system/mongo_accesser_types_decl.hpp>`
- `<yggr/nsql_database_system/c_mongo_gridfs_file_opt.hpp>`
- `<yggr/nsql_database_system/c_mongo_gridfs_file.hpp>`
- `<yggr/nsql_database_system/mongoc_gridfs_file_native_ex.hpp>`
- `<yggr/nsql_database_system/mongoc_gridfs_bucket_native_ex.hpp>`
- `<yggr/nsql_database_system/mongoc_gridfs_bucket_t_native_ex.hpp>`
- `<yggr/nsql_database_system/mongoc_gridfs_bucket_file_t_native_ex.hpp>`

## 使用注意事项

1. 所有方法都是静态的，不需要创建类实例
2. 方法使用模板参数来支持不同的连接类型
3. 错误处理通过 `org_bson_error_type*` 参数进行
4. 许多方法需要有效的 MongoDB 客户端连接、数据库名称和 GridFS 前缀
5. 该类主要作为 MongoDB C 驱动程序 GridFS 功能的 C++ 包装器
6. 支持两种 GridFS API：传统 API 和现代 Bucket API
7. 文件容器操作需要小心管理连接的生命周期

## 错误处理

所有方法都接受一个 `org_bson_error_type*` 参数用于错误报告。错误信息包括错误域、错误代码和描述性消息。

## 线程安全

该类的静态方法设计为可以安全地从多个线程调用，但具体的安全性取决于底层 MongoDB C 驱动程序的行为和使用的连接对象。

## GridFS 说明

GridFS 是 MongoDB 中用于存储超过 16MB 文件的规范。它将大文件分割成小块存储在两个集合中：
- `files` 集合：存储文件元数据
- `chunks` 集合：存储文件数据块

该类提供了对 GridFS 的完整封装，支持文件的上传、下载、查找和删除操作。

## API 版本差异

- 传统 GridFS API：使用 `mongoc_gridfs_*` 函数
- GridFS Bucket API：使用 `mongoc_gridfs_bucket_*` 函数，更现代和推荐使用

## 特殊说明

- 文件保存操作支持从文件路径或内存缓冲区上传
- 文件加载操作将文件内容读取到提供的缓冲区中
- 查找操作使用处理器收集器模式来处理结果集
- 容器操作用于管理多个文件的生命周期
- 删除操作支持单个文件和批量删除