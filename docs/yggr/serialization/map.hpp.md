# `map.hpp`

## 概述

该头文件是 yggdrasil 对关联容器进行序列化的统一入口，覆盖 map 和 multimap：

- `std::map`；
- `std::multimap`；
- `boost::container::map`；
- `boost::container::multimap`；
- Boost 1.56 及以上版本的 yggdrasil 内部 map/multimap 实现。

文件通过键值容器序列化宏生成保存、加载和分离式序列化函数，并接入 BSON 类型适配。

## 头文件

```cpp
#include <yggr/serialization/map.hpp>
```

## 与 Boost 原生头文件的互斥

如果已经定义：

```cpp
BOOST_SERIALIZATION_MAP_HPP
```

文件会触发编译错误，并要求不要同时包含：

```cpp
<boost/serialization/map.hpp>
```

应使用 yggdrasil 提供的关联容器序列化实现。

## 依赖

该文件包含：

```cpp
#include <yggr/serialization/key_value_container.hpp>
#include <yggr/container/map.hpp>
```

并使用：

```cpp
#include <yggr/serialization/detail/container_implementation_level_def.hpp>
```

文件末尾接入：

```cpp
#include <yggr/serialization/map_bson_impl.hpp>
```

## 保存与加载注册

### 标准库容器

```cpp
YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(4, std::map)
YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(4, std::multimap)
```

`4` 对应标准库 map 类模板的四个模板参数。

### Boost.Container 容器

Boost 版本低于 1.56 时：

```cpp
YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(4, boost::container::map)
YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(4, boost::container::multimap)
```

Boost 版本不低于 1.56 时：

```cpp
YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(5, boost::container::map)
YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(5, boost::container::multimap)
```

该分支用于适配 Boost.Container map 模板参数数量变化。

### yggdrasil 内部容器

Boost 版本不低于 1.56 时，额外注册：

```cpp
YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(
    4, yggr::container::detail::map)
YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD(
    4, yggr::container::detail::multimap)
```

## 容器实现级别

文件将支持的关联容器统一设置为：

```cpp
object_serializable
```

示例：

```cpp
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    4, std::map, object_serializable)
```

这表示容器按可序列化对象处理，而不是简单原始值。

## 序列化流程

通过 `key_value_container.hpp` 生成的接口使用以下流程：

```text
serialize()
    -> split_free()
    -> save() 或 load()
```

保存时通常会：

1. 保存元素数量；
2. 保存元素版本；
3. 保存每个键值元素的构造数据；
4. 保存键和值。

加载时通常会：

1. 读取元素数量；
2. 按元素版本构造临时键值对象；
3. 插入目标 map 或 multimap；
4. 修正对象地址跟踪关系。

具体实现由集合保存/加载和键值容器实现提供。

## BSON 支持

文件在主 include guard 结束后包含：

```cpp
#include <yggr/serialization/map_bson_impl.hpp>
```

启用 BSON 输入或输出归档后，BSON 扩展会：

- 将 map 和 multimap 注册为 BSON 数组类型；
- 根据 Boost 版本适配模板参数数量；
- 为键值元素提供 BSON 输入处理。

## Boost 版本兼容

关联容器注册使用以下条件：

```cpp
#if (BOOST_VERSION < 105600)
```

因此：

- Boost 1.56 之前：Boost.Container map/multimap 按 4 个模板参数注册；
- Boost 1.56 及以上：Boost.Container map/multimap 按 5 个模板参数注册；
- Boost 1.56 及以上：启用 yggdrasil 内部 map/multimap 注册。

## 使用示例

```cpp
#include <yggr/serialization/map.hpp>
#include <map>

void prepare_map()
{
    std::map<int, std::string> values;
    values[1] = "one";
    values[2] = "two";

    // 将 values 交给 Boost.Serialization 或项目 BSON 归档。
}
```

multimap 示例：

```cpp
#include <yggr/serialization/map.hpp>
#include <map>

void prepare_multimap()
{
    std::multimap<int, std::string> values;
    values.insert(std::make_pair(1, "one"));
    values.insert(std::make_pair(1, "another"));
}
```

## 注意事项

1. 不要同时包含 Boost 原生 `boost/serialization/map.hpp`，否则会触发显式编译错误。
2. `map` 和 `multimap` 使用键值容器序列化逻辑，不使用线性容器的 `push_back()` 路径。
3. 宏名称中的 `KEY_VALUE_CONTINER` 是源码既有拼写，不要自行改写。
4. Boost.Container map/multimap 在 Boost 1.56 前后模板参数数量不同，升级 Boost 后应验证注册宏。
5. yggdrasil 内部 map/multimap 仅在 Boost 1.56 及以上版本注册。
6. 容器实现级别为 `object_serializable`，键和值类型仍必须满足对应归档的序列化要求。
7. BSON 类型注册将关联容器表示为数组，不代表其内部节点或键值对具有连续内存布局。
8. BSON 键值元素加载依赖 `key_value_container_bson_impl.hpp` 的字段和对象跟踪约定。
9. 输入端和输出端应使用兼容的容器实现、模板参数配置、元素版本和 BSON 归档格式。

## 相关组件

- `yggr/serialization/key_value_container.hpp`
- `yggr/serialization/key_value_container_bson_impl.hpp`
- `yggr/serialization/map_bson_impl.hpp`
- `yggr/serialization/collections_save_imp.hpp`
- `yggr/serialization/collections_load_imp.hpp`
- `yggr/serialization/detail/container_implementation_level_def.hpp`
- `yggr/container/map.hpp`
- `std::map`
- `std::multimap`
- `boost::container::map`
- `boost::container::multimap`
