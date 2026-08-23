# `array_bson_impl.hpp`

## 概述

该头文件为固定长度数组提供 BSON 序列化支持，覆盖：

- C++11 `std::array<T, N>`；
- Boost `boost::array<T, N>`；
- 原生数组 `Val[N]` 的 BSON 类型标识。

具体功能根据 `YGGR_HAS_BSON_OARCHIVE` 和 `YGGR_HAS_BSON_IARCHIVE` 条件编译。

## 头文件

```cpp
#include <yggr/serialization/array_bson_impl.hpp>
```

该文件通常由 BSON 归档相关头文件间接包含。只有启用对应 BSON 输出或输入归档宏时，相关实现才会参与编译。

## 依赖与编译条件

当定义以下任一宏时，会包含 BSON 类型特征和名称-值对支持：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HAS_BSON_IARCHIVE
```

C++ 标准数组支持还受到以下宏影响：

```cpp
YGGR_NO_CXX11_HDR_ARRAY
```

定义该宏时，不会生成 `std::array` 的序列化重载，但 `boost::array` 的重载仍然可用。

## BSON 输出归档

由 `YGGR_HAS_BSON_OARCHIVE` 控制，相关函数位于：

```cpp
boost::serialization
```

### `std::array<T, N>`

当 `skip_auto_id` 为 `false` 时：

```cpp
template<typename T, std::size_t N>
void serialize(
    yggr::archive::bson_oarchive<false>& ar,
    std::array<T, N>& a,
    const unsigned int version);
```

将数组转换为原生数组引用，并以名称 `"elems"` 写入 BSON。

当 `skip_auto_id` 为 `true` 时：

```cpp
template<typename T, std::size_t N>
void serialize(
    yggr::archive::bson_oarchive<true>& ar,
    std::array<T, N>& a,
    const unsigned int version);
```

逐个写入数组元素，元素名称为从 `"0"` 到 `"N - 1"` 的字符串索引。

当 `YGGR_NO_CXX11_HDR_ARRAY` 被定义时，不生成 `std::array` 相关重载。

### `boost::array<T, N>`

`boost::array` 提供与 `std::array` 相同的两种输出形式：

```cpp
template<typename T, std::size_t N>
void serialize(
    yggr::archive::bson_oarchive<false>& ar,
    boost::array<T, N>& a,
    const unsigned int version);

template<typename T, std::size_t N>
void serialize(
    yggr::archive::bson_oarchive<true>& ar,
    boost::array<T, N>& a,
    const unsigned int version);
```

- `bson_oarchive<false>`：将整个数组作为 `"elems"` 写入；
- `bson_oarchive<true>`：按字符串索引逐个写入元素。

## BSON 输入归档

由 `YGGR_HAS_BSON_IARCHIVE` 控制，相关函数位于：

```cpp
boost::serialization
```

### `std::array<T, N>`

```cpp
template<bool skip_auto_id, typename T, std::size_t N>
void serialize(
    yggr::archive::bson_iarchive<skip_auto_id>& ar,
    std::array<T, N>& a,
    const unsigned int version);
```

从名称 `"elems"` 读取数组内容。该重载仅在未定义
`YGGR_NO_CXX11_HDR_ARRAY` 时提供。

### `boost::array<T, N>`

```cpp
template<bool skip_auto_id, typename T, std::size_t N>
void serialize(
    yggr::archive::bson_iarchive<skip_auto_id>& ar,
    boost::array<T, N>& a,
    const unsigned int version);
```

从名称 `"elems"` 读取整个数组内容，与输入归档的 `skip_auto_id` 值无关。

## 原生数组 BSON 类型标识

当启用 BSON 输入或输出归档时，文件为原生数组提供：

```cpp
template<typename Val, std::size_t N>
struct bson_typeid<Val[N]>;
```

该特化定义：

```cpp
typedef Val type[N];
typedef Val wrap_type[N];
```

并将类型标识设置为：

```cpp
bson_typeid_def::E_BSON_TYPE_ARRAY
```

调用 `operator()` 时返回转换为 `u8` 的 BSON 数组类型值。该类型标识只描述 BSON 类型，不负责实际数组元素的序列化。

## 输出格式对比

| 归档类型 | 数组格式 |
| --- | --- |
| `bson_oarchive<false>` | 使用 `"elems"` 包装整个数组。 |
| `bson_oarchive<true>` | 使用 `"0"`、`"1"` 等字符串索引写入元素。 |
| `bson_iarchive<skip_auto_id>` | 从 `"elems"` 读取整个数组。 |

输出和输入格式并非完全对称：输出归档在 `skip_auto_id == true` 时使用索引字段，而输入归档实现固定读取 `"elems"`。

## 使用示例

```cpp
#include <yggr/serialization/array.hpp>

void serialize_array()
{
    std::array<int, 3> values = {{1, 2, 3}};

    // 将 values 交给项目的 BSON 输出归档。
    // bson_oarchive<false> 会使用 "elems" 字段写入。
}
```

## 宏状态管理

文件使用内部宏分别防止输出归档、输入归档和 BSON 类型标识实现被重复展开：

```cpp
__YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE__
__YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_IARCHIVE__
__YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_BSON_VALUE_TYPEID__
```

当三部分实现均完成后，文件会定义最终包含保护宏
`__YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP__`，并清理这些内部阶段宏。

## 注意事项

1. 只有定义 `YGGR_HAS_BSON_OARCHIVE` 或 `YGGR_HAS_BSON_IARCHIVE` 时，BSON 相关依赖和实现才会启用。
2. `std::array` 支持受 `YGGR_NO_CXX11_HDR_ARRAY` 控制。
3. 输出归档的 `bson_oarchive<true>` 按字符串索引写入元素；输入归档当前从 `"elems"` 读取，使用时应确认归档格式约定。
4. 数组长度是编译期常量 `N`，反序列化目标数组必须与数据布局兼容。
5. 原生数组的 `bson_typeid` 特化只提供 BSON 类型识别，不提供完整序列化逻辑。
6. 数组元素类型 `T` 或 `Val` 仍必须满足项目 BSON 序列化和类型特征要求。
7. 具体归档行为由 `bson_oarchive`、`bson_iarchive`、`bson_type_traits` 和名称-值对实现共同决定。

## 相关组件

- `yggr/serialization/export.hpp`
- `yggr/serialization/nvp.hpp`
- `yggr/nsql_database_system/bson_type_traits.hpp`
- `yggr/archive/bson_oarchive.hpp`
- `yggr/archive/bson_iarchive.hpp`
- `std::array`
- `boost::array`