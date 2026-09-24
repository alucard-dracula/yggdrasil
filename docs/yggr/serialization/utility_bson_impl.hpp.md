# `utility_bson_impl.hpp`

## 概述

该头文件为 `std::pair<Key, Val>` 提供 BSON 类型和归档支持。它完成三项工作：

- 将 `std::pair` 注册为 BSON 数组类型；
- 为 BSON 输出归档定义 `save`，使用字段 `"0"` 和 `"1"` 写出键和值；
- 为 BSON 输入归档定义 `load`，使用相同字段读取键和值。

通用的非 BSON `std::pair` 序列化实现位于 `utility.hpp`，本文件提供 BSON 归档的专用重载和条件编译控制。

## 头文件

```cpp
#include <yggr/serialization/utility_bson_impl.hpp>
```

`utility.hpp` 会在自身主 include guard 结束后包含本文件，因此通常不需要单独包含。

## 编译条件

类型 ID 注册和 BSON 相关依赖仅在以下条件满足时启用：

```cpp
#if defined(YGGR_HAS_BSON_OARCHIVE) \
    || defined(YGGR_HASH_BSON_IARCHVIE)
```

源码中输入归档条件使用的是 `YGGR_HASH_BSON_IARCHVIE` 这一拼写，末尾为 `IARCHVIE`。文档按源码实际名称记录；修改前应确认这是项目既有宏名称还是拼写问题。

条件满足时包含：

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/nsql_database_system/bson_type_traits.hpp>
#include <yggr/serialization/nvp.hpp>
#include <sstream>
```

其中 BSON 类型 traits 提供 BSON 类型 ID，`nvp.hpp` 提供按字段名称读写归档值的支持。

## `std::pair` 的 BSON 类型 ID

类型特化位于：

```cpp
yggr::nsql_database_system
```

特化形式为：

```cpp
template<typename Key, typename Val>
struct bson_typeid<std::pair<Key, Val>>
```

该特化定义：

```cpp
value = bson_typeid_def::E_BSON_TYPE_ARRAY
```

并提供两个调用运算符：

```cpp
u8 operator()() const;
u8 operator()(const type&) const;
```

二者都返回 `E_BSON_TYPE_ARRAY` 对应的 BSON 类型 ID。因此，键值对在 BSON 中表现为包含两个字段的数组结构，而不是 BSON 对象结构。

## BSON 输出归档

当定义以下宏时启用输出函数：

```cpp
YGGR_HAS_BSON_OARCHIVE
```

函数接口为：

```cpp
template<bool nld, typename Key, typename Val>
inline void save(
    yggr::archive::bson_oarchive<nld>& ar,
    const std::pair<Key, Val>& value,
    const unsigned int file_version);
```

输出字段如下：

```cpp
ar << YGGR_SERIALIZATION_NAME_NVP(
    "0", const_cast<typef&>(value.first));
ar << YGGR_SERIALIZATION_NAME_NVP(
    "1", value.second);
```

字段含义为：

| BSON 字段 | 内容 |
| --- | --- |
| `"0"` | `pair::first`，键 |
| `"1"` | `pair::second`，值 |

由于 `std::pair` 的 `first` 可能是 `const` 类型，代码先使用 `boost::remove_const<Key>::type` 得到非 const 类型，再通过 `const_cast` 传给命名值对宏。输出过程不会改变键的逻辑值。

## BSON 输入归档

当定义以下宏时启用输入函数：

```cpp
YGGR_HAS_BSON_IARCHIVE
```

函数接口为：

```cpp
template<bool skip_auto_id, typename Key, typename Val>
inline void load(
    yggr::archive::bson_iarchive<skip_auto_id>& ar,
    std::pair<Key, Val>& value,
    const unsigned int file_version);
```

输入字段与输出字段对应：

```cpp
ar >> YGGR_SERIALIZATION_NAME_NVP(
    "0", const_cast<key_type&>(value.first));
ar >> YGGR_SERIALIZATION_NAME_NVP(
    "1", value.second);
```

读取键时同样移除 `Key` 的 const 属性并使用 `const_cast`，这是为了适配 `std::pair<const Key, Val>` 这类映射元素类型。归档字段顺序和字段名称必须与 BSON 输出格式保持一致。

## 与通用 `utility.hpp` 的关系

`utility.hpp` 为普通归档提供：

- `std::pair` 的通用 `save`；
- `std::pair` 的通用 `load`；
- 使用 `split_free` 的 `serialize`；
- `std::pair` 的 `object_serializable` 实现级别。

本文件则针对 BSON 归档覆盖 `save` 和 `load`，并把通用字段名：

```text
"first", "second"
```

替换为 BSON 数组风格的数字字段名：

```text
"0", "1"
```

## 条件编译与重新包含状态

文件分别跟踪三类 BSON 实现状态：

- BSON 类型 ID 是否已注册；
- BSON 输出归档是否可用；
- BSON 输入归档是否可用。

对应的状态宏为：

```cpp
__YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_BSON_VALUE_TYPEID__
__YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_OARCHIVE__
__YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_IARCHIVE__
```

当三者同时定义时，文件定义主 include guard：

```cpp
__YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP__
```

随后清理三个阶段状态宏，使本文件的 BSON 实现完成一次性重新包含流程。

输出和输入分支还会设置或清理不带末尾双下划线的待处理宏：

```cpp
YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_OARCHIVE
YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_IARCHIVE
```

这些宏属于内部控制机制，业务代码不应直接依赖。

## 数据格式

一个 `std::pair<Key, Val>` 的 BSON 逻辑结构为：

```text
[
  "0": first,
  "1": second
]
```

实际 BSON 容器表示由项目的 BSON 归档实现决定，但键值字段名称和类型 ID 由本文件固定为数组类型及数字字符串字段。

## 注意事项

- `std::pair` 的 BSON 类型被注册为 `E_BSON_TYPE_ARRAY`。
- 输入和输出必须使用一致的字段名称：`"0"` 对应键，`"1"` 对应值。
- 对 `pair<const Key, Val>` 的键进行读取或输出时，代码使用 `remove_const` 和 `const_cast` 适配 Boost.Serialization 接口。
- `YGGR_HASH_BSON_IARCHVIE` 是源码中实际出现的输入条件宏拼写，应在修改宏逻辑前检查项目定义。
- 本文件不定义通用 `serialize`，普通归档的拆分序列化仍由 `utility.hpp` 提供。
