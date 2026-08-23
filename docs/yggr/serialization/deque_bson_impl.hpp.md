# `deque_bson_impl.hpp`

## 概述

该头文件为双端队列容器提供 BSON 序列化类型适配，覆盖：

- `std::deque<T, Allocator>`；
- `boost::container::deque<T, Allocator>`。

文件本身主要注册 BSON 类型标识，并将具体的线性容器序列化实现委托给
`liner_container_bson_impl.hpp`。

## 头文件

```cpp
#include <yggr/serialization/deque_bson_impl.hpp>
```

## 编译条件

当以下任一宏已定义时，BSON 类型注册生效：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HAS_BSON_IARCHIVE
```

如果两个宏都未定义，文件不会注册 deque 的 BSON 类型，并设置内部归档阶段宏以配合包含控制逻辑。

## BSON 类型注册

文件通过以下宏将容器标识为 BSON 数组：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    2,
    std::deque,
    bson_typeid_def::E_BSON_TYPE_ARRAY)

YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    2,
    boost::container::deque,
    bson_typeid_def::E_BSON_TYPE_ARRAY)
```

其中 `2` 表示目标容器模板具有两个模板参数，通常对应元素类型和分配器类型。

## 支持的容器

| 容器 | BSON 类型 |
| --- | --- |
| `std::deque<T, Allocator>` | `bson_typeid_def::E_BSON_TYPE_ARRAY` |
| `boost::container::deque<T, Allocator>` | `bson_typeid_def::E_BSON_TYPE_ARRAY` |

该注册只描述容器的 BSON 类型为数组，不负责元素遍历、字段写入或字段读取。

## 具体序列化实现

文件末尾包含：

```cpp
#include <yggr/serialization/liner_container_bson_impl.hpp>
```

该组件负责线性容器的实际 BSON 序列化逻辑。deque 的元素保存、加载、大小信息和字段布局应以该实现及对应归档类型为准。

注意：依赖文件名 `liner_container_bson_impl.hpp` 使用的是源码中的既有拼写。

## 包含保护

文件使用主保护宏：

```cpp
__YGGR_SERIALIZATION_DEQUE_BSON_IMPL_HPP__
```

并使用内部阶段宏：

```cpp
YGGR_SERIALIZATION_DEQUE_BSON_IMPL_HPP_ARCHIVE
```

这些宏用于避免 BSON 类型注册重复展开，通常不应由业务代码直接操作。

## 使用示例

```cpp
#include <yggr/serialization/deque.hpp>
#include <deque>

void prepare_deque()
{
    std::deque<int> values;
    values.push_back(1);
    values.push_back(2);

    // 交给项目 BSON 归档进行序列化。
}
```

## 与通用容器序列化的关系

deque 是线性序列容器，通常使用项目的线性容器 BSON 实现完成元素处理：

1. 通过本文件注册 deque 的 BSON 数组类型；
2. 通过 `liner_container_bson_impl.hpp` 提供具体保存/加载逻辑；
3. 由 BSON 归档处理元素类型的序列化；
4. 元素类型仍需满足对应 BSON 归档要求。

## 注意事项

1. 只有启用 BSON 输入或输出归档时，deque 的 BSON 类型注册才会生效。
2. `std::deque` 和 `boost::container::deque` 都被标记为 BSON 数组类型。
3. 宏参数 `2` 表示 deque 模板具有两个模板参数，使用自定义容器时应匹配实际模板参数数量。
4. 本文件不直接实现 deque 元素的保存和加载。
5. 具体字段布局、集合大小和元素版本处理由线性容器 BSON 实现决定。
6. deque 的元素类型和分配器类型必须满足项目序列化框架的模板要求。
7. 该文件末尾无条件包含 `liner_container_bson_impl.hpp`，其自身实现和宏条件会决定实际生效内容。
8. BSON 类型标识为数组，不代表底层 C++ 容器会转换为连续内存数组。

## 相关组件

- `yggr/nsql_database_system/bson_type_traits.hpp`
- `yggr/serialization/liner_container_bson_impl.hpp`
- `yggr/serialization/collections_save_bson_impl.hpp`
- `yggr/serialization/collection_size_type_bson_impl.hpp`
- `std::deque`
- `boost::container::deque`
