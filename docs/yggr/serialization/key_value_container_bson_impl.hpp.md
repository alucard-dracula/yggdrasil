# `key_value_container_bson_impl.hpp`

## 概述

该头文件为键值容器提供 BSON 输入归档适配，实现 `archive_input_map` 的 BSON 特化。

主要功能包括：

- 按数字字符串字段名读取键值元素；
- 使用元素版本构造临时键值对象；
- 将对象插入目标容器；
- 修正 Boost.Serialization 的对象地址跟踪关系。

实现仅在定义 `YGGR_HAS_BSON_IARCHIVE` 时启用。

## 头文件

```cpp
#include <yggr/serialization/key_value_container_bson_impl.hpp>
```

该文件通常由键值容器或 BSON 输入归档相关组件间接包含，不建议业务代码直接依赖其内部特化。

## 编译条件

当定义以下宏时启用实现：

```cpp
YGGR_HAS_BSON_IARCHIVE
```

未定义该宏时，不生成 BSON 输入映射特化，并设置内部阶段宏以配合包含控制逻辑。

## 命名空间

实现位于：

```cpp
yggr::serialization::stl
```

## 类型特化

```cpp
template<bool skip_auto_id, typename Container>
struct archive_input_map<
    yggr::archive::bson_iarchive<skip_auto_id>,
    Container>;
```

其中：

- `skip_auto_id`：BSON 输入归档的自动 ID 配置；
- `Container`：目标键值容器类型，通常提供 `value_type`、`iterator` 和带 hint 的 `insert()`。

## `operator()`

```cpp
inline typename Container::iterator operator()(
    yggr::archive::bson_iarchive<skip_auto_id>& ar,
    Container& s,
    const unsigned int v,
    typename Container::iterator hint,
    yggr::u64 idx) const;
```

参数说明：

| 参数 | 说明 |
| --- | --- |
| `ar` | BSON 输入归档。 |
| `s` | 目标键值容器。 |
| `v` | 容器元素的序列化版本。 |
| `hint` | 插入目标容器时使用的迭代器提示位置。 |
| `idx` | BSON 字段索引，用于生成数字字符串字段名。 |

返回值是插入元素在目标容器中的迭代器。

## 读取流程

函数按以下步骤读取一个键值元素：

1. 将归档类型别名为 `bson_iarchive<skip_auto_id>`；
2. 获取 `Container::value_type` 作为临时元素类型；
3. 使用 `stack_construct` 根据归档版本 `v` 构造临时对象；
4. 将 `idx` 转换为数字字符串；
5. 使用该字符串作为名称-值对字段名，从 BSON 归档读取临时对象；
6. 使用 `s.insert(hint, t.reference())` 将对象插入目标容器；
7. 调用 `reset_object_address()` 修正对象跟踪地址；
8. 返回插入结果迭代器。

核心读取形式为：

```cpp
std::stringstream ss;
ss << idx;
ar >> YGGR_SERIALIZATION_NAME_NVP(
    ss.str().data(),
    t.reference());
```

## BSON 字段命名

键值元素使用数字字符串作为字段名，字段名由 `idx` 生成：

```text
idx = 0 -> "0"
idx = 1 -> "1"
idx = 2 -> "2"
...
```

字段值是完整的键值容器元素，通常对应 `Container::value_type`，例如 map 的键值对类型。

## 临时对象与版本

临时元素通过以下类型创建：

```cpp
boost::serialization::detail::stack_construct<
    archive_type,
    typename Container::value_type> t(ar, v);
```

该方式允许 Boost.Serialization 根据元素版本 `v` 准备反序列化对象，并在读取完成后取得 `t.reference()`。

## 对象地址跟踪

插入容器后，代码调用：

```cpp
ar.reset_object_address(
    boost::addressof(result->second),
    boost::addressof(t.reference().second));
```

该操作将归档中临时对象的地址跟踪关系重定向到插入容器后的值对象，避免对象跟踪指向已经离开作用域的临时对象。

实现注释说明，该逻辑假设 `map::value_type` 通常未在归档中进行跟踪；具体跟踪行为由 Boost.Serialization 归档配置决定。

## 使用示例

```cpp
#include <yggr/serialization/key_value_container.hpp>
#include <map>

void load_map_element(
    yggr::archive::bson_iarchive<false>& archive,
    std::map<int, int>& values,
    std::map<int, int>::iterator hint)
{
    typedef yggr::serialization::stl::archive_input_map<
        yggr::archive::bson_iarchive<false>,
        std::map<int, int> > loader_type;

    loader_type loader;
    loader(archive, values, 0, hint, 0);
}
```

实际集合加载流程通常会由更高层的 map 或关联容器序列化实现调用该适配器。

## 与集合加载的关系

该文件只负责读取单个键值元素。完整的键值容器加载流程通常还包括：

1. 读取集合大小；
2. 确定元素版本；
3. 为每个索引调用 `archive_input_map::operator()`；
4. 将返回的迭代器作为下一次插入的 hint 或遍历位置。

## 注意事项

1. 只有定义 `YGGR_HAS_BSON_IARCHIVE` 时，该特化才会参与编译。
2. BSON 字段名必须符合当前实现生成的数字字符串格式。
3. `Container` 必须支持 `value_type`、迭代器和带 hint 的 `insert()` 操作。
4. 元素类型需要满足 Boost.Serialization 的构造和反序列化要求。
5. 参数 `v` 会传递给 `stack_construct`，用于选择正确的元素版本。
6. `reset_object_address()` 依赖键值元素的 `second` 成员，因此该实现面向具有键和值成员的关联容器元素。
7. 插入提示 `hint` 的有效性和性能影响由目标容器的 `insert()` 实现决定。
8. 字段索引 `idx` 使用 `yggr::u64`，但最终通过字符串流转换为字段名。
9. 具体对象跟踪和名称-值对行为由 BSON 输入归档与 Boost.Serialization 共同决定。

## 相关组件

- `yggr/serialization/nvp.hpp`
- `yggr/nsql_database_system/bson_type_traits.hpp`
- `yggr/serialization/collections_save_bson_impl.hpp`
- `yggr/serialization/collection_size_type.hpp`
- `yggr/archive/bson_iarchive.hpp`
- `boost/archive/detail/iserializer.hpp`
- `boost/serialization/serialization.hpp`
