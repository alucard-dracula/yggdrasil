# `string.hpp`

## 概述

该头文件为字符串类型提供 Boost.Serialization 支持，覆盖：

- `std::basic_string<Char, Traits, Alloc>`；
- `boost::container::basic_string<Char, Traits, Alloc>`；
- 在编译器和配置支持时提供 `std::u8string`、`std::u16string`、`std::u32string` 及对应的 Boost.Container 字符串类型。

## 头文件

```cpp
#include <yggr/serialization/string.hpp>
```

主要依赖以下组件：

```cpp
#include <yggr/charset/string.hpp>
#include <boost/archive/detail/oserializer.hpp>
#include <boost/archive/detail/iserializer.hpp>
#include <boost/serialization/string.hpp>
```

## 默认序列化模式

当未定义 `YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE` 时，字符串被声明为 primitive 类型：

```cpp
BOOST_CLASS_IMPLEMENTATION(boost::container::string, primitive_type)
BOOST_CLASS_IMPLEMENTATION(std::basic_string, primitive_type)
BOOST_CLASS_IMPLEMENTATION(boost::container::basic_string, primitive_type)
```

其中 `std::basic_string` 和 `boost::container::basic_string` 的实现级别通过以下宏设置：

```cpp
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    3, std::basic_string, primitive_type)

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    3, boost::container::basic_string, primitive_type)
```

在此模式下，归档系统可以将字符串作为基本值直接处理。

## Unicode 字符串支持

以下类型是否启用由对应配置宏控制：

| 类型 | 启用条件 |
| --- | --- |
| `std::wstring` 和 `boost::container::wstring` | 未定义 `BOOST_NO_STD_WSTRING` |
| `std::u8string` 和 `boost::container::u8string` | 未定义 `YGGR_NO_U8STRING_T` |
| `std::u16string` 和 `boost::container::u16string` | 未定义 `YGGR_NO_U16STRING_T` |
| `std::u32string` 和 `boost::container::u32string` | 未定义 `YGGR_NO_U32STRING_T` |

这些类型同样会被声明为 `primitive_type`。

## 非 primitive 序列化模式

定义 `YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE` 后，文件不再使用 primitive 实现级别，而是为两类字符串提供 `save`、`load` 和 `serialize` 函数。

### `std::basic_string`

```cpp
template<typename Archive, typename Char, typename Traits, typename Alloc>
void save(
    Archive& ar,
    const std::basic_string<Char, Traits, Alloc>& value,
    const unsigned int version);

template<typename Archive, typename Char, typename Traits, typename Alloc>
void load(
    Archive& ar,
    std::basic_string<Char, Traits, Alloc>& value,
    const unsigned int version);

template<typename Archive, typename Char, typename Traits, typename Alloc>
void serialize(
    Archive& ar,
    std::basic_string<Char, Traits, Alloc>& value,
    const unsigned int version);
```

### `boost::container::basic_string`

```cpp
template<typename Archive, typename Char, typename Traits, typename Alloc>
void save(
    Archive& ar,
    const boost::container::basic_string<Char, Traits, Alloc>& value,
    const unsigned int version);

template<typename Archive, typename Char, typename Traits, typename Alloc>
void load(
    Archive& ar,
    boost::container::basic_string<Char, Traits, Alloc>& value,
    const unsigned int version);

template<typename Archive, typename Char, typename Traits, typename Alloc>
void serialize(
    Archive& ar,
    boost::container::basic_string<Char, Traits, Alloc>& value,
    const unsigned int version);
```

### 保存与加载过程

当字符串的分配器不是标准 `std::allocator`，或项目要求字符串按容器对象处理时，保存和加载函数使用临时的 `std::basic_string<Char>` 作为归档载体：

- `save`：将原字符串的字符范围复制到临时标准字符串，再通过 `save_primitive` 写入归档；
- `load`：通过 `load_primitive` 读取临时标准字符串，再将字符范围赋值回目标字符串；
- `serialize`：调用 `boost::serialization::split_free`，将保存和加载分派到对应函数。

非 primitive 模式下，两类字符串的实现级别设置为：

```cpp
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    3, std::basic_string, object_serializable)

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    3, boost::container::basic_string, object_serializable)
```

## BSON 集成

文件末尾包含：

```cpp
#include <yggr/serialization/string_bson_impl.hpp>
```

该头文件不直接定义 BSON 序列化函数，而是根据 `YGGR_HAS_BSON_OARCHIVE` 或 `YGGR_HAS_BSON_IARCHIVE` 的定义状态，参与 BSON 实现的条件编译和重新包含流程。相关说明见 `string_bson_impl.hpp`。

## 注意事项

- `YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE` 会改变字符串的实现级别以及保存/加载路径，应在整个相关序列化配置中保持一致。
- Unicode 字符串类型受编译器能力和项目配置宏影响，使用前应确认对应类型可用。
- 该文件中的 `save`、`load` 和 `serialize` 函数位于 `boost::serialization` 命名空间中，通常通过 Boost.Serialization 的常规归档接口调用。
