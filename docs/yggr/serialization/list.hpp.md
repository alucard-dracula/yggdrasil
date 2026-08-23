# `list.hpp`

## 概述

该头文件是 yggdrasil 对链表进行序列化的统一入口，覆盖：

- `std::list<T, Allocator>`；
- `boost::container::list<T, Allocator>`；
- Boost 1.70 及以上版本的 `yggr::container::detail::list`；
- 普通元素和 `bool` 元素的线性容器保存/加载；
- BSON 类型适配。

## 头文件

```cpp
#include <yggr/serialization/list.hpp>
```

## 与 Boost 原生头文件的互斥

如果已经定义：

```cpp
BOOST_SERIALIZATION_LIST_HPP
```

文件会触发编译错误，并要求不要同时包含：

```cpp
<boost/serialization/list.hpp>
```

应使用 yggdrasil 提供的链表序列化实现。

## 依赖

该文件包含：

```cpp
#include <yggr/serialization/liner_container.hpp>
#include <yggr/container/list.hpp>
```

并在文件末尾包含：

```cpp
#include <yggr/serialization/list_bson_impl.hpp>
```

## 普通元素序列化注册

文件使用以下宏注册普通元素的保存和加载函数：

```cpp
YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(2, std::list)
YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(
    2, boost::container::list)
```

在 Boost 版本不低于 1.70 时，还注册：

```cpp
YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(
    2, yggr::container::detail::list)
```

这些宏生成 `save`、`load` 和 `serialize` 函数，实际算法由
`liner_container.hpp` 提供。

## `bool` 元素序列化注册

在旧版 MSVC 兼容条件满足时：

```cpp
!BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
```

文件为以下容器注册 bool 专用保存和加载逻辑：

```cpp
YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(2, std::list)
YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(
    2, boost::container::list)
```

Boost 1.70 及以上时，还为内部 list 注册 bool 支持：

```cpp
YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(
    2, yggr::container::detail::list)
```

该特化用于适配 bool 容器可能提供的代理引用行为。

## 实现级别

文件包含：

```cpp
#include <yggr/serialization/detail/container_implementation_level_def.hpp>
```

并将支持的 list 类型设置为：

```cpp
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    2, std::list, object_serializable)

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    2, boost::container::list, object_serializable)
```

在 Boost 版本不低于 1.70 时，内部 list 也设置为 `object_serializable`。

这表示链表按可序列化对象处理，而不是简单原始值处理。

## BSON 支持

文件在主 include guard 结束后包含：

```cpp
#include <yggr/serialization/list_bson_impl.hpp>
```

启用 BSON 输入或输出归档后，BSON 扩展会将支持的 list 类型注册为 BSON 数组，并接入线性容器 BSON 实现。

## Boost 版本兼容

内部 yggdrasil list 的普通、bool 和 BSON 支持都受以下条件控制：

```cpp
#if !(BOOST_VERSION < 107000)
```

即 Boost 1.70 及以上版本才启用 `yggr::container::detail::list` 相关序列化注册。

`std::list` 和 `boost::container::list` 的注册不受该版本条件影响。

## 使用示例

```cpp
#include <yggr/serialization/list.hpp>
#include <list>

void prepare_list()
{
    std::list<int> values;
    values.push_back(1);
    values.push_back(2);

    // 将 values 交给 Boost.Serialization 或项目 BSON 归档。
}
```

Boost 容器示例：

```cpp
#include <yggr/serialization/list.hpp>
#include <boost/container/list.hpp>

void prepare_boost_list()
{
    boost::container::list<int> values;
    values.push_back(1);
}
```

## 序列化流程

通常的线性容器序列化流程包括：

1. 保存或读取集合元素数量；
2. 保存或读取元素版本信息；
3. 按链表迭代器顺序处理元素；
4. 调用元素类型对应的保存或加载操作；
5. 对 bool 元素使用逐元素的专用逻辑。

链表不会使用 vector 风格的连续数组优化，具体行为由线性容器实现决定。

## 注意事项

1. 不要同时包含 Boost 原生 `boost/serialization/list.hpp`，否则会触发显式编译错误。
2. list 模板参数数量按 `2` 注册，通常为元素类型和分配器类型。
3. `std::list<bool>` 和其他 bool list 使用专用逐元素序列化逻辑。
4. 内部 yggdrasil list 的支持依赖 Boost 1.70 或更高版本。
5. 链表被标记为 `object_serializable`，元素类型仍必须满足对应归档的序列化要求。
6. BSON 支持是否启用取决于 `YGGR_HAS_BSON_OARCHIVE` 和 `YGGR_HAS_BSON_IARCHIVE`。
7. BSON 类型注册只将 list 标识为数组，不代表链表具有连续内存布局。
8. 该文件负责注册和组合实现，不直接定义完整的元素遍历算法。
9. 序列化输入端和输出端应使用兼容的容器实现、元素版本和 BSON 归档配置。

## 相关组件

- `yggr/serialization/liner_container.hpp`
- `yggr/serialization/list_bson_impl.hpp`
- `yggr/serialization/collections_save_imp.hpp`
- `yggr/serialization/collections_load_imp.hpp`
- `yggr/serialization/detail/container_implementation_level_def.hpp`
- `yggr/container/list.hpp`
- `std::list`
- `boost::container::list`
