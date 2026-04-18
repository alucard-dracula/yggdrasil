**c_mongo_client_encryption_datakey_opts.hpp — 文档说明**

概述
- **名称**: `c_mongo_client_encryption_datakey_opts`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_client_encryption_datakey_opts.hpp](yggr/nsql_database_system/c_mongo_client_encryption_datakey_opts.hpp)
- **用途**: 封装 libmongoc 的 `mongoc_client_encryption_datakey_opts_t` 选项结构，提供 C++ 风格的构造、赋值、访问器与缓冲/容器友好的设置方法（例如密钥别名列表、主密钥 BSON、密钥材料字节缓冲）。

主要类型和别名
- `base_type`: `yggr_mongoc_client_encryption_datakey_opts`（底层裸结构适配）
- `org_type`: `mongoc_client_encryption_datakey_opts_t`（原生类型）
- `inner_string_type`: `charset::utf8_string`，用于别名字符串保存
- `bson_type`: `c_bson`（封装的 BSON 类型）
- `bytes_buffer_type`: `charset::string`（用于二进制密钥材料缓冲）

构造与析构
- `c_mongo_client_encryption_datakey_opts()` — 默认构造，内部调用底层 init 函数。
- 模板构造函数 — 支持从容器或迭代器初始化 `keyaltnames`，并可同时设置 `masterkey`（任意派生自 `bson_t` 的类型）和 `keymaterial`（容器或迭代器）。
- `explicit c_mongo_client_encryption_datakey_opts(const org_type* porg)` / `(const org_type& org)` — 从原生指针或引用构造。
- 拷贝、移动构造与赋值均实现（使用 `pro_copy` / `pro_move` 等内部方法），并提供 `swap`。

关键成员与访问器
- `var_masterkey()` — 以 `c_bson` 为主要访问类型的成员访问器；也提供模板重载以获取/设置任意派生自 `bson_t` 的类型。
  - 内部同步底层字段 `base_type::masterkey` 到 `_masterkey`。
- `var_keyaltnames()` / `set_keyaltnames(...)` / `append_keyaltname(...)` / `clear_keyaltnames()` — 管理关键别名列表（以智能指针保存字符串存储，并维护底层 `char*` 数组）。
  - `set_keyaltnames` 支持容器或迭代器重载。
  - `var_keyaltnames_count()` 返回别名数量（同步 `base_type::keyaltnames_count`）。
- `var_keymaterial()` / `set_keymaterial(...)` / `var_keymaterial_size()` — 管理二进制密钥材料缓冲。
  - 支持原始指针/长度、迭代器区间、容器和移动/拷贝赋值多种接口。
  - 自动同步 `base_type::keymaterial` 与 `base_type::keymaterial_len`。

保护与内部方法（简要）
- `pro_rebuild_base()` — 在必要时重建或同步底层 `org_type` 字段（实现细节在源文件）。
- `pro_copy(const org_type* porg) / pro_copy(const this_type& right)` — 复制实现。
- `pro_swap(this_type& right) / pro_move(this_type& right)` — 交换 / 移动实现。
- 静态工具 `pro_s_append_keyaltname(...)`、`pro_s_copy_keyaltname(...)`、`pro_s_swap_keyaltname(...)` — 辅助实现别名列表的复制/追加/交换。

成员变量（概览）
- `_masterkey` — 类型 `bson_type`，包裹 BSON 主密钥。
- `_keyaltnames_datas` — 保存实际字符串对象的容器（智能指针向量）。
- `_keyaltnames` — 对应的 `char*` 视图数组，传给底层 C 结构使用。
- `_keymaterial` — 二进制密钥材料的缓冲（`bytes_buffer_type`）。

语义与注意事项
- 该类在内部维护 C 风格原生结构字段（`base_type::*`），并在修改时同步 `_masterkey`、`_keyaltnames`、`_keymaterial` 到 `base_type` 对应字段。
- 模板接口使用 SFINAE 限定：容器/迭代器/类型必须满足相应 trait（例如 `container_ex::is_container`、`iterator_ex::is_iterator`、`is_base_of<bson_t, ...>` 等）。
- `set_keymaterial` 的迭代器与容器重载区分对待：对非类元素的迭代器采用二进制赋值帮助器；对可拷贝容器采用另一重载。

依赖
- 本类型依赖/包含的头文件：
  - `c_bson.hpp`
  - `mongoc_client_encryption_datakey_opts_native_ex.hpp`（native init / 辅助函数）
  - `detail/binary_buffer_assign_helper.hpp`（二进制缓冲赋值辅助）
  - `yggr/utility/member_var_modify_helper.hpp`（成员修改器包装）

示例
```cpp
// 构造并设置 keyalt 名称列表
std::vector<std::string> names = {"alt1", "alt2"};
yggr::nsql_database_system::c_mongo_client_encryption_datakey_opts opts(names);

// 设置 masterkey（假设 c_bson 或与 bson_t 兼容的类型）
c_bson master;
// ... 填充 master ...
opts.var_masterkey() = master;

// 设置二进制 keymaterial
std::string keymat = "\x01\x02\x03";
opts.set_keymaterial(reinterpret_cast<const u8*>(keymat.data()), keymat.size());

// 也可以使用迭代器或容器重载来设置 keymaterial
```

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_client_encryption_datakey_opts.md](yggr/nsql_database_system/c_mongo_client_encryption_datakey_opts.md)

扩展建议
- 如需我可以进一步：
  - 展开每个模板重载的完整签名与 SFINAE 要求；
  - 添加更多使用场景的完整代码片段；
  - 读取并引用对应源文件实现以补充 `pro_*` 方法的行为说明。
