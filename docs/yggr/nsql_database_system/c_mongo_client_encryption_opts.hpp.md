**c_mongo_client_encryption_opts.hpp — 文档说明**

概述
- **名称**: `c_mongo_client_encryption_opts`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_client_encryption_opts.hpp](yggr/nsql_database_system/c_mongo_client_encryption_opts.hpp)
- **用途**: 封装 libmongoc 的 `mongoc_client_encryption_opts_t`，为客户端端加密（Client-Side Field Level Encryption）提供 C++ 风格的构造、访问器与回调设置（包括 keyvault 命名空间、KMS providers、TLS 选项与凭证回调）。

主要类型别名
- `base_type`: `yggr_mongoc_client_encryption_opts`（底层裸结构适配）
- `org_type`: `mongoc_client_encryption_opts_t`（原生类型）
- `bson_type`: `c_bson`（用于 `kms_providers` / `tls_opts`）
- `inner_string_type`: `charset::utf8_string`（用于 keyvault db/coll 与 namespace）
- `mc_kms_credentials_callback_type`: `c_mc_kms_credentials_callback`（KMS 凭证回调封装）

构造与赋值
- 默认构造：`c_mongo_client_encryption_opts()`，内部会在 `pro_init()` 中设置初始状态。
- 模板构造：可同时初始化 `keyvault_db`、`keyvault_coll`、`kms_providers`、`tls_opts`，并提供 KMS 凭证回调函数与用户数据（可为 `void*` 或模板化的用户类型）。构造后会调用 `pro_rebuild_base()` 同步到底层 C 结构。
- 支持从原生 `org_type` 指针或引用构造，并实现拷贝/移动/swap 语义。

关键访问器
- `var_keyvault_db()` / `var_keyvault_coll()` — 字符串成员的修改器（修改时自动同步到 `base_type::keyvault_db` / `keyvault_coll`）。
- `keyvault_namespace()` — 返回 `{db}.{coll}` 形式的命名空间字符串，并支持多种返回类型（通过模板与字符集转换）。也可通过 `keyvault_namespace()` 的 setter 将 `"db.coll"` 分解并设置对应字段。
- `var_kms_providers()` / `var_tls_opts()` — 通过宏 `YGGR_TMP_PP_MONGO_ENCRYPTION_VAR_BSON_GET_SET` 提供对 BSON 成员（`kms_providers`、`tls_opts`）的访问与修改接口。
- `var_creds_cb()` / `creds_cb_fn()` / `creds_cb_userdata()` — 访问与设置 KMS 凭证回调函数与用户数据（支持模板化用户数据指针）。

实现/语义备注
- 修改字符串或 BSON 成员时，类会自动将 C++ 层成员值同步到底层 `base_type` 的 C 字段（例如 `base_type::keyvault_db` 指向 `_keyvault_db.data()`）。
- KMS 凭证回调通过 `mongoc_client_encryption_opts_native_ex::s_mongoc_client_encryption_opts_set_kms_credential_provider_callback` 进行设置（封装了 native 侧的细节并支持模板用户数据）。
- 字符串 getter/setter 使用了工程内宏以支持可变参数 / 字符集转换（由 `detail/string_member_var_get.hpp` 与相关宏实现）。

示例
```cpp
// 构造并设置 kms providers / tls opts
c_bson kms_providers = /* 填充 BSON */;
c_bson tls_opts = /* TLS 选项 BSON */;

yggr::nsql_database_system::c_mongo_client_encryption_opts opts(
    std::string("keyvault_db"), std::string("keyvault_coll"),
    kms_providers, tls_opts,
    my_kms_callback_fn, /* userdata */ nullptr);

// 访问命名空间
auto ns = opts.keyvault_namespace(); // 返回 utf8_string

// 设置回调 userdata 为具体类型指针
struct MyUser { int x; } mu;
opts.pro_set_kms_credential_provider_callback(my_kms_callback_fn, mu);
```

依赖
- 头文件：
  - `c_bson.hpp`
  - `c_mongo_mc_kms_credentials_callback.hpp`
  - `mongoc_client_encryption_opts_native_ex.hpp`
  - `detail/string_member_var_get.hpp` 与 `detail/mongo_encryption_bson_member_get_set_macro.hpp`

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_client_encryption_opts.md](yggr/nsql_database_system/c_mongo_client_encryption_opts.md)

扩展建议
- 我可以继续：
  - 展开 `YGGR_TMP_PP_MONGO_ENCRYPTION_VAR_BSON_GET_SET` 宏生成的具体重载签名；
  - 从实现文件补充 `pro_*` 方法的行为与错误处理细节；
  - 添加更完整的使用示例（比如与 `mongoc_client_t` 的交互流程）。
