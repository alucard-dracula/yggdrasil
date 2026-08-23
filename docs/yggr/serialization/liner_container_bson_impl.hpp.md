# `liner_container_bson_impl.hpp`

## 概述

该头文件为线性容器提供 BSON 输入归档适配，实现
`yggr::serialization::stl::archive_input_seq` 的特化。

尽管文件名包含 `impl`，当前实现只包含 BSON 线性容器加载逻辑，不直接定义 BSON 输出逻辑。

## 头文件

```cpp
#include <yggr/serialization/liner_container_bson_impl.hpp>
```

该文件通常由 `deque_bson_impl.hpp`、其他线性容器 BSON 适配器或 BSON 输入归档相关组件间接包含。

## 编译条件

实现仅在定义以下宏时启用：

```cpp
YGGR_HAS_BSON_IARCHIVE
```

未定义该宏时，不生成 BSON 输入序列容器特化，并设置内部阶段宏以配合包含控制逻辑。

## 命名空间

实现位于：

```cpp
yggr::serialization::stl
```

特化的基础模板为：

```cpp
archive_input_seq<Archive, Container>
```

## 类型特化

```cpp
template<bool skip_auto_id, typename Container>
struct archive_input_seq<
    yggr::archive::bson_iarchive<skip_auto_id>,
    Container>;
```

类型别名：

```cpp
typedef yggr::archive::bson_iarchive<skip_auto_id> archive_type;
```

其中 `skip_auto_id` 保留 BSON 输入归档的自动 ID 配置。

## `operator()`

```cpp
inline typename Container::iterator operator()(
    archive_type& ar,
    Container& s,
    const unsigned int v,
    typename Container::iterator hint,
    yggr::u64 idx) const;
```

读取一个线性容器元素并追加到容器末尾。

参数说明：

| 参数 | 说明 |
| --- | --- |
| `ar` | BSON 输入归档。 |
| `s` | 目标线性容器。 |
| `v` | 元素序列化版本，传递给 `stack_construct`。 |
| `hint` | 由上层加载流程传入的迭代器提示；当前实现原样返回。 |
| `idx` | 当前元素的数字索引，用于生成 BSON 字段名。 |

返回值为传入的 `hint`，实际插入位置通过 `push_back()` 确定。

## 加载流程

函数按以下步骤读取一个元素：

1. 获取 `Container::value_type` 作为元素类型；
2. 使用 `stack_construct<archive_type, type>` 根据版本 `v` 构造临时对象；
3. 将 `idx` 转换为数字字符串；
4. 使用该字符串作为名称-值对字段名读取临时对象；
5. 调用 `s.push_back(t.reference())` 将元素追加到目标容器；
6. 调用 `reset_object_address()` 将临时对象的跟踪地址重定向到容器中的实际元素；
7. 返回原始 `hint`。

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

该实现不读取集合大小或元素版本字段；这些元数据通常由上层集合加载流程负责读取并传入 `v` 与 `idx`。

## 对象地址跟踪

元素插入后，函数调用：

```cpp
ar.reset_object_address(
    boost::addressof(s.back()),
    boost::addressof(t.reference()));
```

这样可以将归档中临时构造对象的地址跟踪关系转移到目标容器中的实际对象，避免跟踪信息继续指向已结束生命周期的临时对象。

## 适用容器

目标 `Container` 应满足线性容器加载所需的接口，至少包括：

```cpp
typedef value_type;
typedef iterator;
iterator push_back(const value_type& value); // 或等价的追加操作
value_type& back();
```

实际 `push_back()` 返回值并未使用，`back()` 用于取得刚插入的元素。

典型适用类型包括：

- `std::deque`；
- `boost::container::deque`；
- 其他提供兼容接口的顺序容器。

## 使用示例

```cpp
#include <yggr/serialization/liner_container.hpp>
#include <deque>

void load_deque_element(
    yggr::archive::bson_iarchive<false>& archive,
    std::deque<int>& values,
    std::deque<int>::iterator hint)
{
    typedef yggr::serialization::stl::archive_input_seq<
        yggr::archive::bson_iarchive<false>,
        std::deque<int> > loader_type;

    loader_type loader;
    loader(archive, values, 0, hint, 0);
}
```

实际使用时通常由 `load_collection()` 或容器专用加载流程自动调用该适配器。

## 与其他 BSON 实现的关系

- `deque_bson_impl.hpp` 为 deque 注册 BSON 数组类型，并接入本文件；
- 线性容器的 BSON 输入元素读取由本文件执行；
- 集合数量、元素版本和整体遍历通常由上层集合加载实现执行；
- BSON 输出集合实现由其他文件提供，例如 `collections_save_bson_impl.hpp`。

## 注意事项

1. 只有定义 `YGGR_HAS_BSON_IARCHIVE` 时，该特化才会参与编译。
2. `idx` 对应的 BSON 字段名必须是数字字符串。
3. 当前函数使用 `push_back()`，不支持只提供关联容器插入接口的 map 类容器。
4. `hint` 不参与实际插入位置选择，函数会原样返回它。
5. 元素类型必须满足 `stack_construct`、BSON 输入归档和 `push_back()` 的要求。
6. `reset_object_address()` 依赖 `s.back()` 返回刚插入元素，并用于修正对象跟踪关系。
7. 该文件不读取集合数量；调用者应确保上层流程只调用正确的元素次数。
8. 本文件不提供 BSON 输出逻辑，输出端应使用对应的线性容器保存实现。
9. 具体字段读取、版本处理和异常行为由 BSON 输入归档及 Boost.Serialization 共同决定。

## 相关组件

- `yggr/serialization/deque_bson_impl.hpp`
- `yggr/serialization/collections_save_bson_impl.hpp`
- `yggr/serialization/collections_load_imp.hpp`
- `yggr/serialization/nvp.hpp`
- `yggr/archive/bson_iarchive.hpp`
- `boost/archive/detail/iserializer.hpp`
