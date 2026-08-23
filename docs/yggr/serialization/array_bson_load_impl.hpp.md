# `array_bson_load_impl.hpp`

## 概述

该头文件为 BSON 输入归档提供原生数组加载实现，特化了 Boost.Serialization 的
`load_array_type`，用于将 BSON 数字字段反序列化到 C/C++ 数组。

实现仅在定义 `YGGR_HAS_BSON_IARCHIVE` 时启用。

## 头文件

```cpp
#include <yggr/serialization/array_bson_load_impl.hpp>
```

该文件通常由 BSON 输入归档相关组件间接包含，不建议在业务代码中直接依赖其内部特化。

## 编译条件

当定义以下宏时启用实现：

```cpp
YGGR_HAS_BSON_IARCHIVE
```

未定义该宏时，文件不会启用 `load_array_type` 特化，并设置内部阶段宏以配合包含控制逻辑。

## 类型特化

```cpp
template<bool skip_auto_id>
struct load_array_type<
    yggr::archive::bson_iarchive<skip_auto_id> >;
```

该特化位于：

```cpp
boost::archive::detail
```

其中 `skip_auto_id` 保留 BSON 输入归档的自动 ID 配置，但当前数组加载流程使用数字字符串字段名定位数据。

## `invoke`

```cpp
template<typename T>
static void invoke(
    yggr::archive::bson_iarchive<skip_auto_id>& ar,
    T& t);
```

将 BSON 输入归档 `ar` 中的数组数据加载到数组 `t`。

### 处理流程

1. 使用 `remove_extent<T>::type` 获取数组元素类型；
2. 根据数组对象总大小和相邻元素地址差计算目标数组容量；
3. 从字段名 `"0"` 读取序列化元素数量 `count`；
4. 将计数索引递增到 `1`；
5. 如果 `count` 大于目标数组容量，抛出 `array_size_too_short`；
6. 从字段名 `"1"` 开始，按连续数字字段名读取每个数组元素；
7. 读取字段依次为 `"1"`、`"2"`，直到数组元素全部加载完成。

## BSON 字段布局

当前实现期望 BSON 数组采用以下字段布局：

| 字段名 | 内容 |
| --- | --- |
| `"0"` | 数组实际元素数量。 |
| `"1"` | 第一个数组元素。 |
| `"2"` | 第二个数组元素。 |
| ... | 后续数组元素。 |

元素数量不是 C/C++ 数组容量，而是归档数据中实际保存的元素数。目标数组必须能够容纳该数量。

## 容量检查

目标数组容量通过以下方式计算：

```cpp
sizeof(t) /
(
    static_cast<char *>(static_cast<void *>(&t[1])) -
    static_cast<char *>(static_cast<void *>(&t[0]))
)
```

如果归档中的 `count` 大于计算出的容量，将抛出：

```cpp
boost::archive::archive_exception::array_size_too_short
```

因此，输入数据不会被静默截断。

## 使用示例

```cpp
#include <yggr/serialization/array_bson_load_impl.hpp>

void load_fixed_array(yggr::archive::bson_iarchive<false>& archive)
{
    int values[3] = {0, 0, 0};

    // BSON 字段 "0" 应保存元素数量，
    // 字段 "1"、"2"、"3" 应保存对应元素。
    archive >> values;
}
```

## 异常行为

当归档元素数量超过目标数组容量时，函数通过
`boost::serialization::throw_exception()` 抛出 `archive_exception`，异常类型为：

```cpp
boost::archive::archive_exception::array_size_too_short
```

其他读取错误由 `bson_iarchive` 和底层序列化框架处理。

## 与 BSON 数组输出的关系

该实现读取的是数字字符串字段格式：

```text
"0" -> count
"1" -> element 0
"2" -> element 1
...
```

使用时应确保 BSON 输出端生成的字段布局与该读取约定一致。它与某些以 `"elems"` 包装整个数组的 BSON 序列化实现并不完全相同。

## 注意事项

1. 只有定义 `YGGR_HAS_BSON_IARCHIVE` 时，相关特化才会参与编译。
2. 字段 `"0"` 被解释为元素数量，而不是数组的第一个元素。
3. 元素从字段 `"1"` 开始读取，并按连续数字字符串命名。
4. 归档元素数量超过目标数组容量时会抛出 `array_size_too_short`，不会自动截断。
5. 目标类型 `T` 的元素必须满足 BSON 输入归档的反序列化要求。
6. 容量计算依赖数组元素地址差和数组对象布局，使用时应传入实际数组而不是无界指针。
7. 固定数组的容量由编译期布局决定，归档中的 `count` 只表示本次保存的有效元素数。
8. 具体字段读取、名称-值对和异常行为由 BSON 输入归档及 Boost.Serialization 共同决定。

## 相关组件

- `yggr/serialization/nvp.hpp`
- `yggr/serialization/collection_size_type.hpp`
- `yggr/archive/bson_iarchive.hpp`
- `yggr/serialization/array_bson_impl.hpp`
- `boost/archive/detail/iserializer.hpp`
- `boost/serialization/array.hpp`
