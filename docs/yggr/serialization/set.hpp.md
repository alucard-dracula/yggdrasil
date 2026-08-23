# `set.hpp`

## 概述

该头文件是 yggdrasil 对集合容器进行序列化的统一入口，覆盖 set 和 multiset：

- `std::set`；
- `std::multiset`；
- `boost::container::set`；
- `boost::container::multiset`；
- Boost 1.56 及以上版本的 yggdrasil 内部 set/multiset 实现。

文件通过非线性容器序列化宏生成保存、加载和分离式序列化函数，并接入 BSON 类型适配。

## 头文件

```cpp
#include <yggr/serialization/set.hpp>
```

## 与 Boost 原生头文件的互斥

如果已经定义：

```cpp
BOOST_SERIALIZATION_SET_HPP
```

文件会触发编译错误，并要求不要同时包含：

```cpp
<boost/serialization/set.hpp>
```

应使用 yggdrasil 提供的集合序列化实现。

## 依赖

该文件包含：

```cpp
#include <yggr/serialization/unliner_container.hpp>
#include <yggr/container/set.hpp>
```

并使用：

```cpp
#include <yggr/serialization/detail/container_implementation_level_def.hpp>
```

文件末尾接入：

```cpp
#include <yggr/serialization/set_bson_impl.hpp>
```

## 保存与加载注册

### 标准库容器

```cpp
YGGR_SERIALIZATION_UNLINER_CONTINER_SAVE_LOAD(3, std::set)
YGGR_SERIALIZATION_UNLINER_CONTINER_SAVE_LOAD(3, std::multiset)
```

`3` 对应标准库 set 类模板的三个模板参数，通常为元素类型、比较器和分配器。

### Boost.Container 容器

Boost 版本低于 1.56 时：

```cpp
YGGR_SERIALIZATION_UNLINER_CONTINER_SAVE_LOAD(3, boost::container::set)
YGGR_SERIALIZATION_UNLINER_CONTINER_SAVE_LOAD(3, boost::container::multiset)
```

Boost 版本不低于 1.56 时：

```cpp
YGGR_SERIALIZATION_UNLINER_CONTINER_SAVE_LOAD(4, boost::container::set)
YGGR_SERIALIZATION_UNLINER_CONTINER_SAVE_LOAD(4, boost::container::multiset)
```

该分支用于适配 Boost.Container set 模板参数数量变化。

### yggdrasil 内部容器

Boost 版本不低于 1.56 时，额外注册：

```cpp
YGGR_SERIALIZATION_UNLINER_CONTINER_SAVE_LOAD(
    3, yggr::container::detail::set)
YGGR_SERIALIZATION_UNLINER_CONTINER_SAVE_LOAD(
    3, yggr::container::detail::multiset)
```

## 容器实现级别

文件将支持的集合容器统一设置为：

```cpp
object_serializable
```

示例：

```cpp
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    3, std::set, object_serializable)
```

这表示 set/multiset 按可序列化对象处理，而不是简单原始值。

## 序列化流程

通过 `unliner_container.hpp` 生成的接口通常使用以下流程：

### 保存

1. 保存集合元素数量；
2. 保存元素版本和必要的构造数据；
3. 按集合迭代顺序保存元素。

### 加载

1. 读取集合元素数量；
2. 根据元素版本构造临时元素；
3. 将元素插入目标 set 或 multiset；
4. 由集合容器维护排序和唯一性/重复性约束。

set 不使用 vector 风格的连续数组优化。

## BSON 支持

文件在主 include guard 结束后包含：

```cpp
#include <yggr/serialization/set_bson_impl.hpp>
```

启用 BSON 输入或输出归档后，BSON 扩展会将支持的 set/multiset 注册为 BSON 数组，并使用非线性容器 BSON 实现处理元素。

## Boost 版本兼容

关联的 Boost.Container 注册使用以下条件：

```cpp
#if (BOOST_VERSION < 105600)
```

因此：

- Boost 1.56 之前：Boost.Container set/multiset 按 3 个模板参数注册；
- Boost 1.56 及以上：Boost.Container set/multiset 按 4 个模板参数注册；
- Boost 1.56 及以上：启用 yggdrasil 内部 set/multiset 注册。

## 使用示例

```cpp
#include <yggr/serialization/set.hpp>
#include <set>

void prepare_set()
{
    std::set<int> values;
    values.insert(1);
    values.insert(2);

    // 将 values 交给 Boost.Serialization 或项目 BSON 归档。
}
```

multiset 示例：

```cpp
#include <yggr/serialization/set.hpp>
#include <set>

void prepare_multiset()
{
    std::multiset<int> values;
    values.insert(1);
    values.insert(1);
}
```

## 注意事项

1. 不要同时包含 Boost 原生 `boost/serialization/set.hpp`，否则会触发显式编译错误。
2. set 和 multiset 使用非线性容器序列化逻辑，通过插入恢复集合内容。
3. 宏名称中的 `UNLINER_CONTINER` 是源码既有拼写，不要自行改写。
4. Boost.Container set/multiset 在 Boost 1.56 前后模板参数数量不同，升级 Boost 后应验证注册宏。
5. yggdrasil 内部 set/multiset 仅在 Boost 1.56 及以上版本注册。
6. set 被标记为 `object_serializable`，元素类型仍必须满足对应归档的序列化要求。
7. set 的排序和唯一性由底层容器维护，multiset 的重复元素语义也由底层容器维护。
8. BSON 类型注册将 set/multiset 表示为数组，不代表其节点存储具有连续内存布局。
9. 序列化输入端和输出端应使用兼容的容器实现、元素版本和 BSON 归档配置。

## 相关组件

- `yggr/serialization/unliner_container.hpp`
- `yggr/serialization/set_bson_impl.hpp`
- `yggr/serialization/collections_save_imp.hpp`
- `yggr/serialization/collections_load_imp.hpp`
- `yggr/serialization/detail/container_implementation_level_def.hpp`
- `yggr/container/set.hpp`
- `std::set`
- `std::multiset`
- `boost::container::set`
- `boost::container::multiset`
