# `set_bson_impl.hpp`

## 概述

该头文件为集合容器提供 BSON 类型适配，覆盖 set 和 multiset：

- `std::set`；
- `std::multiset`；
- `boost::container::set`；
- `boost::container::multiset`；
- Boost 1.56 及以上版本的 yggdrasil 内部 set/multiset 实现。

文件主要负责注册 BSON 类型标识，具体的非线性容器元素保存/加载由
`unliner_container_bson_impl.hpp` 提供。

## 头文件

```cpp
#include <yggr/serialization/set_bson_impl.hpp>
```

## 编译条件

当以下任一宏已定义时，BSON 类型注册生效：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HAS_BSON_IARCHIVE
```

未启用 BSON 输入或输出归档时，文件不会注册集合类型，并设置内部归档阶段宏以配合包含控制逻辑。

## BSON 类型注册

所有支持的 set 类型都注册为：

```cpp
bson_typeid_def::E_BSON_TYPE_ARRAY
```

### 标准库容器

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    3, std::set, bson_typeid_def::E_BSON_TYPE_ARRAY)

YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    3, std::multiset, bson_typeid_def::E_BSON_TYPE_ARRAY)
```

标准库 set 模板通常按三个模板参数注册：元素类型、比较器和分配器。

### Boost.Container 容器

Boost 版本低于 1.56 时，按三个模板参数注册：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    3, boost::container::set,
    bson_typeid_def::E_BSON_TYPE_ARRAY)
```

Boost 版本不低于 1.56 时，按四个模板参数注册：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    4, boost::container::set,
    bson_typeid_def::E_BSON_TYPE_ARRAY)
```

`boost::container::multiset` 使用相同的版本分支。

### yggdrasil 内部容器

Boost 版本不低于 1.56 时，额外注册：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    3, yggr::container::detail::set,
    bson_typeid_def::E_BSON_TYPE_ARRAY)

YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    3, yggr::container::detail::multiset,
    bson_typeid_def::E_BSON_TYPE_ARRAY)
```

## 支持的容器

| 容器 | BSON 类型 | 条件 |
| --- | --- | --- |
| `std::set` | `E_BSON_TYPE_ARRAY` | BSON 归档启用。 |
| `std::multiset` | `E_BSON_TYPE_ARRAY` | BSON 归档启用。 |
| `boost::container::set` | `E_BSON_TYPE_ARRAY` | BSON 归档启用；模板参数数量随 Boost 版本变化。 |
| `boost::container::multiset` | `E_BSON_TYPE_ARRAY` | BSON 归档启用；模板参数数量随 Boost 版本变化。 |
| `yggr::container::detail::set` | `E_BSON_TYPE_ARRAY` | Boost >= 1.56 且 BSON 归档启用。 |
| `yggr::container::detail::multiset` | `E_BSON_TYPE_ARRAY` | Boost >= 1.56 且 BSON 归档启用。 |

BSON 数组标识表示归档层的集合类型，不代表底层 set 具有连续内存布局。

## 具体序列化实现

文件末尾包含：

```cpp
#include <yggr/serialization/unliner_container_bson_impl.hpp>
```

该组件负责非线性容器的 BSON 元素处理，包括集合遍历、元素读取或写入以及相关对象跟踪。

本文件不直接实现 set 元素的保存和加载算法。

## Boost 版本兼容

Boost.Container set/multiset 的模板参数数量受以下条件控制：

```cpp
#if (BOOST_VERSION < 105600)
```

因此：

- Boost 1.56 之前：按 3 个模板参数注册；
- Boost 1.56 及以上：按 4 个模板参数注册；
- Boost 1.56 及以上：启用 yggdrasil 内部 set/multiset 注册。

## 使用示例

```cpp
#include <yggr/serialization/set.hpp>
#include <set>

void prepare_set()
{
    std::set<int> values;
    values.insert(1);
    values.insert(2);

    // 交给项目 BSON 归档进行序列化。
}
```

multiset 示例：

```cpp
#include <yggr/serialization/set.hpp>
#include <set>

void prepare_multiset()
{
    std::multiset<int> values;
    values.insert(1);
    values.insert(1);
}
```

## 与非线性容器 BSON 实现的关系

处理流程通常为：

1. 本文件注册 set/multiset 的 BSON 类型为数组；
2. `unliner_container_bson_impl.hpp` 提供具体元素处理；
3. 上层集合实现负责元素数量、版本和遍历；
4. BSON 归档负责元素类型的具体编码和解码。

set 的排序、唯一性和重复元素语义由底层容器保留，BSON 类型注册不会改变这些语义。

## 包含保护

文件使用主保护宏：

```cpp
__YGGR_SERIALIZATION_SET_BSON_IMPL_HPP__
```

并使用内部阶段宏：

```cpp
YGGR_SERIALIZATION_SET_BSON_IMPL_HPP_ARCHIVE
```

这些宏用于控制 BSON 类型注册的重复展开，通常不应由业务代码直接操作。

## 注意事项

1. 只有启用 BSON 输入或输出归档时，set 类型注册才会生效。
2. set 和 multiset 都被注册为 BSON 数组类型。
3. Boost.Container set/multiset 在 Boost 1.56 前后模板参数数量不同，升级 Boost 后应验证注册宏。
4. yggdrasil 内部 set/multiset 仅在 Boost 1.56 及以上版本注册。
5. 本文件不直接实现集合元素的 BSON 保存和加载。
6. 具体字段布局、集合大小、元素版本和对象跟踪由非线性容器 BSON 实现及上层集合实现决定。
7. BSON 数组类型不代表 set 的节点存储是连续内存。
8. set 元素必须满足项目 BSON 序列化框架的要求；multiset 的重复元素语义应由加载端正确保留。
9. 宏参数数量必须与对应 Boost 版本的实际模板参数一致。

## 相关组件

- `yggr/nsql_database_system/bson_type_traits.hpp`
- `yggr/serialization/unliner_container_bson_impl.hpp`
- `yggr/serialization/collections_save_bson_impl.hpp`
- `yggr/serialization/collection_size_type_bson_impl.hpp`
- `yggr/container/set.hpp`
- `std::set`
- `std::multiset`
- `boost::container::set`
- `boost::container::multiset`
