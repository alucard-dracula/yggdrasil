# `multi_index_collections_save_bson_impl.hpp`

## 概述

该头文件为 BSON 输出归档提供多索引容器保存实现，特化
`yggr::serialization::container::multi_index_collection_saver_switch`。

主要职责包括：

- 保存多索引容器元素数量；
- 保存元素版本；
- 保存元素构造数据；
- 按数字字符串字段名保存每个容器元素。

实现仅在定义 `YGGR_HAS_BSON_OARCHIVE` 时启用。

## 头文件

```cpp
#include <yggr/serialization/multi_index_collections_save_bson_impl.hpp>
```

该文件通常由多索引容器序列化组件间接包含，不建议业务代码直接依赖其内部特化。

## 编译条件

当定义以下宏时启用实现：

```cpp
YGGR_HAS_BSON_OARCHIVE
```

未定义该宏时，不生成 BSON 多索引容器保存特化，并设置内部阶段宏以配合包含控制逻辑。

## 命名空间

实现位于：

```cpp
yggr::serialization::container
```

## 类型特化

文件为两种 BSON 输出归档分别提供特化：

```cpp
template<typename Container>
struct multi_index_collection_saver_switch<
    yggr::archive::bson_oarchive<false>,
    Container>;

template<typename Container>
struct multi_index_collection_saver_switch<
    yggr::archive::bson_oarchive<true>,
    Container>;
```

模板参数 `Container` 是待保存的多索引容器类型，通常需要提供 `size()`、`begin()`、`const_iterator` 和 `value_type`。

## `operator()`

### `bson_oarchive<false>`

```cpp
void operator()(
    yggr::archive::bson_oarchive<false>& ar,
    const Container& s) const;
```

保存布局：

| 字段名 | 内容 |
| --- | --- |
| `"0"` | 集合元素数量。 |
| `"1"` | 元素版本。 |
| `"2"` 起 | 多索引容器元素。 |

### `bson_oarchive<true>`

```cpp
void operator()(
    yggr::archive::bson_oarchive<true>& ar,
    const Container& s) const;
```

不保存集合数量和元素版本，直接从字段 `"0"` 开始保存元素：

| 字段名 | 内容 |
| --- | --- |
| `"0"` | 第一个容器元素。 |
| `"1"` | 第二个容器元素。 |
| `"2"` 起 | 后续容器元素。 |

## 保存流程

两个特化的共同流程如下：

1. 根据 `s.size()` 创建 `collection_size_type count`；
2. 根据 `Container::value_type` 获取元素版本；
3. 初始化数字字段索引 `idx`；
4. 按归档模式保存数量和元素版本；
5. 从 `s.begin()` 开始遍历容器；
6. 对每个元素调用 `save_construct_data_adl()`；
7. 使用当前 `idx` 生成数字字符串字段名；
8. 保存元素并递增索引，直到处理完全部元素。

## 元素版本

元素版本通过以下表达式获取：

```cpp
boost::serialization::version<
    typename Container::value_type>::value
```

在 `bson_oarchive<false>` 中，版本包装为：

```cpp
boost::serialization::item_version_type item_version(...);
```

每个元素调用构造数据保存时使用元素类型版本，而不是容器版本：

```cpp
boost::serialization::save_construct_data_adl(
    ar,
    boost::addressof(*it),
    boost::serialization::version<
        typename Container::value_type>::value);
```

## 构造数据与元素保存

每个元素先保存构造数据，再保存元素本身：

```cpp
boost::serialization::save_construct_data_adl(
    ar,
    boost::addressof(*it),
    item_version);

ar << YGGR_SERIALIZATION_NAME_NVP(
    field_name,
    *it++);
```

这样可以支持需要额外构造数据的多索引元素类型。

## BSON 字段布局对比

| 归档类型 | 数量字段 | 元素版本字段 | 首个元素字段 |
| --- | --- | --- | --- |
| `bson_oarchive<false>` | `"0"` | `"1"` | `"2"` |
| `bson_oarchive<true>` | 不保存 | 不保存 | `"0"` |

加载端应使用匹配的字段布局和归档配置。

## 使用示例

```cpp
#include <yggr/serialization/multi_index_collections_save_impl.hpp>

void save_multi_index(
    yggr::archive::bson_oarchive<false>& archive,
    const my_multi_index_container& container)
{
    typedef yggr::serialization::container::
        multi_index_collection_saver_switch<
            yggr::archive::bson_oarchive<false>,
            my_multi_index_container> saver_type;

    saver_type saver;
    saver(archive, container);
}
```

实际项目中通常由多索引容器的上层序列化函数自动选择该特化。

## 与多索引加载的关系

对应的 BSON 加载实现是：

```text
yggr/serialization/multi_index_collections_load_bson_impl.hpp
```

保存和加载应保持以下约定一致：

- 数量字段和元素版本字段是否存在；
- 元素字段的起始索引；
- 元素版本类型；
- 构造数据及对象跟踪方式。

## 注意事项

1. 只有定义 `YGGR_HAS_BSON_OARCHIVE` 时，该特化才会参与编译。
2. `bson_oarchive<false>` 会保存数量和元素版本，`bson_oarchive<true>` 会省略这两项。
3. 元素字段名使用数字字符串，不是固定的 `"item"` 字段。
4. `Container::value_type` 必须满足 Boost.Serialization 的版本、构造数据和元素保存要求。
5. 保存次数由 `s.size()` 决定，容器大小与迭代器范围应保持一致。
6. 多索引容器通常具有多个索引，但这里只按主容器迭代顺序保存元素，不单独保存各索引结构。
7. `save_construct_data_adl()` 在每个元素本体之前调用，元素类型应满足相应构造数据约定。
8. BSON 输入端必须匹配当前输出模式，否则可能将数量、版本或元素误读为其他字段。
9. 具体 BSON 类型、字段写入和异常行为由 BSON 归档与 Boost.Serialization 共同决定。

## 相关组件

- `yggr/serialization/multi_index_collections_load_bson_impl.hpp`
- `yggr/serialization/collections_save_bson_impl.hpp`
- `yggr/serialization/collection_size_type.hpp`
- `yggr/serialization/array_bson_save_impl.hpp`
- `yggr/serialization/nvp.hpp`
- `boost/archive/detail/oserializer.hpp`
- `boost/serialization/version.hpp`
- `yggr/archive/bson_oarchive.hpp`
