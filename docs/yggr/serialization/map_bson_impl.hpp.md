# `map_bson_impl.hpp`

## 概述

该头文件为关联容器提供 BSON 类型适配，覆盖 map 和 multimap 容器，并将它们注册为 BSON 数组类型。

文件本身主要完成类型标识注册，键值元素的具体 BSON 输入处理由
`key_value_container_bson_impl.hpp` 提供。

## 头文件

```cpp
#include <yggr/serialization/map_bson_impl.hpp>
```

## 编译条件

当以下任一宏已定义时，BSON 类型注册生效：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HAS_BSON_IARCHIVE
```

如果两个宏都未定义，文件不会注册 map 类型，并设置内部归档阶段宏以配合包含控制逻辑。

## BSON 类型注册

### 标准库关联容器

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    4, std::map, bson_typeid_def::E_BSON_TYPE_ARRAY)

YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    4, std::multimap, bson_typeid_def::E_BSON_TYPE_ARRAY)
```

`std::map` 和 `std::multimap` 按 4 个模板参数注册，通常对应键类型、值类型、比较器和分配器。

### Boost.Container 关联容器

当 `BOOST_VERSION < 105600` 时，使用 4 个模板参数注册：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    4, boost::container::map, bson_typeid_def::E_BSON_TYPE_ARRAY)

YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    4, boost::container::multimap,
    bson_typeid_def::E_BSON_TYPE_ARRAY)
```

当 Boost 版本不低于 1.56 时，使用 5 个模板参数注册：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    5, boost::container::map, bson_typeid_def::E_BSON_TYPE_ARRAY)

YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    5, boost::container::multimap,
    bson_typeid_def::E_BSON_TYPE_ARRAY)
```

该差异用于适配不同 Boost 版本中 Boost.Container map 模板参数数量的变化。

### yggdrasil 内部关联容器

Boost 版本不低于 1.56 时，还注册：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    4, yggr::container::detail::map,
    bson_typeid_def::E_BSON_TYPE_ARRAY)

YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    4, yggr::container::detail::multimap,
    bson_typeid_def::E_BSON_TYPE_ARRAY)
```

## 支持的容器

| 容器 | BSON 类型 | 条件 |
| --- | --- | --- |
| `std::map` | `E_BSON_TYPE_ARRAY` | BSON 归档启用。 |
| `std::multimap` | `E_BSON_TYPE_ARRAY` | BSON 归档启用。 |
| `boost::container::map` | `E_BSON_TYPE_ARRAY` | BSON 归档启用；模板参数数量随 Boost 版本变化。 |
| `boost::container::multimap` | `E_BSON_TYPE_ARRAY` | BSON 归档启用；模板参数数量随 Boost 版本变化。 |
| `yggr::container::detail::map` | `E_BSON_TYPE_ARRAY` | Boost >= 1.56 且 BSON 归档启用。 |
| `yggr::container::detail::multimap` | `E_BSON_TYPE_ARRAY` | Boost >= 1.56 且 BSON 归档启用。 |

BSON 数组标识描述容器的归档类型，不代表关联容器会变为连续内存结构。

## 键值容器实现

文件末尾包含：

```cpp
#include <yggr/serialization/key_value_container_bson_impl.hpp>
```

该实现负责 BSON 输入归档中的单个键值元素读取，包括：

- 按数字字符串字段名读取元素；
- 使用元素版本构造临时键值对象；
- 插入目标关联容器；
- 修正 Boost.Serialization 对象地址跟踪。

## BSON 数据语义

关联容器在 BSON 类型系统中注册为数组，但每个数组元素通常是一个键值对对象。

具体字段布局、集合大小、元素版本及键值对读取方式由关联容器和 BSON 序列化实现共同决定。

## 使用示例

```cpp
#include <yggr/serialization/map.hpp>
#include <map>

void prepare_map()
{
    std::map<int, std::string> values;
    values[1] = "one";

    // 交给项目 BSON 归档进行序列化。
}
```

## 包含保护

文件使用主保护宏：

```cpp
__YGGR_SERIALIZATION_MAP_BSON_IMPL_HPP__
```

并使用内部阶段宏：

```cpp
YGGR_SERIALIZATION_MAP_BSON_IMPL_HPP_ARCHIVE
```

这些宏用于控制 BSON 类型注册的重复展开，通常不应由业务代码直接操作。

## 注意事项

1. 只有启用 BSON 输入或输出归档时，map 类型注册才会生效。
2. map 和 multimap 都被注册为 BSON 数组类型。
3. Boost.Container map 在 Boost 1.56 前后模板参数数量不同，注册宏参数必须与实际版本匹配。
4. yggdrasil 内部 map 和 multimap 仅在 Boost 1.56 及以上版本注册。
5. 本文件主要负责 BSON 类型标识，不直接实现完整的 map 保存/加载流程。
6. 键值元素输入处理依赖 `key_value_container_bson_impl.hpp`。
7. BSON 数组类型不表示 map 的键值元素具有连续内存布局。
8. 键和值类型必须分别满足项目 BSON 序列化要求。
9. 升级 Boost 版本后，应重新验证 Boost.Container 模板参数数量和相关宏注册。

## 相关组件

- `yggr/nsql_database_system/bson_type_traits.hpp`
- `yggr/serialization/key_value_container_bson_impl.hpp`
- `yggr/serialization/collections_save_bson_impl.hpp`
- `yggr/serialization/collection_size_type_bson_impl.hpp`
- `yggr/container/map.hpp`
- `std::map`
- `std::multimap`
- `boost::container::map`
- `boost::container::multimap`
