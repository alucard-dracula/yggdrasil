# c_bson.hpp 文档

**概述**:
- **说明**: `c_bson` 是对 MongoDB C 驱动 `bson_t` 的高层封装，提供 JSON 解析/序列化、字段读写、迭代器、与各种 BSON 值类型的互操作、以及方便的保存/加载 API。该文件还包含 `c_bson_const_iterator`、`c_bson_access_proxy` 以及若干非成员辅助函数。
- **所在文件**: [yggr/nsql_database_system/c_bson.hpp](yggr/nsql_database_system/c_bson.hpp)

**命名空间**:
- `yggr::nsql_database_system`

**主要类型与职责**:
- `c_bson_access_proxy`:
  - 用作通用适配器，封装对 `bson_t`/`bson_iter_t` 的 `save`/`load`/`save_value`/`load_value`/`save_begin`/`load_begin` 等操作，允许模板化地把值保存到或从 BSON 读取。

- `c_bson_const_iterator`:
  - 继承自 `bson_iter_t`，实现作为只读迭代器的封装。
  - 支持构造、查找 (`find`、`find_descendant`)、键访问 (`get_key`/`get_key_pointer`)、类型检查与加载（`load`, `load_value`）、前进运算符 (`operator++`) 等。
  - 提供与 `bson_iter_t` 派生类型之间的比较、swap 与非成员运算符。

- `c_bson`:
  - 继承自 `bson_t`，提供：
    - 构造/赋值：从 JSON 字符串、`bson_value_t`、其它 `bson_t`、或移动语义初始化。
    - `assign` / `pro_assign_bson_value_t`：从 JSON 或 `bson_value_t` 赋值。
    - 清理与状态检测：`init`, `clear`, `clear_to_nil`, `is_nil`, `empty`, `is_array`, `size`, `count_key`。
    - 查找/访问：`find`, `get_value` (返回 `c_bson_value` 或模板化的目标类型)、`has_field`、子数组/子文档迭代器 (`sub_array_iterator`, `sub_document_iterator`)。
    - 迭代器支持：`begin`/`end`/`cbegin`/`cend` 返回 `c_bson_const_iterator`。
    - JSON 序列化：`to_json`, `to_json_canonical`, `to_json_relaxed` 及模板化版本，返回指定字符串类型。
    - 选择/过滤复制：`select(handler)` 可基于 handler 复制部分文档。
    - 保存 API：泛型 `save(name, val)` 与 `save_value(val)` 的大量重载，支持原生整数、浮点、字符串、布尔、`bson_t`、`bson_iter_t`、复杂对象（通过 `bson_inner_data_access_proxy`）以及数组/文档起止 (`save_array_start`/`save_array_end`, `save_document_start`/`save_document_end`)。
    - 广泛使用 `bson_native_ex` / `bson_value_native_ex` 项目辅助函数来处理底层 bson 操作。

- 非成员/全局支持函数：
  - `operator+`, `operator+=`, `operator==`, `operator!=` 在 `bson_t`/`c_bson` 与 `bson_value_t`/其他 `bson_t` 间提供方便的组合与比较。
  - `swap` 支持已导出到 `yggr::nsql_database_system::swap_support` 并引入 `std` 与 `boost`。

**重要接口说明（精简）**:
- JSON 与 BSON 转换
  - `c_bson::assign(const String& json)`：从 JSON 字符串解析填充 BSON。
  - `c_bson::to_json()` / `to_json_canonical()` / `to_json_relaxed()`：序列化为 JSON（不同模式）。

- 字段读写
  - `save(const String& name, T val)`：将各种 T 类型追加为字段（大量重载/模板）。
  - `save_value(T val)`：将值保存为独立 BSON 值（用于内部 value 存储）。
  - `load(const String& name, T& val)`：按字段名载入到目标类型（存在众多重载，按类型安全检查）。
  - `get_value(const String& key)`：返回 `c_bson_value` 或模板化目标类型。

- 迭代与查找
  - `begin()` / `end()` 返回 `c_bson_const_iterator`；`iterator.find(key)` 用于定位字段；`find_descendant` 支持以点分层路径查找。

- 类型/状态
  - `is_nil()`, `empty()`, `is_array()`, `size()`, `count_key()` 等简洁状态查询。

**示例**:

```cpp
using yggr::nsql_database_system::c_bson;
using yggr::nsql_database_system::c_bson_value;

c_bson doc;
doc.save(std::string("name"), std::string("Alice"));
doc.save(std::string("age"), 30);

std::string json = doc.to_json();
std::cout << json << std::endl;

auto val = doc.get_value<std::pair<int,bool>>(std::string("age"));
if(val.second) { int age = val.first; }

// 遍历
for(auto it = doc.begin(); it != doc.end(); ++it) {
    std::cout << it.get_key() << std::endl;
}
```

**依赖**:
- Boost（type_traits, enable_if, mpl, move 等）。
- Mongo C 驱动（`bson_t`, `bson_iter_t`, `bson_value_t` 等）。
- 项目内部扩展：`bson_native_ex`, `bson_value_native_ex`, `bson_inner_data_access_proxy`, 以及各 `c_bson_*` 类型（`c_bson_value`, `c_bson_date`, `c_bson_time` 等）。

**位置**:
- 源文件: [yggr/nsql_database_system/c_bson.hpp](yggr/nsql_database_system/c_bson.hpp)

需要我将文档扩展为：
- 按函数签名的完整 API 列表（包含模板签名与返回类型），或
- 添加更多示例（如复杂对象序列化/反序列化、迭代器用法、性能注意点）。
