**c_mongo_client_encryption_encrypt_opts.hpp — 文档说明**

概述
- **名称**: `c_mongo_client_encryption_encrypt_opts`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_client_encryption_encrypt_opts.hpp](yggr/nsql_database_system/c_mongo_client_encryption_encrypt_opts.hpp)
- **用途**: C++ 封装 `mongoc_client_encryption_encrypt_opts_t`，提供类型安全的访问器与多态模板构造/赋值接口，用于设置数据加密操作的选项（如 keyId、算法、别名、争用因子、查询模式与范围选项）。

主要类型别名
- `base_type`: `yggr_mongoc_client_encryption_encrypt_opts`（底层裸结构适配）
- `org_type`: `mongoc_client_encryption_encrypt_opts_t`（原生类型）
- `bson_value_type`: `c_bson_value`（封装的 BSON 值类型，用于 `keyid`）
- `inner_string_type`: `charset::utf8_string`（用于算法、别名、查询模式等字符串）
- `range_opts_type`: `c_mongo_client_encryption_encrypt_range_opts`（范围加密专用选项）

构造与赋值
- `c_mongo_client_encryption_encrypt_opts()` — 默认构造并调用内部 `pro_init()`。
- 模板构造函数 — 支持多参数构造：仅 `keyid` 与 `algorithm`；或包含 `keyaltname`、`contention_factor` 与 `query_mode`；或额外包含 `range_opts`。模板接口会调用 `pro_init()` 并通过 `var_*()` 接口初始化成员。
- 支持从原生 `org_type` 指针或引用构造：`c_mongo_client_encryption_encrypt_opts(const org_type* ptr)` / `(const org_type& right)`。
- 拷贝/移动构造与赋值均实现（内部使用 `pro_copy` / `pro_move` / `pro_swap` 等），并提供 `swap`。

关键成员访问器
- `var_keyid()` — 返回对底层 `base_type::keyid` 的 `c_bson_value` 封装引用，可直接赋值/读取 BSON 值。
- `var_algorithm()` — 字符串成员访问器（带 member_var_modify_helper），修改时自动同步到 `base_type::algorithm`。
- `var_keyaltname()` — 字符串别名访问器，同步 `base_type::keyaltname`。
- `var_contention_factor()` / `var_contention_factor_value()` / `var_contention_factor_set()` — 管理争用因子（`contention_factor` 字段使用 `detail::setmark_value<s64>` 封装以区分是否已设置）。
- `var_query_mode()` — 查询模式访问器，同步 `base_type::query_type`。
- `var_range_opts()` — 返回并设置 `range_opts_type`（仅当算法为 range/preview 时，会设置 `base_type::range_opts`，否则清空并置为 0）。

辅助与保护方法（概览）
- `pro_init()` — 初始化底层结构并设置默认值。
- `pro_rebuild_base()` — 将内部 C++ 成员同步回底层 C 结构。
- `pro_clear()` / `pro_copy()` / `pro_swap()` / `pro_move()` — 内部复制/清理/交换/移动操作的实现（具体细节在实现文件）。

常量与算法/查询模式别名
- 静态函数提供一组预定义算法与查询模式字符串，来自 `mongoc_client_encryption_encrypt_opts_native_ex`：
  - `s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_random()`
  - `s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_deterministic()`
  - `s_encrypt_algorithm_indexed()` / `s_encrypt_algorithm_unindexed()` / `s_encrypt_algorithm_range_preview()` / `s_encrypt_algorithm_range()`
  - `s_encrypt_query_mode_equality()` / `s_encrypt_query_mode_range_preview()` / `s_encrypt_query_mode_range()`

实现注意事项
- 字符串成员的 getter/setter 使用了宏 `YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME` / `YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF`，支持多种参数传入形式（可变参数或差异化重载，取决于是否启用 variadic templates）。
- `var_range_opts` 的 setter 会检查 `_algorithm` 是否为 range-preview/range，否则不会将范围选项传入底层。
- 争用因子使用封装类型与底层结构的二进制兼容性断言（`BOOST_MPL_ASSERT`）。

示例
```cpp
// 构造：只指定 keyId 与 算法
yggr::nsql_database_system::c_mongo_client_encryption_encrypt_opts opts(keyid_val, 
    yggr::nsql_database_system::c_mongo_client_encryption_encrypt_opts::s_encrypt_algorithm_aead_aes_256_cbc_hmac_sha_512_random());

// 设置 keyaltname, contention factor, query mode
opts.var_keyaltname() = "my_alt";
opts.var_contention_factor_value() = 42;
opts.var_contention_factor_set() = true;
opts.var_query_mode() = yggr::nsql_database_system::c_mongo_client_encryption_encrypt_opts::s_encrypt_query_mode_equality();

// 若要使用 range 加密，设置算法为 range 并提供 range_opts
opts.var_algorithm() = yggr::nsql_database_system::c_mongo_client_encryption_encrypt_opts::s_encrypt_algorithm_range();
opts.var_range_opts() = range_opts_instance;
```

依赖
- 包含头文件：
  - `c_bson_value.hpp`、`c_mongo_client_encryption_encrypt_range_opts.hpp`
  - `mongoc_client_encryption_encrypt_opts_native_ex.hpp`（本地常量与辅助）
  - `detail/setmark_value.hpp`、`detail/string_member_var_get.hpp`

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_client_encryption_encrypt_opts.md](yggr/nsql_database_system/c_mongo_client_encryption_encrypt_opts.md)

扩展建议
- 如需更详细的 API（例如每个宏生成的重载签名、变参支持示例或 `pro_*` 实现细节），我可以读取并引用对应的实现文件并补充说明。
