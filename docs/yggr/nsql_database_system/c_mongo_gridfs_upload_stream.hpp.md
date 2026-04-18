# c_mongo_gridfs_upload_stream.hpp 文档

## 文件概述

`c_mongo_gridfs_upload_stream.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于 MongoDB GridFS 上传流的 C++ 类。该类继承自 `c_mongo_basic_stream`，提供了对 MongoDB GridFS 上传操作的封装。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 类定义

### c_mongo_gridfs_upload_stream

该类用于管理 MongoDB GridFS 的上传流操作。它不支持复制，只能移动。

#### 类型定义

- `base_type`：基类 `c_mongo_basic_stream`。
- `org_type`：原始类型，从基类继承。
- `base_org_type`：基础原始类型，从基类继承。
- `stream_ptr_type`：流指针类型，从基类继承。

#### 构造函数

- `c_mongo_gridfs_upload_stream(void)`：默认构造函数。
- `c_mongo_gridfs_upload_stream(base_org_type* pstream)`：使用现有的 MongoDB 流指针构造对象。
- `c_mongo_gridfs_upload_stream(base_org_type* pstream, bool need_del)`：使用现有的 MongoDB 流指针构造对象，并指定是否需要删除。
- `c_mongo_gridfs_upload_stream(BOOST_RV_REF(this_type) right)`：移动构造函数。

#### 析构函数

- `~c_mongo_gridfs_upload_stream(void)`：析构函数。

#### 成员函数

- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值运算符。
- `swap(BOOST_RV_REF(this_type) right)`：交换函数的重载版本。
- `swap(this_type& right)`：交换函数。

#### 使用说明

该类主要用于创建和管理系统 MongoDB GridFS 的上传流。通过继承自 `c_mongo_basic_stream`，它提供了基本的流操作功能。

#### 注意事项

- 该类使用 Boost.Move 库，不支持复制操作。
- 在使用时，需要确保 MongoDB 连接和 GridFS bucket 已正确初始化。

## 依赖

- `c_mongo_basic_stream.hpp`：基类定义。
- `mplex/tag_sfinae.hpp`：SFINAE 标签定义。

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。