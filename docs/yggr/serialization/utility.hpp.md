# `utility.hpp`

## 概述

该头文件为 `std::pair<Key, Val>` 提供通用的 Boost.Serialization 支持。它在 `boost::serialization` 命名空间中定义 `save`、`load` 和 `serialize` 自由函数，并将 pair 的两个成员分别以命名值对字段 `"first"` 和 `"second"` 进行归档。

文件还将 `std::pair` 的 Boost.Serialization 实现级别设置为 `object_serializable`，并在主 include guard 结束后接入 BSON 专用实现。

## 头文件

```cpp
#include <yggr/serialization/utility.hpp>
```

## 依赖

主要依赖包括：

```cpp
#include <yggr/base/yggrdef.h>
#include <boost/config.hpp>
#include <yggr/serialization/nvp.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/mpl/and.hpp>
#include <utility>
```

各依赖的作用如下：

- `nvp.hpp` 提供命名值对宏 `YGGR_SERIALIZATION_NAME_NVP`；
- `remove_const.hpp` 用于处理 `std::pair<const Key, Val>` 的键类型；
- `split_free.hpp` 用于将 `serialize` 拆分为 `save` 和 `load`；
- `collection_traits.hpp` 在文件后半部分提供 `implementation_level` 相关基础设施；
- `<utility>` 提供 `std::pair`。

## `save`

接口为：

```cpp
template<typename Archive, typename Key, typename Val>
inline void save(
    Archive& ar,
    const std::pair<Key, Val>& value,
    const unsigned int file_version);
```

实现将两个成员写入归档：

```cpp
typedef typename boost::remove_const<Key>::type typef;
ar << YGGR_SERIALIZATION_NAME_NVP(
    "first", const_cast<typef&>(value.first));
ar << YGGR_SERIALIZATION_NAME_NVP(
    "second", value.second);
```

字段含义如下：

| 字段名 | 内容 |
| --- | --- |
| `"first"` | `pair::first` |
| `"second"` | `pair::second` |

`file_version` 参数仅用于匹配 Boost.Serialization 的自由函数接口，当前实现不根据版本执行分支。

## `load`

接口为：

```cpp
template<typename Archive, typename Key, typename Val>
inline void load(
    Archive& ar,
    std::pair<Key, Val>& value,
    const unsigned int file_version);
```

实现按与保存端相同的字段名称读取：

```cpp
typedef typename boost::remove_const<Key>::type typef;
ar >> YGGR_SERIALIZATION_NAME_NVP(
    "first", const_cast<typef&>(value.first));
ar >> YGGR_SERIALIZATION_NAME_NVP(
    "second", value.second);
```

保存端和加载端必须保持字段名称一致，否则 pair 无法按预期恢复。

## const 键处理

关联容器的元素类型通常是：

```cpp
std::pair<const Key, Val>
```

由于 `first` 可能是 const 成员类型，代码使用：

```cpp
boost::remove_const<Key>::type
```

移除类型层面的 const，再通过 `const_cast` 将键传给归档命名值对接口。这是为了适配 Boost.Serialization 对可写归档对象的接口要求；序列化逻辑本身不应通过该路径改变键的语义。

## `serialize`

接口为：

```cpp
template<typename Archive, typename Key, typename Val>
inline void serialize(
    Archive& ar,
    std::pair<Key, Val>& value,
    const unsigned int file_version);
```

实现使用显式模板参数调用 `split_free`：

```cpp
boost::serialization::split_free<
    Archive,
    std::pair<Key, Val>
>(ar, value, file_version);
```

这样归档系统会根据输入或输出方向分别调用本文件的 `load` 或 `save`。

## 实现级别

文件包含：

```cpp
#include <boost/serialization/collection_traits.hpp>
```

并为 `std::pair<Key, Val>` 特化 `implementation_level`：

```cpp
template<typename Key, typename Val>
struct implementation_level<std::pair<Key, Val>>
{
    typedef mpl::integral_c_tag tag;
    typedef mpl::int_<object_serializable> type;
    YGGR_STATIC_CONSTANT(
        int,
        value = object_serializable);
};
```

这表示 pair 按对象级别参与 Boost.Serialization，而不是被当作更高层的可追踪对象结构处理。

## BSON 集成

主 include guard 结束后包含：

```cpp
#include <yggr/serialization/utility_bson_impl.hpp>
```

该 BSON 实现为 `std::pair` 提供归档专用重载，并将 pair 映射为 BSON 数组：

- 通用归档字段名为 `"first"` 和 `"second"`；
- BSON 归档字段名为 `"0"` 和 `"1"`；
- BSON 类型 ID 为 `bson_typeid_def::E_BSON_TYPE_ARRAY`。

因此，通用 pair 序列化和 BSON pair 序列化共用相同的类型，但字段格式由归档类型对应的实现决定。

## 数据格式对比

| 归档实现 | 第一个成员 | 第二个成员 |
| --- | --- | --- |
| 通用 `save`/`load` | `"first"` | `"second"` |
| BSON 专用实现 | `"0"` | `"1"` |

## 序列化流程

```text
std::pair<Key, Val>
  -> boost::serialization::serialize
  -> split_free
      -> save 或 load
          -> YGGR_SERIALIZATION_NAME_NVP
```

BSON 归档会根据更具体的 BSON 重载使用数字字段名和 BSON 数组类型注册。

## 使用限制与注意事项

- `std::pair` 的两个成员必须满足对应归档类型的序列化要求。
- 关联容器元素常见的 `std::pair<const Key, Val>` 依赖当前的 const 键适配逻辑。
- 保存和加载两端必须使用一致的字段名称和成员顺序。
- 本文件的 `serialize` 使用自由函数拆分，不应再为相同类型重复定义冲突的通用序列化函数。
- `implementation_level` 特化和 BSON 实现宏均属于序列化基础设施，业务代码不应随意覆盖。
