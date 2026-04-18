# c_bson_time.hpp 文档

**概述**:
- **说明**: `c_bson_time` 是对 BSON 日期/时间（以秒为基础）的封装类型，提供与 `timeval`、`boost::xtime`、`std::tm` 等时间类型的互转、比较、序列化与字符串格式化。
- **所在文件**: [yggr/nsql_database_system/c_bson_time.hpp](yggr/nsql_database_system/c_bson_time.hpp)

**命名空间**:
- `yggr::nsql_database_system`

**类: `c_bson_time`**
- **类型别名**:
  - `base_type`: 基本存储类型，签名整型（s64），表示秒。
  - `bridge_time_type`: 与 Boost xtime 的桥接类型（`boost::xtime`）。

- **常量**:
  - `E_S_AND_MS_RATIO`, `E_MS_AND_MCS_RATIO`, `E_S_AND_USECS_RATIO` 等：用于秒、毫秒、微秒之间的比率常量，便于在 BSON（以毫秒为单位）与内部秒值之间转换。

- **构造与赋值**:
  - 默认构造、从 `base_type`（秒）构造。
  - 从 `bson_value_t`、`timeval`、`boost::xtime`、`c_bson_date` 构造或赋值的重载，支持 BSON 原生值的解析。
  - 支持 `swap`、赋值运算，以及 `pro_assign_bson_value_t` 之类的辅助函数以便从 BSON 值赋值。

- **访问器与转换**:
  - `time()`: 返回内部秒值（`base_type`）。
  - `date()`: 返回以毫秒表示的时间戳（u64），用于 BSON 存储。
  - `bridge_time()`: 返回 `boost::xtime` 表示（桥接类型）。
  - `operator base_type()`, `operator bridge_time_type()`: 隐式/显式转换支持。

- **比较函数**:
  - `compare(...)`, `compare_eq(...)`: 支持与 `base_type`、`timeval`、`boost::xtime`、`c_bson_date`、`bson_value_t` 等类型的比较。
  - 文件中还定义了大量非成员比较运算符（`==, !=, <, <=, >, >=`）以便与上述类型直接比较。

- **字符串与格式化**:
  - `to_string(...)`, `to_local_string(...)`：使用项目的日期/时间格式化宏与格式化器将时间转换为字符串，支持多种字符集/格式化模板（通过宏封装实现）。
  - `to_tm()` / `to_local_tm()`：返回 `std::tm`（UTC 或本地时间）。

- **序列化 (BSON) 支持**:
  - `save(Bson& bs, const String& name)`: 将该时间以 BSON 的 `time_t` 字段追加到给定 `bson_t`（或派生类型）中，接受字符串字段名（支持 UTF-8/其它编码的 helper）。
  - `save_value(Bson& bs)`: 将值以 BSON value 的日期(毫秒)形式保存到 `bson_value_t`。
  - `load(const BsonIter& iter, const String& name)`: 从 `bson_iter_t` 迭代器按字段名加载时间。
  - `load_value(const BsonIter& iter)`: 从迭代器加载作为值的日期时间（毫秒->秒转换由内部读取函数处理）。

- **流输出**:
  - 已实现 `operator<<`，在不同 BSON JSON 模式下输出不同的 JSON 表示（legacy/relaxed/canonical），例如 `{ "$date" : "..." }` 或 `{ "$date": { "$numberLong": "..." } }`。

- **互操作**:
  - 与 `c_bson_date`、`timeval`、`boost::xtime`、`bson_value_t` 之间提供全套比较运算符与互转支持。

**示例**:

- 构造与比较:

```cpp
using yggr::nsql_database_system::c_bson_time;

c_bson_time t1(1609459200); // 2021-01-01T00:00:00 UTC (秒)
boost::xtime xt = t1.bridge_time();
if(t1 == 1609459200) { /* ... */ }
```

- 保存到 BSON:

```cpp
bson_t bs;
bson_init(&bs);
c_bson_time t(1609459200);
// 将字段名 "created_at" 以 BSON date 保存
bool ok = t.save(bs, std::string("created_at"));
```

- 输出为 JSON (流):

```cpp
std::cout << t; // 使用默认 legacy 模式输出
```

**依赖**:
- Boost（xtime、enable_if、mpl、type_traits 等）。
- Mongo C 驱动数据类型（`bson_t`, `bson_value_t`, `bson_iter_t` 等）以及项目内的 `bson_native_ex` 辅助。
- 项目时间/格式化工具：`yggr::time` 与 `bson_date_time_format` 宏/格式。

**位置**:
- 源文件: [yggr/nsql_database_system/c_bson_time.hpp](yggr/nsql_database_system/c_bson_time.hpp)

若需，我可以进一步：
- 增补更完整的 API 列表（按函数签名逐条列出）。
- 添加更多使用示例或基于项目的典型序列化/反序列化示例。
