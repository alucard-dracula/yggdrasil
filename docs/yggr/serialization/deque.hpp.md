# `deque.hpp`

## 概述

该头文件是 yggdrasil 对双端队列进行序列化的统一入口，覆盖：

- `std::deque<T, Allocator>`；
- `boost::container::deque<T, Allocator>`；
- 普通元素和 `bool` 元素的线性容器保存/加载；
- BSON 类型适配和序列化扩展。

## 头文件

```cpp
#include <yggr/serialization/deque.hpp>
```

## 与 Boost 原生头文件的互斥

如果已经定义：

```cpp
BOOST_SERIALIZATION_DEQUE_HPP
```

文件会触发编译错误，并要求不要同时包含：

```cpp
<boost/serialization/deque.hpp>
```

应使用本文件提供的 yggdrasil 序列化实现。

## 依赖

该文件包含：

```cpp
#include <yggr/serialization/liner_container.hpp>
#include <yggr/container/deque.hpp>
```

并在文件末尾包含：

```cpp
#include <yggr/serialization/deque_bson_impl.hpp>
```

## 支持的容器

| 容器 | 模板参数数量 | 序列化类型 |
| --- | --- | --- |
| `std::deque<T, Allocator>` | `2` | 线性容器对象序列化。 |
| `boost::container::deque<T, Allocator>` | `2` | 线性容器对象序列化。 |

宏参数 `2` 表示 deque 模板具有两个模板参数。

## 保存与加载注册

文件使用以下宏为两种 deque 注册线性容器保存和加载实现：

```cpp
YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(2, std::deque)
YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(
    2, boost::container::deque)
```

这些宏提供普通元素类型的序列化支持，具体实现由
`yggr/serialization/liner_container.hpp` 决定。

## `bool` 元素支持

在以下兼容条件满足时：

```cpp
!BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
```

文件额外注册：

```cpp
YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(2, std::deque)
YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(
    2, boost::container::deque)
```

用于处理 `std::deque<bool>` 和 `boost::container::deque<bool>` 的特殊代理引用和位压缩行为。

在较旧的 MSVC 版本上，该 bool 特化不会启用。

## 实现级别

文件包含：

```cpp
#include <yggr/serialization/detail/container_implementation_level_def.hpp>
```

并将两种 deque 的 Boost.Serialization 实现级别设置为：

```cpp
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    2, std::deque, object_serializable)

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    2, boost::container::deque, object_serializable)
```

这表示 deque 按可序列化对象处理，而不是简单原始值处理。

## BSON 支持

文件在主 include guard 结束后包含：

```cpp
#include <yggr/serialization/deque_bson_impl.hpp>
```

当启用 `YGGR_HAS_BSON_OARCHIVE` 或 `YGGR_HAS_BSON_IARCHIVE` 时，BSON 扩展会将两种 deque 注册为 BSON 数组类型，并使用线性容器 BSON 实现处理实际元素。

## 使用示例

```cpp
#include <yggr/serialization/deque.hpp>
#include <deque>

void prepare_deque()
{
    std::deque<int> values;
    values.push_back(1);
    values.push_back(2);

    // 将 values 交给 Boost.Serialization 或项目 BSON 归档。
}
```

Boost 容器示例：

```cpp
#include <yggr/serialization/deque.hpp>
#include <boost/container/deque.hpp>

void prepare_boost_deque()
{
    boost::container::deque<int> values;
    values.push_back(1);
}
```

## 序列化流程

通常的线性容器序列化流程包括：

1. 保存或读取集合元素数量；
2. 保存或读取元素版本信息；
3. 按迭代器顺序处理每个元素；
4. 调用元素类型对应的保存或加载操作；
5. 对 `bool` deque 使用专用线性容器处理逻辑。

实际字段名称和归档布局由线性容器实现及具体归档类型决定。

## 注意事项

1. 不要同时包含 Boost 原生 `boost/serialization/deque.hpp`，否则会触发显式编译错误。
2. deque 模板参数数量固定按 `2` 注册，通常为元素类型和分配器类型。
3. `std::deque<bool>` 具有特殊代理引用语义，依赖项目提供的 bool 线性容器特化。
4. 旧版 MSVC 不启用 bool deque 特化，具体边界由 `BOOST_WORKAROUND` 条件决定。
5. 该文件负责注册和组合实现，不直接定义完整的元素遍历算法。
6. BSON 扩展是否启用取决于 BSON 输入/输出归档宏。
7. deque 被标记为 `object_serializable`，元素类型仍必须满足对应归档的序列化要求。
8. 序列化输入端和输出端应使用兼容的容器实现、元素版本和 BSON 归档配置。

## 相关组件

- `yggr/serialization/liner_container.hpp`
- `yggr/serialization/deque_bson_impl.hpp`
- `yggr/serialization/collections_save_imp.hpp`
- `yggr/serialization/detail/container_implementation_level_def.hpp`
- `yggr/container/deque.hpp`
- `std::deque`
- `boost::container::deque`
