# `array_bson_save_impl.hpp`

## 概述

该头文件为 BSON 输出归档提供固定数组保存实现，特化 Boost.Serialization 的
`save_array_type`，用于将 C/C++ 数组写入 BSON。

实现仅在定义 `YGGR_HAS_BSON_OARCHIVE` 时启用。

## 头文件

```cpp
#include <yggr/serialization/array_bson_save_impl.hpp>
```

该文件通常由 BSON 输出归档相关组件间接包含，不建议在业务代码中直接依赖其内部特化。

## 编译条件

当定义以下宏时启用实现：

```cpp
YGGR_HAS_BSON_OARCHIVE
```

未定义该宏时，不生成 `save_array_type` 特化，并设置内部阶段宏以配合包含控制逻辑。

## 类型特化

文件为以下两种 BSON 输出归档分别提供特化：

```cpp
template<>
struct save_array_type<yggr::archive::bson_oarchive<false> >;

template<>
struct save_array_type<yggr::archive::bson_oarchive<true> >;
```

特化位于：

```cpp
boost::archive::detail
```

模板参数 `false` 和 `true` 对应 BSON 输出归档的自动 ID 配置。

## `save_array_type<bson_oarchive<false> >::invoke`

```cpp
template<class T>
static void invoke(
    yggr::archive::bson_oarchive<false>& ar,
    const T& t);
```

保存数组时使用数字字符串字段名，布局如下：

| 字段名 | 内容 |
| --- | --- |
| `"0"` | 数组元素数量。 |
| `"1"` | 第一个数组元素。 |
| `"2"` | 第二个数组元素。 |
| ... | 后续数组元素。 |

处理流程：

1. 结束归档前导信息；
2. 根据数组对象大小和相邻元素地址差计算数组容量；
3. 将容量包装为 `collection_size_type`；
4. 以字段名 `"0"` 保存元素数量；
5. 从字段名 `"1"` 开始逐个保存数组元素。

## `save_array_type<bson_oarchive<true> >::invoke`

```cpp
template<class T>
static void invoke(
    yggr::archive::bson_oarchive<true>& ar,
    const T& t);
```

该模式不保存单独的元素数量字段，直接使用数字字符串字段名保存数组元素：

| 字段名 | 内容 |
| --- | --- |
| `"0"` | 第一个数组元素。 |
| `"1"` | 第二个数组元素。 |
| `"2"` | 后续数组元素。 |
| ... | 后续数组元素。 |

## 数组容量计算

两个特化都使用以下方式计算数组元素数量：

```cpp
sizeof(t) /
(
    static_cast<const char *>(static_cast<const void *>(&t[1])) -
    static_cast<const char *>(static_cast<const void *>(&t[0]))
)
```

该计算考虑了元素可能存在的对齐间隙，适用于固定长度数组对象。

## 归档前导处理

两个 `invoke()` 实现都会调用：

```cpp
save_access::end_preamble(ar);
```

用于结束 Boost.Serialization 输出归档的前导部分，再写入 BSON 数组字段。

## 使用示例

```cpp
#include <yggr/serialization/array_bson_save_impl.hpp>

void save_fixed_array()
{
    const int values[3] = {1, 2, 3};

    // 将 values 交给项目的 BSON 输出归档。
    // bson_oarchive<false> 会写入 "0" 数量字段和后续元素字段。
}
```

## 与数组加载的关系

输出格式必须与输入端的字段约定保持一致：

- `bson_oarchive<false>` 输出 `"0"` 为数量、`"1"` 起为元素；
- `bson_oarchive<true>` 输出 `"0"` 起为元素，不写数量字段；
- `array_bson_load_impl.hpp` 当前读取 `"0"` 作为数量，并从 `"1"` 开始读取元素。

因此，使用 `bson_oarchive<true>` 生成的数据与当前固定数组加载实现的字段布局并不直接对称，使用时应确认归档配置和兼容格式。

## 注意事项

1. 只有定义 `YGGR_HAS_BSON_OARCHIVE` 时，相关特化才会参与编译。
2. `bson_oarchive<false>` 会额外写入数量字段；`bson_oarchive<true>` 不写入数量字段。
3. 数组字段名使用数字字符串，而不是 `"elems"` 包装字段。
4. 数组容量依赖对象大小和元素地址差，调用时应传入实际固定数组而不是无界指针。
5. 数组元素类型必须满足 BSON 输出归档的序列化要求。
6. `save_array_type` 负责数组遍历和字段布局，不负责元素类型的 BSON 类型注册。
7. `collection_size_type`、名称-值对和归档前导行为由项目序列化组件提供。
8. 当前输出的两种模式与 `array_bson_load_impl.hpp` 的输入约定存在差异，跨模式读取前应验证实际 BSON 数据格式。

## 相关组件

- `yggr/serialization/nvp.hpp`
- `yggr/serialization/collection_size_type.hpp`
- `yggr/serialization/array_bson_load_impl.hpp`
- `yggr/archive/bson_oarchive.hpp`
- `boost/archive/detail/oserializer.hpp`
- `boost/serialization/array.hpp`
