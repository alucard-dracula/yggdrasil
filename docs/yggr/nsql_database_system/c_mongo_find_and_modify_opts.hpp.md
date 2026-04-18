**c_mongo_find_and_modify_opts.hpp — 文档说明**

概述
- **名称**: `c_mongo_find_and_modify_opts`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_find_and_modify_opts.hpp](yggr/nsql_database_system/c_mongo_find_and_modify_opts.hpp)
- **用途**: 封装 libmongoc 的 `mongoc_find_and_modify_opts_t`，提供 C++ 风格的成员访问器与常量定义，用于配置 `findAndModify`（查找并修改）操作的选项，例如排序、更新文档、返回字段、行为标志、超时和扩展字段（`extra`）。

常量与标志
- 提供与 native_ex 对应的静态常量：
  - `E_MONGOC_BYPASS_DOCUMENT_VALIDATION_FALSE` / `_TRUE` / `_DEFAULT`
  - `E_MONGOC_FIND_AND_MODIFY_NONE` / `_REMOVE` / `_UPSERT` / `_RETURN_NEW`

主要类型别名
- `base_type`: `yggr_mongoc_find_and_modify_opts`（底层裸结构）
- `org_type`: `mongoc_find_and_modify_opts_t`（原生类型）
- `bson_type`: `c_bson`（用于封装 `bson_t` 成员）

构造与生命周期
- 默认构造与从原生 `org_type` 指针/引用构造均受支持；同时实现拷贝/移动语义与 `swap`。

成员访问器
- `var_sort()` / `var_update()` / `var_fields()` — 访问 `sort`、`update`、`fields` 三个 BSON 成员，均提供返回 `c_bson&` 与模板重载以按派生 `bson_t` 类型返回引用。
- `var_flags()` — 访问 `flags` 字段（位标志，使用上面定义的静态常量）。
- `var_bypass_document_validation()` — 访问 `bypass_document_validation`（使用对应静态常量指示 true/false/default）。
- `var_max_time_ms()` — 访问 `max_time_ms`（操作超时，单位毫秒）。
- `var_extra()` — 访问 `extra` BSON，用于扩展/hedge 等用途；同样支持模板重载返回具体 BSON 派生类型引用。

实现/语义备注
- 内部维护 `_sort`、`_update`、`_fields` 三个 `c_bson` 成员，当需要与底层 `base_type` 同步时，可通过 `prv_init` / `prv_copy` / `prv_swap` / `pro_move` 实现。
- 模板重载通过 `boost::enable_if<boost::is_base_of<bson_t, Bson>>` 限定，以保证对原生 BSON 派生类型的安全转换。

示例
```cpp
using namespace yggr::nsql_database_system;

c_mongo_find_and_modify_opts opts;
// 设置排序
opts.var_sort().append("age", 1);

// 设置更新
opts.var_update().append("$set", c_bson("{ 'active': true }"));

// 设置返回新文档
opts.var_flags() |= c_mongo_find_and_modify_opts::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW;

// 设置 max time
opts.var_max_time_ms() = 5000;

// 使用 extra
opts.var_extra().append("hint", c_bson("{ 'index': 1 }"));
```

依赖
- 头文件：`c_bson.hpp`, `mongoc_find_and_modify_opts_native_ex.hpp`

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_find_and_modify_opts.md](yggr/nsql_database_system/c_mongo_find_and_modify_opts.md)

扩展建议
- 我可以继续：
  - 补充 `prv_*` / `pro_move` 方法的实现细节（需读取对应实现文件）；
  - 给出更完整的 `findAndModify` 使用示例，展示如何把该选项对象传给调用函数并处理返回值。
