# c_bson_timestamp.hpp 文档

**概述**:
- **说明**: `c_bson_timestamp` 是对 BSON `timestamp`（含 `t`/`i` 两部分）结构的封装与增强，提供构造/赋值、比较、序列化与友好的输出（JSON 表示）。
- **所在文件**: [yggr/nsql_database_system/c_bson_timestamp.hpp](yggr/nsql_database_system/c_bson_timestamp.hpp)

**命名空间**:
- `yggr::nsql_database_system`

**结构: `bson_timestamp_t`**
- 定义: 一个 union 包含 `struct { u32 t; u32 i; }` 与 `u64 tms`，用于将两个 32 位域视作一个 64 位整型访问。
- 字段:
  - `t`: 时间戳（秒，u32）
  - `i`: 增量（increment，u32）
  - `tms`: 二者合并的 64 位表示

**类: `c_bson_timestamp`**
- 继承自 `bson_timestamp_t`，并提供：
  - 构造函数：默认构造、`c_bson_timestamp(u32 time, u32 inc)`、从 `bson_value_t` 构造、从 `bson_timestamp_t` 或者拷/移动构造。
  - 赋值与移动支持：支持来自 `bson_value_t` 的赋值模板，以及来自 `bson_timestamp_t`/`c_bson_timestamp` 的拷贝/移动赋值。
  - `grab()`：递增 `i` 并返回自身（用于生成新 increment）。
  - 访问器：`time()`、`inc()`、`timestamp()`（返回 `tms`）及对应的非常量/常量重载。
  - `compare_eq(...)`：与 `bson_value_t` 或任意 `bson_timestamp_t` 可比较（使用内部 `bson_value_native_ex::s_value_cmp_eq_timestamp` 以比较 BSON value）。
  - 静态方法：`value_typeid()` 与 `s_value_typeid()` 返回 BSON 类型 ID（`E_BSON_TYPE_TIMESTAMP`）。
  - 序列化：
    - `save(Bson& bs, const String& name)`：将 timestamp 以字段名追加到 `bson_t` 中（使用 `bson_append_timestamp`）。
    - `save_value(Bson& bs)`：将 timestamp 以 value 形式保存到 `bson_value_t`（检查 `sizeof(base_type) == sizeof(u64)`）。
    - `load(const BsonIter& iter, const String& name)`、`load_value(const BsonIter& iter)`：从 `bson_iter_t` 读取 `t` 与 `i` 并赋值。

- 保护成员：`pro_assign_bson_value_t` 用于从 `bson_value_t` 解析并赋值（对 `operator=` 调用）。

**输出与 JSON 表示**:
- 为 `bson_timestamp_t` 与 `c_bson_timestamp` 提供了 `operator<<` 实现，JSON 形式为：
  - `{ "$timestamp" : { "t" : <t>, "i" : <i> } }`

**非成员运算符/互操作**:
- 提供与 `bson_timestamp_t`、任意 `bson_timestamp_t` 派生类型和 `bson_value_t` 的 `==` / `!=` 比较重载，方便在容器或 BSON 值间直接比较。
- 提供 `swap` 支持（全局函数模板与特化），并将其放入 `yggr::nsql_database_system::swap_support`、并引入 `std`/`boost` 命名空间。

**示例**:

```cpp
using yggr::nsql_database_system::c_bson_timestamp;

c_bson_timestamp ts(1609459200, 1); // t=1609459200, i=1
std::cout << ts; // 输出 JSON 格式: { "$timestamp" : { "t" : 1609459200, "i" : 1 } }

bson_t bs;
bson_init(&bs);
if(ts.save(bs, std::string("ts_field"))) {
    // 成功追加
}
```

**依赖**:
- Boost（type_traits, enable_if, mpl, move 支持等）。
- Mongo C 驱动类型：`bson_t`, `bson_value_t`, `bson_iter_t` 等，以及项目 `bson_native_ex`、`bson_value_native_ex` 辅助。

**位置**:
- 源文件: [yggr/nsql_database_system/c_bson_timestamp.hpp](yggr/nsql_database_system/c_bson_timestamp.hpp)

需要我将文档扩展为更详细的按函数签名列表或补充更多使用场景示例吗？