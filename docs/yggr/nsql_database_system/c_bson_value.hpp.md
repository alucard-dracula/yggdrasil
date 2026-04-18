# c_bson_value.hpp 文档

**概述**:
- **说明**: `c_bson_value` 是对 `bson_value_t` 的完整封装，支持构造/赋值于 BSON 各种原始类型与复杂类型（字符串、二进制、对象/数组、时间、OID、正则、代码等），提供类型转换、模板化的 `get_value`/`assign`、序列化、比较与 JSON 风格输出。
- **所在文件**: [yggr/nsql_database_system/c_bson_value.hpp](yggr/nsql_database_system/c_bson_value.hpp)

**命名空间**:
- `yggr::nsql_database_system`

**类: `c_bson_value`**
- 继承自 `bson_value_t`，并将其作为 `base_type` / `org_type`。

- **构造与赋值**:
  - 支持从 C++ 原生类型构造: `s32, s64, bool, f32, f64`。
  - 支持从多种字符串类型构造（`const char*`、`wchar_t*`、`utf8_string_view`、各种 `BasicString`）、以及项目内字符串辅助类型。
  - 支持从 BSON 专用类型构造/赋值：`c_bson_null`, `c_bson_undefined`, `c_bson_maxkey`, `c_bson_minkey`, `c_bson_date`, `c_bson_time`, `timeval`/`c_bson_timeval`, `bson_oid_t`/`c_bson_oid`, `bson_decimal128_t`/`c_bson_decimal128`、二进制缓冲、正则、代码、带作用域的代码、timestamp、dbpointer、`bson_t`（document/array）等。
  - 支持从任意 `bson_value_t`（或其派生）以及 `bson_t` 的移动与拷贝（包含移动语义替换底层内存表示）。
  - 提供模板化 `assign(...)` 与 `pro_init()` 内部辅助函数（封装了具体类型的赋值逻辑）。

- **类型与标识**:
  - `value_typeid()` 返回当前实例的 BSON 类型值（读取 `base_type::value_type`）。
  - `s_value_typeid()` 静态返回 `E_BSON_TYPE_ANY_VALUE`（泛型标识）。

- **访问器与类型转换**:
  - `org_pointer()` 返回底层 `bson_value_t*`。
  - 提供大量 `operator std::pair<T, bool>() const` 隐式转换：用于尝试将 `c_bson_value` 解析为某个目标类型并返回值与是否成功的标志。支持整数、浮点、字符串视图、自定义字符串类型、`c_bson_null` / `c_bson_undefined` / `c_bson_maxkey` / `c_bson_minkey`、日期/时间类型、OID、binary、regex、code、symbol、codewscope、timestamp、dbpointer、document/array 等多种类型。

- **比较与交换**:
  - 内部实现 `pro_swap` 与模板化 `swap`，并将 `swap` 引入 `yggr::nsql_database_system::swap_support`，并导入 `std`/`boost`。
  - 与 `bson_value_t` 互操作时使用 `bson_value_native_ex` 的比较函数来实现 `compare_eq`/`compare`（文件中存在相应的模板与非成员比较运算符重载）。

- **序列化（save/load）**:
  - 类中实现了模板化的 `save(Bson& bs, const String& name)` 与 `save_value(Bson& bs)`，可把 `c_bson_value` 作为文档字段或独立值追加到 `bson_t`/`bson_value_t`，并支持 UTF-8/其它编码的字段名 helper。
  - `load` / `load_value` 通过 `bson_iter_t` 读取并填充 `bson_value_t`，支持不同编码的字段名模板重载。

- **字符串化与 JSON 输出**:
  - 通过项目的 `bson_date_time_format` 与 `YGGR_PP` 宏为内含日期/时间类型提供 `to_string()`/`to_local_string()` 格式化支持。
  - 提供 `operator<<` 输出 JSON 风格表示（对日期/时间使用 `$date`、对 timestamp 使用 `$timestamp`、对 undefined 使用 `$undefined` 等，根据 `bson_json_mode_def` 不同模式输出 canonical/relaxed/legacy 形式）。

- **异常与断言**:
  - 多处使用 `assert` 检查预期的 BSON 类型与操作成功（例如解析 `bson_value_t` 时的检查）。

**示例**:

```cpp
using yggr::nsql_database_system::c_bson_value;

c_bson_value v1(42);                     // int32
c_bson_value v2(std::string("hello")); // utf8 string

// 尝试解析为 int32
auto [i, ok] = v1.operator std::pair<s32,bool>();
if(ok) { /* i 为值 */ }

// 保存到 bson_t
bson_t bs;
bson_init(&bs);
v2.save(bs, std::string("greeting"));

// 使用 JSON 输出
std::cout << v2;
```

**依赖**:
- Boost（enable_if、mpl、move、type_traits 等）。
- Mongo C 驱动类型与项目扩展：`bson_t`, `bson_value_t`, `bson_iter_t`，以及 `bson_native_ex`、`bson_value_native_ex` 等辅助。
- 项目内其他类型：`c_bson_null`/`c_bson_undefined`/`c_bson_date`/`c_bson_time`/`c_bson_timeval`/`c_bson_oid`/`c_bson_binary_buffer`/`c_bson_regex`/`c_bson_code`/`c_bson_symbol`/`c_bson_code_w_scope`/`c_bson_timestamp`/`c_bson_dbpointer`/`c_bson_decimal128` 等。

**位置**:
- 源文件: [yggr/nsql_database_system/c_bson_value.hpp](yggr/nsql_database_system/c_bson_value.hpp)

如果你需要，我可以：
- 将文档扩展为按函数签名逐条列出的完整 API（含模板签名与返回类型）；
- 添加更多基于项目的序列化/反序列化示例；
- 或为另一个文件继续生成文档。