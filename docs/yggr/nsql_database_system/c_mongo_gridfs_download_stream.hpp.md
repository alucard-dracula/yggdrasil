**c_mongo_gridfs_download_stream.hpp — 文档说明**

概述
- **名称**: `c_mongo_gridfs_download_stream`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_gridfs_download_stream.hpp](yggr/nsql_database_system/c_mongo_gridfs_download_stream.hpp)
- **用途**: 封装 libmongoc 的 GridFS 下载流（`mongoc_stream_t`），继承自 `c_mongo_basic_stream`，用于从 GridFS bucket 打开并读取文件流。

类型别名
- `base_type`: `c_mongo_basic_stream`
- `org_type` / `base_org_type`: 底层原生流类型（`mongoc_stream_t` 派生）
- `stream_ptr_type`: 智能指针类型（继承自基类）

构造与生命周期
- `c_mongo_gridfs_download_stream()` — 默认构造。
- `c_mongo_gridfs_download_stream(base_org_type* pstream)` — 使用已创建的原生流指针进行包装。
- `c_mongo_gridfs_download_stream(base_org_type* pstream, bool need_del)` — 指定是否在析构时销毁原生流。
- 支持移动构造/移动赋值，拷贝被禁用。

主要方法
- `operator=(BOOST_RV_REF(this_type))` — 移动赋值（通过基类 `pro_move` 实现）。
- `swap(...)` — 交换实现并已导出到 `std` / `boost` 名称空间。

语义与说明
- 该类不在头文件内实现具体的读取逻辑；它复用 `c_mongo_basic_stream` 中定义的读写接口与 iovec 支持。
- 常见用法：先通过 `mongoc_gridfs_bucket_open_download_stream` 或通过项目的 mongo access helper 获得 `mongoc_stream_t*`，然后用本类包装以便在 C++ 代码中以 RAII 方式管理流的生命周期与读写操作。

示例
```cpp
// 假设已有 bucket 和 file_id
mongoc_gridfs_bucket_t* bucket = /* ... */;
mongoc_stream_t* native_stream = mongoc_gridfs_bucket_open_download_stream(bucket, file_id, nullptr);

yggr::nsql_database_system::c_mongo_gridfs_download_stream s(native_stream, true);
char buf[4096];
ssize_t n = s.read(buf, sizeof(buf));
// 处理读取的数据
```

依赖
- 头文件：`c_mongo_basic_stream.hpp`
- 运行时：libmongoc（GridFS 流创建函数）

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_gridfs_download_stream.md](yggr/nsql_database_system/c_mongo_gridfs_download_stream.md)

扩展建议
- 如需我可以：
  - 展开 `c_mongo_basic_stream` 的读写方法签名并在本文件中补充示例；
  - 添加错误处理与平台差异说明（例如 I/O 行为与阻塞/非阻塞语义）。
