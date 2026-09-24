# `unliner_container_bson_impl.hpp`

## 概述

该头文件为非线性容器提供 BSON 输入归档支持。它针对 `yggr::archive::bson_iarchive<skip_auto_id>` 特化 `archive_input_set`，负责从 BSON 中读取一个元素、将元素插入容器，并维护 Boost.Serialization 的对象地址映射。

文件名中的 `unliner` 对应项目中的非线性容器输入处理流程。源文件顶部的历史注释写有 `vector.hpp`，但实际 include guard、命名空间和实现均属于 `unliner_container_bson_impl.hpp`。

## 头文件

```cpp
#include <yggr/serialization/unliner_container_bson_impl.hpp>
```

## 编译条件

实现仅在定义以下宏时启用：

```cpp
YGGR_HAS_BSON_IARCHIVE
```

启用 BSON 输入归档后，文件会定义内部 include guard，并包含相关实现：

```cpp
#define __YGGR_SERIALIZATION_UNLINER_CONTAINER_BSON_IMPL_HPP__
```

当 `YGGR_HAS_BSON_IARCHIVE` 未定义时，不生成输入特化，而是设置待处理标记：

```cpp
YGGR_SERIALIZATION_UNLINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE
```

该标记用于后续 BSON 输入归档环境建立后的重新包含流程。

## 依赖

启用实现时包含：

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/serialization/nvp.hpp>
#include <boost/ref.hpp>
#include <sstream>
```

这些依赖分别提供 Yggdrasil 基础类型、命名值对支持、地址处理辅助以及数字索引字符串构造能力。

## `archive_input_set` 特化

特化位于以下命名空间：

```cpp
yggr::serialization::stl
```

主要接口为：

```cpp
template<bool skip_auto_id, typename Container>
struct archive_input_set<
    yggr::archive::bson_iarchive<skip_auto_id>,
    Container>
{
    typename Container::iterator operator()(
        yggr::archive::bson_iarchive<skip_auto_id>& ar,
        Container& container,
        const unsigned int version,
        typename Container::iterator hint,
        yggr::u64 index) const;
};
```

该特化适用于使用 BSON 输入归档的容器，并保留 `skip_auto_id` 参数对归档类型的影响。

## 元素读取流程

调用 `operator()` 时，处理步骤如下：

1. 使用 `Container::value_type` 创建归档构造辅助对象：

   ```cpp
   boost::serialization::detail::stack_construct<archive_type, type>
   ```

2. 将元素下标 `idx` 转换为十进制字符串，例如 `0`、`1`、`2`。

3. 使用该字符串作为 BSON 字段名称读取元素：

   ```cpp
   ar >> YGGR_SERIALIZATION_NAME_NVP(ss.str().data(), t.reference());
   ```

4. 通过带提示位置的 `insert` 将读取出的元素插入容器：

   ```cpp
   typename Container::iterator result =
       s.insert(hint, t.reference());
   ```

5. 调用 `reset_object_address`，将容器中已插入元素的地址与临时构造对象地址关联起来。

6. 返回插入元素的迭代器。

## BSON 字段命名

元素字段名由 `yggr::u64 idx` 转换得到：

```cpp
std::stringstream ss;
ss << idx;
```

因此 BSON 输入应使用数字字符串作为元素键。实际 BSON 文档结构由上层非线性容器序列化逻辑决定，本文件只负责单个元素的读取和插入。

## 对象地址修复

读取过程中元素首先存放在 `stack_construct` 创建的临时对象中。插入容器后，临时对象地址与容器内最终对象地址可能不同，因此实现调用：

```cpp
ar.reset_object_address(
    boost::addressof(*result),
    boost::addressof(t.reference()));
```

这一步使归档系统中的对象跟踪信息指向容器内实际存储的元素，避免后续引用或对象关系仍指向临时对象。

## 宏清理

当 `YGGR_HAS_BSON_IARCHIVE` 已定义时，文件在完成特化定义后会清理：

```cpp
YGGR_SERIALIZATION_UNLINER_CONTAINER_BSON_IMPL_HPP
```

如果未启用 BSON 输入归档，则设置：

```cpp
YGGR_SERIALIZATION_UNLINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE
```

这些宏属于内部实现机制，业务代码不应直接定义、取消定义或依赖其状态。

## 注意事项

- 本文件只处理 BSON 输入归档，不提供 BSON 输出归档实现。
- `Container` 必须提供 `value_type`、`iterator` 以及支持提示迭代器的 `insert` 操作。
- 归档中的元素索引由 `idx` 转换为数字字符串，字段命名必须与上层 BSON 容器格式保持一致。
- `version` 参数会传递给 `stack_construct`，但本特化自身不根据版本执行分支逻辑。
- 缺少 `YGGR_HAS_BSON_IARCHIVE` 时，输入特化不会参与编译。
