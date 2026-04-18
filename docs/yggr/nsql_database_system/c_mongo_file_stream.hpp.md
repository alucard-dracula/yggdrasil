**c_mongo_file_stream.hpp — 文档说明**

概述
- **名称**: `c_mongo_file_stream`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_file_stream.hpp](yggr/nsql_database_system/c_mongo_file_stream.hpp)
- **用途**: 基于 `c_mongo_basic_stream` 的文件流封装，用于将操作系统文件描述符或文件路径包装为 `mongoc_stream_file_t`，以便与 libmongoc 的流接口互操作（例如 GridFS 或其它需要底层流的场景）。

主要类型别名
- `base_type`: `c_mongo_basic_stream`（基类）
- `org_type`: `mongoc_stream_file_t`（原生类型）
- `base_org_type`: 基类暴露的原生流类型

构造与析构
- `c_mongo_file_stream()` — 默认构造。
- `c_mongo_file_stream(base_org_type* pstream)` / `(base_org_type* pstream, bool need_del)` — 包装已有原生流指针，可指定是否由本对象负责删除。
- `c_mongo_file_stream(int fd)` — 通过文件描述符构造（包装为 `mongoc_stream_file_new_for_fd`）。
- `c_mongo_file_stream(const char* fpath, int flags = O_RDONLY, int mode = 0)` — 使用 C 字符串路径构造。
- 模板构造：支持多种字符串类型（`Basic_String<char,...>`、宽字符或自定义字符类型），并使用 `charset::string_converter::s_conv` 将路径转换为 `char` 路径后再构造底层 `mongoc_stream_file`。
- 移动构造/移动赋值受支持，拷贝被禁用。
- 析构会正确释放底层流（若构造时需删除）。

主要方法与属性
- `org_pointer()` / `operator org_type*()` — 获取原生 `mongoc_stream_file_t*`。
- `fd()` — 返回底层文件描述符（调用 `mongoc_stream_file_get_fd`），若无流则返回 0。
- `swap()` / `operator=`（移动） — 交换或移动资源。

细节与兼容性
- 提供了多种构造器重载以支持不同字符类型的文件路径，并通过 SFINAE (`is_native_char` 与 `charset::is_string_t`) 限定重载选择。
- 在 Windows 平台（`YGGR_AT_WINDOWS`）使用 `utf8_string` 到本地 `string` 的转换以传递给 `mongoc_stream_file_new_for_path`。

示例
```cpp
// 从路径创建
yggr::nsql_database_system::c_mongo_file_stream fs("/tmp/data.bin", O_RDONLY);
int fd = fs.fd();

// 从现有 fd 创建
yggr::nsql_database_system::c_mongo_file_stream fs2(fd);

// 使用基本 string 模板重载
std::string path = "/tmp/data.bin";
yggr::nsql_database_system::c_mongo_file_stream fs3(path, O_RDONLY);
```

依赖
- 头文件：`c_mongo_basic_stream.hpp`、`yggr/charset/string_converter.hpp`、`yggr/charset/utf8_string.hpp`。

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_file_stream.md](yggr/nsql_database_system/c_mongo_file_stream.md)

扩展建议
- 如需我可以：
  - 展示 `mongoc_stream_file_new_for_fd` / `mongoc_stream_file_new_for_path` 在不同平台的行为差异；
  - 增加错误处理与异常语义说明（例如路径不存在或权限不足时的行为）。
