# c_bson_timeval.hpp 文档

**概述**:
- **说明**: `c_bson_timeval` 是对 POSIX `timeval` 的封装，提供与 BSON 日期/时间（以毫秒为单位）互操作、与 `boost::xtime` 的桥接、精度转换、比较、序列化与字符串格式化。
- **所在文件**: [yggr/nsql_database_system/c_bson_timeval.hpp](yggr/nsql_database_system/c_bson_timeval.hpp)

**命名空间**:
- `yggr::nsql_database_system`

**类: `c_bson_timeval`（继承自 `timeval`）**
- **类型别名**:
  - `base_type`, `org_type`: 基类 `timeval`。
  - `bridge_time_type`: `boost::xtime`，用于与 Boost xtime 的互转。
  - `base_sec_val_type`, `base_usec_val_type`: 平台相关的秒/微秒底层类型别名（在 Windows/Darwin/Linux 下可能不同）。
  - `tv_usec_modify_helper_type`: 用于安全修改 `tv_usec` 的辅助类型。

- **常量**:
  - 定义了一系列时间比率常量（秒↔毫秒↔微秒↔纳秒），例如 `E_S_AND_MS_RATIO`（1000）、`E_MS_AND_MCS_RATIO`（1000）等，用于精度换算。

- **构造与赋值**:
  - 默认构造、从秒/微秒构造、从 `bson_value_t` 构造。
  - 从 `c_bson_date`、`c_bson_time`、`boost::xtime` 构造（支持 time_cast）。
  - 支持来自 `timeval` 的拷/移动构造与赋值，支持模板赋值（SFINAE）以接受任意 `timeval` 派生类型。
  - `operator=(const bson_value_t&)` 使用 `pro_assign_bson_value_t` 解析 BSON value 并赋值。

- **访问器与互转**:
  - `org_pointer()` / `org_pointer() const`：返回指向底层 `timeval` 的指针。
  - `operator bridge_time_type()`：隐式转换到 `boost::xtime`（支持 time_cast）。
  - `time()`：返回秒（u64）。
  - `date()`：返回以毫秒表示的时间戳（u64），用于 BSON 存储（秒*1000 + usec/1000）。
  - `bridge_time()`：返回 `boost::xtime` 表示（sec/nsec）。
  - `var_tv_sec()` / `var_tv_usec()`：以安全方式读取/修改 `tv_sec`/`tv_usec`，`var_tv_usec()` 使用 `tv_usec_modify_helper_type` 提供修改接口并支持按毫秒/纳秒设置微秒字段。

- **比较函数**:
  - `compare_eq`：重载以支持原生秒/微秒、`bson_value_t`、`c_bson_date`、`c_bson_time`、`boost::xtime` 和任意 `timeval` 派生类型。
  - `compare`：返回 -1/0/1，支持与同类型及上述类型比较（内部使用 `bson_date_time_native_ex` 或 `bson_value_native_ex` 的比较函数）。

- **字符串与格式化**:
  - `to_string()` / `to_local_string()`：使用项目的日期/时间格式化宏与格式化器（宏封装的多重重载，返回 `utf8_string` 或自定义字符串类型）。
  - `to_tm()` / `to_local_tm()`：返回 `std::tm`（UTC 或本地时间）。

- **序列化 (BSON) 支持**:
  - `save(Bson& bs, const String& name)`：以 `bson_append_timeval` 将 `timeval` 追加到 `bson_t` 字段中（接受 UTF-8/其它编码的字段名 helper）。
  - `save_value(Bson& bs)`：将 `timeval` 转换为 BSON 日期值（以毫秒为单位）并保存为 `bson_value_t`。
  - `load(const BsonIter& iter, const String& name)` / `load_value(const BsonIter& iter)`：从 `bson_iter_t` 读取为 `timeval`（使用 `bson_iter_timeval`）。

- **流输出**:
  - 已实现 `operator<<`，在不同 BSON JSON 模式下输出：legacy/relaxed/canonical，eg. `{ "$date": "..." }`、`{ "$date": { "$numberLong" : "..." } }` 等。

- **互操作与非成员运算符**:
  - 提供与 `timeval` 派生类型、`c_bson_date`、`c_bson_time`、`boost::xtime` 以及 `bson_value_t` 的全套比较运算符重载（`==, !=, <, <=, >, >=`）。
  - 提供 `swap` 支持并将其放入 `yggr::nsql_database_system::swap_support`，同时引入 `std` 与 `boost` 的 `swap`。

**示例**:

```cpp
using yggr::nsql_database_system::c_bson_timeval;

c_bson_timeval tv(1609459200, 123456); // 2021-01-01T00:00:00 + 123456 usec
u64 ms = tv.date(); // 毫秒表示
std::cout << tv; // JSON 输出（legacy 模式）

bson_t bs;
bson_init(&bs);
if(tv.save(bs, std::string("created_at"))) {
    // 成功追加 field
}
```

**依赖**:
- Boost（xtime、type_traits、enable_if、move 支持等）。
- Mongo C 驱动类型与项目扩展：`bson_t`, `bson_value_t`, `bson_iter_t`，`bson_native_ex`、`bson_value_native_ex`、`bson_timeval_native_ex`、`bson_date_time_native_ex` 等。
- 项目时间/格式化工具与字符串 helper（`yggr::time`、`bson_date_time_format`、charset helpers）。

**位置**:
- 源文件: [yggr/nsql_database_system/c_bson_timeval.hpp](yggr/nsql_database_system/c_bson_timeval.hpp)

如需，我可以：
- 将文档按函数签名逐条展开（包含具体模板签名与返回类型）；
- 添加更多序列化/反序列化示例；
- 或继续为下一个文件生成文档。