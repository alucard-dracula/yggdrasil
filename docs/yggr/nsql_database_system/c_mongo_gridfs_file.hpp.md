# c_mongo_gridfs_file

Source: [yggr/nsql_database_system/c_mongo_gridfs_file.hpp](yggr/nsql_database_system/c_mongo_gridfs_file.hpp#L1-L1027)

**概述**
- `c_mongo_gridfs_file` 是对 `mongoc_gridfs_file_t` 的 C++ 封装，提供安全的智能指针持有、字段访问器、读写/seek/ tell/ save/ remove 操作以及与 `c_mongo_gridfs_file_opt` 的互转。
- 适用于操作 MongoDB GridFS 中的单个文件对象（读取文件元数据、读写数据、保存与删除）。

**主要类型别名**
- `org_type`：原生 `mongoc_gridfs_file_t`。
- `gridfs_file_ptr_type`：持有 `org_type` 的 `yggr::shared_ptr`。
- `inner_string_type` / `inner_string_view_type`：用于字符串字段（content_type、filename、md5）。
- `file_opt_type`：`c_mongo_gridfs_file_opt`，表示文件选项结构。
- `bson_type` / `bson_value_type` / `bson_oid_type` / `bson_error_type`：BSON 及错误类型别名。

**常量**
- `E_seek_pos_set` / `E_seek_pos_cur` / `E_seek_pos_end`：seek 的定位常量（对应 SEEK_SET/SEEK_CUR/SEEK_END）。

**构造 / 析构**
- `c_mongo_gridfs_file()`：默认构造（空对象）。
- `template<typename Opts> c_mongo_gridfs_file(mongoc_gridfs_t* pgridfs, const Opts& opts)`：通过 `file_opt` 和 `pgridfs` 初始化，构造 GridFS 文件对象。
- `c_mongo_gridfs_file(mongoc_gridfs_t* pgridfs, mongoc_stream_t* pstream, const Opts& opts)`：基于流创建（要求 `opts.filename` 不为空）。
- `c_mongo_gridfs_file(org_type* pfile)`：从原生指针构造（请避免重复封装）。
- 拷贝与移动语义：支持拷贝/移动构造与赋值，并提供 `swap`。

**字段访问 / 修改方法（概览）**
- aliases / metadata：
  - `bson_type aliases() const` / `bson_member_modify_helper_type aliases()`：读取或通过 helper 修改 aliases。
  - `bson_type metadata() const` / `bson_member_modify_helper_type metadata()`：同上。
- content / file_name / md5：
  - `inner_string_view_type content() const` / `inner_string_member_modify_helper_type content()`：content_type 字段。
  - `inner_string_view_type file_name() const` / `inner_string_member_modify_helper_type file_name()`：文件名字段。
  - `inner_string_view_type md5() const` / `inner_string_member_modify_helper_type md5()`：md5 字段。
- chunk_size：
  - `int32_t chunk_size() const`：返回 chunk size（`file_opt()` 时也会包含该值）。
- id / var_id：
  - `bson_oid_type id() const` / `bson_oid_member_modify_helper_type id()`：文件 id（通常为 ObjectId）。
  - `bson_value_type var_id() const` / `bson_value_member_modify_helper_type var_id()`：更通用的 id 访问。
- error / remove / save：
  - `bson_error_type error() const` / `bool error(org_bson_error_type& err) const`：获取最后错误。
  - `bson_error_type remove()` / `bool remove(org_bson_error_type* perr)`：从 GridFS 删除文件。
  - `bool save()` / `bool save(org_bson_error_type* perr)`：保存（插入/更新）文件对象。

**IO 操作**
- `ssize_t read(Container& cont, size_t min_bytes = -1, int32_t timeout_msec = 0) const`：支持传入 iovec 容器读取（容器元素应为 `mongoc_iovec_t` 或兼容类型）。
- `ssize_t write(Container& cont, int32_t timeout_msec = 0)`：支持传入 iovec 容器写入数据。
- 还有多种重载：接受 `mongoc_iovec_t*`、单个 `mongoc_iovec` 或 `c_mongo_iovec` 等。

**工具方法**
- `void clear()`：释放内部原生对象指针。
- `bool empty() const`：是否为空（未初始化）。
- `int64_t size() const` / `length()`：文件长度（字节）。
- `bool seek(int64_t delta, int whence)`：移动文件位置。
- `uint64_t tell() const`：获取当前位置偏移。

**比较运算符**
- 提供非成员比较运算符：`<, <=, >, >=, ==, !=`，基于 `file_name()` 或 `id()` 实现。

**示例（简短）**
```cpp
// 假设已建立 mongoc_client_t 和 mongoc_gridfs_t (gridfs)
yggr::nsql_database_system::c_mongo_gridfs_file file(gridfs, opts);
if(!file.empty()) {
    // 读文件元数据
    auto name = file.file_name();
    auto md5 = file.md5();

    // 读取数据块
    std::vector<mongoc_iovec_t> iov(1);
    // iov[0].iov_base = buffer; iov[0].iov_len = buf_len;
    ssize_t r = file.read(iov, 0);

    // 修改 metadata
    auto meta_helper = file.metadata();
    // meta_helper.assign( ... );

    // 保存并关闭
    file.save();
}
```

**依赖**
- libmongoc / libbson：`mongoc_gridfs_file_t`, `mongoc_gridfs_t`, `mongoc_iovec_t`, `bson_t` 等。
- Boost：type_traits, range, mpl, enable_if 等。
- yggr 内部：`c_bson`, `c_mongo_gridfs_file_opt`, `c_mongo_iovec`, `bson_error` helpers, `utility::member_var_modify_helper` 等。

**实现注意事项 & 建议**
- `var_id` / `id` 的写入会在内部抛出 `bson_error_type` 异常（`pro_set_var_id`），使用时注意捕获或使用提供的 error 重载。
- IO 方法对 iovec 的使用依赖 native `mongoc_gridfs_file_readv/writev`；传入容器必须满足元素类型要求。
- 建议补充：
  - 详细签名文档（列出每个模板重载的确切 SFINAE 条件）。
  - 可编译的 end-to-end 示例：创建 GridFS、写入本地文件、读取和验证 md5。
  - 单元测试：模拟不同 `file_opt` 参数与错误条件（network/permission）。

**文件位置**
- [yggr/nsql_database_system/c_mongo_gridfs_file.hpp](yggr/nsql_database_system/c_mongo_gridfs_file.hpp#L1-L1027)

---
自动生成于 2026-02-25。需要我将文档扩展为签名级别或添加完整可编译示例吗？
