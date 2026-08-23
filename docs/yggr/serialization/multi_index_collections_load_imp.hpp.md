# `multi_index_collections_load_imp.hpp`

## 概述

该头文件提供多索引容器的通用反序列化实现，主要用于加载哈希容器或其他需要自定义元素插入逻辑的容器。

核心设计是将整体加载流程与单个元素的加载方式分离：

- 本文件负责清空容器、读取数量、读取元素版本和控制循环；
- `InputFunction` 负责读取并插入单个元素；
- BSON 输入归档可通过专用特化使用数字字段名读取元素。

## 头文件

```cpp
#include <yggr/serialization/multi_index_collections_load_imp.hpp>
```

## 命名空间

实现位于：

```cpp
yggr::serialization::container
```

## 依赖

主要依赖：

- `yggr/serialization/nvp.hpp`：名称-值对读取；
- `yggr/serialization/collection_size_type.hpp`：集合大小类型；
- `boost/archive/detail/basic_iarchive.hpp`：归档库版本类型；
- `boost/serialization/item_version_type.hpp`：元素版本类型。

文件末尾包含：

```cpp
#include <yggr/serialization/multi_index_collections_load_bson_impl.hpp>
```

用于接入 BSON 输入归档特化。

## `multi_index_collection_loader_switch`

```cpp
template<typename Archive,
         typename Container,
         typename InputFunction>
struct multi_index_collection_loader_switch;
```

这是多索引容器的通用加载器。

### 模板参数

| 参数 | 说明 |
| --- | --- |
| `Archive` | 输入归档类型。 |
| `Container` | 目标多索引容器类型。 |
| `InputFunction` | 单个元素的加载和插入函数对象。 |

### `operator()`

```cpp
void operator()(Archive& ar, Container& s) const;
```

从归档 `ar` 加载整个容器到 `s`。

## 加载流程

函数执行以下步骤：

1. 调用 `s.clear()` 清空目标容器；
2. 创建 `collection_size_type count` 保存元素数量；
3. 创建 `item_version_type item_version(0)` 保存元素版本；
4. 读取归档库版本 `ar.get_library_version()`；
5. 根据库版本读取元素数量；
6. 当库版本大于 `3` 时读取元素版本；
7. 创建 `InputFunction` 实例；
8. 循环调用 `ifunc(ar, s, item_version)` 加载元素，直到数量归零。

## 归档库版本兼容

### 非版本 6

当归档库版本不等于 `6` 时，直接读取 `collection_size_type`：

```cpp
ar >> YGGR_SERIALIZATION_NVP(count);
```

### 版本 6

当归档库版本等于 `6` 时，先使用 `unsigned int` 读取数量，再转换为
`collection_size_type`：

```cpp
unsigned int c;
ar >> YGGR_SERIALIZATION_NVP(c);
count = c;
```

这是对 Boost.Serialization 版本 6 中集合大小类型变化的兼容处理，尤其针对哈希容器在部分平台上的历史实现差异。

### 元素版本

当库版本大于 `3` 时读取：

```cpp
ar >> YGGR_SERIALIZATION_NVP(item_version);
```

版本不大于 `3` 时不读取元素版本，保留初始值 `0`。

## `InputFunction` 约定

通用加载器按以下形式调用输入函数：

```cpp
ifunc(ar, s, item_version);
```

因此，`InputFunction` 应提供兼容的函数调用操作，例如：

```cpp
void operator()(
    Archive& ar,
    Container& container,
    const boost::serialization::item_version_type& item_version) const;
```

该函数通常负责：

- 读取一个元素；
- 根据元素版本构造对象；
- 将元素插入多索引容器；
- 处理对象地址跟踪和重复键等情况。

## `load_multi_index_collection`

```cpp
template<typename Archive,
         typename Container,
         typename InputFunction>
void load_multi_index_collection(
    Archive& ar,
    Container& s);
```

创建 `multi_index_collection_loader_switch<Archive, Container, InputFunction>`，并调用其 `operator()`。

使用时通常需要显式提供 `InputFunction` 模板参数，因为该参数无法仅从函数的两个形参中自动推导：

```cpp
load_multi_index_collection<
    archive_type,
    container_type,
    input_function_type>(archive, container);
```

## 使用示例

```cpp
#include <yggr/serialization/multi_index_collections_load_imp.hpp>

struct input_function
{
    void operator()(
        my_archive& archive,
        my_container& container,
        const boost::serialization::item_version_type& version) const
    {
        my_value value;
        archive >> value;
        container.insert(value);
        (void)version;
    }
};

void load_container(my_archive& archive, my_container& container)
{
    yggr::serialization::container::load_multi_index_collection<
        my_archive,
        my_container,
        input_function>(archive, container);
}
```

## BSON 分派

文件末尾接入 `multi_index_collections_load_bson_impl.hpp`。当归档类型为
`yggr::archive::bson_iarchive<skip_auto_id>` 时，会选择 BSON 专用特化：

- 数量和元素版本使用数字字符串字段名读取；
- 元素索引传递给 `InputFunction`；
- 具体元素读取和插入仍由输入函数完成。

## 容器要求

`Container` 至少应支持：

```cpp
void clear();
```

并且必须能被 `InputFunction` 按项目定义的方式插入元素。通用加载器不直接要求 `Container` 提供 `push_back()` 或 `insert()`，这些要求由 `InputFunction` 决定。

## 数据布局

通用归档通常按以下逻辑顺序保存：

1. 集合元素数量；
2. 元素版本，当归档库版本大于 `3` 时；
3. 按顺序保存的集合元素。

字段的具体名称和表示由归档类型及名称-值对宏决定。BSON 归档可能使用数字字符串字段名。

## 注意事项

1. `operator()` 会先调用 `s.clear()`，加载前的容器内容会被删除。
2. 归档库版本为 `6` 时，集合数量按 `unsigned int` 读取。
3. 归档库版本不大于 `3` 时不会读取元素版本字段。
4. `InputFunction` 必须提供与通用加载器匹配的三参数调用接口。
5. `load_multi_index_collection()` 的 `InputFunction` 通常需要显式指定。
6. 元素数量决定输入函数的调用次数，错误的数量可能导致读取不完整或越界读取。
7. 通用加载器不负责元素插入、重复键处理或对象地址跟踪，这些行为由 `InputFunction` 实现。
8. BSON 版本特化会将元素索引作为额外参数传给输入函数，通用归档版本则不传索引。
9. 具体版本兼容、异常和对象跟踪行为由 Boost.Serialization、目标归档和输入函数共同决定。

## 相关组件

- `yggr/serialization/multi_index_collections_load_bson_impl.hpp`
- `yggr/serialization/collections_load_imp.hpp`
- `yggr/serialization/key_value_container_bson_impl.hpp`
- `yggr/serialization/liner_container_bson_impl.hpp`
- `yggr/serialization/collection_size_type.hpp`
- `boost/archive/detail/basic_iarchive.hpp`
- `boost/serialization/item_version_type.hpp`
