# `multi_index_container_bson_impl.hpp`

## 概述

该头文件为 Boost.MultiIndex 容器提供 BSON 类型适配：

```cpp
boost::multi_index::multi_index_container<...>
```

文件本身主要注册 BSON 类型标识，并将具体的多索引容器保存/加载实现委托给
`multi_index_container_t_bson_impl.hpp`。

## 头文件

```cpp
#include <yggr/serialization/multi_index_container_bson_impl.hpp>
```

## 编译条件

当以下任一宏已定义时，BSON 类型注册生效：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HAS_BSON_IARCHIVE
```

如果两个宏都未定义，文件不会注册多索引容器的 BSON 类型，并设置内部归档阶段宏以配合包含控制逻辑。

## BSON 类型注册

文件使用以下宏注册类型：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    3,
    boost::multi_index::multi_index_container,
    bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT)
```

其中：

- `3`：表示宏按三个模板参数处理目标容器；
- `boost::multi_index::multi_index_container`：目标容器模板；
- `E_BSON_TYPE_SPLICE_OBJECT`：该容器使用拼接对象 BSON 类型表示。

## 支持类型

目标容器通常具有以下形式：

```cpp
boost::multi_index::multi_index_container<
    Value,
    IndexSpecifierList,
    Allocator>
```

实际模板参数及默认参数由当前 Boost.MultiIndex 版本决定，类型注册宏的参数数量应与项目适配层保持一致。

## BSON 类型语义

该容器被标识为：

```cpp
bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT
```

这表示多索引容器在 BSON 类型系统中采用项目定义的拼接对象表示，而不是普通 BSON 数组。

类型标识只描述 BSON 层的容器类别，不负责元素遍历、索引保存或索引重建。

## 具体序列化实现

文件末尾包含：

```cpp
#include <yggr/serialization/multi_index_container_t_bson_impl.hpp>
```

该依赖负责多索引容器的具体 BSON 保存和加载行为，包括可能的：

- 容器元素处理；
- 元素版本和集合大小处理；
- 多索引元素插入；
- 索引结构重建；
- BSON 字段读写。

本文件不直接实现这些流程。

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

void prepare_multi_index()
{
    typedef boost::multi_index::multi_index_container<
        record,
        boost::multi_index::indexed_by<
            boost::multi_index::ordered_unique<
                boost::multi_index::member<record, int, &record::id> > > >
        container_type;

    container_type records;

    // 交给项目 BSON 归档进行序列化。
}
```

实际使用时，通常由多索引容器专用序列化入口自动包含本文件。

## 包含保护

文件使用主保护宏：

```cpp
__YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_BSON_IMPL_HPP__
```

并使用内部阶段宏：

```cpp
YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_BSON_IMPL_HPP_ARCHIVE
```

这些宏用于控制 BSON 类型注册的重复展开，通常不应由业务代码直接操作。

## 与多索引序列化的关系

处理流程通常为：

1. 本文件注册多索引容器的 BSON 类型；
2. `multi_index_container_t_bson_impl.hpp` 提供实际 BSON 序列化实现；
3. 多索引集合保存/加载实现处理元素数量、版本和元素分派；
4. 目标容器根据元素内容重新建立索引。

多索引结构通常不需要将每个索引单独序列化，加载时可以根据元素重新构建索引。

## 注意事项

1. 只有启用 BSON 输入或输出归档时，类型注册才会生效。
2. 多索引容器被注册为 `E_BSON_TYPE_SPLICE_OBJECT`，不是普通 BSON 数组。
3. 宏参数 `3` 是当前适配层使用的模板参数数量，应与项目支持的 Boost 版本保持一致。
4. 本文件不直接实现元素保存、加载或索引重建。
5. 具体 BSON 字段布局和版本处理由 `multi_index_container_t_bson_impl.hpp` 决定。
6. 多索引容器的元素类型、索引描述和分配器必须满足 Boost.MultiIndex 与项目序列化框架要求。
7. 序列化通常保存元素数据，索引结构可能在反序列化时重新建立。
8. 升级 Boost.MultiIndex 或项目 BSON 适配层后，应验证模板参数数量和类型标识兼容性。

## 相关组件

- `yggr/nsql_database_system/bson_type_traits.hpp`
- `yggr/serialization/multi_index_container_t_bson_impl.hpp`
- `yggr/serialization/multi_index_collections_save_bson_impl.hpp`
- `yggr/serialization/multi_index_collections_load_bson_impl.hpp`
- `boost/multi_index_container.hpp`
- `boost/multi_index/indexed_by.hpp`
