# `multi_index_container_t_bson_impl.hpp`

## 概述

该头文件为 Boost.MultiIndex 容器提供 BSON 输入归档的单元素加载实现，特化
`archive_input_multi_index`。

主要职责包括：

- 按数字字符串字段名读取一个多索引元素；
- 根据元素版本构造临时对象；
- 通过 `back_insert()` 将元素插入多索引容器；
- 插入成功后修正 Boost.Serialization 的对象地址跟踪。

实现仅在定义 `YGGR_HAS_BSON_IARCHIVE` 时启用。

## 头文件

```cpp
#include <yggr/serialization/multi_index_container_t_bson_impl.hpp>
```

该文件通常由 `multi_index_container_bson_impl.hpp` 或多索引容器加载流程间接包含，不建议业务代码直接依赖其内部特化。

## 编译条件

当定义以下宏时启用实现：

```cpp
YGGR_HAS_BSON_IARCHIVE
```

未定义该宏时，不生成 BSON 多索引元素加载特化，并设置内部阶段宏以配合包含控制逻辑。

## 命名空间

实现位于：

```cpp
yggr::serialization::container
```

## 类型特化

```cpp
template<bool skip_auto_id, typename Container>
struct archive_input_multi_index<
    yggr::archive::bson_iarchive<skip_auto_id>,
    Container>;
```

模板参数：

| 参数 | 说明 |
| --- | --- |
| `skip_auto_id` | BSON 输入归档的自动 ID 配置。 |
| `Container` | 目标多索引容器类型。 |

## `operator()`

```cpp
inline void operator()(
    yggr::archive::bson_iarchive<skip_auto_id>& ar,
    Container& s,
    const unsigned int v,
    u64 idx) const;
```

加载并尝试插入一个多索引元素。

参数说明：

| 参数 | 说明 |
| --- | --- |
| `ar` | BSON 输入归档。 |
| `s` | 目标多索引容器。 |
| `v` | 元素序列化版本，传递给 `stack_construct`。 |
| `idx` | BSON 数字字段索引，用于生成元素字段名。 |

该函数没有返回值，插入是否成功通过内部 `std::pair` 的 `second` 字段判断。

## 加载流程

1. 将 BSON 输入归档类型保存为 `archive_type`；
2. 获取 `Container::value_type` 作为元素类型；
3. 使用 `stack_construct<archive_type, type>` 按版本 `v` 构造临时对象；
4. 将 `idx` 转换为数字字符串；
5. 从对应数字字段读取临时对象；
6. 调用 `back_insert(s, t.reference())` 插入目标多索引容器；
7. 插入成功时调用 `reset_object_address()` 修正对象地址跟踪。

核心读取形式为：

```cpp
std::stringstream ss;
ss << idx;
ar >> YGGR_SERIALIZATION_NAME_NVP(
    ss.str().data(),
    t.reference());
```

## BSON 字段命名

元素字段名由 `idx` 生成数字字符串：

```text
idx = 0 -> "0"
idx = 1 -> "1"
idx = 2 -> "2"
...
```

集合数量和元素版本通常由上层多索引集合加载器读取；本文件只负责读取指定索引对应的单个元素。

## 元素插入

元素通过以下函数插入：

```cpp
std::pair<typename Container::const_iterator, bool> result =
    back_insert(s, t.reference());
```

`back_insert()` 会根据多索引容器第 0 个索引的类型选择插入方式：

- 普通索引使用 `insert()`；
- sequenced 索引使用 `push_back()`；
- random_access 索引使用 `push_back()`。

如果插入失败，例如违反唯一索引约束，则不会执行对象地址跟踪重置。

## 对象地址跟踪

仅当插入成功时，函数才调用：

```cpp
ar.reset_object_address(
    boost::addressof(*result.first),
    boost::addressof(t.reference()));
```

该操作将临时对象的归档跟踪地址关联到容器中的实际元素，避免跟踪信息继续指向已经结束生命周期的临时对象。

## 使用示例

```cpp
#include <yggr/serialization/multi_index_container_t.hpp>

void load_multi_index_element(
    yggr::archive::bson_iarchive<false>& archive,
    my_multi_index_container& container)
{
    typedef yggr::serialization::container::
        archive_input_multi_index<
            yggr::archive::bson_iarchive<false>,
            my_multi_index_container> loader_type;

    loader_type loader;
    loader(archive, container, 0, 0);
}
```

实际加载流程通常由 `multi_index_collections_load_bson_impl.hpp` 传入元素版本和字段索引后调用该适配器。

## 与相关组件的关系

- `multi_index_container_bson_impl.hpp`：注册多索引容器的 BSON 类型；
- `multi_index_collections_load_bson_impl.hpp`：读取数量、版本并循环分派元素；
- `multi_index_container_inserter.hpp`：根据索引类型执行实际插入；
- 本文件：读取并插入单个多索引元素。

## 注意事项

1. 只有定义 `YGGR_HAS_BSON_IARCHIVE` 时，该特化才会参与编译。
2. `idx` 对应的 BSON 字段名必须是数字字符串。
3. `Container` 必须是可由 `back_insert()` 处理的 Boost.MultiIndex 容器。
4. `Container::value_type` 必须满足 `stack_construct` 和 BSON 输入归档的反序列化要求。
5. 元素版本 `v` 会影响临时对象的构造和加载行为。
6. 插入失败时不会调用 `reset_object_address()`，失败原因可能包括唯一索引冲突。
7. 多索引容器的其他索引通常由 Boost.MultiIndex 根据插入元素自动维护。
8. 本文件不负责读取集合数量、元素版本或控制整体元素循环。
9. 该实现依赖 Boost.MultiIndex 的 detail 插入适配和对象地址跟踪机制，升级相关库后应验证兼容性。

## 相关组件

- `yggr/serialization/multi_index_container_bson_impl.hpp`
- `yggr/serialization/multi_index_collections_load_bson_impl.hpp`
- `yggr/serialization/multi_index_container_inserter.hpp`
- `yggr/serialization/nvp.hpp`
- `yggr/archive/bson_iarchive.hpp`
- `boost/archive/detail/iserializer.hpp`
- `boost/multi_index_container.hpp`
