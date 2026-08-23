# `collections_load_bson_impl.hpp`

## 概述

该头文件为 BSON 输入归档提供通用集合加载实现，特化
`yggr::serialization::container::collections_loader_switch`。

主要职责包括：

- 清空目标集合；
- 读取集合元素数量；
- 按归档库版本读取元素版本；
- 调用集合状态/容量处理器 `R`；
- 使用 `InputFunction` 逐个读取元素；
- 在元素加载过程中传递和更新插入提示迭代器。

实现仅在定义 `YGGR_HAS_BSON_IARCHIVE` 时启用。

## 头文件

```cpp
#include <yggr/serialization/collections_load_bson_impl.hpp>
```

该文件通常由集合或容器 BSON 序列化组件间接包含。

## 编译条件

当定义以下宏时启用实现：

```cpp
YGGR_HAS_BSON_IARCHIVE
```

未定义该宏时，不生成 BSON 集合加载特化，并设置内部阶段宏以配合包含控制逻辑。

## 命名空间

实现位于：

```cpp
yggr::serialization::container
```

## 类型特化

```cpp
template<bool skip_auto_id,
         typename Container,
         typename InputFunction,
         typename R>
struct collections_loader_switch<
    yggr::archive::bson_iarchive<skip_auto_id>,
    Container,
    InputFunction,
    R>;
```

模板参数：

| 参数 | 说明 |
| --- | --- |
| `skip_auto_id` | BSON 输入归档的自动 ID 配置。 |
| `Container` | 目标集合类型。 |
| `InputFunction` | 单个元素的读取和插入函数对象。 |
| `R` | 集合加载前的状态、容量或预分配处理器。 |

## `operator()`

```cpp
void operator()(
    yggr::archive::bson_iarchive<skip_auto_id>& ar,
    Container& s) const;
```

从 BSON 输入归档 `ar` 加载整个集合到 `s`。

函数开始时执行：

```cpp
s.clear();
```

因此集合中的原有元素会被删除。

## 加载流程

1. 清空目标集合；
2. 创建 `collection_size_type count`；
3. 读取归档库版本；
4. 从数字字段 `"0"` 读取集合元素数量；
5. 当库版本大于 `3` 时，从数字字段 `"1"` 读取元素版本；
6. 创建 `R` 并调用 `rx(s, count)`；
7. 创建 `InputFunction`；
8. 将 `hint` 初始化为 `s.begin()`；
9. 循环调用 `ifunc(ar, s, item_version, hint, idx)`；
10. 使用返回的迭代器更新下一次调用的 `hint`。

## BSON 字段布局

当前实现使用数字字符串字段名：

| 条件 | 字段名 | 内容 |
| --- | --- | --- |
| 所有支持版本 | `"0"` | 集合元素数量。 |
| 归档库版本大于 `3` | `"1"` | 元素版本。 |
| 归档库版本大于 `3` | `"2"` 起 | 集合元素。 |
| 归档库版本不大于 `3` | `"1"` 起 | 集合元素。 |

元素字段的实际读取由 `InputFunction` 完成。

## 元素版本

元素版本初始化为：

```cpp
boost::serialization::item_version_type item_version(0);
```

当归档库版本大于 `3` 时读取归档中的元素版本，否则保留 `0`。

## `R` 处理器

`R` 在元素加载前被调用：

```cpp
R rx;
rx(s, count);
```

该处理器可用于：

- 根据集合数量预留容量；
- 调整目标集合状态；
- 为特定容器执行加载前准备。

具体行为由调用方提供的 `R` 类型决定。

## `InputFunction` 约定

`InputFunction` 应提供兼容以下形式的调用操作：

```cpp
typename Container::iterator operator()(
    archive_type& ar,
    Container& container,
    const boost::serialization::item_version_type& item_version,
    typename Container::iterator hint,
    yggr::u64 idx) const;
```

该函数通常负责：

- 根据 `idx` 生成 BSON 字段名；
- 读取一个元素；
- 将元素插入容器；
- 返回下一次加载所需的插入 hint。

## Hint 迭代器

加载器将 hint 初始化为：

```cpp
typename Container::iterator hint = s.begin();
```

每次调用 `InputFunction` 后，用其返回值更新 hint：

```cpp
hint = ifunc(ar, s, item_version, hint, idx);
```

这允许关联容器根据上一次插入位置优化后续元素插入。

## 使用示例

```cpp
#include <yggr/serialization/collections_load_impl.hpp>

void load_collection(
    yggr::archive::bson_iarchive<false>& archive,
    std::vector<int>& values)
{
    typedef my_input_function input_function;
    typedef my_reserve_function reserve_function;

    typedef yggr::serialization::container::collections_loader_switch<
        yggr::archive::bson_iarchive<false>,
        std::vector<int>,
        input_function,
        reserve_function> loader_type;

    loader_type loader;
    loader(archive, values);
}
```

实际项目中通常由容器通用加载实现自动选择该特化。

## 与其他 BSON 加载实现的关系

- `array_bson_load_impl.hpp`：处理固定数组加载；
- `liner_container_bson_impl.hpp`：处理线性容器的单元素读取；
- `key_value_container_bson_impl.hpp`：处理键值容器的单元素读取；
- 本文件：负责集合整体数量、版本、预处理和元素循环。

## 注意事项

1. 只有定义 `YGGR_HAS_BSON_IARCHIVE` 时，该特化才会参与编译。
2. `operator()` 会先清空目标集合，原有元素不会保留。
3. 数量字段固定从 `"0"` 读取，元素版本字段在库版本大于 `3` 时占用 `"1"`。
4. 归档库版本不大于 `3` 时不读取元素版本，元素字段从 `"1"` 开始。
5. `R` 必须提供 `operator()(Container&, collection_size_type&)` 或兼容调用形式。
6. `InputFunction` 必须返回可作为下一次 hint 的容器迭代器。
7. 元素数量决定 `InputFunction` 的调用次数，错误数量可能导致读取失败或数据不完整。
8. 字段索引 `idx` 使用 `yggr::u64`，元素字段名由输入函数负责生成和读取。
9. 容器的预留容量、插入策略、对象跟踪和异常处理由 `R`、`InputFunction` 及归档实现共同决定。

## 相关组件

- `yggr/serialization/array_bson_load_impl.hpp`
- `yggr/serialization/liner_container_bson_impl.hpp`
- `yggr/serialization/key_value_container_bson_impl.hpp`
- `yggr/serialization/multi_index_collections_load_bson_impl.hpp`
- `yggr/serialization/collection_size_type.hpp`
- `yggr/archive/bson_iarchive.hpp`
