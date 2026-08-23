# `liner_container.hpp`

## 概述

该头文件提供线性容器序列化的宏生成实现，用于为 vector、deque 等具有线性元素顺序的容器生成 Boost.Serialization 的保存、加载和分离式序列化函数。

文件同时处理：

- 普通元素的集合序列化；
- vector 的连续数组优化；
- `bool` 特化容器的逐元素序列化；
- Boost.Serialization 旧版本兼容；
- BSON 数组实现所需的线性容器基础设施。

## 头文件

```cpp
#include <yggr/serialization/liner_container.hpp>
```

## 与 Boost 原生 vector 头文件的互斥

如果已经定义：

```cpp
BOOST_SERIALIZATION_VECTOR_HPP
```

文件会触发编译错误，并要求不要同时包含：

```cpp
<boost/serialization/vector.hpp>
```

应使用 yggdrasil 提供的线性容器序列化实现。

## 兼容配置

如果没有预先定义 `BOOST_SERIALIZATION_VECTOR_VERSIONED`，文件会提供默认定义：

```cpp
#define BOOST_SERIALIZATION_VECTOR_VERSIONED(V) (V == 4 || V == 5)
```

该默认值用于兼容 Boost.Serialization 1.34.1 文件格式，不兼容 1.35 文件格式的特殊情况。

## 宏接口

### `YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD`

```cpp
#define YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(
    __template_n__, __type__)
```

注意：`LINER` 和 `CONTINER` 是源码中的既有拼写，调用时必须使用该宏名称。

参数：

| 参数 | 说明 |
| --- | --- |
| `__template_n__` | 容器模板参数数量。 |
| `__type__` | 容器模板名称，例如 `std::vector` 或 `std::deque`。 |

该宏生成普通元素的 `save`、`load` 和 `serialize` 函数，并根据容器类型和归档能力选择普通集合路径或数组优化路径。

### `YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD`

```cpp
#define YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(
    __template_n__, __type__)
```

为第一个模板参数为 `bool` 的线性容器生成专用保存、加载和 `serialize` 函数。

该宏使用 `BOOST_PP_SUB(__template_n__, 1)` 生成剩余模板参数列表，适用于 `std::vector<bool>`、`std::deque<bool>` 等特殊 bool 容器。

## 普通元素序列化

### 保存

普通路径调用：

```cpp
yggr::serialization::container::save_collection(ar, t);
```

当容器不适合连续数组优化，集合保存实现会保存集合大小、元素版本、构造数据和每个元素。

### 加载

普通路径调用：

```cpp
yggr::serialization::container::load_collection(ar, t);
```

加载时使用：

- `archive_input_seq<Archive, Container>`：读取并追加元素；
- vector 使用 `reserve_imp`；
- 其他线性容器使用 `no_reserve_imp`。

## 数组优化路径

当容器被识别为 vector，且归档支持 `use_array_optimization` 时，宏会选择连续数组路径。

### 保存

保存流程：

1. 将 `t.size()` 包装为 `collection_size_type`；
2. 保存集合数量；
3. 当容器非空时，通过 `get_data(t)` 获取连续存储地址；
4. 使用 `make_array()` 批量保存元素。

逻辑形式为：

```cpp
collection_size_type count(t.size());
ar << YGGR_SERIALIZATION_NVP(count);
if(!t.empty())
{
    ar << make_array(get_data(t), t.size());
}
```

### 加载

加载流程：

1. 读取集合数量；
2. 调用 `t.resize(count)` 调整 vector 大小；
3. 根据 `BOOST_SERIALIZATION_VECTOR_VERSIONED(ar.get_library_version())` 判断是否读取元素版本；
4. 当容器非空时，通过 `make_array()` 批量加载连续元素。

该路径要求容器提供连续存储和兼容的 `get_data()` 支持。

## 优化路径选择

宏会根据以下两个条件决定是否使用数组优化：

- 容器是否被 `is_vector` 类型特征识别为 vector；
- `boost::serialization::use_array_optimization<Archive>` 是否允许当前归档优化。

非 vector 容器强制使用普通集合路径，即使其底层存储可能连续。

## `bool` 线性容器序列化

`bool` 容器不使用通用数组优化，而是逐元素处理。

### 保存

```cpp
collection_size_type count(t.size());
ar << YGGR_SERIALIZATION_NVP(count);

while(count-- > 0)
{
    bool value = *it++;
    ar << YGGR_SERIALIZATION_NAME_NVP("item", value);
}
```

### 加载

```cpp
collection_size_type count;
ar >> YGGR_SERIALIZATION_NVP(count);
t.clear();

while(count-- > 0)
{
    bool value;
    ar >> YGGR_SERIALIZATION_NAME_NVP("item", value);
    t.push_back(value);
}
```

这种方式适配 `vector<bool>` 等返回代理引用而不是普通 `bool&` 的容器。

## 分离式序列化

两个宏都会生成：

```cpp
template<typename Archive, ...>
void serialize(
    Archive& ar,
    Container& t,
    const unsigned int file_version)
{
    boost::serialization::split_free(ar, t, file_version);
}
```

保存和加载函数因此分别由 `split_free()` 分派。

## 使用示例

以下示例为 vector 注册线性容器序列化支持：

```cpp
#include <vector>
#include <yggr/serialization/liner_container.hpp>

namespace boost
{
namespace serialization
{

YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(2, std::vector)

} // namespace serialization
} // namespace boost
```

`bool` 容器的注册形式：

```cpp
YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(2, std::vector)
```

实际使用时通常由 `vector.hpp`、`deque.hpp` 等容器专用头文件调用这些宏。

## 与 BSON 的关系

该文件依赖集合保存/加载基础实现，并被 BSON 线性容器实现使用：

- 通用 BSON 输出通常通过集合保存逻辑或专用 BSON 特化处理；
- BSON 输入的线性容器元素读取由 `liner_container_bson_impl.hpp` 提供；
- deque 等容器通过专用 BSON 文件注册为 BSON 数组类型。

## 注意事项

1. 不要同时包含 Boost 原生 `boost/serialization/vector.hpp`，否则会触发编译错误。
2. 宏名称中的 `LINER` 和 `CONTINER` 是源码既有拼写，不要自行改写。
3. `__template_n__` 必须与目标容器模板参数数量匹配。
4. 数组优化只对被识别为 vector 且归档允许优化的情况生效。
5. 数组优化要求容器元素可以通过连续内存批量处理。
6. `bool` 线性容器使用逐元素保存和加载，以兼容代理引用实现。
7. vector 加载会先 `resize(count)`，原有容器内容可能被覆盖或调整。
8. 旧 Boost 文件格式的元素版本读取由 `BOOST_SERIALIZATION_VECTOR_VERSIONED` 决定。
9. `save_collection()` 和 `load_collection()` 的元素版本、构造数据及对象跟踪行为由相关集合实现负责。
10. BSON 输入输出时，应确保归档模式与对应容器字段布局保持兼容。

## 相关组件

- `yggr/serialization/collections_save_imp.hpp`
- `yggr/serialization/collections_load_imp.hpp`
- `yggr/serialization/liner_container_bson_impl.hpp`
- `yggr/serialization/array.hpp`
- `yggr/serialization/deque.hpp`
- `yggr/serialization/detail/type_traits.hpp`
- `yggr/serialization/detail/get_data.hpp`
- `boost/serialization/split_free.hpp`
- `boost/serialization/vector.hpp`
