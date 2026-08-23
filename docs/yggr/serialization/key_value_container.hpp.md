# `key_value_container.hpp`

## 概述

该头文件提供键值容器序列化的宏生成工具，用于为 map 类关联容器生成 Boost.Serialization 所需的 `save`、`load` 和 `serialize` 自由函数。

文件本身不直接为具体容器实例化序列化函数，实际容器需要在其他头文件中调用宏。

## 头文件

```cpp
#include <yggr/serialization/key_value_container.hpp>
```

## 依赖

主要依赖：

- `yggr/serialization/utility.hpp`；
- `yggr/serialization/collections_save_imp.hpp`；
- `yggr/serialization/collections_load_imp.hpp`；
- `boost/serialization/split_free.hpp`；
- `yggr/ppex/typedef.hpp`：生成模板参数列表。

## 宏

### `YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD`

```cpp
#define YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(
    __template_n__, __type__)
```

注意：`CONTINER` 是源码中的既有拼写，调用时必须使用该名称。

参数：

| 参数 | 说明 |
| --- | --- |
| `__template_n__` | 容器模板参数数量。 |
| `__type__` | 容器模板名称，例如 `std::map`。 |

宏会为指定容器生成三个序列化函数模板。

## 生成的 `save`

宏生成的函数逻辑等价于：

```cpp
template<typename Archive, typename ...Types>
void save(
    Archive& ar,
    const Container<Types...>& value,
    const unsigned int file_version)
{
    yggr::serialization::container::save_collection<
        Archive,
        Container<Types...> >(ar, value);
}
```

`file_version` 当前未使用。实际保存工作委托给通用的
`yggr::serialization::container::save_collection()`。

键值元素的构造数据、元素版本和集合数量由集合保存实现负责处理。

## 生成的 `load`

宏生成的函数逻辑等价于：

```cpp
template<typename Archive, typename ...Types>
void load(
    Archive& ar,
    Container<Types...>& value,
    const unsigned int file_version)
{
    yggr::serialization::container::load_collection<
        Archive,
        Container<Types...>,
        yggr::serialization::stl::archive_input_map<
            Archive,
            Container<Types...> >,
        yggr::serialization::stl::no_reserve_imp<
            Container<Types...> > >(ar, value);
}
```

加载过程使用：

- `archive_input_map`：读取并插入键值元素；
- `no_reserve_imp`：表示该容器不使用预留容量操作；
- `load_collection()`：执行集合数量读取和元素加载流程。

## 生成的 `serialize`

宏生成的函数逻辑等价于：

```cpp
template<typename Archive, typename ...Types>
void serialize(
    Archive& ar,
    Container<Types...>& value,
    const unsigned int file_version)
{
    boost::serialization::split_free(
        ar,
        value,
        file_version);
}
```

该函数使用 `split_free()` 将保存和加载分别分派到宏生成的 `save()` 与 `load()`。

## 使用示例

以下示例为 `std::map` 生成序列化函数：

```cpp
#include <map>
#include <yggr/serialization/key_value_container.hpp>

namespace boost
{
namespace serialization
{

YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(2, std::map)

} // namespace serialization
} // namespace boost
```

`2` 表示 `std::map<Key, Value, Compare, Allocator>` 的模板参数数量应与项目宏约定一致；实际使用时应以项目对 `YGGR_PP_TEMPLATE_PARAMS_TYPES` 的定义为准。

## 处理流程

### 保存

```text
serialize()
    -> split_free()
    -> save()
    -> save_collection()
```

### 加载

```text
serialize()
    -> split_free()
    -> load()
    -> load_collection()
    -> archive_input_map
```

## 与 BSON 支持的关系

通用 `save()` 和 `load()` 的归档类型可以是 BSON 归档。

当 BSON 输入归档启用时，`archive_input_map` 会使用
`key_value_container_bson_impl.hpp` 中的特化，按数字字符串字段名读取键值元素，并处理对象地址跟踪。

保存端的具体 BSON 行为由集合 BSON 保存实现和归档类型分派决定。

## 注意事项

1. 宏名称中的 `CONTINER` 是源码既有拼写，不要改写为 `CONTAINER`。
2. `__template_n__` 必须与目标容器宏模板参数数量匹配。
3. 宏应在 `boost::serialization` 命名空间中使用，以生成正确的自由函数。
4. 生成的 `serialize()` 使用 `split_free()`，因此保存和加载函数必须同时可见。
5. 目标容器必须满足 `save_collection()` 和 `load_collection()` 的接口要求。
6. 键值容器的元素类型需要支持构造数据、键和值的序列化。
7. `load()` 使用 `no_reserve_imp`，不会对关联容器执行顺序容器式的容量预留。
8. `file_version` 参数目前未被保存和加载函数直接使用。
9. BSON 输入归档的字段布局和对象跟踪行为由对应 BSON 特化实现决定。

## 相关组件

- `yggr/serialization/collections_save_imp.hpp`
- `yggr/serialization/collections_load_imp.hpp`
- `yggr/serialization/key_value_container_bson_impl.hpp`
- `yggr/serialization/utility.hpp`
- `yggr/ppex/typedef.hpp`
- `boost/serialization/split_free.hpp`
