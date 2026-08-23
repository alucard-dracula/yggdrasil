# `multi_index_collections_load_bson_impl.hpp`

## 概述

该头文件为 BSON 输入归档提供多索引容器加载实现，特化
`multi_index_collection_loader_switch`，用于从 BSON 数据恢复多索引容器。

主要职责包括：

- 清空目标容器；
- 按归档库版本读取集合元素数量；
- 读取元素版本信息；
- 按数字字符串字段索引逐项加载元素；
- 将具体元素插入或恢复逻辑委托给 `InputFunction`。

实现仅在定义 `YGGR_HAS_BSON_IARCHIVE` 时启用。

## 头文件

```cpp
#include <yggr/serialization/multi_index_collections_load_bson_impl.hpp>
```

该文件通常由多索引容器序列化组件间接包含，不建议业务代码直接依赖其内部特化。

## 编译条件

当定义以下宏时启用实现：

```cpp
YGGR_HAS_BSON_IARCHIVE
```

未定义该宏时，不生成 BSON 多索引容器加载特化，并设置内部阶段宏以配合包含控制逻辑。

## 命名空间

实现位于：

```cpp
yggr::serialization::container
```

## 类型特化

```cpp
template<bool skip_auto_id,
         typename Container,
         typename InputFunction>
struct multi_index_collection_loader_switch<
    yggr::archive::bson_iarchive<skip_auto_id>,
    Container,
    InputFunction>;
```

模板参数：

| 参数 | 说明 |
| --- | --- |
| `skip_auto_id` | BSON 输入归档的自动 ID 配置。 |
| `Container` | 目标多索引容器类型。 |
| `InputFunction` | 单个元素的加载函数对象，负责读取并插入一个元素。 |

## `operator()`

```cpp
void operator()(
    yggr::archive::bson_iarchive<skip_auto_id>& ar,
    Container& s) const;
```

从 BSON 输入归档 `ar` 加载整个多索引容器到 `s`。

函数开始时会调用：

```cpp
s.clear();
```

因此目标容器中原有元素会被删除。

## 加载流程

### 1. 初始化状态

创建以下状态对象：

```cpp
collection_size_type count;
boost::serialization::item_version_type item_version(0);
boost::archive::library_version_type library_version(
    ar.get_library_version());
```

同时将字段索引 `idx` 初始化为 `0`。

### 2. 读取元素数量

当归档库版本不是 `6` 时，从数字字段名 `"0"` 读取
`collection_size_type`：

```cpp
ar >> YGGR_SERIALIZATION_NAME_NVP("0", count);
```

当归档库版本为 `6` 时，使用 `unsigned int` 读取字段 `"0"`，随后赋值给 `count`：

```cpp
unsigned int c;
ar >> YGGR_SERIALIZATION_NAME_NVP("0", c);
count = c;
```

该分支用于兼容 Boost.Serialization 版本 6 中集合大小类型变更造成的哈希容器兼容问题。

### 3. 读取元素版本

当归档库版本大于 `3` 时，从下一个数字字段读取元素版本：

```cpp
ar >> YGGR_SERIALIZATION_NAME_NVP(
    "1",
    item_version);
```

版本不大于 `3` 时不读取元素版本，保留初始值 `0`。

### 4. 加载元素

创建 `InputFunction` 对象，并循环调用：

```cpp
ifunc(ar, s, item_version, idx);
```

每次调用后递增 `idx`，直到加载完 `count` 个元素。

## BSON 字段布局

字段名由递增的数字字符串构成：

### 归档库版本不等于 6 且大于 3

| 字段名 | 内容 |
| --- | --- |
| `"0"` | 集合元素数量。 |
| `"1"` | 元素版本。 |
| `"2"` 起 | 多索引容器元素。 |

### 归档库版本等于或不大于 3

- 版本 `6`：`"0"` 保存 `unsigned int` 数量；
- 版本不大于 `3`：`"0"` 保存数量，不读取元素版本；
- 元素字段从当前 `idx` 开始，具体起始索引取决于是否读取元素版本。

每个元素的实际读取方式由 `InputFunction` 决定。

## `InputFunction` 约定

`InputFunction` 应提供兼容以下形式的调用操作：

```cpp
InputFunction::operator()(
    archive_type& ar,
    Container& s,
    const boost::serialization::item_version_type& item_version,
    u64 idx);
```

实际参数类型以目标多索引容器加载实现的定义为准。

该函数对象通常负责：

- 使用 `idx` 生成 BSON 字段名；
- 从归档读取一个元素；
- 根据元素版本构造对象；
- 将元素插入多索引容器；
- 修正对象地址跟踪关系。

## 使用示例

```cpp
#include <yggr/serialization/multi_index_collections_load_impl.hpp>

void load_multi_index(
    yggr::archive::bson_iarchive<false>& archive,
    my_multi_index_container& container)
{
    typedef yggr::serialization::container::
        multi_index_collection_loader_switch<
            yggr::archive::bson_iarchive<false>,
            my_multi_index_container,
            my_input_function> loader_type;

    loader_type loader;
    loader(archive, container);
}
```

实际项目中通常由更高层的多索引容器序列化接口自动选择该特化。

## 版本兼容

该实现包含两个历史兼容分支：

- 归档库版本为 `6` 时，将集合大小按 `unsigned int` 读取；
- 归档库版本大于 `3` 时才读取元素版本字段。

版本处理用于兼容不同 Boost.Serialization 文件格式，尤其是版本 6 中集合大小类型变化带来的差异。

## 与其他 BSON 加载实现的关系

- `array_bson_load_impl.hpp` 负责数组类型的 BSON 加载；
- `key_value_container_bson_impl.hpp` 负责键值容器单个元素加载；
- `liner_container_bson_impl.hpp` 负责线性容器单个元素加载；
- 本文件负责多索引容器整体的数量、版本和元素循环分派。

## 注意事项

1. 只有定义 `YGGR_HAS_BSON_IARCHIVE` 时，该特化才会参与编译。
2. `operator()` 会先清空目标容器，加载过程不会保留已有元素。
3. BSON 字段名使用数字字符串，数量字段从 `"0"` 开始。
4. 归档库版本为 `6` 时，数量字段必须能按 `unsigned int` 读取。
5. 归档库版本不大于 `3` 时不会读取元素版本字段。
6. `InputFunction` 必须提供与当前特化兼容的调用接口，并负责具体元素插入。
7. 元素数量决定 `InputFunction` 的调用次数，错误数量可能导致读取失败或数据不完整。
8. 目标容器的索引重建、元素跟踪和构造数据处理由 `InputFunction` 及上层实现负责。
9. 具体 BSON 字段读取、版本和异常行为由 BSON 输入归档及 Boost.Serialization 共同决定。

## 相关组件

- `yggr/serialization/array_bson_load_impl.hpp`
- `yggr/serialization/key_value_container_bson_impl.hpp`
- `yggr/serialization/liner_container_bson_impl.hpp`
- `yggr/serialization/collections_load_imp.hpp`
- `boost/serialization/collections_load_imp.hpp`
- `boost/serialization/item_version_type.hpp`
- `yggr/archive/bson_iarchive.hpp`
