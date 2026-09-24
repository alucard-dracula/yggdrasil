# `unordered_collections_load_imp.hpp`

## 概述

该头文件提供无序集合的通用 Boost.Serialization 加载实现。它负责读取集合级元数据、准备目标容器，并将每个元素的具体读取和插入操作委托给 `InputFunction`。

该文件适用于哈希集合等无序容器，具体容器类型不在本文件中固定。

## 头文件

```cpp
#include <yggr/serialization/unordered_collections_load_imp.hpp>
```

主要依赖包括：

```cpp
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/collection_size_type.hpp>
#include <boost/archive/detail/basic_iarchive.hpp>
#include <boost/serialization/item_version_type.hpp>
```

## `unordered_collections_loader_switch`

核心实现是以下策略类：

```cpp
template<typename Archive, typename Container, typename InputFunction>
struct unordered_collections_loader_switch
{
    void operator()(Archive& ar, Container& container) const;
};
```

该类位于：

```cpp
yggr::serialization::container
```

模板参数含义：

| 参数 | 含义 |
| --- | --- |
| `Archive` | 使用的输入归档类型 |
| `Container` | 要恢复的无序容器类型 |
| `InputFunction` | 单个元素的读取和插入策略 |

## 加载流程

`operator()` 按以下顺序处理目标容器：

1. 清空容器：

   ```cpp
   container.clear();
   ```

2. 获取归档库版本：

   ```cpp
   boost::archive::library_version_type library_version(
       ar.get_library_version());
   ```

3. 读取元素数量 `count` 和桶数量 `bucket_count`；
4. 在归档版本大于 3 时读取 `item_version`；
5. 在支持 `reserve` 的编译环境中预留桶容量；
6. 创建 `InputFunction` 实例；
7. 按元素数量循环调用输入策略。

## 元数据读取

### 非版本 6 归档

当归档库版本不等于 6 时，直接读取：

```cpp
collection_size_type count;
collection_size_type bucket_count;

ar >> YGGR_SERIALIZATION_NVP(count);
ar >> YGGR_SERIALIZATION_NVP(bucket_count);
```

### 版本 6 归档

当归档库版本等于 6 时，先使用 `unsigned int` 临时变量读取集合大小和桶数量，再转换到 `collection_size_type`：

```cpp
unsigned int c;
unsigned int bc;

ar >> YGGR_SERIALIZATION_NVP(c);
count = c;
ar >> YGGR_SERIALIZATION_NVP(bc);
bucket_count = bc;
```

这是为了兼容 Boost archive version 6 中 hashed collection 使用 `unsigned int` 表示集合大小的历史格式问题，尤其考虑 Windows 64 位环境下的兼容性。

## 元素版本

`item_version` 默认初始化为 0：

```cpp
boost::serialization::item_version_type item_version(0);
```

当归档库版本大于 3 时，从归档读取实际的元素版本：

```cpp
ar >> YGGR_SERIALIZATION_NVP(item_version);
```

之后将该版本传递给 `InputFunction`，由具体元素加载逻辑处理。

## 桶容量预留

在未定义 `__MWERKS__` 时，加载器会调用：

```cpp
container.reserve(bucket_count);
```

这样可以根据归档中的桶数量提前准备哈希容器，减少逐个插入时的重新哈希。定义 `__MWERKS__` 时会跳过该操作，以兼容相关编译环境。

## 元素输入策略

元素处理由 `InputFunction` 完成：

```cpp
InputFunction ifunc;
while(count-- > 0)
{
    ifunc(ar, container, item_version);
}
```

`InputFunction` 通常负责：

- 从归档读取一个元素；
- 构造或恢复元素对象；
- 将元素插入目标容器；
- 必要时维护对象地址映射。

这样集合级加载逻辑与具体容器的元素插入方式相互分离。

## `load_unordered_collection`

文件提供便捷函数：

```cpp
template<typename Archive, typename Container, typename InputFunction>
void load_unordered_collection(
    Archive& ar,
    Container& container);
```

其实现创建对应的 `unordered_collections_loader_switch`，再调用其函数对象：

```cpp
unordered_collections_loader_switch<
    Archive,
    Container,
    InputFunction> loader;
loader(ar, container);
```

由于 `InputFunction` 是显式模板参数而不是函数参数，调用方通常需要通过显式模板参数或外层模板封装确定该策略类型。

## BSON 集成

文件末尾包含：

```cpp
#include <yggr/serialization/unordered_collections_load_bson_impl.hpp>
```

该头文件为 BSON 输入归档提供 `unordered_collections_loader_switch` 的专用特化。BSON 版本会使用数字字符串字段名读取集合元数据和元素，并将索引传递给元素输入函数。

因此：

- 本文件提供通用归档加载流程；
- BSON 专用头文件覆盖 BSON 输入归档的字段读取方式；
- 具体元素读取仍由输入策略完成。

## 注意事项

- 归档中的元素数量、桶数量和元素版本字段顺序必须与保存端格式一致。
- `Container` 必须支持 `clear`，并且通常需要支持 `reserve` 以及由输入策略要求的插入操作。
- `InputFunction` 必须能够接受 `(Archive&, Container&, item_version)` 三个参数。
- 版本 6 的兼容分支只影响集合数量和桶数量的读取类型，不改变后续元素处理流程。
- 该文件负责通用加载，不直接实现具体无序容器的 `save`、`load` 或 `serialize` 重载。
