# `unordered_unliner_container_bson_impl.hpp`

## 概述

该头文件为无序线性容器提供 BSON 输入归档实现。它针对 `yggr::archive::bson_iarchive<skip_auto_id>` 特化普通无序集合和无序多重集合的元素输入策略，负责按 BSON 数字索引读取元素、插入目标容器，并维护 Boost.Serialization 的对象地址映射。

本文件只处理单个元素的 BSON 输入。集合数量、桶数量、元素版本和遍历流程由无序集合加载实现负责。

## 头文件

```cpp
#include <yggr/serialization/unordered_unliner_container_bson_impl.hpp>
```

该文件通常由以下具体容器 BSON 适配头间接包含：

```cpp
#include <yggr/serialization/unordered_set_bson_impl.hpp>
```

## 编译条件

实现仅在定义以下宏时启用：

```cpp
YGGR_HAS_BSON_IARCHIVE
```

启用时定义主 include guard：

```cpp
__YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTAINER_BSON_IMPL_HPP__
```

如果未定义 `YGGR_HAS_BSON_IARCHIVE`，文件不生成 BSON 输入特化，而是设置内部待处理宏：

```cpp
YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE
```

该宏用于后续 BSON 输入归档环境建立后的重新包含流程。当 BSON 输入归档宏已生效时，如果待处理宏此前存在，文件会先将其取消定义。

## 依赖

启用实现时包含：

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/serialization/nvp.hpp>
#include <boost/ref.hpp>
#include <sstream>
```

这些依赖分别提供基础类型、命名值对读取支持、对象地址辅助以及数字索引字符串构造能力。

## 命名空间

两个输入特化均位于：

```cpp
yggr::serialization::container
```

## `archive_input_unordered_unliner`

该特化用于普通无序集合，例如 `unordered_set`：

```cpp
template<bool skip_auto_id, typename Container>
struct archive_input_unordered_unliner<
    yggr::archive::bson_iarchive<skip_auto_id>,
    Container>
{
    void operator()(
        yggr::archive::bson_iarchive<skip_auto_id>& ar,
        Container& container,
        const unsigned int version,
        u64 index) const;
};
```

### 处理流程

调用 `operator()` 时依次执行：

1. 使用 `Container::value_type` 和 `stack_construct` 创建临时元素；
2. 将 `index` 转换为十进制字符串；
3. 使用该字符串作为 BSON 字段名读取元素；
4. 调用 `container.insert` 插入元素；
5. 仅当插入成功时，调用 `reset_object_address` 将临时对象地址映射到容器中实际元素地址。

核心读取代码为：

```cpp
std::stringstream ss;
ss << index;
ar >> YGGR_SERIALIZATION_NAME_NVP(
    ss.str().data(),
    value_construct.reference());
```

插入和地址修复为：

```cpp
std::pair<typename Container::const_iterator, bool> result =
    container.insert(value_construct.reference());

if(result.second)
{
    ar.reset_object_address(
        boost::addressof(*result.first),
        boost::addressof(value_construct.reference()));
}
```

普通无序集合不允许重复元素，因此当插入失败时，不会更新对象地址。

## `archive_input_unordered_multi_unliner`

该特化用于无序多重集合，例如 `unordered_multiset`：

```cpp
template<bool skip_auto_id, typename Container>
struct archive_input_unordered_multi_unliner<
    yggr::archive::bson_iarchive<skip_auto_id>,
    Container>
{
    void operator()(
        yggr::archive::bson_iarchive<skip_auto_id>& ar,
        Container& container,
        const unsigned int version,
        u64 index) const;
};
```

它同样使用数字字符串索引读取临时元素，但 `insert` 返回新插入元素的迭代器：

```cpp
typename Container::const_iterator result =
    container.insert(value_construct.reference());
```

由于多重集合允许重复元素，每次插入都会产生一个元素，因此随后直接执行：

```cpp
ar.reset_object_address(
    boost::addressof(*result),
    boost::addressof(value_construct.reference()));
```

## BSON 字段命名

元素索引通过 `std::stringstream` 转换为数字字符串：

```cpp
std::stringstream ss;
ss << index;
```

因此元素字段通常按以下形式读取：

```text
"0", "1", "2", ...
```

本文件不决定索引从哪个值开始。索引由上层 BSON 无序集合加载器传入，因此普通归档可能在读取集合元数据后从后续索引开始，而 `skip_auto_id` 模式可能直接从 `"0"` 开始。

## 临时对象与地址跟踪

元素首先由 `stack_construct` 构造在临时存储中。插入目标容器后，容器内元素通常拥有不同地址，因此实现通过 `reset_object_address` 更新归档中的对象跟踪关系，避免后续引用继续指向临时对象。

普通无序集合只在插入成功时执行地址修复；无序多重集合每次插入都执行地址修复。

## 与无序集合加载流程的关系

该文件与以下通用加载流程配合使用：

```text
unordered_collections_load_bson_impl.hpp
  -> 读取集合元数据和元素数量
  -> 生成元素索引
  -> 调用输入策略
      -> 本文件读取 BSON 元素
      -> 插入目标容器
      -> 修复对象地址
```

`InputFunction` 的选择通常由 `is_multi_container<Container>` 决定：

- 普通无序集合使用 `archive_input_unordered_unliner`；
- 无序多重集合使用 `archive_input_unordered_multi_unliner`。

## 宏清理与注意事项

- 本文件只提供 BSON 输入归档实现，不处理 BSON 输出归档。
- `YGGR_HAS_BSON_IARCHIVE` 必须在包含本文件前正确设置，否则相关特化不会参与编译。
- `Container` 必须提供 `value_type`、`insert` 以及与对应插入返回值匹配的容器接口。
- 元素字段名由上层传入的 `u64 index` 决定，必须与 BSON 保存端的字段索引保持一致。
- `version` 会传递给 `stack_construct`，本文件自身不根据版本执行额外分支。
- `YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE` 属于内部重新包含控制宏，业务代码不应直接依赖。
