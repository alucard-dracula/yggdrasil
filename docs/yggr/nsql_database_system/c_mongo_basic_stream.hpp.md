# c_mongo_basic_stream.hpp 文档

**概述**:
- **说明**: `c_mongo_basic_stream` 封装了 libmongoc 的 `mongoc_stream_t`，提供安全的 shared_ptr 管理、读写（支持常见容器/IO向量）、关闭/刷新、释放原生指针和类型信息查询，方便在 C++ 代码中操作 MongoDB 底层流。
- **所在文件**: [yggr/nsql_database_system/c_mongo_basic_stream.hpp](yggr/nsql_database_system/c_mongo_basic_stream.hpp)

**命名空间**:
- `yggr::nsql_database_system`

**主要类型**:
- `c_mongo_basic_stream`:
  - 原生类型别名: `org_type = mongoc_stream_t`。
  - 智能指针: `stream_ptr_type = yggr::shared_ptr<mongoc_stream_t>`，带自定义删除器 `detail::mongoc_stream_t_shared_ptr_deleter`（根据 `need_delete` 控制是否销毁原生流）。
  - 流类型常量：`E_STREAM_T_SOCKET`, `E_STREAM_T_FILE`, `E_STREAM_T_BUFFERED`, `E_STREAM_T_GRIDFS`, `E_STREAM_T_TLS`, `E_STREAM_T_GRIDFS_UPLOAD`, `E_STREAM_T_GRIDFS_DOWNLOAD`。

- 辅助模板:
  - `detail::c_mongo_basic_stream_read_helper<Container, is_iovec>`: 针对 `Container` 是否由 `mongoc_iovec_t` 元素组成提供 `read` 的实现（`mongoc_stream_readv` 或 `mongoc_stream_read`）。
  - `detail::c_mongo_basic_stream_write_helper<Container, is_iovec>`: 针对 `Container` 是否为 iovec 提供 `write` 的实现（`mongoc_stream_writev` 或 `mongoc_stream_write`）。

**关键方法**:
- 构造/析构:
  - `c_mongo_basic_stream()` 默认构造。
  - `c_mongo_basic_stream(mongoc_stream_t* pstream, bool need_del = true)` 从原生指针构造并可指定是否由封装负责销毁。
  - 移动构造/赋值与 `swap` 支持（非拷贝类型）。

- 状态与原生访问:
  - `org_pointer()` / `operator mongoc_stream_t*()` 获取原生指针。
  - `empty()` / `operator bool()` 检查是否有有效流。
  - `need_delete()` 读写是否在析构时销毁原生流。
  - `typeinfo()` 返回流的类型（`mongoc_stream_t::type`）。
  - `release()` 释放智能指针并返回原生指针（临时将 `_need_del` 设置为 false，随后重置为 true）。
  - `clear()` 释放内部指针并重置删除标志。

- I/O 操作:
  - `close()` / `flush()`：调用 `mongoc_stream_close` / `mongoc_stream_flush`。
  - `read` 重载：
    - `template<typename Container> ssize_t read(Container& cont, size_t min_bytes = -1, int32_t timeout_msec = 0) const`：支持容器或 iovec。
    - `ssize_t read(void* buf, size_t count, size_t min_bytes = -1, int32_t timeout_msec = 0)`：读取到缓冲区。
    - `ssize_t read(mongoc_iovec_t* iov, size_t iovcnt, size_t min_bytes = -1, int32_t timeout_msec = 0)`：iovec 形式。
  - `write` 重载：
    - `template<typename Container> ssize_t write(Container& cont, int32_t timeout_msec = 0) const`：支持容器或 iovec。
    - `ssize_t write(void* buf, size_t count, int32_t timeout_msec = 0)`、`ssize_t write(mongoc_iovec_t* iov, size_t iovcnt, int32_t timeout_msec = 0)`。

**行为细节**:
- 对非 iovec 容器，read/write 要求迭代器为随机访问且元素为 POD（非 class），并使用 `yggr::bytes::byte_size` 计算字节长度。
- `detail::mongoc_stream_t_shared_ptr_deleter` 在删除时会检查 `_outside._need_del` 决定是否调用 `mongoc_stream_destroy`。

**示例**:

```cpp
using yggr::nsql_database_system::c_mongo_basic_stream;

// 从原生流构造（假设 p is mongoc_stream_t*）
c_mongo_basic_stream s(p, true);

// 读取到 std::vector<char>
std::vector<char> buf(1024);
ssize_t n = s.read(buf, 1, 1000);

// 写入
std::string msg = "hello";
ssize_t w = s.write(msg, 1000);

// 释放原生指针
mongoc_stream_t* p_raw = s.release();
```

**依赖**:
- libmongoc (`mongoc_stream_t`, `mongoc_stream_read`, `mongoc_stream_write`, `mongoc_stream_readv`, `mongoc_stream_writev`, `mongoc_stream_close`, `mongoc_stream_flush`, `mongoc_stream_destroy`)。
- Boost Range（`boost::range_value` / `boost::range_iterator` / `boost::size` 等）。
- yggr 辅助库（`yggr::shared_ptr`, `bytes::byte_size`, `iterator_category_check` 等）。

**位置**:
- 源文件: [yggr/nsql_database_system/c_mongo_basic_stream.hpp](yggr/nsql_database_system/c_mongo_basic_stream.hpp)

需要我把文档展开为按函数签名的完整 API 列表或添加更多使用示例（例如 iovec 使用示例）吗？