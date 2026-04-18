# c_bson_null.hpp 文档

## 概述

`c_bson_null.hpp` 是 Yggdrasil 项目中的 C++ 头文件，封装了 BSON 中的 `Null` 类型为 C++ 类型 `c_bson_null`。该类型表示 BSON 的 `null` 值，提供类型标识、序列化/反序列化接口、比较运算符、流输出以及交换支持。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`
- `yggr::nsql_database_system::detail`
- `yggr::nsql_database_system::swap_support`

## 主要功能

- 提供 `c_bson_null` 类，表示 BSON `null` 值
- 注册 BSON 类型 ID: `bson_typeid_def::E_BSON_TYPE_NULL`
- 支持从 `bson_value_t` 构造和赋值（包含类型校验）
- 保存到 BSON (`save` / `save_value`) 与加载接口（`load` / `load_value`，但对 Null 无需实际载入数据）
- 运算符重载：`==` / `!=` 与任意 `bson_value_t` 派生类型的互操作性
- JSON 风格的流输出：`null`
- 无状态、轻量（所有 Null 实例相互等价）

## 类型 ID 注册

```cpp
YGGR_PP_BSON_TYPEID_IMPL(c_bson_null, bson_typeid_def::E_BSON_TYPE_NULL)
```

将 `c_bson_null` 注册为 BSON Null 类型，便于框架内的类型分发和序列化。

## 类 `c_bson_null` 概要

- 无数据成员（无状态类型）
- 支持默认、拷贝、移动构造
- 支持从 `bson_value_t` 构造和赋值（会进行类型校验 s_value_to_null）
- `value_typeid()` / `s_value_typeid()` 返回类型 ID
- `compare_eq()` 与其它 BSON 值比较是否为 Null（对另一个 Null 始终返回 true）
- `save()` / `save_value()` 将 Null 保存到 `bson_t`
- `load()` / `load_value()` 不需要从迭代器中载入具体数据（函数体为空或断言）

## 关键方法和行为

- 构造与赋值
  - `c_bson_null()` 默认构造
  - `c_bson_null(const bson_value_t& val)` 从 BSON 值构造（内部调用 `pro_assign_bson_value_t`）
  - 模板赋值运算符支持任意继承自 `bson_value_t` 的类型

- 类型 ID
  - `static u32 value_typeid()` 返回 `bson_typeid_def::E_BSON_TYPE_NULL`
  - `static u32 s_value_typeid()` 同上

- 比较
  - `compare_eq(const BsonValue& val)` 使用 `bson_value_native_ex::s_value_cmp_eq_null` 校验
  - `compare_eq(const c_bson_null&)` 返回 `true`（所有 Null 相等）

- 序列化
  - `save(Bson& bs, const String& name)` 将字段名以 UTF-8 格式写入 BSON 并 append null
  - `save_value(Bson& bs)` 将 Null 值写入 BSON（调用 `s_bson_save_value_of_null`）
  - `load(...)` / `load_value(...)` 无需从迭代器中载入具体数据（函数体为空或断言）

- 流输出
  - `operator<<` 输出 `null`，通过 `bson_operator_outter<c_bson_null>` 实现

- 交换
  - `swap(c_bson_null&, c_bson_null&)` 为空实现（无状态）

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/ppex/friend.hpp>`
- `<yggr/charset/utf8_string.hpp>`
- `<yggr/nsql_database_system/bson_typeid.hpp>`
- `<yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>`
- `<yggr/nsql_database_system/bson_native_ex.hpp>`
- `<yggr/nsql_database_system/bson_value_native_ex.hpp>`
- `<yggr/nsql_database_system/bson_visit_t.hpp>`
- `<boost/type_traits/is_base_of.hpp>`
- `<cassert>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/c_bson_null.hpp>

using yggr::nsql_database_system::c_bson_null;

// 默认构造
c_bson_null n1;

// 从 bson_value_t 构造（需保证 value 表示 null）
bson_value_t v; // 假设已初始化为 NULL 类型
c_bson_null n2(v);

// 比较
bool eq = (n1 == n2); // true

// 保存到 bson
bson_t b;
bson_init(&b);
n1.save(b, "field");

// 流输出
std::cout << n1 << std::endl; // null

// 赋值操作
n2 = n1; // 复制赋值
```

## Null 的特性

1. **排序特性**: Null 在排序中小于大多数值（语义依赖于数据库实现）
2. **相等性**: 所有 Null 值在本类型语义下视为等价
3. **无状态**: Null 没有实际的数据内容，是一个标记值

## 设计考虑

- Null 为无状态标记值，类实现尽可能轻量且零开销
- 通过模板与 traits 保证仅对正确类型启用构造/赋值
- 序列化接口与项目的 BSON 本地扩展保持一致
- 所有 Null 实例相互等价，比较与交换实现简单

## BSON JSON 表示

在 BSON 的 JSON 表示中 Null 对应：

```json
null
```

## 兼容性

- 支持 C++11/14/17/20
- 依赖 Boost 类型特征
- 与 MongoDB BSON 规范及项目内 BSON 扩展接口兼容

## 相关类型

- `bson_value_t`: BSON 值基础类型
- `c_bson_minkey` / `c_bson_maxkey`: 对应的边界特殊值类型
- `bson_native_ex` / `bson_value_native_ex`: 本地扩展，用于类型检查和保存/加载操作
