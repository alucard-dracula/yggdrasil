# `unordered_collections_save_imp.hpp`

## 概述

该头文件提供无序集合的通用 Boost.Serialization 保存实现。它负责写出集合级元数据、元素构造数据以及每个元素本身，具体归档格式由 `Archive` 类型决定。

核心组件包括：

- `unordered_collections_saver_switch`：通用无序集合保存策略；
- `save_unordered_collection`：调用保存策略的便捷函数。

## 头文件

```cpp
#include <yggr/serialization/unordered_collections_save_imp.hpp>
```

主要依赖包括：

```cpp
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/collection_size_type.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/item_version_type.hpp>
#include <boost/ref.hpp>
```

## `unordered_collections_saver_switch`

核心模板位于：

```cpp
yggr::serialization::container
```

接口形式为：

```cpp
template<typename Archive, typename Container>
struct unordered_collections_saver_switch
{
    void operator()(
        Archive& ar,
        const Container& container) const;
};
```

模板参数含义：

| 参数 | 含义 |
| --- | --- |
| `Archive` | 输出归档类型 |
| `Container` | 要保存的无序容器类型 |

## 集合元数据

保存开始时读取以下信息：

```cpp
collection_size_type count(container.size());
const collection_size_type bucket_count(container.bucket_count());
const boost::serialization::item_version_type item_version(
    boost::serialization::version<
        typename Container::value_type>::value);
```

随后依次写入：

```cpp
ar << YGGR_SERIALIZATION_NVP(count);
ar << YGGR_SERIALIZATION_NVP(bucket_count);
ar << YGGR_SERIALIZATION_NVP(item_version);
```

字段含义如下：

- `count`：集合中的元素数量；
- `bucket_count`：无序容器当前的哈希桶数量；
- `item_version`：容器元素类型的 Boost.Serialization 版本。

## 元素保存流程

元数据写出后，保存器从 `container.begin()` 开始遍历：

```cpp
typename Container::const_iterator it = container.begin();
while(count-- > 0)
{
    ...
}
```

每个元素按以下顺序处理。

### 保存构造数据

首先调用：

```cpp
boost::serialization::save_construct_data_adl(
    ar,
    boost::addressof(*it),
    boost::serialization::version<
        typename Container::value_type>::value);
```

该调用用于保存元素在加载阶段构造对象所需的附加信息。

### 保存元素对象

随后使用名称 `item` 写入元素：

```cpp
ar << YGGR_SERIALIZATION_NAME_NVP("item", *it++);
```

元素顺序由无序容器的 `const_iterator` 决定，不表示稳定的排序顺序。

## `save_unordered_collection`

文件提供便捷函数：

```cpp
template<typename Archive, typename Container>
void save_unordered_collection(
    Archive& ar,
    const Container& container);
```

其实现创建对应的保存策略并调用：

```cpp
unordered_collections_saver_switch<Archive, Container> saver;
saver(ar, container);
```

该函数适合由具体无序容器的序列化接口转发调用。

## 容器要求

目标 `Container` 通常需要提供：

- `size()`；
- `bucket_count()`；
- `const_iterator`；
- `begin()`；
- `value_type`。

此外，`value_type` 应具备：

- Boost.Serialization 版本信息；
- 对应的 `save_construct_data_adl` 支持；
- 可通过归档接口保存的序列化实现。

## 与加载实现的对应关系

本文件输出的元数据和元素顺序应由对应加载实现读取：

- `count` 对应加载时的元素数量；
- `bucket_count` 对应加载时的桶数量；
- `item_version` 对应加载时传递给元素输入策略的版本；
- 每个 `item` 对应一个容器元素。

通用加载实现位于 `unordered_collections_load_imp.hpp`。

## BSON 集成

文件末尾包含：

```cpp
#include <yggr/serialization/unordered_collections_save_bson_impl.hpp>
```

该头文件为 BSON 输出归档提供专用 `unordered_collections_saver_switch` 特化。BSON 模式会将元数据和元素转换为数字字符串字段，并根据 `skip_auto_id` 选择是否写出集合级元数据。

因此：

- 本文件提供通用归档保存流程；
- BSON 专用头文件覆盖 BSON 输出归档行为；
- 具体元素构造数据仍通过 Boost.Serialization 的构造数据接口保存。

## 注意事项

- 无序容器的迭代顺序通常不稳定，不应依赖序列化后的元素顺序。
- `bucket_count` 是无序容器的运行时状态，加载端可能使用它进行容量预留。
- `item_version` 来自 `Container::value_type`，元素类型版本变化时应关注归档兼容性。
- `save_construct_data_adl` 的调用发生在元素对象保存之前，加载端应采用匹配的构造数据恢复流程。
- 本文件不直接定义具体容器的 `serialize` 重载，通常由其他容器序列化头文件调用。
