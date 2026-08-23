# `multi_index_container.hpp`

## 概述

该头文件是 yggdrasil 对 Boost.MultiIndex 容器进行序列化的统一入口。

它主要完成：

- 引入 yggdrasil 的多索引容器定义；
- 引入多索引容器通用保存/加载实现；
- 为 `boost::multi_index::multi_index_container` 生成序列化自由函数；
- 接入多索引容器的 BSON 类型适配。

## 头文件

```cpp
#include <yggr/serialization/multi_index_container.hpp>
```

## 依赖

该文件包含：

```cpp
#include <yggr/container/multi_index_container.hpp>
#include <yggr/serialization/multi_index_container_t.hpp>
```

文件末尾包含：

```cpp
#include <yggr/serialization/multi_index_container_bson_impl.hpp>
```

## 序列化注册

文件在 `boost::serialization` 命名空间中调用：

```cpp
YGGR_SERIALIZATION_MULTI_INDEX_CONTINER_SAVE_LOAD(
    3,
    boost::multi_index::multi_index_container)
```

其中 `3` 表示当前注册宏按三个模板参数处理目标容器。

宏会生成：

- `save()`；
- `load()`；
- `serialize()`。

`serialize()` 使用 `boost::serialization::split_free()` 分别分派保存和加载操作。

## 目标容器

目标类型为：

```cpp
boost::multi_index::multi_index_container<
    Value,
    IndexSpecifierList,
    Allocator>
```

通常包括：

- 元素类型 `Value`；
- 多索引描述 `IndexSpecifierList`；
- 分配器类型 `Allocator`。

具体模板参数和默认参数由 Boost.MultiIndex 版本决定。

## 序列化流程

### 保存

```text
serialize()
    -> split_free()
    -> save()
    -> save_multi_index_collection()
```

保存过程通常包括：

1. 保存容器元素数量；
2. 保存元素版本；
3. 按容器迭代顺序保存每个元素的构造数据；
4. 保存元素本体。

### 加载

```text
serialize()
    -> split_free()
    -> load()
    -> load_multi_index_collection()
    -> archive_input_multi_index
    -> back_insert()
```

加载过程通常包括：

1. 清空目标容器；
2. 读取元素数量和版本；
3. 构造临时元素；
4. 将元素插入多索引容器第 0 个索引；
5. 由 Boost.MultiIndex 根据元素重建其他索引。

## 元素插入

通用元素加载器使用 `back_insert()` 插入元素：

- 普通索引使用 `insert()`；
- sequenced 索引使用 `push_back()`；
- random_access 索引使用 `push_back()`。

如果唯一索引冲突，插入可能失败；失败时不会执行对应的对象地址跟踪重置。

## BSON 支持

文件在主 include guard 结束后包含：

```cpp
#include <yggr/serialization/multi_index_container_bson_impl.hpp>
```

启用 BSON 输入或输出归档后，多索引容器会被注册为：

```cpp
bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT
```

BSON 的具体字段处理和单元素加载由：

```text
yggr/serialization/multi_index_container_t_bson_impl.hpp
```

及相关多索引 BSON 实现负责。

## 实现级别

源码中保留了实现级别设置的注释，但当前未启用：

```cpp
// #include <yggr/serialization/detail/container_implementation_level_def.hpp>
// YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(...)
```

因此，该文件不会显式覆盖 Boost.MultiIndex 自身的序列化实现级别。

## 使用示例

```cpp
#include <yggr/serialization/multi_index_container.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

struct record
{
    int id;
};

typedef boost::multi_index::multi_index_container<
    record,
    boost::multi_index::indexed_by<
        boost::multi_index::ordered_unique<
            boost::multi_index::member<record, int, &record::id> > > >
    record_container;

void prepare_records()
{
    record_container records;
    records.insert(record{1});
}
```

包含本文件后，`record_container` 可交给兼容的 Boost.Serialization 或项目 BSON 归档处理。

## 注意事项

1. 宏名称中的 `CONTINER` 是源码既有拼写，不要改写为 `CONTAINER`。
2. 宏参数 `3` 必须与目标 Boost.MultiIndex 容器模板参数约定匹配。
3. 多索引容器加载时会清空目标容器，原有元素不会保留。
4. 序列化通常保存元素，不单独保存每个索引结构；索引在加载时重建。
5. 元素插入固定从第 0 个索引开始，唯一索引冲突可能导致数据未插入。
6. BSON 类型为 `E_BSON_TYPE_SPLICE_OBJECT`，不等同于普通 BSON 数组。
7. 当前文件未显式设置容器实现级别，相关代码仍保持注释状态。
8. 元素类型、索引描述和分配器必须满足 Boost.MultiIndex 与归档框架要求。
9. 输入端和输出端应使用兼容的元素版本、构造数据、对象跟踪和 BSON 字段布局。

## 相关组件

- `yggr/container/multi_index_container.hpp`
- `yggr/serialization/multi_index_container_t.hpp`
- `yggr/serialization/multi_index_container_bson_impl.hpp`
- `yggr/serialization/multi_index_container_t_bson_impl.hpp`
- `yggr/serialization/multi_index_container_inserter.hpp`
- `yggr/serialization/multi_index_collections_save_imp.hpp`
- `yggr/serialization/multi_index_collections_load_imp.hpp`
- `boost/multi_index_container.hpp`
