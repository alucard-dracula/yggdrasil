# c_bson_minkey.hpp 文档

## 概述

`c_bson_minkey.hpp` 是 Yggdrasil 项目中的 C++ 头文件，为 BSON 的 MinKey 类型提供了一个 C++ 封装类 `c_bson_minkey`。MinKey 是 MongoDB BSON 中的一个特殊标记值，在排序时总是小于任何其他值（除了其他 MinKey）。该文件实现了类型标识、序列化/反序列化接口、比较运算符、流输出以及交换支持。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`
- `yggr::nsql_database_system::detail`
- `yggr::nsql_database_system::swap_support`

## 主要功能

- 提供 `c_bson_minkey` 类，表示 BSON MinKey 值
- 注册 BSON 类型 ID: `bson_typeid_def::E_BSON_TYPE_MINKEY`
- 支持从 `bson_value_t` 构造和赋值（包含类型校验）
- 提供保存到 BSON (`save` / `save_value`) 与加载接口（`load` / `load_value`，但对 MinKey 无需实际载入数据）
- 运算符重载：`==` / `!=` 与任意 `bson_value_t` 派生类型的互操作性
- JSON 风格的流输出：`{ "$minKey" : 1 }`
- 无状态、轻量（所有 MinKey 实例相互等价）

## 类型 ID 注册

```cpp
YGGR_PP_BSON_TYPEID_IMPL(c_bson_minkey, bson_typeid_def::E_BSON_TYPE_MINKEY)
```

将 `c_bson_minkey` 注册为 BSON MinKey 类型，便于框架内的类型分发和序列化。

## 类 `c_bson_minkey` 概要

- 无数据成员（无状态类型）
- 支持默认、拷贝、移动构造
- 支持从 `bson_value_t` 构造和赋值（会进行 `s_value_to_minkey` 校验）
- `value_typeid()` / `s_value_typeid()` 返回类型 ID
- `compare_eq()` 与其它 BSON 值比较是否为 MinKey（对另一个 MinKey 始终返回 true）
- `save()` / `save_value()` 将 MinKey 保存到 `bson_t`
- `load()` / `load_value()` 不需要载入实际数据（空实现或断言检查）

## 关键方法和行为

- 构造与赋值
  - `c_bson_minkey()` 默认构造
  - `c_bson_minkey(const bson_value_t& val)` 从 BSON 值构造（内部调用 `pro_assign_bson_value_t`）
  - 模板赋值运算符支持任意继承自 `bson_value_t` 的类型

- 类型 ID
  - `static u32 value_typeid()` 返回 `bson_typeid_def::E_BSON_TYPE_MINKEY`
  - `static u32 s_value_typeid()` 同上

- 比较
  - `compare_eq(const BsonValue& val)` 使用 `bson_value_native_ex::s_value_cmp_eq_minkey` 校验
  - `compare_eq(const c_bson_minkey&)` 返回 `true`（所有 MinKey 相等）

- 序列化
  - `save(Bson& bs, const String& name)` 将字段名以 UTF-8 格式写入 BSON 并 append minkey
  - `save_value(Bson& bs)` 将 MinKey 值写入 BSON（调用 `s_bson_save_value_of_minkey`）
  - `load(...)` / `load_value(...)` 无需从迭代器中载入具体数据（函数体为空或断言）

- 流输出
  - `operator<<` 输出 `{ "$minKey" : 1 }`，通过 `bson_operator_outter<c_bson_minkey>` 实现

- 交换
  - `swap(c_bson_minkey&, c_bson_minkey&)` 为空实现（无状态）

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/ppex/friend.hpp>`
- `<yggr/charset/utf8_string.hpp>`
- `<yggr/nsql_database_system/bson_typeid.hpp>`
- `<yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>`
- `<yggr/nsql_database_system/bson_native_ex.hpp>`
- `<yggr/nsql_database_system/bson_value_native_ex.hpp>`
- `<yggr/nsql_database_system/bson_visit_t.hpp>`
- `<boost/utility/enable_if.hpp>`
- `<boost/type_traits/is_base_of.hpp>`
- `<cassert>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/c_bson_minkey.hpp>

using yggr::nsql_database_system::c_bson_minkey;

// 默认构造
c_bson_minkey m1;

// 从 bson_value_t 构造（需保证 value 表示 minkey）
bson_value_t v; // 假设已初始化为 MINKEY 类型
c_bson_minkey m2(v);

// 比较
bool eq = (m1 == m2); // true

// 保存到 bson
bson_t b;
bson_init(&b);
m1.save(b, "field");

// 流输出
std::cout << m1 << std::endl; // { "$minKey" : 1 }
```

## 设计考虑

- MinKey 为无状态标记值，类实现尽可能轻量且零开销
- 通过模板与 traits 保证仅对正确类型启用构造/赋值
- 序列化接口与项目的 BSON 本地扩展保持一致
- 所有 MinKey 实例相互等价，比较与交换实现简单

## BSON JSON 表示

在 BSON 的 JSON 表示中 MinKey 对应：

```json
{ "$minKey" : 1 }
```

## 兼容性

- 支持 C++11/14/17/20
- 依赖 Boost 类型特征与 enable_if
- 与 MongoDB BSON 规范及项目内 BSON 扩展接口兼容

## 相关类型

- `bson_value_t`: BSON 值基础类型
- `c_bson_maxkey`: 对应的 MaxKey 类（相对的特殊值）
- `bson_native_ex` / `bson_value_native_ex`: 本地扩展，用于类型检查和保存/加载操作
