# `unordered_set.hpp`

## 概述

该头文件为标准库和 Boost 的无序集合容器接入 Yggdrasil/Boost.Serialization 支持。它不直接实现集合元素的保存和加载算法，而是完成以下适配工作：

- 包含无序线性容器的通用序列化入口；
- 将 `unordered_multiset` 标记为允许重复元素的多重容器；
- 为四种无序集合类型展开 `save`、`load` 和 `serialize` 自由函数；
- 设置容器的 Boost.Serialization 实现级别；
- 在文件末尾接入 BSON 类型和归档实现。

## 头文件

```cpp
#include <yggr/serialization/unordered_set.hpp>
```

该头文件适配以下容器：

```cpp
std::unordered_set
std::unordered_multiset
boost::unordered_set
boost::unordered_multiset
```

## 依赖

主要依赖包括：

```cpp
#include <yggr/serialization/unordered_unliner_container.hpp>
#include <yggr/container/unordered_set.hpp>
#include <yggr/serialization/detail/type_traits.hpp>
```

各依赖的职责如下：

- `unordered_unliner_container.hpp` 提供无序集合的通用序列化宏和元素输入策略；
- `yggr/container/unordered_set.hpp` 提供项目使用的标准库/Boost 无序集合类型支持；
- `detail/type_traits.hpp` 提供 `is_multi_container` 等序列化类型特征基础设施。

## `is_multi_container` 特化

特化位于：

```cpp
yggr::serialization::detail
```

### 标准库多重集合

当未定义 `YGGR_NO_CXX11_HDR_UNORDERED_SET` 时，为标准库多重集合提供特化：

```cpp
template<
    typename Key,
    typename HashFoo,
    typename Cmp,
    typename Alloc>
struct is_multi_container<
    std::unordered_multiset<Key, HashFoo, Cmp, Alloc>>
    : public boost::mpl::true_
{
};
```

### Boost 多重集合

Boost 多重集合始终提供对应特化：

```cpp
template<
    typename Key,
    typename HashFoo,
    typename Cmp,
    typename Alloc>
struct is_multi_container<
    boost::unordered_multiset<Key, HashFoo, Cmp, Alloc>>
    : public boost::mpl::true_
{
};
```

该特征由通用 `load` 实现读取，用于在普通集合输入策略和多重集合输入策略之间进行选择。普通 `unordered_set` 不在本文件中显式特化，使用默认的非多重容器判断。

## 序列化函数注册

注册发生在：

```cpp
boost::serialization
```

文件通过以下宏为每个容器展开序列化函数：

```cpp
YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(
    4,
    container_type)
```

这里的 `4` 表示无序集合模板的参数数量。注册项如下：

```cpp
#ifndef YGGR_NO_CXX11_HDR_UNORDERED_SET
YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(
    4, std::unordered_set)
YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(
    4, std::unordered_multiset)
#endif

YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(
    4, boost::unordered_set)
YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(
    4, boost::unordered_multiset)
```

宏名称中的 `CONTINER` 拼写是源码中既有名称，调用时必须保持不变。

宏生成：

- `save`：委托给 `save_unordered_collection`；
- `load`：委托给 `load_unordered_collection`，并根据 `is_multi_container` 选择元素输入策略；
- `serialize`：通过 `boost::serialization::split_free` 分离保存和加载。

## 加载策略选择

生成的 `load` 会根据容器是否为多重容器选择：

- 普通无序集合使用 `archive_input_unordered_unliner`；
- 无序多重集合使用 `archive_input_unordered_multi_unliner`。

普通集合的输入策略会在插入失败时跳过地址重置；多重集合输入策略则按每次插入产生的新元素更新对象地址。具体元素读取和插入逻辑由 `unordered_unliner_container.hpp` 提供。

## 标准库容器条件

标准库无序集合相关代码受以下宏控制：

```cpp
YGGR_NO_CXX11_HDR_UNORDERED_SET
```

当该宏未定义时，注册：

- `std::unordered_set`；
- `std::unordered_multiset`；
- `std::unordered_multiset` 的多重容器特征。

当该宏已定义时，跳过标准库无序集合相关代码，但 Boost 无序集合仍然注册。

## 序列化实现级别

文件包含：

```cpp
#include <yggr/serialization/detail/container_implementation_level_def.hpp>
```

随后将四种容器的实现级别设置为 `object_serializable`：

```cpp
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    4, std::unordered_set, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    4, std::unordered_multiset, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    4, boost::unordered_set, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    4, boost::unordered_multiset, object_serializable)
```

标准库容器的两个实现级别定义同样受 `YGGR_NO_CXX11_HDR_UNORDERED_SET` 控制。这里的 `4` 必须与目标无序集合模板的参数数量保持一致。

## BSON 集成

文件主 include guard 结束后包含：

```cpp
#include <yggr/serialization/unordered_set_bson_impl.hpp>
```

该 BSON 适配头负责：

- 将四种无序集合注册为 BSON 数组类型；
- 根据 BSON 归档宏控制相关实现是否启用；
- 接入 `unordered_unliner_container_bson_impl.hpp` 提供的 BSON 元素处理。

因此，包含本文件不仅获得通用无序集合序列化支持，在 BSON 归档条件满足时也会获得对应的 BSON 适配。

## 包含关系

典型处理链如下：

```text
unordered_set.hpp
  -> unordered_unliner_container.hpp
      -> 通用无序集合 save/load 实现
  -> yggr/container/unordered_set.hpp
  -> serialization/detail/type_traits.hpp
  -> container_implementation_level_def.hpp
  -> unordered_set_bson_impl.hpp
      -> unordered_unliner_container_bson_impl.hpp
```

其中 `unordered_unliner_container.hpp` 负责通用算法，本文件负责将具体集合模板接入该算法。

## 使用限制与注意事项

- 使用标准库无序集合时，目标环境必须支持对应的 C++11 无序集合，且不能定义 `YGGR_NO_CXX11_HDR_UNORDERED_SET`。
- `unordered_multiset` 的 `is_multi_container` 特征必须保持为 `boost::mpl::true_`，否则加载时会使用错误的普通集合插入策略。
- 容器模板参数数量当前按 `4` 注册；如果底层容器模板签名变化，应同步调整序列化和实现级别宏的参数。
- 本文件依赖 `unordered_unliner_container.hpp` 中定义的宏，不能脱离该通用入口单独展开注册。
- BSON 适配的具体字段格式和元素地址处理由相关 BSON 实现文件负责。
