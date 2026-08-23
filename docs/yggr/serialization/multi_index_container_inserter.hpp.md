# `multi_index_container_inserter.hpp`

## 概述

该头文件为 Boost.MultiIndex 容器提供序列化加载所需的插入器适配。

它根据索引类型选择合适的插入操作：

- 普通索引：调用 `insert()`；
- sequenced 索引：调用 `push_back()`；
- random_access 索引：调用 `push_back()`；
- 多索引容器：通过第 0 个索引执行插入。

## 头文件

```cpp
#include <yggr/serialization/multi_index_container_inserter.hpp>
```

## 命名空间

```cpp
yggr::serialization::container
```

## `back_inserter`

```cpp
template<typename Index_Container>
struct back_inserter;
```

该模板是索引容器插入器的默认实现。

### 类型别名

```cpp
typedef Index_Container cont_type;
typedef typename cont_type::const_iterator citer_type;
```

### `operator()`

```cpp
template<typename T>
std::pair<citer_type, bool> operator()(
    cont_type& s,
    T& val) const;
```

默认实现调用：

```cpp
return s.insert(val);
```

返回值通常由插入位置迭代器和插入是否成功组成。

## sequenced 索引特化

```cpp
template<typename SuperMeta, typename TagList>
struct back_inserter<
    boost::multi_index::detail::sequenced_index<SuperMeta, TagList> >;
```

该特化的 `operator()` 调用：

```cpp
return s.push_back(val);
```

用于支持按序列顺序追加元素的 sequenced 索引。

## random_access 索引特化

```cpp
template<typename SuperMeta, typename TagList>
struct back_inserter<
    boost::multi_index::detail::random_access_index<SuperMeta, TagList> >;
```

该特化同样调用：

```cpp
return s.push_back(val);
```

用于保持 random_access 索引的追加顺序。

## `back_insert`

```cpp
template<typename V, typename I, typename A>
std::pair<
    typename boost::multi_index::multi_index_container<
        V, I, A>::const_iterator,
    bool>
back_insert(
    boost::multi_index::multi_index_container<V, I, A>& midx,
    V& v);
```

向多索引容器 `midx` 插入元素 `v`。

### 处理流程

1. 将目标容器类型定义为 `midx_type`；
2. 获取第 0 个索引的类型：

   ```cpp
   boost::multi_index::nth_index<midx_type, 0>::type
   ```

3. 为该索引创建 `back_inserter`；
4. 调用 `midx.get<0>()` 获取第 0 个索引；
5. 将元素传给对应插入器；
6. 返回插入结果。

## 插入行为对比

| 索引类型 | 使用的操作 |
| --- | --- |
| 普通索引 | `insert(val)` |
| `sequenced_index` | `push_back(val)` |
| `random_access_index` | `push_back(val)` |

## 使用示例

```cpp
#include <yggr/serialization/multi_index_container_inserter.hpp>
#include <boost/multi_index_container.hpp>

void insert_record(
    my_multi_index_container& container,
    record& value)
{
    const std::pair<
        my_multi_index_container::const_iterator,
        bool> result =
        yggr::serialization::container::back_insert(
            container,
            value);

    (void)result;
}
```

直接使用索引插入器：

```cpp
void insert_into_index(my_index_container& index, record& value)
{
    typedef yggr::serialization::container::back_inserter<
        my_index_container> inserter_type;

    inserter_type inserter;
    inserter(index, value);
}
```

## 在反序列化中的用途

多索引容器加载流程通常需要将归档中的元素重新插入目标容器。该文件通过适配不同索引类型，隐藏 `insert()` 和 `push_back()` 的差异，使上层加载代码可以使用统一的插入调用。

插入完成后，Boost.MultiIndex 会根据容器声明的索引配置维护其他索引结构。

## 注意事项

1. `back_insert()` 固定使用多索引容器的第 0 个索引，插入结果和顺序由该索引类型决定。
2. 普通索引的 `insert()` 可能因为唯一性约束导致插入失败。
3. sequenced 和 random_access 索引使用 `push_back()`，其返回值具体形式由 Boost.MultiIndex 版本决定。
4. `V`、`I` 和 `A` 必须满足 `boost::multi_index::multi_index_container` 的模板约束。
5. 元素类型 `T` 或 `V` 必须可被目标索引容器接受，并满足相应的构造和赋值要求。
6. 文件依赖 Boost.MultiIndex 的 detail 索引类型，升级 Boost 版本后应验证这些内部类型名称是否保持兼容。
7. 该文件只负责插入适配，不负责序列化数据读取、重复元素处理或索引定义。
8. 插入失败时应检查返回值中的 `bool`，不能假定元素一定已经加入容器。

## 相关组件

- `yggr/serialization/multi_index_collections_load_imp.hpp`
- `yggr/serialization/multi_index_collections_load_bson_impl.hpp`
- `yggr/serialization/multi_index_container.hpp`
- `yggr/serialization/multi_index_container_t.hpp`
- `boost/multi_index_container.hpp`
- `boost/multi_index/sequenced_index.hpp`
- `boost/multi_index/random_access_index.hpp`
