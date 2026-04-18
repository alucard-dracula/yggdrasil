**c_mongo_connection_init.hpp — 文档说明**

概述
- **名称**: `c_mongo_connection_init`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_connection_init.hpp](yggr/nsql_database_system/c_mongo_connection_init.hpp)
- **用途**: 表示创建/初始化 MongoDB 连接所需的配置信息，继承自 `c_mongo_uri`，在开启 SSL 支持时也继承 `c_mongo_ssl_opt`，以组合 URI 与 TLS/SSL 选项。

主要类型与别名
- `inner_string_type`: `utf8_string`（内部字符串类型）
- `uri_type`: `c_mongo_uri`（URI 表示类型）
- `ssl_opt_type`（若启用 SSL）: `c_mongo_ssl_opt`（TLS/SSL 选项类型）

构造与赋值
- `c_mongo_connection_init()` — 默认构造。
- 模板构造：`c_mongo_connection_init(const String& str_uri)`，支持任意可构造为 `utf8_string` 的字符串类型。
- 支持以 `uri_type` 或 (`uri_type`,`ssl_opt_type`) 的右值/左值方式构造。
- 若编译环境定义 `MONGOC_ENABLE_SSL`，还提供多参数构造以直接传入证书路径、密码、CA、CRL 等以及一组 TLS 参数（或内部 TLS 选项类型）。
- 实现移动与拷贝赋值，移动赋值使用 `copy_or_move_or_swap` 以在不同语义下高效转移数据。

属性与方法
- `empty()` — 判断是否含有有效 URI（委托给 `uri_type::empty()`）。
- `is_using_tls()` — 若启用 SSL，则返回 URI 要求 TLS 且 `ssl_opt` 非空；否则返回 false。
- `uri()` / `uri() const` — 访问底层 `uri_type`。
- `ssl_opt()` / `ssl_opt() const`（若启用 SSL）— 访问 TLS/SSL 配置。
- `swap()` / `operator=` 等提供交换与复制/移动支持。

实现注意事项
- 当定义 `MONGOC_ENABLE_SSL` 时，类多重继承 `c_mongo_uri` 与 `c_mongo_ssl_opt`，并在构造时通过不同的模板与重载支持丰富的 TLS 选项初始化。
- 使用 `BOOST_COPYABLE_AND_MOVABLE` 宏与 `BOOST_RV_REF` 等宏兼容较老的 C++ move 语法。
- `copy_or_move_or_swap` 在移动赋值中用于避免不必要的拷贝并保持异常安全。

示例
```cpp
// 基本：通过 URI 字符串构造
yggr::nsql_database_system::c_mongo_connection_init init("mongodb://localhost:27017");

#ifdef MONGOC_ENABLE_SSL
// 带 TLS/SSL 选项构造（示例）
yggr::nsql_database_system::c_mongo_connection_init init_ssl(
    "mongodb+srv://example.com",
    "client.pem", "pem_pwd",
    "ca.pem", "/etc/ssl/certs", "crl.pem",
    false, false, /* internal_tls_opts */ some_tls_opts);
#endif

if(!init.empty()) {
    // 使用 init.uri() 获取连接字符串
}
```

依赖
- `c_mongo_uri.hpp`
- `c_mongo_ssl_opt.hpp`（仅当编译时启用 SSL）
- `yggr/charset/string.hpp`, `yggr/utility/copy_or_move_or_swap.hpp`

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_connection_init.md](yggr/nsql_database_system/c_mongo_connection_init.md)

扩展建议
- 可以继续：
  - 为 SSL 构造的各个参数写更详细的说明与典型值；
  - 展示如何将 `c_mongo_connection_init` 与 `c_mongo_connection` 协同使用完成连接初始化。
