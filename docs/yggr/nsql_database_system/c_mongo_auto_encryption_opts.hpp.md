# c_mongo_auto_encryption_opts.hpp 文档

**概述**:
- **说明**: `c_mongo_auto_encryption_opts` 是对 libmongoc 的 `mongoc_auto_encryption_opts_t` 配置结构的 C++ 封装，提供键保管（keyVault）命名空间、KMS 提供者、TLS 选项、模式映射、加密字段映射、额外选项以及 KMS 凭证回调的管理、序列化与与原生结构的互操作。
- **所在文件**: [yggr/nsql_database_system/c_mongo_auto_encryption_opts.hpp](yggr/nsql_database_system/c_mongo_auto_encryption_opts.hpp)

**命名空间**:
- `yggr::nsql_database_system`

**主要类型与别名**:
- `base_type` = `yggr_mongoc_auto_encryption_opts`（受保护继承）；`org_type` = `mongoc_auto_encryption_opts_t`（原生结构）。
- `bson_type` = `c_bson`。
- 字符串类型：`inner_string_type` = `utf8_string`。
- KMS 回调相关类型：`mc_kms_credentials_callback_type` 与 `kms_credentials_provider_callback_fn_type`。

**构造与赋值**:
- 支持多种构造函数：默认、基于 keyvault DB/collection 与多个 BSON 配置的构造（支持模板化字符串/BSON 类型以便接受多种字符串/容器类型）、从 `mongoc_auto_encryption_opts_t*` 或引用构造、拷/移动构造。
- 支持赋值来自原生 `org_type` 或 `base_type`，以及拷/移赋值。
- 内部提供 `pro_init()`, `pro_rebuild_base()` 等辅助以同步封装字段到原生结构。

**成员访问与修改**:
- `var_keyvault_db()` / `var_keyvault_coll()` 提供字符串读写接口（返回修改助手 `member_var_modify_helper`），同时同步更新底层 `base_type::keyvault_db` / `keyvault_coll` 指针。
- `keyvault_namespace()` 读取或设置组合 namespace（`db.collection` 格式），并拆分写回 `_keyvault_db` / `_keyvault_coll`。
- 对 BSON 成员（`_kms_providers`, `_tls_opts`, `_schema_map`, `_encrypted_fields_map`, `_extra`）提供宏生成的 getter/setter（`YGGR_TMP_PP_MONGO_ENCRYPTION_VAR_BSON_GET_SET`），以支持模板化的 BSON 类型访问/修改。
- `var_bypass_auto_encryption()` / `var_bypass_query_analysis()` 访问底层布尔标志。
- KMS 凭证回调访问器：`var_creds_cb()`, `creds_cb_fn()`, `creds_cb_userdata()` 提供对回调与用户数据的读写（并提供模板化的用户数据指针转换）。

**验证与行为**:
- `validate()` 返回是否包含必要字段（`keyvault_db`, `keyvault_coll`, `kms_providers`, `schema_map`）。
- `pro_set_kms_credential_provider_callback(...)` 将回调通过本地 native_ex 接口设置到底层结构（支持传入用户数据对象引用模板重载）。

**内部成员**:
- `_keyvault_db`, `_keyvault_coll`（字符串）
- `_kms_providers`, `_tls_opts`, `_schema_map`, `_encrypted_fields_map`, `_extra`（均为 `c_bson`）

**示例**:

```cpp
using yggr::nsql_database_system::c_mongo_auto_encryption_opts;

c_mongo_auto_encryption_opts opts(
    std::string("keyvault_db"),
    std::string("keyvault_coll"),
    some_kms_providers_bson,
    some_tls_opts_bson,
    some_schema_map_bson);

if(opts.validate()) {
    mongoc_auto_encryption_opts_t* native = opts.org_pointer();
    // 传递 native 给 mongoc 客户端创建函数
}

// 设置回调与用户数据
opts.pro_set_kms_credential_provider_callback(my_callback_fn, my_userdata);
```

**依赖**:
- libmongoc（`mongoc_auto_encryption_opts_t`、KMS 回调签名等）以及项目的 `mongoc_auto_encryption_opts_native_ex` 辅助。
- `c_bson` 与字符集转换工具（`utf8_string`、`string_converter`）。
- Boost（enable_if、move 等）。

**位置**:
- 源文件: [yggr/nsql_database_system/c_mongo_auto_encryption_opts.hpp](yggr/nsql_database_system/c_mongo_auto_encryption_opts.hpp)

我已将文档保存到同目录。要我把文档展开为每个 getter/setter 与模板签名的详尽列表，还是继续为下一个文件生成文档？