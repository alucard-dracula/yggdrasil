**c_mongo_client_encryption_encrypt_range_opts.hpp — 文档说明**

概述
- **名称**: `c_mongo_client_encryption_encrypt_range_opts`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_client_encryption_encrypt_range_opts.hpp](yggr/nsql_database_system/c_mongo_client_encryption_encrypt_range_opts.hpp)
- **用途**: 封装 libmongoc 的 `mongoc_client_encryption_encrypt_range_opts_t`（范围加密选项），提供 C++ 友好的访问器、构造与设置方法，用于配置范围加密的 min/max、sparsity、precision、trim_factor 等参数。

主要类型
- `base_type`: `yggr_mongoc_client_encryption_encrypt_range_opts`（底层裸结构）
- `org_type`: `mongoc_client_encryption_encrypt_range_opts_t`（原生类型）
- `bson_value_type`: `c_bson_value`（用于 min/max 的 BSON 值封装）
- `min_max_setmark_value_type`: `detail::setmark_value<org_bson_value_type, detail::bson_value_setter>`（带设置标志的 BSON 值包装）
- `precision_setmark_value_type` / `sparsity_setmark_value_type` / `trim_factor_setmark_value_type` — 根据 libmongoc 版本对整数字段的封装类型（使用静态断言保证与底层二进制兼容）。

构造与赋值
- `c_mongo_client_encryption_encrypt_range_opts()` — 默认构造并初始化。
- `c_mongo_client_encryption_encrypt_range_opts(s64 arg_sparsity)` — 直接指定 `sparsity` 的构造版本。
- 模板构造：`(min, max, sparsity=0, precision=0, trim_factor=0)` — 支持任意可转换为 BSON 值的 `min`/`max`，会根据类型决定是否设置 `precision`。
- 支持从原生 `org_type` 指针或引用构造，拷贝/移动语义与 `swap`。

关键访问器
- `var_min()` / `var_max()` — 返回对底层 `min` / `max` 的 `min_max_setmark_value_type` 引用（可读写，并维护是否已设置）。
- `var_min_value()` / `var_max_value()` — 返回嵌套的 `bson_value_type`（或模板重载返回具体 BSON 值派生类型引用）。
- `var_min_set()` / `var_max_set()` / `min_is_seted()` / `max_is_seted()` — 查询/设置是否存在 min/max 值。
- `var_sparsity()` / `var_sparsity_value()` / `var_sparsity_set()` — 管理 sparsity，取决于 libmongoc 版本，可能为直接数值或带 set 标志的包装。
- `var_precision()` / `var_precision_value()` / `var_precision_set()` — precision 字段的访问器（带 set 标志）。
- `var_trim_factor()` / `var_trim_factor_value()` / `var_trim_factor_set()` — trim_factor 字段访问；在较旧 libmongoc 版上该字段被忽略并返回默认/不可设置行为。

行为细节与兼容性
- 该头通过宏 `YGGR_MONGOC_HEX()` 检查 libmongoc 版本，并根据版本调整字段类型与可用性（例如 `trim_factor` 在旧版本为无效/忽略）。
- 使用 `detail::bson_value_setter` 来支持对 `min`/`max` 的值进行复制、移动或 swap 操作。
- 构造函数在 `min`/`max` 类型不匹配时会清除 `min`/`max` 的设置。
- 所有 set 操作最终会影响 `base_type` 的原生字段内存布局，代码中使用静态断言保证二进制兼容。

方法（保护）
- `pro_init()` / `pro_clear()` / `pro_copy()` / `pro_swap()` / `pro_move()` — 内部初始化、清理、复制、交换与移动实现（详见实现文件）。
- `clear()` — 调用 `pro_clear()` 清空所有设置。

示例
```cpp
// 使用 min/max 构造
auto opts = yggr::nsql_database_system::c_mongo_client_encryption_encrypt_range_opts(1.0, 10.0, 100, 2);

// 设置 min/max 单独字段
opts.var_min() = 0.5; // 设置并标记为已设置
opts.var_max() = 9.5;

// 访问 sparsity
if(opts.sparsity_is_seted()) {
    auto sp = opts.var_sparsity_value();
}

// 精度设置（浮点/decimal 类型有关）
opts.var_precision() = 3;

// 清空
opts.clear();
```

依赖
- 包含头：
  - `c_bson_value.hpp`
  - `mongoc_client_encryption_encrypt_range_opts_native_ex.hpp`
  - `detail/setmark_value.hpp`

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_client_encryption_encrypt_range_opts.md](yggr/nsql_database_system/c_mongo_client_encryption_encrypt_range_opts.md)

扩展建议
- 如需更详细的兼容性矩阵（针对不同 `YGGR_MONGOC_HEX()` 值列出字段差异），或展开 `pro_*` 方法的实现细节，我可以继续读取实现文件并补充。
