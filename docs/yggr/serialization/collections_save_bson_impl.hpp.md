# `collections_save_bson_impl.hpp`

## 概述

该头文件为 BSON 输出归档提供通用容器保存实现，特化
`yggr::serialization::container::collections_saver_switch`，支持将具有标准容器接口的对象写入 BSON。

实现要求容器提供：

- `size()`；
- `begin()`；
- `end()`；
- `const_iterator`；
- `value_type`。

## 头文件

```cpp
#include <yggr/serialization/collections_save_bson_impl.hpp>
```

该文件通常由 BSON 输出归档组件间接包含，不建议直接依赖其内部特化。

## 编译条件

实现仅在定义以下宏时启用：

```cpp
YGGR_HAS_BSON_OARCHIVE
```

启用后定义内部包含保护宏，并包含名称-值对、数组 BSON 保存和 Boost 引用支持。

未启用 BSON 输出归档时，不生成集合保存特化。

## 命名空间

实现位于：

```cpp
yggr::serialization::container
```

并特化 `collections_saver_switch`。

## `bson_oarchive<false>` 特化

```cpp
template<typename Container>
struct collections_saver_switch<
    yggr::archive::bson_oarchive<false>, Container>;
```

调用接口：

```cpp
void operator()(
    yggr::archive::bson_oarchive<false>& ar,
    const Container& s) const;
```

### 保存布局

该模式按数字字符串字段保存以下元数据和元素：

| 字段名 | 内容 |
| --- | --- |
| `"0"` | 集合元素数量。 |
| `"1"` | 元素类型版本。 |
| `"2"` 起 | 集合元素。 |

### 处理流程

1. 使用 `s.size()` 构造 `collection_size_type count`；
2. 获取 `Container::value_type` 的 Boost.Serialization 版本；
3. 将元素版本保存为 `item_version_type`；
4. 从容器起始迭代器开始遍历；
5. 对每个元素调用 `save_construct_data_adl()` 保存构造数据；
6. 使用递增的数字字符串字段名保存元素本身；
7. 直到保存完 `count` 个元素。

## `bson_oarchive<true>` 特化

```cpp
template<typename Container>
struct collections_saver_switch<
    yggr::archive::bson_oarchive<true>, Container>;
```

调用接口：

```cpp
void operator()(
    yggr::archive::bson_oarchive<true>& ar,
    const Container& s) const;
```

### 保存布局

该模式不额外保存集合数量和元素版本，直接按数字字符串字段保存元素：

| 字段名 | 内容 |
| --- | --- |
| `"0"` | 第一个集合元素。 |
| `"1"` | 第二个集合元素。 |
| `"2"` 起 | 后续集合元素。 |

### 处理流程

1. 使用 `s.size()` 构造遍历计数；
2. 获取 `Container::value_type` 的元素版本；
3. 从容器起始迭代器开始遍历；
4. 对每个元素调用 `save_construct_data_adl()`；
5. 使用递增的数字字符串字段名保存元素；
6. 直到保存完所有元素。

## 元素版本与构造数据

元素版本通过以下表达式获取：

```cpp
boost::serialization::version<
    typename Container::value_type>::value
```

每个元素在保存自身之前调用：

```cpp
boost::serialization::save_construct_data_adl(
    ar,
    boost::addressof(*it),
    item_version);
```

这允许具有特殊构造数据要求的元素类型在正式序列化前保存必要的构造信息。

## 使用示例

```cpp
#include <yggr/serialization/collections_save_impl.hpp>
#include <vector>

void save_collection(const std::vector<int>& values)
{
    // 将 values 交给项目的 BSON 输出归档。
    // bson_oarchive<false> 会先保存数量和元素版本。
}
```

## 格式对比

| 归档类型 | 数量字段 | 元素版本字段 | 首个元素字段 |
| --- | --- | --- | --- |
| `bson_oarchive<false>` | `"0"` | `"1"` | `"2"` |
| `bson_oarchive<true>` | 不保存 | 不保存 | `"0"` |

使用输入归档读取时，必须确保加载实现采用与输出模式一致的 BSON 字段布局。

## 注意事项

1. 只有定义 `YGGR_HAS_BSON_OARCHIVE` 时，集合保存特化才会参与编译。
2. 容器必须提供标准的大小、迭代器和元素类型接口。
3. `bson_oarchive<false>` 会保存集合数量和元素版本，`bson_oarchive<true>` 会省略这两项元数据。
4. 元素保存前会调用 `save_construct_data_adl()`，元素类型应满足对应的 Boost.Serialization 构造数据约定。
5. 保存过程使用 `s.size()` 作为遍历次数，因此容器迭代器范围应与 `size()` 保持一致。
6. 元素字段名为数字字符串，不是传统的 `"item"` 或 `"elems"` 字段。
7. `collection_size_type` 和 `item_version_type` 的 BSON 类型及序列化行为由项目其他组件决定。
8. `skip_auto_id` 两种模式的 BSON 布局不同，输入输出归档配置必须匹配。
9. 具体元素序列化行为由元素类型和 BSON 归档实现共同决定。

## 相关组件

- `yggr/serialization/nvp.hpp`
- `yggr/serialization/collection_size_type.hpp`
- `yggr/serialization/array_bson_save_impl.hpp`
- `boost/archive/detail/oserializer.hpp`
- `boost/serialization/collections_save_imp.hpp`
- `boost/serialization/version.hpp`
- `boost/serialization/serialization.hpp`
