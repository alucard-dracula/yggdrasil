# `unordered_map_bson_impl.hpp`

## 概述

该头文件是无序映射容器的 BSON 类型注册和实现接入文件。它根据 BSON 输入或输出归档是否可用，为 `std::` 和 `boost::` 的无序映射及无序多重映射注册 BSON 数组类型，并在文件末尾包含无序键值容器的 BSON 元素输入实现。

本文件不直接实现集合元素的保存或读取逻辑：

- BSON 类型 ID 注册由 `YGGR_PP_CONTAINER_BSON_TYPEID_IMPL` 完成；
- 集合级保存/加载由无序集合 BSON 实现负责；
- 单个键值元素的 BSON 输入由 `unordered_key_value_container_bson_impl.hpp` 负责。

## 头文件

```cpp
#include <yggr/serialization/unordered_map_bson_impl.hpp>
```

该文件通常由以下具体容器头间接包含：

```cpp
#include <yggr/serialization/unordered_map.hpp>
```

## 编译条件

文件的主要实现仅在以下任一宏定义时启用：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HASH_BSON_IARCHIVE
```

注意：源码中使用的输入归档宏名称是 `YGGR_HASH_BSON_IARCHIVE`，这里按源码原样记录。若该名称不是项目预期的宏，应结合项目其他 BSON 头文件确认是否属于既有拼写或兼容性约定。

启用条件满足时，文件定义主 include guard：

```cpp
__YGGR_SERIALIZATION_UNORDERED_MAP_BSON_IMPL_HPP__
```

当两个宏都未定义时，不执行 BSON 类型注册，而是设置内部待处理宏：

```cpp
YGGR_SERIALIZATION_UNORDERED_MAP_BSON_IMPL_HPP_ARCHIVE
```

当后续重新包含时，如果 BSON 归档条件已经满足，该待处理宏会被取消定义。

## BSON 类型注册依赖

启用实现时包含：

```cpp
#include <yggr/nsql_database_system/bson_type_traits.hpp>
```

该头文件提供 BSON 类型 ID 定义，包括：

```cpp
bson_typeid_def::E_BSON_TYPE_ARRAY
```

## 容器类型注册

每个无序映射类型都被注册为 BSON 数组类型：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    5,
    ContainerType,
    bson_typeid_def::E_BSON_TYPE_ARRAY)
```

其中 `5` 表示无序映射模板的参数数量。注册的容器包括：

| 容器 | 启用条件 | BSON 类型 |
| --- | --- | --- |
| `std::unordered_map` | 未定义 `YGGR_NO_CXX11_HDR_UNORDERED_MAP` | `E_BSON_TYPE_ARRAY` |
| `std::unordered_multimap` | 未定义 `YGGR_NO_CXX11_HDR_UNORDERED_MAP` | `E_BSON_TYPE_ARRAY` |
| `boost::unordered_map` | 始终注册 | `E_BSON_TYPE_ARRAY` |
| `boost::unordered_multimap` | 始终注册 | `E_BSON_TYPE_ARRAY` |

标准库容器的两项注册包在以下条件中：

```cpp
#ifndef YGGR_NO_CXX11_HDR_UNORDERED_MAP
// std::unordered_map 和 std::unordered_multimap
#endif
```

因此，在目标环境不提供 C++11 标准无序容器时，只有 Boost 无序容器仍会注册。

## BSON 实现接入

文件主体的 include guard 结束后，无论前面的归档条件是否满足，都会包含：

```cpp
#include <yggr/serialization/unordered_key_value_container_bson_impl.hpp>
```

该头文件为 `bson_iarchive<skip_auto_id>` 特化以下两个元素输入策略：

- `archive_input_unordered_key_value`；
- `archive_input_unordered_multi_key_value`。

元素字段名使用数字字符串索引。普通无序集合的元数据和元素遍历由 `unordered_collections_load_bson_impl.hpp` 负责，当前文件只负责把具体无序映射类型的 BSON 支持连接到这条处理链上。

## 与 `unordered_map.hpp` 的关系

`unordered_map.hpp` 负责通用的无序映射序列化注册，并在文件末尾包含本文件：

```cpp
#include <yggr/serialization/unordered_map_bson_impl.hpp>
```

典型的包含关系如下：

```text
unordered_map.hpp
  -> unordered_key_value_container.hpp
  -> 通用无序映射 save/load/serialize
  -> unordered_map_bson_impl.hpp
      -> BSON 容器类型 ID 注册
      -> unordered_key_value_container_bson_impl.hpp
```

因此，使用 `unordered_map.hpp` 时，具体容器的通用序列化接口和 BSON 类型适配会在适当条件下同时生效。

## 宏状态总结

| 条件 | 行为 |
| --- | --- |
| `YGGR_HAS_BSON_OARCHIVE` 或 `YGGR_HASH_BSON_IARCHIVE` 已定义 | 定义主 include guard，注册 BSON 类型 ID，并接入 BSON 键值容器实现 |
| 两个宏均未定义 | 设置 `YGGR_SERIALIZATION_UNORDERED_MAP_BSON_IMPL_HPP_ARCHIVE`，暂不注册类型 |
| 未定义 `YGGR_NO_CXX11_HDR_UNORDERED_MAP` | 注册 `std::` 无序映射和多重映射 |
| 定义 `YGGR_NO_CXX11_HDR_UNORDERED_MAP` | 跳过 `std::` 无序映射注册，保留 Boost 容器注册 |

## 注意事项

- 本文件主要负责 BSON 类型 ID 注册和实现接入，不直接提供完整的 BSON 保存/加载算法。
- 四种无序映射都映射为 BSON 数组类型，而不是 BSON 对象类型。
- `YGGR_SERIALIZATION_UNORDERED_MAP_BSON_IMPL_HPP_ARCHIVE` 和主 include guard 都是内部控制宏，业务代码不应手工定义或取消定义。
- 输入归档条件宏的源码拼写为 `YGGR_HASH_BSON_IARCHIVE`，修改前应检查项目中是否存在同名宏及其重新包含流程。
- 具体元素字段格式和对象地址修复由 `unordered_key_value_container_bson_impl.hpp` 负责。
