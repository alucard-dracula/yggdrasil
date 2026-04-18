# c_mongo_gridfs_stream 文档

## 概述

`c_mongo_gridfs_stream` 是一个 C++ 类，位于 `yggr::nsql_database_system` 命名空间中。它继承自 `c_mongo_basic_stream`，提供对 MongoDB GridFS 文件的流式访问接口。该类不支持复制，仅支持移动语义。

## 成员类型

- `base_type`: 基类 `c_mongo_basic_stream` 的别名。
- `org_type`: 从基类继承的原始类型。
- `base_org_type`: 基类的原始类型。
- `stream_ptr_type`: 流指针类型。

## 构造函数

- `c_mongo_gridfs_stream(void)`: 默认构造函数。
- `c_mongo_gridfs_stream(base_org_type* pstream)`: 使用流指针初始化。
- `c_mongo_gridfs_stream(base_org_type* pstream, bool need_del)`: 使用流指针和删除标志初始化。
- `c_mongo_gridfs_stream(File& file)`: 从文件对象初始化，使用模板参数 `File`。
- `c_mongo_gridfs_stream(BOOST_RV_REF(this_type) right)`: 移动构造函数。

## 运算符

- `operator=(BOOST_RV_REF(this_type) right)`: 移动赋值运算符。

## 公共方法

- `swap(BOOST_RV_REF(this_type) right)`: 与右值引用交换。
- `swap(this_type& right)`: 与左值引用交换。

## 保护方法

- `pro_init(mongoc_gridfs_file_t* pfile)`: 使用 GridFS 文件指针初始化。

## 使用示例

```cpp
#include <yggr/nsql_database_system/c_mongo_gridfs_stream.hpp>

// 示例：从文件创建流
mongoc_gridfs_file_t* gridfs_file = /* 获取 GridFS 文件 */;
yggr::nsql_database_system::c_mongo_gridfs_stream stream(gridfs_file);

// 移动赋值
yggr::nsql_database_system::c_mongo_gridfs_stream another_stream = std::move(stream);