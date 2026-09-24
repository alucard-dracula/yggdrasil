# `unordered_key_value_container_bson_impl.hpp`

## 概述

该头文件为无序键值容器提供 BSON 输入归档实现。它针对 `yggr::archive::bson_iarchive<skip_auto_id>` 特化两个输入处理器，分别处理无序映射容器和无序多重映射容器的元素读取、插入以及 Boost.Serialization 对象地址修复。

文件本身只负责单个键值元素的输入处理；集合数量、桶数量和元素遍历由无序容器加载流程负责。

## 头文件

```cpp
#include <yggr/serialization/unordered_key_value_container_bson_impl.hpp>
```

该文件通常由无序映射容器的序列化实现间接包含。

## 编译条件

实现仅在定义以下宏时启用：

```cpp
YGGR_HAS_BSON_IARCHIVE
```

启用 BSON 输入归档后，文件定义主 include guard：

```cpp
__YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTAINER_BSON_IMPL_HPP__
```

如果未定义 `YGGR_HAS_BSON_IARCHIVE`，文件不生成输入处理器特化，而是设置内部待处理宏：

```cpp
YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTAINER_BSON_IMPL_HPP_IARCHIVE
```

该宏用于项目的 BSON 实现重新包含流程。相关宏均属于内部实现细节，业务代码不应直接依赖。

## 依赖

启用实现时包含：

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/serialization/nvp.hpp>
#include <boost/ref.hpp>
#include <sstream>
```

其中 `yggrdef.h` 提供基础类型（包括 `u64`），`nvp.hpp` 提供按名称读取归档值所需的命名值对支持，`boost/ref.hpp` 用于获取对象地址，`sstream` 用于生成 BSON 字段名。

## 命名空间

两个特化均位于：

```cpp
yggr::serialization::container
```

## `archive_input_unordered_key_value`

该特化用于无序映射容器，例如 `unordered_map`：

```cpp
template<bool skip_auto_id, typename Container>
struct archive_input_unordered_key_value<
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

调用 `operator()` 时：

1. 通过 `Container::value_type` 和 `stack_construct` 创建临时键值对象。
2. 将 `index` 转换为十进制字符串，作为 BSON 字段名。
3. 使用 `YGGR_SERIALIZATION_NAME_NVP` 从归档读取一个键值对象。
4. 调用 `container.insert` 将对象插入目标容器。
5. 如果插入成功，调用 `reset_object_address` 将归档中的对象地址从临时对象修正为容器中实际存储的值对象地址。

核心读取形式为：

```cpp
std::stringstream ss;
ss << index;
ar >> YGGR_SERIALIZATION_NAME_NVP(
    ss.str().data(),
    value_construct.reference());
```

### 插入结果与地址修复

无序映射容器的 `insert` 返回一个包含迭代器和插入状态的 `std::pair`。只有在 `result.second` 为 `true` 时，才将容器中元素的 `second` 地址与临时对象的 `second` 地址关联：

```cpp
ar.reset_object_address(
    boost::addressof(result.first->second),
    boost::addressof(value_construct.reference().second));
```

这符合键值容器中键通常不可修改、而映射值可能参与对象跟踪的使用方式。若键已存在导致插入失败，则不会执行该地址修复。

## `archive_input_unordered_multi_key_value`

该特化用于允许重复键的无序多重映射容器，例如 `unordered_multimap`：

```cpp
template<bool skip_auto_id, typename Container>
struct archive_input_unordered_multi_key_value<
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

处理步骤与普通无序映射基本一致：

1. 使用 `stack_construct` 创建 `Container::value_type` 临时对象。
2. 将元素索引转换为数字字符串 BSON 字段名。
3. 从 BSON 输入归档读取键值对象。
4. 使用 `container.insert` 插入元素。
5. 将归档对象地址重置为容器中插入元素的值地址。

由于多重映射允许重复键，`insert` 返回插入元素的迭代器，而不是带布尔状态的 `pair`：

```cpp
typename Container::const_iterator result =
    container.insert(value_construct.reference());
```

随后执行：

```cpp
ar.reset_object_address(
    boost::addressof(result->second),
    boost::addressof(value_construct.reference()));
```

## BSON 字段命名

元素索引通过流转换为数字字符串：

```cpp
std::stringstream ss;
ss << index;
```

因此上层无序容器 BSON 数据通常按类似以下名称读取元素：

```text
"0", "1", "2", ...
```

本文件不负责读取集合大小、桶数量或元素版本；这些元数据和遍历逻辑由无序集合加载实现提供。`version` 参数会传给 `stack_construct`，本文件自身不基于版本执行分支。

## 临时对象与地址跟踪

元素先由 `boost::serialization::detail::stack_construct` 构造在临时存储中，插入容器后通常会获得新的实际存储地址。调用 `reset_object_address` 可以更新归档跟踪表，使后续引用指向容器中的对象，而不是已经结束生命周期的临时对象。

普通无序映射只在插入成功时修复地址；无序多重映射每次插入都会产生新元素，因此每次都会执行地址修复。

## 使用限制

- 本文件只处理 BSON 输入归档，不提供 BSON 输出归档实现。
- `Container` 必须提供 `value_type` 和与对应容器类型匹配的 `insert` 操作。
- BSON 元素字段名必须与上层加载流程使用的数字索引一致。
- `YGGR_HAS_BSON_IARCHIVE` 未定义时，相关特化不会参与编译。
- include guard 和 `_IARCHIVE` 后缀宏是内部控制宏，不应由业务代码手工设置或取消定义。
