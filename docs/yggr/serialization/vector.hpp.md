# `vector.hpp`

## 概述

该头文件为标准库、Boost.Container 和 Yggdrasil 内部 vector 容器接入 Boost.Serialization 支持。它主要负责类型特征注册、通用序列化函数展开、`vector<bool>` 特殊路径、实现级别设置以及 BSON 适配接入。

支持的容器包括：

- `std::vector`；
- `boost::container::vector`；
- `yggr::container::detail::vector`。

## 头文件

```cpp
#include <yggr/serialization/vector.hpp>
```

## 依赖

主要依赖包括：

```cpp
#include <yggr/container/vector.hpp>
#include <yggr/serialization/liner_container.hpp>
#include <yggr/serialization/detail/type_traits.hpp>
```

其中：

- `yggr/container/vector.hpp` 提供项目支持的 vector 类型；
- `liner_container.hpp` 提供线性容器的保存、加载和宏生成实现；
- `detail/type_traits.hpp` 提供 `is_vector` 等类型特征基础设施。

## `is_vector` 特化

特化位于：

```cpp
yggr::serialization::detail
```

### `std::vector`

```cpp
template<typename Val, typename Alloc>
struct is_vector<std::vector<Val, Alloc>>
    : public boost::mpl::true_
{
};
```

### Boost 旧版本

当 `BOOST_VERSION < 106700` 时，Boost.Container vector 使用两个模板参数：

```cpp
template<typename Val, typename Alloc>
struct is_vector<boost::container::vector<Val, Alloc>>
    : public boost::mpl::true_
{
};
```

### Boost 新版本和 Yggdrasil vector

当 `BOOST_VERSION >= 106700` 时，Boost.Container vector 使用三个模板参数，并额外支持 Yggdrasil 内部 vector：

```cpp
template<typename Val, typename Alloc, typename Options>
struct is_vector<
    boost::container::vector<Val, Alloc, Options>>
    : public boost::mpl::true_
{
};

template<typename Val, typename Alloc>
struct is_vector<
    yggr::container::detail::vector<Val, Alloc>>
    : public boost::mpl::true_
{
};
```

该特征会被线性容器通用实现用于决定是否启用 vector 相关的数组优化和容量预留。

## 普通线性容器序列化注册

文件在 `boost::serialization` 命名空间中使用以下宏生成通用 `save`、`load` 和 `serialize` 函数：

```cpp
YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(
    template_parameter_count,
    container_type)
```

宏名称中的 `CONTINER` 是源码既有拼写，调用时必须保持不变。

注册项如下：

```cpp
YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(
    2, std::vector)

#if (BOOST_VERSION < 106700)
YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(
    2, boost::container::vector)
#else
YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(
    3, boost::container::vector)
YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(
    2, yggr::container::detail::vector)
#endif
```

这些函数最终委托给：

- `save_collection`：保存集合大小和元素；
- `load_collection`：读取集合大小、调整容器并加载元素；
- `split_free`：根据归档方向分派 `save` 或 `load`。

对 vector 类型，线性容器实现还可以根据 `is_vector` 和归档的数组优化能力选择连续内存路径。

## `vector<bool>` 专用注册

`std::vector<bool>` 使用位代理引用，不能完全按普通 vector 元素引用处理。因此文件额外使用：

```cpp
YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(
    template_parameter_count,
    container_type)
```

该宏注册专用的 `save`、`load` 和 `serialize`：

- 保存时逐个读取 bool 值，并使用字段名 `"item"` 写出；
- 加载时逐个读取临时 `bool`，再调用 `push_back`；
- 不依赖连续元素地址或普通 vector 的数组优化。

注册同样覆盖：

```cpp
std::vector<bool>
boost::container::vector<bool, ...>
yggr::container::detail::vector<bool, ...>
```

但该专用路径受以下编译器兼容条件限制：

```cpp
#if !BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
```

MSVC 版本不高于 13.00 时跳过 `vector<bool>` 专用宏注册。

## Boost 版本兼容

Boost.Container vector 的模板参数数量随 Boost 版本变化：

| Boost 版本 | `boost::container::vector` 参数数量 | 额外类型 |
| --- | ---: | --- |
| `< 1.67` | `2` | 无 |
| `>= 1.67` | `3` | `yggr::container::detail::vector` 使用 `2` 个参数 |

该差异同时影响：

- `is_vector` 特化；
- 普通线性容器序列化宏；
- `vector<bool>` 专用序列化宏；
- 容器实现级别宏；
- BSON 类型 ID 注册。

## 实现级别

文件包含：

```cpp
#include <yggr/serialization/detail/container_implementation_level_def.hpp>
```

并将支持的 vector 类型设置为 `object_serializable`：

```cpp
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    2, std::vector, object_serializable)
```

Boost 版本分支对应设置：

```cpp
#if (BOOST_VERSION < 106700)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    2, boost::container::vector, object_serializable)
#else
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    3, boost::container::vector, object_serializable)
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    2, yggr::container::detail::vector, object_serializable)
#endif
```

## BSON 集成

主 include guard 结束后包含：

```cpp
#include <yggr/serialization/vector_bson_impl.hpp>
```

该 BSON 适配头负责为相同的 vector 类型注册 BSON 数组类型，并接入 `liner_container_bson_impl.hpp` 的具体 BSON 保存/加载逻辑。

典型包含关系如下：

```text
vector.hpp
  -> liner_container.hpp
      -> 通用线性容器 save/load
  -> vector_bson_impl.hpp
      -> vector BSON 类型 ID 注册
      -> liner_container_bson_impl.hpp
```

## 序列化流程

普通 vector：

```text
vector
  -> YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD
  -> is_vector / 数组优化判断
  -> save_collection 或 load_collection
```

`vector<bool>`：

```text
vector<bool>
  -> YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD
  -> 逐项读取或写出 bool
  -> push_back 恢复元素
```

## 使用限制与注意事项

- 包含该文件前应避免直接包含 `boost/serialization/vector.hpp`，线性容器基础头对该情况有专门限制。
- vector 模板参数数量必须与当前 Boost 版本匹配。
- `vector<bool>` 应使用专用布尔序列化路径，不应强行套用普通连续内存优化。
- `is_vector` 特征是数组优化和容量预留判断的重要依据，新增 vector 类型时应同步提供特化。
- `YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD` 和 `YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD` 属于内部序列化宏，不应在无关容器上直接复用。
- BSON 字段索引和元素格式由 `vector_bson_impl.hpp` 及线性容器 BSON 实现负责。
