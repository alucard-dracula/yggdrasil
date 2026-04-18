**c_mongo_client_encryption_rewrap_many_datakey_result.hpp — 文档说明**

概述
- **名称**: `c_mongo_client_encryption_rewrap_many_datakey_result`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_client_encryption_rewrap_many_datakey_result.hpp](yggr/nsql_database_system/c_mongo_client_encryption_rewrap_many_datakey_result.hpp)
- **用途**: 封装 libmongoc 的 `mongoc_client_encryption_rewrap_many_datakey_result_t` 结构，表示对多数据密钥重包（rewrap）操作的批量写结果（`bulk_write_result`）。提供 C++ 风格的构造、访问与管理接口。

主要类型
- `base_type`: `yggr_mongoc_client_encryption_rewrap_many_datakey_result_t`（底层裸结构）
- `org_type`: `mongoc_client_encryption_rewrap_many_datakey_result_t`（原生类型）
- `bson_type`: `c_bson`（用于封装 `bulk_write_result`）

构造与赋值
- `c_mongo_client_encryption_rewrap_many_datakey_result()` — 默认构造，调用 `pro_init()` 初始化内置 BSON 结构。
- `c_mongo_client_encryption_rewrap_many_datakey_result(const bson_t& bs_data)` — 以已有 BSON 数据初始化结果。
- 支持从原生 `org_type` 指针或引用构造。
- 实现了拷贝、移动、以及基于 `base_type` / `org_type` 的赋值操作（`pro_copy` / `pro_move` / `pro_swap` 等内置方法）。

主要方法
- `result()` — 返回 `bulk_write_result` 的 `c_bson&` 或 const 版本；并提供模板重载以将结果作为派生自 `bson_t` 的类型返回。
- `clear()` — 清空内部 `bulk_write_result`（调用 `pro_clear()`）。
- `empty()` — 判断结果是否为空（基于 `bulk_write_result.len` 与 `bson_empty`）。
- 隐式转换运算符：支持转换为原生 `org_type&`/`org_type*`，以及 `bson_type&`/`const bson_type&`，方便与原生 API 互操作。

实现/语义备注
- 构造与移动实现会确保内部 `bulk_write_result` 的 BSON 结构被正确初始化（例如使用 `::bson_init`）并在复制/移动/销毁时正确管理内存。
- `result()` 的模板重载使用 `boost::enable_if` 限定以确保安全的向下转换到 `bson_t` 的派生类型。
- `empty()` 同时检查底层长度与 `bson_empty` 以判断语义上的“无结果”。

示例
```cpp
// 从 bson 初始化
bson_t bs = /* 从操作得到的 bulk write result */;
yggr::nsql_database_system::c_mongo_client_encryption_rewrap_many_datakey_result res(bs);

// 访问结果
const c_bson& b = res.result();
if(!res.empty()) {
    // 使用 b 进行检查/解析
}

// 清空
res.clear();
```

依赖
- 包含头：`c_bson.hpp`、`mongoc_client_encryption_rewrap_many_datakey_result_native_ex.hpp`

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_client_encryption_rewrap_many_datakey_result.md](yggr/nsql_database_system/c_mongo_client_encryption_rewrap_many_datakey_result.md)

扩展建议
- 如需我可以：
  - 从实现文件补充 `pro_*` 方法的具体行为与内存管理细节；
  - 添加更完整的示例，展示如何将该类型与 `mongoc_client_encryption_rewrap_many_datakey` 的调用配合使用。
