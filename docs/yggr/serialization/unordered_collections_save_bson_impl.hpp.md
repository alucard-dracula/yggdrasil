# `unordered_collections_save_bson_impl.hpp`

## 概述

该头文件为无序集合提供 BSON 输出归档实现。它针对两种 BSON 输出归档模式特化 `unordered_collections_saver_switch`，负责输出集合元数据、元素构造信息以及按数字字符串索引排列的集合元素。

## 头文件

```cpp
#include <yggr/serialization/unordered_collections_save_bson_impl.hpp>
```

该文件通常由无序集合序列化实现间接包含。

## 编译条件

实现仅在定义以下宏时启用：

```cpp
YGGR_HAS_BSON_OARCHIVE
```

启用时定义主 include guard：

```cpp
__YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_BSON_IMPL_HPP__
```

如果 `YGGR_HAS_BSON_OARCHIVE` 未定义，则不会生成 BSON 输出特化，而是设置内部待处理宏：

```cpp
YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_BSON_IMPL_HPP_OARCHIVE
```

当 BSON 输出归档宏已经生效时，如果该待处理宏此前存在，文件会先将其取消定义。

## 依赖

启用实现时包含：

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/array_bson_save_impl.hpp>
#include <boost/ref.hpp>
#include <sstream>
```

其中 `nvp.hpp` 用于输出命名值对，`array_bson_save_impl.hpp` 提供相关 BSON 数组保存支持，`sstream` 用于生成数字字符串字段名。

## `unordered_collections_saver_switch` 特化

特化位于：

```cpp
yggr::serialization::container
```

文件提供以下两种策略：

```cpp
template<typename Container>
struct unordered_collections_saver_switch<
    yggr::archive::bson_oarchive<false>,
    Container>;

template<typename Container>
struct unordered_collections_saver_switch<
    yggr::archive::bson_oarchive<true>,
    Container>;
```

两种策略都要求容器提供：

- `size()`；
- `bucket_count()`；
- `const_iterator`；
- `begin()`；
- 元素类型 `value_type`。

## 普通 BSON 输出模式

`bson_oarchive<false>` 的输出接口为：

```cpp
void operator()(
    yggr::archive::bson_oarchive<false>& ar,
    const Container& container) const;
```

### 元数据

该模式首先计算：

```cpp
collection_size_type count(container.size());
const collection_size_type bucket_count(container.bucket_count());
const boost::serialization::item_version_type item_version(
    boost::serialization::version<typename Container::value_type>::value);
```

然后依次写入三个字段：

1. `count`：元素数量；
2. `bucket_count`：哈希桶数量；
3. `item_version`：元素类型的 Boost.Serialization 版本。

字段名由递增数字字符串生成：

```text
"0", "1", "2"
```

### 元素输出

元数据写入完成后，从容器的 `begin()` 开始遍历。每个元素输出前先调用：

```cpp
boost::serialization::save_construct_data_adl(
    ar,
    boost::addressof(*it),
    boost::serialization::version<
        typename Container::value_type
    >::value);
```

随后使用当前数字索引输出元素：

```cpp
ar << YGGR_SERIALIZATION_NAME_NVP(
    ss.str().data(),
    *it++);
```

因此，第一个元素通常从字段 `"3"` 开始。

## `skip_auto_id` 输出模式

`bson_oarchive<true>` 的输出接口为：

```cpp
void operator()(
    yggr::archive::bson_oarchive<true>& ar,
    const Container& container) const;
```

该模式跳过自动 ID 和集合级元数据，不写入 `count`、`bucket_count` 或 `item_version`。它直接从索引 `"0"` 开始输出元素。

每个元素仍会先调用：

```cpp
boost::serialization::save_construct_data_adl(...);
```

然后以数字字符串索引作为 BSON 字段名写出元素内容。

## 字段格式对比

| 归档类型 | 元数据 | 元素起始索引 |
| --- | --- | --- |
| `bson_oarchive<false>` | 写入 `count`、`bucket_count`、`item_version` | `"3"` |
| `bson_oarchive<true>` | 不写入集合元数据 | `"0"` |

## 元素版本与构造数据

元素版本由以下表达式获取：

```cpp
boost::serialization::version<typename Container::value_type>::value
```

该版本既用于生成 `item_version`，也传给 `save_construct_data_adl`。构造数据保存允许元素类型在反序列化时恢复必要的构造信息。

## 容器遍历顺序

元素按照容器 `const_iterator` 从 `begin()` 到结尾的顺序输出。由于无序容器本身不保证稳定顺序，不应将 BSON 字段索引解释为元素的持久化排序依据。

## BSON 输入兼容

普通输出模式写入的字段顺序与无序集合 BSON 输入实现对应：

- 索引 `"0"`：元素数量；
- 索引 `"1"`：桶数量；
- 索引 `"2"`：元素版本；
- 从索引 `"3"` 开始：集合元素。

`bson_oarchive<true>` 则与跳过自动 ID 的输入路径对应，直接从索引 `"0"` 读取元素。

## 注意事项

- 该文件只提供 BSON 输出归档实现，不处理 BSON 输入归档。
- `Container::bucket_count()` 是必需接口；不适合没有哈希桶概念的容器。
- 无序容器遍历顺序可能变化，因此相同元素集合不一定产生相同的字段索引对应关系。
- `YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_BSON_IMPL_HPP_OARCHIVE` 属于内部重新包含控制宏，业务代码不应直接依赖。
- 输出格式必须与相应的 BSON 输入实现保持一致，尤其是普通模式下三个元数据字段的位置。
