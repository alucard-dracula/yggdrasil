# `unordered_set_bson_impl.hpp`

## 概述

该头文件是无序集合容器的 BSON 类型注册和实现接入文件。它根据 BSON 输入或输出归档是否可用，为标准库和 Boost 的无序集合及无序多重集合注册 BSON 数组类型，并在文件末尾包含无序线性容器的 BSON 实现。

本文件不直接实现集合元素的保存或读取算法，主要职责包括：

- 注册四种无序集合的 BSON 类型 ID；
- 根据归档宏控制 BSON 支持是否启用；
- 处理 BSON 实现的延迟重新包含标记；
- 接入 `unordered_unliner_container_bson_impl.hpp`。

## 头文件

```cpp
#include <yggr/serialization/unordered_set_bson_impl.hpp>
```

该文件通常由以下具体容器头间接包含：

```cpp
#include <yggr/serialization/unordered_set.hpp>
```

## 编译条件

主要实现仅在以下任一宏定义时启用：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HASH_BSON_IARCHIVE
```

源码中输入归档条件使用的是 `YGGR_HASH_BSON_IARCHIVE` 这一拼写，本文件按源码原样记录。启用条件满足时，文件定义主 include guard：

```cpp
__YGGR_SERIALIZATION_UNORDERED_SET_BSON_IMPL_HPP__
```

如果两个宏都未定义，则不注册 BSON 类型，而是设置内部待处理宏：

```cpp
YGGR_SERIALIZATION_UNORDERED_SET_BSON_IMPL_HPP_ARCHIVE
```

在后续重新包含且 BSON 归档条件满足时，该待处理宏会被取消定义。

## BSON 类型注册依赖

启用实现时包含：

```cpp
#include <yggr/nsql_database_system/bson_type_traits.hpp>
```

该依赖提供 BSON 类型 ID 定义，包括：

```cpp
bson_typeid_def::E_BSON_TYPE_ARRAY
```

无序集合和无序多重集合都映射为 BSON 数组类型。

## 容器类型注册

注册宏的形式为：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    4,
    ContainerType,
    bson_typeid_def::E_BSON_TYPE_ARRAY)
```

其中 `4` 表示无序集合模板的参数数量。文件注册以下容器：

| 容器 | 启用条件 | BSON 类型 |
| --- | --- | --- |
| `std::unordered_set` | 未定义 `YGGR_NO_CXX11_HDR_UNORDERED_SET` | `E_BSON_TYPE_ARRAY` |
| `std::unordered_multiset` | 未定义 `YGGR_NO_CXX11_HDR_UNORDERED_SET` | `E_BSON_TYPE_ARRAY` |
| `boost::unordered_set` | 始终注册 | `E_BSON_TYPE_ARRAY` |
| `boost::unordered_multiset` | 始终注册 | `E_BSON_TYPE_ARRAY` |

标准库容器注册受以下条件控制：

```cpp
#ifndef YGGR_NO_CXX11_HDR_UNORDERED_SET
// std::unordered_set 和 std::unordered_multiset
#endif
```

当目标环境不提供 C++11 标准无序集合时，跳过标准库容器注册，但保留 Boost 容器注册。

## BSON 实现接入

主 include guard 结束后，文件包含：

```cpp
#include <yggr/serialization/unordered_unliner_container_bson_impl.hpp>
```

该头文件负责无序线性容器的 BSON 元素处理。当前文件只负责把以下具体容器的 BSON 类型注册与通用 BSON 序列化流程连接起来：

- `std::unordered_set`；
- `std::unordered_multiset`；
- `boost::unordered_set`；
- `boost::unordered_multiset`。

与无序映射不同，无序集合元素没有键值对中的 `second` 成员，因此其元素输入、插入和对象地址处理由无序线性容器 BSON 实现负责。

## 与 `unordered_set.hpp` 的关系

`unordered_set.hpp` 负责通用无序集合序列化注册，并在文件末尾包含本文件：

```cpp
#include <yggr/serialization/unordered_set_bson_impl.hpp>
```

典型包含关系如下：

```text
unordered_set.hpp
  -> unordered_unliner_container.hpp
  -> 通用无序集合 save/load/serialize
  -> unordered_set_bson_impl.hpp
      -> BSON 容器类型 ID 注册
      -> unordered_unliner_container_bson_impl.hpp
```

`unordered_set.hpp` 使用 4 个模板参数注册标准库和 Boost 的无序集合，而本文件使用相同的参数数量为其注册 BSON 类型 ID。

## 宏状态总结

| 条件 | 行为 |
| --- | --- |
| `YGGR_HAS_BSON_OARCHIVE` 或 `YGGR_HASH_BSON_IARCHIVE` 已定义 | 定义主 include guard，注册 BSON 类型 ID，并接入无序线性容器 BSON 实现 |
| 两个宏均未定义 | 设置 `YGGR_SERIALIZATION_UNORDERED_SET_BSON_IMPL_HPP_ARCHIVE`，暂不注册类型 |
| 未定义 `YGGR_NO_CXX11_HDR_UNORDERED_SET` | 注册标准库无序集合和无序多重集合 |
| 定义 `YGGR_NO_CXX11_HDR_UNORDERED_SET` | 跳过标准库容器注册，保留 Boost 容器注册 |

## 注意事项

- 本文件主要负责 BSON 类型 ID 注册和实现接入，不直接提供完整的 BSON 保存/加载算法。
- 四种无序集合都映射为 BSON 数组类型，而不是 BSON 对象类型。
- 容器模板参数数量是 `4`，与无序映射使用的 `5` 不同；修改注册时必须保持一致。
- `YGGR_SERIALIZATION_UNORDERED_SET_BSON_IMPL_HPP_ARCHIVE` 和主 include guard 都是内部控制宏，业务代码不应手工定义或取消定义。
- 输入归档条件宏的源码拼写为 `YGGR_HASH_BSON_IARCHIVE`，修改前应检查项目中的宏定义和重新包含流程。
- 具体元素格式和无序集合加载行为由 `unordered_unliner_container_bson_impl.hpp` 及相关通用实现负责。
