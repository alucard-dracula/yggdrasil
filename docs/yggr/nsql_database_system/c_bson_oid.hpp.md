# c_bson_oid.hpp 文档

## 概述

`c_bson_oid.hpp` 是 Yggdrasil 项目中用于封装 MongoDB BSON ObjectId 的 C++ 头文件。它定义了 `c_bson_oid` 类，基于 `bson_oid_t`，提供字符串/二进制互转、比较、序列化、赋值与交换等便捷操作，同时与项目的 BSON 本地扩展接口互通。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`
- `yggr::nsql_database_system::detail`
- `yggr::nsql_database_system::swap_support`

## 文件描述

`c_bson_oid` 是对 `bson_oid_t` 的 C++ 包装，保持与底层 C 结构的内存兼容（static_assert 检查）。类提供多种构造/赋值接口以支持字符串、字节数组、容器、迭代器、以及来自 BSON 值的初始化。它还实现了广泛的比较运算符重载（支持与字符串、容器、`bson_value_t` 与其它 OID 类型的比较）、JSON 风格流输出、保存/加载到 `bson_t`、以及交换操作。

## 常量

- `E_bson_oid_byte_size`: OID 的字节数（12）
- `E_bson_oid_string_size`: OID 字符串长度（24）
- `E_bson_oid_char_buffer_size`: 字符缓冲区大小（25，含终止符）

这些常量使用 `bson_oid_native_ex` 定义的值。

## 类型别名

- `id_type`, `base_type`, `org_type`：均指向 `bson_oid_t`

## 构造函数

- `explicit c_bson_oid(bson_context_t* pcontext = 0)`：默认/上下文构造
- `explicit c_bson_oid(const Char* str_oid)`：从字符串构造（支持原生字符类型）
- `explicit c_bson_oid(const char* str_oid, std::size_t len)`：从 C 字符串和长度构造
- 支持从任意字符串类型、字符串视图、charset helper、字节数组、容器、迭代器构造
- `c_bson_oid(const bson_value_t& val)`：从 BSON 值构造
- 支持从 `bson_oid_t` 基类复制/移动构造

## 赋值运算符

支持多种形式的赋值：
- 字符串（UTF-8 等）
- 原始字节指针
- 容器
- BSON 值
- 其它 `bson_oid_t` 派生类型
- 移动/复制赋值

赋值函数内部调用 `assign(...)`，并根据输入长度或类型选择合适的初始化（例如 `bson_oid_init_from_string` 或 `bson_oid_init_from_data`），不匹配时初始化为零 OID。

## 转换与字符串获取

- `utf8_string to_string(const string& ret_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME()) const`：返回 OID 的 UTF-8 字符串表现
- 支持隐式转换到各种 `Basic_String<Char,Traits,Alloc>`，返回对应字符集的字符串
- `operator bool()`：非空 OID 返回 true
- `empty()`：检查是否为零 OID（与 `s_gen_empty_oid()` 比较）

## 比较 API

提供 `compare_eq` 与 `compare` 系列重载，支持：
- 与 C 字符串/字符串类型比较
- 与字节指针/容器比较
- 与迭代器范围比较
- 与 `bson_value_t` 比较（通过 `bson_value_native_ex`）
- 与其它 `bson_oid_t` 派生类型比较

这些底层比较由 `bson_oid_native_ex` / `bson_value_native_ex` 提供实际实现。

`compare` 返回 `s32` (-1/0/1)；`compare_eq` 返回布尔。

## 生成与序列化

- `static const this_type& s_gen_empty_oid()`：返回静态零 OID
- `static this_type s_gen_oid()`：生成并返回新 OID（调用默认构造）

- `save(Bson& bs, const String& name) const`：将 OID 作为字段保存到 `bson_t`，对字符串名进行 UTF-8 转换后调用 `bson_append_oid`
- `save_value(Bson& bs) const`：将 OID 作为 BSON 值保存，调用 `s_bson_save_value_of_oid`
- `load(const BsonIter& iter, const String& name)`：从 BSON 迭代器读取 OID 并复制数据
- `load_value(const BsonIter& iter)`：从迭代器读取值并复制到自身

## 非成员运算符重载

实现了广泛的比较运算符重载，支持与多种类型的双向比较：
- `==`, `!=`, `<`, `<=`, `>`, `>=`
- 参数类型包括：C 字符串、`Basic_String`、字节指针、容器、`bson_value_t`、`bson_oid_t` 派生类型等

## 流输出

通过 `detail::bson_operator_outter<c_bson_oid>`，将 `c_bson_oid` 转发到底层 `bson_oid_t` 的输出实现，支持：

```cpp
std::cout << c_bson_oid_instance;
```
输出为 BSON JSON 表示的 OID（由底层 `bson_oid_t` 的输出控制）。

## 交换支持

提供 `swap` overloads：
- 适配 GCC 早期版本的特殊实现
- 通用模板 `swap(c_bson_oid&, BsonOid&)` 调用成员 `swap`
- `swap(c_bson_oid&, c_bson_oid&)` 调用成员 `swap`
- 在 `std` 与 `boost` 命名空间中导入 `swap`

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/charset/utf8_string.hpp>`
- `<yggr/move/move.hpp>`
- `<yggr/ppex/friend.hpp>`
- `<yggr/type_traits/is_native_char.hpp>`
- `<yggr/mplex/tag_sfinae.hpp>`
- `<yggr/nsql_database_system/bson_typeid.hpp>`
- `<yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>`
- `<yggr/nsql_database_system/bson_value_native_ex.hpp>`
- `<yggr/nsql_database_system/bson_visit_t.hpp>`
- `<yggr/nsql_database_system/detail/string_member_var_get.hpp>`
- Boost 类型特征与 MPL
- C 标准库 `<cassert>`, `<memory>`, `<cstring>` 等

## 示例

```cpp
#include <yggr/nsql_database_system/c_bson_oid.hpp>

using yggr::nsql_database_system::c_bson_oid;

// 从字符串构造
c_bson_oid id1("5f1b5a3e8d1e8a7f1c2a3b4c");

// 从字节数组构造
uint8_t raw[12] = { /* 12 bytes */ };
c_bson_oid id2(raw);

// 复制 / 赋值
c_bson_oid id3 = id1;
id2 = id1;

// 转为字符串
auto s = id1.to_string();

// 比较
if(id1 == id3) { /* equal */ }
if(id1 < "6a...") { /* compare */ }

// 序列化到 bson
bson_t b;
bson_init(&b);
id1.save(b, "_id");

// 从迭代器加载
bson_iter_t iter;
// assume iter points to an oid field
c_bson_oid id4;
id4.load_value(iter);

// 输出
std::cout << id1 << std::endl;
```

## 设计考虑

- 提供宽泛的构造/赋值接口以兼容项目内各种字符串/容器类型
- 通过 `enable_if` 与 traits 保证仅对合适类型启用重载
- 保持与底层 C 类型的二进制兼容性
- 把实际比较/转换实现委托给 `bson_oid_native_ex` / `bson_value_native_ex`，使接口简洁且可测试

## 保存路径

`e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\c_bson_oid.md`
