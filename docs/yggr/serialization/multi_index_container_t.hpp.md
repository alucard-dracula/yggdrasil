# `multi_index_container_t.hpp`

## 概述

该头文件为 Boost.MultiIndex 容器提供通用序列化支持，主要负责：

- 定义多索引元素的加载适配器；
- 通过宏生成容器的 `save`、`load` 和 `serialize` 自由函数；
- 将整体保存和加载委托给多索引集合实现；
- 使用统一插入器将元素重新插入多索引容器。

## 头文件

```cpp
#include <yggr/serialization/multi_index_container_t.hpp>
```

## 命名空间

实现位于：

```cpp
yggr::serialization::container
```

生成的 Boost.Serialization 自由函数通常应位于：

```cpp
boost::serialization
```

## 依赖

主要依赖：

- `multi_index_collections_save_imp.hpp`：多索引容器整体保存；
- `multi_index_collections_load_imp.hpp`：多索引容器整体加载；
- `multi_index_container_inserter.hpp`：元素插入适配；
- `boost/serialization/split_free.hpp`：分离式保存/加载；
- `boost/serialization/detail/stack_constructor.hpp`：临时元素构造；
- `yggr/ppex/typedef.hpp`：生成模板参数列表。

## `archive_input_multi_index`

```cpp
template<typename Archive, typename Container>
struct archive_input_multi_index;
```

该适配器负责从归档中读取一个多索引元素，并将其插入目标容器。

### `operator()`

```cpp
void operator()(
    Archive& ar,
    Container& s,
    const unsigned int v) const;
```

参数说明：

| 参数 | 说明 |
| --- | --- |
| `ar` | 输入归档。 |
| `s` | 目标多索引容器。 |
| `v` | 元素序列化版本。 |

### 处理流程

1. 获取 `Container::value_type`；
2. 使用 `stack_construct<Archive, type>` 按版本 `v` 构造临时元素；
3. 从名称 `"item"` 读取元素；
4. 调用 `back_insert(s, t.reference())` 插入目标容器；
5. 插入成功时调用 `reset_object_address()` 修正对象地址跟踪。

核心逻辑：

```cpp
boost::serialization::detail::stack_construct<
    Archive,
    typename Container::value_type> t(ar, v);

ar >> YGGR_SERIALIZATION_NAME_NVP(
    "item",
    t.reference());

std::pair<typename Container::const_iterator, bool> result =
    back_insert(s, t.reference());
```

如果插入失败，不会执行对象地址跟踪重置。

## 宏

### `YGGR_SERIALIZATION_MULTI_INDEX_CONTINER_SAVE_LOAD`

```cpp
#define YGGR_SERIALIZATION_MULTI_INDEX_CONTINER_SAVE_LOAD(
    __template_n__, __type__)
```

注意：`CONTINER` 是源码中的既有拼写，调用时必须使用该名称。

参数：

| 参数 | 说明 |
| --- | --- |
| `__template_n__` | 目标容器模板参数数量。 |
| `__type__` | 多索引容器模板名称。 |

该宏为指定容器生成 `save`、`load` 和 `serialize` 三组函数。

## 生成的 `save`

宏生成的保存函数将调用：

```cpp
yggr::serialization::container::save_multi_index_collection(
    ar,
    t);
```

整体保存流程由 `multi_index_collection_saver_switch` 负责，包括元素数量、元素版本、构造数据和元素本体。

## 生成的 `load`

宏生成的加载函数将调用：

```cpp
yggr::serialization::container::load_multi_index_collection(
    ar,
    t);
```

加载时使用：

```cpp
yggr::serialization::container::archive_input_multi_index<
    Archive,
    Container>
```

由该适配器负责单个元素读取和插入。

## 生成的 `serialize`

宏生成的序列化函数使用：

```cpp
boost::serialization::split_free(ar, t, file_version);
```

因此保存和加载会分别分派给宏生成的 `save()` 与 `load()`。

## 使用示例

```cpp
#include <yggr/serialization/multi_index_container_t.hpp>
#include <boost/multi_index_container.hpp>

namespace boost
{
namespace serialization
{

YGGR_SERIALIZATION_MULTI_INDEX_CONTINER_SAVE_LOAD(
    3,
    boost::multi_index::multi_index_container)

} // namespace serialization
} // namespace boost
```

实际模板参数数量和容器注册方式应与当前 Boost.MultiIndex 版本及项目宏约定一致。

## 序列化流程

```text
serialize()
    -> split_free()
    -> save() 或 load()
```

保存：

```text
save()
    -> save_multi_index_collection()
    -> multi_index_collection_saver_switch
```

加载：

```text
load()
    -> load_multi_index_collection()
    -> multi_index_collection_loader_switch
    -> archive_input_multi_index
    -> back_insert()
```

## BSON 支持

当归档类型为 BSON 输入归档时，`multi_index_container_t_bson_impl.hpp` 会提供
`archive_input_multi_index` 的专用特化：

- 使用数字字符串字段索引读取元素；
- 将元素版本和索引传入加载器；
- 通过相同的 `back_insert()` 逻辑插入容器。

## 注意事项

1. 宏名称中的 `CONTINER` 是源码既有拼写，不要改写为 `CONTAINER`。
2. `__template_n__` 必须与目标多索引容器模板参数数量匹配。
3. 生成的 `serialize()` 使用 `split_free()`，保存和加载函数必须同时可见。
4. 元素加载通过 `stack_construct` 完成，元素类型必须满足对应归档的构造和反序列化要求。
5. 插入通过多索引容器第 0 个索引执行，唯一索引冲突可能导致插入失败。
6. 插入失败时不会调用 `reset_object_address()`，调用方应根据插入结果进行相应处理。
7. 多索引结构通常不单独序列化，加载元素时由 Boost.MultiIndex 重建其他索引。
8. BSON 输入特化会覆盖通用元素加载字段布局，输入输出归档应使用兼容配置。
9. 具体元素版本、对象跟踪和异常行为由 Boost.Serialization、归档类型及元素类型共同决定。

## 相关组件

- `yggr/serialization/multi_index_collections_save_imp.hpp`
- `yggr/serialization/multi_index_collections_load_imp.hpp`
- `yggr/serialization/multi_index_container_bson_impl.hpp`
- `yggr/serialization/multi_index_container_t_bson_impl.hpp`
- `yggr/serialization/multi_index_container_inserter.hpp`
- `boost/serialization/split_free.hpp`
- `boost/multi_index_container.hpp`
