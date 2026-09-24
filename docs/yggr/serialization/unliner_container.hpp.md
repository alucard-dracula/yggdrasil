# `unliner_container.hpp`

## 概述

该头文件为非线性容器提供通用 Boost.Serialization 接口生成宏。它本身不直接实现某一种具体容器，而是通过预处理器宏为指定模板容器生成 `save`、`load` 和 `serialize` 函数。

生成的实现统一使用 Yggdrasil 的集合序列化基础设施：

- 保存使用 `yggr::serialization::container::save_collection`；
- 加载使用 `yggr::serialization::container::load_collection`；
- 加载元素使用 `stl::archive_input_set`；
- 容器容量处理使用 `stl::no_reserve_imp`；
- 统一入口使用 `boost::serialization::split_free`。

## 头文件

```cpp
#include <yggr/serialization/unliner_container.hpp>
```

主要依赖包括：

```cpp
#include <yggr/ppex/typedef.hpp>
#include <yggr/serialization/collections_save_imp.hpp>
#include <yggr/serialization/collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>
```

## 接口生成宏

文件定义以下宏：

```cpp
YGGR_SERIALIZATION_UNLINER_CONTINER_SAVE_LOAD(__template_n__, __type__)
```

宏名称中的 `CONTINER` 是源代码中既有的拼写形式，使用时应保持完全一致。

参数含义如下：

| 参数 | 含义 |
| --- | --- |
| `__template_n__` | 容器模板参数的数量 |
| `__type__` | 容器模板名称，例如某个具有一个或多个类型参数的容器 |

宏展开后会为 `__type__<T...>` 生成三个函数模板。

## 生成的保存函数

```cpp
template<typename Archive, ...>
void save(
    Archive& ar,
    const __type__<T...>& value,
    const unsigned int version);
```

生成的 `save` 函数调用：

```cpp
yggr::serialization::container::save_collection<
    Archive,
    __type__<T...>
>(ar, value);
```

具体元素遍历、元素归档以及集合格式由 `save_collection` 统一处理。宏生成的保存函数不使用 `version` 参数。

## 生成的加载函数

```cpp
template<typename Archive, ...>
void load(
    Archive& ar,
    __type__<T...>& value,
    const unsigned int version);
```

加载函数调用：

```cpp
yggr::serialization::container::load_collection<
    Archive,
    __type__<T...>,
    yggr::serialization::stl::archive_input_set<
        Archive,
        __type__<T...>
    >,
    yggr::serialization::stl::no_reserve_imp<
        __type__<T...>
    >
>(ar, value);
```

其中：

- `archive_input_set<Archive, Container>` 负责从归档读取元素并插入容器；
- `no_reserve_imp<Container>` 表示加载前不执行容器容量预留；
- `load_collection` 负责统一的集合读取流程。

该配置适合没有统一 `reserve` 接口，或该容器加载路径不要求预分配容量的非线性容器。

## 生成的序列化函数

```cpp
template<typename Archive, ...>
void serialize(
    Archive& ar,
    __type__<T...>& value,
    const unsigned int file_version);
```

该函数通过 `boost::serialization::split_free` 将归档操作分派给生成的 `save` 或 `load` 函数：

```cpp
boost::serialization::split_free(ar, value, file_version);
```

## 使用方式

具体容器的序列化头文件可以先包含本文件，再调用宏生成接口。例如，具有两个模板类型参数的容器可按以下形式使用：

```cpp
YGGR_SERIALIZATION_UNLINER_CONTINER_SAVE_LOAD(2, container_type)
```

实际使用时，`container_type` 必须是适合作为模板模板参数展开的容器模板，并且其元素类型和插入操作应满足 `load_collection` 与 `archive_input_set` 的要求。

## 设计特点

- 使用宏统一生成重复的模板序列化代码；
- 保存路径统一委托给 `save_collection`；
- 加载路径统一委托给 `load_collection`；
- 输入元素通过 `archive_input_set` 插入目标容器；
- 不依赖容器的 `reserve` 操作；
- 使用 `split_free` 区分保存和加载逻辑；
- 容器具体类型、模板参数数量和归档类型均在实例化时确定。

## 与 BSON 支持的关系

本文件只提供通用的非线性容器序列化接口生成，不直接定义 BSON 类型特征或 BSON 专用输入逻辑。使用 BSON 输入归档时，具体容器可能通过专用实现头文件提供 `archive_input_set` 特化，例如：

```cpp
#include <yggr/serialization/unliner_container_bson_impl.hpp>
```

因此，BSON 容器的实际元素读取行为取决于归档类型对应的 `archive_input_set` 实现。

## 注意事项

- `__template_n__` 必须与容器模板实际需要的类型参数数量一致。
- 容器必须满足 `save_collection`、`load_collection` 以及对应输入插入器的接口要求。
- `no_reserve_imp` 不会为目标容器预留容量，容器较大时可能影响加载性能。
- 该文件中的宏定义属于内部序列化基础设施，通常由具体容器序列化头文件调用。
