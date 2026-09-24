# `unordered_unliner_container.hpp`

## 概述

该头文件为无序线性容器提供通用的 Boost.Serialization 接口。它面向无序集合和无序多重集合，定义对应的元素输入策略，并通过预处理器宏为具体容器模板生成 `save`、`load` 和 `serialize` 自由函数。

文件本身不绑定具体容器，也不直接实现集合元数据的读写。集合级保存和加载委托给无序集合通用实现，单个元素的构造、读取、插入和对象地址修复由本文件中的输入策略完成。

文件名中的 `unliner` 是项目现有命名，用于表示无序集合的线性元素输入路径。

## 头文件

```cpp
#include <yggr/serialization/unordered_unliner_container.hpp>
```

具体的 `unordered_set` 和 `unordered_multiset` 序列化适配通常通过以下头文件接入：

```cpp
#include <yggr/serialization/unordered_set.hpp>
```

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

这些依赖分别提供基础定义、预处理器模板参数工具、命名值对支持、无序集合保存/加载流程、自由函数序列化拆分、临时对象构造和对象地址处理能力。

## 命名空间

两个元素输入策略位于：

```cpp
yggr::serialization::container
```

宏生成的 `save`、`load` 和 `serialize` 函数通常在 `boost::serialization` 命名空间中调用。

## `archive_input_unordered_unliner`

该策略用于不允许重复元素的无序集合，例如 `unordered_set`：

```cpp
template<typename Archive, typename Container>
struct archive_input_unordered_unliner
{
    void operator()(
        Archive& ar,
        Container& container,
        const unsigned int version) const;
};
```

### 处理流程

调用 `operator()` 时：

1. 使用 `Container::value_type` 和 `stack_construct` 创建临时元素；
2. 以固定名称 `"item"` 从归档读取元素；
3. 调用 `container.insert` 插入元素；
4. 当插入成功时，调用 `reset_object_address`，将临时对象地址映射到容器内实际元素地址。

核心逻辑为：

```cpp
typedef typename Container::value_type type;
boost::serialization::detail::stack_construct<Archive, type>
    value_construct(ar, version);
ar >> YGGR_SERIALIZATION_NAME_NVP(
    "item", value_construct.reference());
std::pair<typename Container::const_iterator, bool> result =
    container.insert(value_construct.reference());
```

只有 `result.second` 为 `true` 时才进行地址修复：

```cpp
if(result.second)
{
    ar.reset_object_address(
        boost::addressof(*result.first),
        boost::addressof(value_construct.reference()));
}
```

这适用于不允许重复元素的容器；如果归档中的元素已存在，插入失败且不会覆盖已有元素的地址映射。

## `archive_input_unordered_multi_unliner`

该策略用于允许重复元素的无序多重集合，例如 `unordered_multiset`：

```cpp
template<typename Archive, typename Container>
struct archive_input_unordered_multi_unliner
{
    void operator()(
        Archive& ar,
        Container& container,
        const unsigned int version) const;
};
```

它同样创建临时元素并以 `"item"` 读取，但多重集合的 `insert` 返回新插入元素的迭代器：

```cpp
typename Container::const_iterator result =
    container.insert(value_construct.reference());
```

由于每次插入都会生成一个元素，随后直接更新对象地址：

```cpp
ar.reset_object_address(
    boost::addressof(*result),
    boost::addressof(value_construct.reference()));
```

## 临时对象与地址跟踪

元素先由 `stack_construct` 构造在临时存储中，插入容器后通常会位于新的实际存储地址。调用 `reset_object_address` 可以更新 Boost.Serialization 的对象跟踪信息，使后续引用指向容器内的元素，而不是生命周期即将结束的临时对象。

普通无序集合只在插入成功时执行地址修复；无序多重集合每次插入都执行地址修复。

## `YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD`

该宏为一个无序线性容器模板生成三类自由函数：

```cpp
YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(
    __template_n__,
    __type__)
```

宏名称中的 `CONTINER` 是源码中的既有拼写，调用时必须保持不变。

参数含义如下：

| 参数 | 含义 |
| --- | --- |
| `__template_n__` | 容器模板参数数量；无序集合通常为 `4` |
| `__type__` | 容器模板名称，例如 `std::unordered_set` |

### 生成的 `save`

生成的 `save` 函数将容器转交给：

```cpp
yggr::serialization::container::save_unordered_collection<
    Archive,
    Container>(ar, container);
```

集合元素遍历和集合格式由无序集合保存实现统一处理。

### 生成的 `load`

生成的 `load` 函数将容器转交给：

```cpp
yggr::serialization::container::load_unordered_collection<
    Archive,
    Container,
    InputFunction>(ar, container);
```

其中 `InputFunction` 通过 `is_multi_container<Container>` 选择：

- 多重容器使用 `archive_input_unordered_multi_unliner`；
- 其他容器使用 `archive_input_unordered_unliner`。

这样集合级加载逻辑与普通集合/多重集合的插入语义相互分离。

### 生成的 `serialize`

生成的 `serialize` 函数使用 Boost.Serialization 的自由函数拆分机制：

```cpp
boost::serialization::split_free(ar, container, file_version);
```

归档系统会根据输入或输出方向分别调用生成的 `load` 或 `save`。

## 具体容器注册示例

`unordered_set.hpp` 使用该宏为标准库和 Boost 容器生成序列化函数：

```cpp
#ifndef YGGR_NO_CXX11_HDR_UNORDERED_SET
YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(
    4, std::unordered_set)
YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(
    4, std::unordered_multiset)
#endif

YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(
    4, boost::unordered_set)
YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD(
    4, boost::unordered_multiset)
```

具体容器头还会为 `unordered_multiset` 特化 `is_multi_container`，使 `load` 自动使用多重集合输入策略。

## 与 BSON 支持的关系

本文件提供与归档类型无关的通用输入策略。BSON 输入归档可以通过特化同名策略接管元素读取，例如：

```cpp
#include <yggr/serialization/unordered_unliner_container_bson_impl.hpp>
```

BSON 特化会将元素索引转换为数字字符串字段名，例如 `"0"`、`"1"`、`"2"`，而普通归档策略使用固定名称 `"item"`。集合级 BSON 元数据和索引遍历由无序集合 BSON 加载实现负责。

## 使用限制与注意事项

- `Container` 必须提供 `value_type`、`insert` 以及对应输入策略所需的插入返回值。
- `is_multi_container<Container>` 必须准确反映容器是否允许重复元素。
- 宏的模板参数数量必须与目标容器模板实际参数数量匹配。
- `save_unordered_collection`、`load_unordered_collection` 和元素输入策略共同决定完整的序列化行为，本文件不应重复实现集合级流程。
- `YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTINER_SAVE_LOAD` 属于内部序列化基础设施，通常只由具体无序集合序列化头文件调用。
- BSON 专用元素读取、字段索引和归档条件控制由对应 BSON 实现头文件负责。
