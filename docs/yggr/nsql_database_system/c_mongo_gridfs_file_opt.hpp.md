# c_mongo_gridfs_file_opt

Source: [yggr/nsql_database_system/c_mongo_gridfs_file_opt.hpp](yggr/nsql_database_system/c_mongo_gridfs_file_opt.hpp#L1-L606)

**概述**
- 封装 `mongoc_gridfs_file_opt_t` 的 C++ wrapper：`c_mongo_gridfs_file_opt`。
- 提供对 GridFS 文件选项的类型安全访问与易用构造（md5、filename、content_type、aliases、metadata、chunk_size 等字段）。

**主要类型别名**
- `inner_string_type`：内部字符串类型（用于 md5/filename/content_type 等）。
- `bson_type`：内部 BSON 类型（用于 aliases/metadata）。

**主要成员（字段）**
- `_md5`：文件的 MD5 字符串。
- `_fname`：文件名。
- `_content`：content_type 字符串。
- `_aliases`：aliases（BSON）
- `_metadata`：metadata（BSON）
- `base_type::chunk_size`：块大小（uint32_t）。

**构造 / 赋值**
- 提供默认构造、从原生 `mongoc_gridfs_file_opt_t` 构造、拷贝/移动构造与赋值、以及多种接受不同字符串编码/容器的构造重载。

**访问器 / 修改器**
- `var_md5()` / `var_file_name()` / `var_content()`：字符串成员访问器，支持多种重载（直接读取与通过 `member_var_modify_helper` 修改）。
- `var_aliases()` / `var_metadata()`：BSON 成员访问器，既提供 `const bson_type&` 的只读访问，也提供返回 `member_var_modify_helper` 的可写访问重载（支持传入不同 Bson 类型模板）。
- `var_chunk_size()`：读写块大小（返回 `uint32_t&` 或 `uint32_t` 常量）。

实现细节要点：
- 使用 `pro_set_var_*` / `pro_get_var_*` 保护方法封装原生值的读取/写入，并把内部缓存（如 `_aliases/_metadata`）与 `base_type` 的指针字段保持同步（例如根据 `_aliases.empty()` 决定 `base_type::aliases` 是否为 `0`）。
- 对字符串访问器使用宏（YGGR_PP_*）生成多个重载以适配不同字符类型与容器。
- 提供 `swap` 支持，并将 `swap` 导入到 `std` 与 `boost` 命名空间。

**使用示例（简洁）**
```cpp
// 创建并设置简单字段
yggr::nsql_database_system::c_mongo_gridfs_file_opt opt;
opt.var_md5() = "0123456789abcdef";
opt.var_file_name() = "example.bin";
opt.var_chunk_size() = 4096u;

// 访问只读 BSON metadata
const auto &meta = opt.var_metadata();
// 可写访问通过返回的 helper（示例依赖 yggr::utility::member_var_modify_helper 的用法）
// opt.var_metadata().assign(some_bson);
```

**依赖**
- libmongoc / libbson（`mongoc_gridfs_file_opt_t`, `bson_t` 等）。
- Boost（type_traits、enable_if、addressof 等）。
- yggr 内部工具：`utility::member_var_modify_helper`、string/encoding helpers、safe wrapper 与 smart_ptr_ex 等。

**建议的扩展 / 改进**
- 添加序列化/反序列化帮助（to/from `bson_t` 或 JSON），便于持久化或网络传输示例。
- 补充单元测试覆盖：各种字符串编码、metadata/aliases 为空或非空、chunk_size 边界值。
- 提供更详细的示例：配合 GridFS 上传/下载流程的端到端示例。

**文件位置**
- [yggr/nsql_database_system/c_mongo_gridfs_file_opt.hpp](yggr/nsql_database_system/c_mongo_gridfs_file_opt.hpp#L1-L606)

---
自动生成于 2026-02-25。若需要更详尽的签名级 API 参考或包含示例中的完整可编译示例，我可以继续补充。
