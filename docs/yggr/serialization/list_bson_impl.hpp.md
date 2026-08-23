# `list_bson_impl.hpp`

## 概述

该头文件为链表容器提供 BSON 序列化类型适配，覆盖：

- `std::list<T, Allocator>`；
- `boost::container::list<T, Allocator>`；
- Boost 1.70 及以上版本中的 yggdrasil 内部 list 实现。

文件本身主要注册 BSON 类型标识，并将具体线性容器元素处理委托给
`liner_container_bson_impl.hpp`。

## 头文件

```cpp
#include <yggr/serialization/list_bson_impl.hpp>
```

## 编译条件

当以下任一宏已定义时，BSON 类型注册生效：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HAS_BSON_IARCHIVE
```

如果两个宏都未定义，文件不会注册 list 的 BSON 类型，并设置内部归档阶段宏以配合包含控制逻辑。

## BSON 类型注册

文件使用以下宏将链表标识为 BSON 数组：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    2,
    std::list,
    bson_typeid_def::E_BSON_TYPE_ARRAY)

YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    2,
    boost::container::list,
    bson_typeid_def::E_BSON_TYPE_ARRAY)
```

当 Boost 版本不低于 1.70 时，还注册：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    2,
    yggr::container::detail::list,
    bson_typeid_def::E_BSON_TYPE_ARRAY)
```

## 支持的容器

| 容器 | 条件 | BSON 类型 |
| --- | --- | --- |
| `std::list<T, Allocator>` | BSON 输入或输出归档启用 | `E_BSON_TYPE_ARRAY` |
| `boost::container::list<T, Allocator>` | BSON 输入或输出归档启用 | `E_BSON_TYPE_ARRAY` |
| `yggr::container::detail::list<T, Allocator>` | BSON 归档启用且 Boost >= 1.70 | `E_BSON_TYPE_ARRAY` |

宏参数 `2` 表示这些 list 模板具有两个模板参数。

## 具体序列化实现

文件末尾包含：

```cpp
#include <yggr/serialization/liner_container_bson_impl.hpp>
```

该组件负责线性容器的具体 BSON 加载逻辑，包括数字字符串字段读取、元素追加和对象地址跟踪。

本文件不直接实现元素遍历、保存或加载算法。

## BSON 数据语义

上述容器在 BSON 类型系统中都表示为数组：

```cpp
bson_typeid_def::E_BSON_TYPE_ARRAY
```

这只描述容器的 BSON 类型，不代表链表会转换为连续内存数组。链表节点仍由容器自身管理。

## Boost 版本兼容

内部 yggdrasil list 的 BSON 类型注册受以下条件控制：

```cpp
#if !(BOOST_VERSION < 107000)
```

因此：

- Boost 1.70 及以上：注册 `yggr::container::detail::list`；
- 低于 Boost 1.70：不注册该内部类型；
- `std::list` 和 `boost::container::list` 的注册不受该版本条件影响。

## 使用示例

```cpp
#include <yggr/serialization/list.hpp>
#include <list>

void prepare_list()
{
    std::list<int> values;
    values.push_back(1);
    values.push_back(2);

    // 交给项目 BSON 归档进行序列化。
}
```

## 与线性容器 BSON 实现的关系

处理流程通常为：

1. 本文件注册 list 的 BSON 类型为数组；
2. 线性容器 BSON 实现提供元素读取逻辑；
3. 上层集合加载流程负责集合数量、版本和遍历；
4. BSON 归档负责每个元素的具体序列化。

## 包含保护

文件使用主保护宏：

```cpp
__YGGR_SERIALIZATION_LIST_BSON_IMPL_HPP__
```

并使用内部阶段宏：

```cpp
YGGR_SERIALIZATION_LIST_BSON_IMPL_HPP_ARCHIVE
```

这些宏用于控制 BSON 类型注册的重复展开，通常不应由业务代码直接操作。

## 注意事项

1. 只有启用 BSON 输入或输出归档时，list 的 BSON 类型注册才会生效。
2. `std::list`、`boost::container::list` 和内部 list 都被标记为 BSON 数组类型。
3. 宏参数 `2` 表示目标 list 模板具有两个模板参数。
4. 内部 yggdrasil list 的注册依赖 Boost 1.70 或更高版本。
5. 本文件不直接实现 list 元素的保存和加载。
6. 具体字段布局、集合大小、元素版本和对象跟踪由线性容器 BSON 实现及上层集合实现决定。
7. BSON 数组类型不意味着链表具有连续内存布局。
8. 链表元素类型和分配器类型必须满足项目 BSON 序列化框架的要求。
9. 该文件末尾无条件包含 `liner_container_bson_impl.hpp`，实际生效内容仍受其自身宏条件控制。

## 相关组件

- `yggr/nsql_database_system/bson_type_traits.hpp`
- `yggr/serialization/liner_container_bson_impl.hpp`
- `yggr/serialization/collections_save_bson_impl.hpp`
- `yggr/serialization/collection_size_type_bson_impl.hpp`
- `yggr/container/list.hpp`
- `std::list`
- `boost::container::list`
