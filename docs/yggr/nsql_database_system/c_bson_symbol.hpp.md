# c_bson_symbol.hpp 文档

## 概述

`c_bson_symbol.hpp` 是 Yggdrasil 项目中的 C++ 头文件，提供 BSON `Symbol` 类型的 C++ 封装 `c_bson_symbol`。该类型基于 `c_bson_string_item`，用于表示 BSON 中的 `symbol` 字符串类型，包含字符串存取、比较、序列化/反序列化、流输出等功能。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`
- `yggr::nsql_database_system::detail`
- `yggr::nsql_database_system::swap_support`

## 类型 ID 注册

```cpp
YGGR_PP_BSON_TYPEID_IMPL(c_bson_symbol, bson_typeid_def::E_BSON_TYPE_SYMBOL)
```

## 类 `c_bson_symbol` 概要

- 继承自 `c_bson_string_item`，内部使用 `utf8_string` 存储字符串
- 提供多种构造方式：C 字符串、宽字符、char8/16/32、字符串视图、容器、移动/复制构造，以及从 `bson_value_t` 构造
- 支持赋值、移动、交换、比较、保存/加载及流输出

## 主要方法与行为

### 构造函数
- `c_bson_symbol()` 默认构造
- `c_bson_symbol(const bson_value_t& val)` 从 BSON 值构造
- `c_bson_symbol(const char* symbol_str)` 和多种字符类型的重载构造
- 模板构造支持 `Basic_String` / `BasicStringView` / `charset::string_charset_helper` 等类型
- 支持 `BOOST_RV_REF(string_type)` 与拷贝构造

### 赋值运算符
- `operator=(const BsonValue&)` 从 BSON 值赋值
- `operator=(const Char* )`、`operator=(const String&)` 等支持多种字符/字符串类型
- 移动与复制赋值支持

### 类型 ID
- `value_typeid()` / `s_value_typeid()` 返回 `bson_typeid_def::E_BSON_TYPE_SYMBOL`

### 比较
- `compare_eq(const BsonValue& val)` 使用 `bson_value_native_ex::s_value_cmp_eq_symbol` 比较 BSON 值
- `compare_eq(const c_bson_symbol& right)` 复用基类比较（字符串相等）

### 序列化
- `save(Bson& bs, const String& name)` 将 symbol 字段追加到 `bson_t`（调用 `bson_append_symbol`）
- `save_value(Bson& bs)` 将 symbol 作为 BSON 值保存（调用 `s_bson_save_value_of_symbol`）
- `load(BsonIter& iter, const String& name)` / `load_value(const BsonIter& iter)` 从迭代器读取 symbol 数据并写入内部字符串

### 访问器
- `string_type& symbol()` 返回对内部字符串的引用
- 通过宏 `YGGR_PP_C_MONGO_STRING_MEMBER_GET_*` 提供了多种访问重载（支持参数化构造/访问）

### 流输出（JSON）
- 使用 `detail::bson_operator_outter<c_bson_symbol>` 实现 JSON 输出
- 在 canonical/relaxed 模式下输出：
  ```json
  { "$symbol" : "..." }
  ```
- 在 legacy 模式下输出字符串本身（带转义）

## 非成员运算符
- `operator==` / `operator!=` 与 `bson_value_t` 派生类型互操作

## 交换支持
- `YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_bson_symbol)` 宏提供 swap 实现
- 在 `swap_support` 命名空间导出 `swap` 并在 `std` 与 `boost` 中引入

## 依赖项
- `<yggr/charset/utf8_string.hpp>`
- `<yggr/nsql_database_system/c_bson_string_item.hpp>`
- `<yggr/nsql_database_system/bson_native_ex.hpp>`
- `<yggr/nsql_database_system/bson_value_native_ex.hpp>`
- `<yggr/nsql_database_system/json_escape_string.hpp>`
- `<yggr/nsql_database_system/bson_visit_t.hpp>`
- Boost 类型特征与 MPL

## 使用示例

```cpp
#include <yggr/nsql_database_system/c_bson_symbol.hpp>

using yggr::nsql_database_system::c_bson_symbol;

c_bson_symbol s1("mysymbol");

// 保存到 BSON
bson_t b; bson_init(&b);
s1.save(b, "sym");

// 从迭代器加载
bson_iter_t iter; // assume points to a symbol
c_bson_symbol s2;
s2.load(iter, "sym");

// 比较
if(s1 == s2) { /* ... */ }

// 输出 JSON
std::cout << s1 << std::endl;
```

## 保存路径

`e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\c_bson_symbol.md`
