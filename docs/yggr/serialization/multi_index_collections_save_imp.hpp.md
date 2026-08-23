# `multi_index_collections_save_imp.hpp`

## 概述

该头文件提供多索引容器的通用序列化保存实现，核心是
`yggr::serialization::container::multi_index_collection_saver_switch`。

实现将容器整体保存流程与具体归档类型分离：

- 本文件负责保存集合数量、元素版本和元素遍历；
- 每个元素的构造数据由 `save_construct_data_adl()` 保存；
- 元素本体使用名称 `"item"` 写入；
- BSON 输出归档通过文件末尾的专用实现进行特化。

## 头文件

```cpp
#include <yggr/serialization/multi_index_collections_save_imp.hpp>
```

## 命名空间

实现位于：

```cpp
yggr::serialization::container
```

## 依赖

主要依赖：

- `yggr/serialization/nvp.hpp`：名称-值对保存；
- `yggr/serialization/collection_size_type.hpp`：集合大小类型；
- `boost/serialization/serialization.hpp`：基础序列化设施；
- `boost/serialization/version.hpp`：元素版本信息；
- `boost/serialization/item_version_type.hpp`：元素版本包装类型；
- `boost/ref.hpp`：元素地址获取支持。

## `multi_index_collection_saver_switch`

```cpp
template<typename Archive, typename Container>
struct multi_index_collection_saver_switch;
```

通用多索引容器保存器。

### 模板参数

| 参数 | 说明 |
| --- | --- |
| `Archive` | 输出归档类型。 |
| `Container` | 待保存的多索引容器类型。 |

### `operator()`

```cpp
void operator()(Archive& ar, const Container& s) const;
```

将容器 `s` 保存到归档 `ar`。

`Container` 通常需要提供：

```cpp
typedef value_type;
typedef const_iterator;
size_type size() const;
const_iterator begin() const;
```

## 保存流程

函数按以下顺序保存：

1. 使用 `s.size()` 创建集合大小对象；
2. 获取 `Container::value_type` 的序列化版本；
3. 保存集合数量；
4. 保存元素版本；
5. 按 `s.begin()` 返回的顺序遍历元素；
6. 对每个元素调用 `save_construct_data_adl()`；
7. 使用名称 `"item"` 保存元素本体；
8. 直到保存完全部元素。

核心逻辑等价于：

```cpp
collection_size_type count(s.size());
const boost::serialization::item_version_type item_version(
    boost::serialization::version<
        typename Container::value_type>::value);

ar << YGGR_SERIALIZATION_NVP(count);
ar << YGGR_SERIALIZATION_NVP(item_version);

for(; count-- > 0;)
{
    boost::serialization::save_construct_data_adl(
        ar,
        boost::addressof(*it),
        boost::serialization::version<
            typename Container::value_type>::value);
    ar << YGGR_SERIALIZATION_NAME_NVP("item", *it++);
}
```

## `save_multi_index_collection`

```cpp
template<typename Archive, typename Container>
void save_multi_index_collection(
    Archive& ar,
    const Container& s);
```

创建对应的保存器并执行：

```cpp
multi_index_collection_saver_switch<Archive, Container> saver;
saver(ar, s);
```

该函数为上层多索引容器序列化代码提供统一入口。

## 元素版本和构造数据

元素版本通过以下特征取得：

```cpp
boost::serialization::version<
    typename Container::value_type>::value
```

每个元素正式保存之前，都会调用：

```cpp
boost::serialization::save_construct_data_adl(
    ar,
    boost::addressof(*it),
    element_version);
```

这允许具有特殊构造数据要求的元素类型保存必要的初始化信息。

## 通用归档布局

默认归档通常包含以下逻辑数据：

| 数据 | 内容 |
| --- | --- |
| `count` | 多索引容器元素数量。 |
| `item_version` | 元素类型版本。 |
| `item` | 按容器迭代顺序保存的元素。 |

多索引容器可能包含多个索引，但本实现只保存容器元素，不单独保存索引结构；索引通常由目标容器在加载时重建。

## BSON 分派

文件末尾包含：

```cpp
#include <yggr/serialization/multi_index_collections_save_bson_impl.hpp>
```

对于 BSON 输出归档，会选择专用保存器：

- `bson_oarchive<false>`：使用数字字符串字段保存数量、元素版本和元素；
- `bson_oarchive<true>`：省略数量和元素版本，直接保存数字字符串元素。

因此 BSON 归档的字段布局可能不同于通用归档的 `count`、`item_version`、`item` 逻辑名称。

## 使用示例

```cpp
#include <yggr/serialization/multi_index_collections_save_imp.hpp>

void save_multi_index(
    my_archive& archive,
    const my_multi_index_container& container)
{
    yggr::serialization::container::save_multi_index_collection<
        my_archive,
        my_multi_index_container>(archive, container);
}
```

## 注意事项

1. 容器的 `size()` 应与从 `begin()` 开始可遍历的元素数量一致。
2. 元素类型必须满足 Boost.Serialization 的版本、构造数据和保存要求。
3. 元素构造数据会在元素本体之前保存。
4. 本实现按容器迭代顺序保存元素，不直接保存多索引结构。
5. 多索引容器加载端需要根据保存数据重新建立各个索引。
6. BSON 专用实现会覆盖 BSON 归档的默认保存行为，字段布局可能与通用归档不同。
7. `save_multi_index_collection()` 的元素保存细节由 `Input` 之外的归档类型和元素类型共同决定。
8. 归档读取端必须与保存端使用兼容的数量、版本、构造数据和字段布局约定。

## 相关组件

- `yggr/serialization/multi_index_collections_save_bson_impl.hpp`
- `yggr/serialization/multi_index_collections_load_imp.hpp`
- `yggr/serialization/collection_size_type.hpp`
- `yggr/serialization/nvp.hpp`
- `boost/serialization/serialization.hpp`
- `boost/serialization/version.hpp`
- `boost/serialization/item_version_type.hpp`
