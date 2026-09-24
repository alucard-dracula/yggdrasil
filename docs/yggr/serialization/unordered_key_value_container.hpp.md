# `unordered_key_value_container.hpp`

## 概述

该头文件为无序键值容器提供通用的 Boost.Serialization 支持。它定义无序映射和无序多重映射的元素输入策略，并通过宏为具体容器生成 `save`、`load` 和 `serialize` 函数。

该文件不绑定某一种归档格式。集合级保存和加载分别委托给无序集合通用实现，单个元素则根据容器是否允许重复键选择不同的输入策略。

## 头文件

```cpp
#include <yggr/serialization/unordered_key_value_container.hpp>
```

具体容器头通常负责传入容器模板，例如 `std::unordered_map`、`std::unordered_multimap`、`boost::unordered_map` 和 `boost::unordered_multimap`。

## 依赖

主要依赖包括：

```cpp
#include <yggr/base/yggrdef.h>
#include <boost/config.hpp>
#include <yggr/ppex/typedef.hpp>
#include <yggr/serialization/utility.hpp>
#include <yggr/serialization/unordered_collections_save_imp.hpp>
#include <yggr/serialization/unordered_collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/detail/stack_constructor.hpp>
#include <boost/ref.hpp>
```

这些依赖分别提供 Yggdrasil 基础定义、预处理器模板参数工具、归档命名值对支持、无序集合保存/加载流程、自由函数序列化拆分、临时对象构造以及对象地址处理能力。

## 元素输入策略

两个策略均位于以下命名空间：

```cpp
yggr::serialization::container
```

### `archive_input_unordered_key_value`

该策略用于不允许重复键的无序映射容器：

```cpp
template<typename Archive, typename Container>
struct archive_input_unordered_key_value
{
    void operator()(
        Archive& ar,
        Container& container,
        const unsigned int version) const;
};
```

处理步骤如下：

1. 使用 `Container::value_type` 和 `stack_construct` 创建临时键值对象。
2. 以固定字段名 `"item"` 从归档读取对象。
3. 调用 `container.insert` 插入对象。
4. 当插入成功时，调用 `reset_object_address`，把临时对象中的 `second` 地址映射到容器内实际元素的 `second` 地址。

核心逻辑为：

```cpp
boost::serialization::detail::stack_construct<Archive, type>
    value_construct(ar, version);
ar >> YGGR_SERIALIZATION_NAME_NVP(
    "item", value_construct.reference());
std::pair<typename Container::const_iterator, bool> result =
    container.insert(value_construct.reference());
```

只有 `result.second` 为 `true` 时才进行地址修复，因为重复键会导致普通无序映射插入失败。

### `archive_input_unordered_multi_key_value`

该策略用于允许重复键的无序多重映射容器：

```cpp
template<typename Archive, typename Container>
struct archive_input_unordered_multi_key_value
{
    void operator()(
        Archive& ar,
        Container& container,
        const unsigned int version) const;
};
```

它同样先读取临时键值对象，再调用 `insert`。由于多重映射允许重复键，`insert` 返回插入元素的迭代器：

```cpp
typename Container::const_iterator result =
    container.insert(value_construct.reference());
```

随后将归档对象地址重置为新插入元素的地址。该策略不需要判断插入是否成功，因为每次插入都会产生一个元素。

## 对象地址修复

元素读取时先存储在 `stack_construct` 创建的临时对象中。插入无序容器后，容器中的实际对象通常位于不同地址，因此输入策略调用 `ar.reset_object_address` 更新 Boost.Serialization 的对象跟踪关系。

普通无序映射修复映射值地址：

```cpp
ar.reset_object_address(
    boost::addressof(result.first->second),
    boost::addressof(value_construct.reference().second));
```

无序多重映射将插入元素的值地址与临时对象关联：

```cpp
ar.reset_object_address(
    boost::addressof(result->second),
    boost::addressof(value_construct.reference()));
```

源码中的地址修复假设映射的 `value_type` 未被归档跟踪；这是无序键值容器通常采用的处理方式。

## `YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTINER_SAVE_LOAD`

该宏为一个无序键值容器模板生成三类自由函数。宏名称中的 `CONTINER` 拼写是源码中既有名称，使用时必须保持不变。

```cpp
YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTINER_SAVE_LOAD(
    template_parameter_count,
    container_template)
```

宏的两个参数含义如下：

| 参数 | 含义 |
| --- | --- |
| `__template_n__` | 容器模板参数数量，例如键值容器使用 `5` |
| `__type__` | 容器模板名称，例如 `std::unordered_map` |

### 生成的 `save`

生成的 `save` 函数将容器转交给：

```cpp
yggr::serialization::container::save_unordered_collection<
    Archive,
    Container>(ar, container);
```

具体的元素数量、桶信息和元素保存格式由无序集合保存实现决定。

### 生成的 `load`

生成的 `load` 函数将容器转交给：

```cpp
yggr::serialization::container::load_unordered_collection<
    Archive,
    Container,
    InputFunction>(ar, container);
```

其中 `InputFunction` 通过 `is_multi_container<Container>` 选择：

- 多重容器使用 `archive_input_unordered_multi_key_value`；
- 其他容器使用 `archive_input_unordered_key_value`。

这种分派将集合级加载与具体的键值插入语义分开。

### 生成的 `serialize`

生成的 `serialize` 函数使用 Boost.Serialization 的自由函数拆分机制：

```cpp
boost::serialization::split_free(ar, container, file_version);
```

因此 `save` 和 `load` 会根据归档方向分别调用，而不是由单一函数同时处理输入和输出。

## 具体容器的使用方式

`unordered_map.hpp` 使用该宏为以下容器生成序列化函数：

```cpp
YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTINER_SAVE_LOAD(
    5, std::unordered_map)
YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTINER_SAVE_LOAD(
    5, std::unordered_multimap)
YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTINER_SAVE_LOAD(
    5, boost::unordered_map)
YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTINER_SAVE_LOAD(
    5, boost::unordered_multimap)
```

标准库无序容器的注册受 `YGGR_NO_CXX11_HDR_UNORDERED_MAP` 控制；Boost 无序容器始终由对应的容器适配头注册。

具体容器头还会通过 `is_multi_container` 将 `unordered_multimap` 标记为多重容器，并设置容器的 Boost.Serialization 实现级别。

## BSON 集成

本文件本身不直接实现 BSON 字段读取，但其生成的通用 `save`/`load` 流程可被 BSON 特化接管：

- 通用无序集合加载器负责集合级流程；
- `unordered_key_value_container_bson_impl.hpp` 为 BSON 输入归档特化两个元素输入策略；
- BSON 特化使用元素索引生成数字字符串字段名，例如 `"0"`、`"1"`、`"2"`；
- 具体容器头通常在文件末尾包含对应的 BSON 实现头。

因此，本文件是无序键值容器序列化接口的通用入口，BSON 头文件则提供格式相关的元素输入实现。

## 使用限制与注意事项

- `Container` 必须提供 `value_type`、`insert` 等接口，并满足对应无序映射容器的语义。
- `is_multi_container<Container>` 必须能够正确区分普通映射和多重映射。
- 宏的模板参数数量必须与容器模板实际参数数量匹配。
- 该文件依赖自由函数序列化和 `split_free`，不要将宏生成的函数改为成员序列化函数。
- `YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTINER_SAVE_LOAD` 是项目内部宏，不应在无序键值容器之外随意复用。
- 输入策略中的地址修复依赖归档对象跟踪行为；修改 `value_type` 跟踪策略时应同步检查 `reset_object_address` 的参数。
