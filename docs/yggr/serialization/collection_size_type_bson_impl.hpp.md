# `collection_size_type_bson_impl.hpp`

## 概述

该头文件为集合大小类型提供 BSON 类型标识适配，覆盖：

- `boost::serialization::collection_size_type`；
- `yggr::serialization::collection_size_type`。

它不实现集合序列化流程，只向 yggdrasil BSON 类型系统注册这两种类型对应的 BSON 基础类型。

## 头文件

```cpp
#include <yggr/serialization/collection_size_type_bson_impl.hpp>
```

## 编译条件

实现仅在以下任一宏已定义时启用：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HAS_BSON_IARCHIVE
```

也就是说，BSON 输出归档或 BSON 输入归档可用时，文件会生成对应的 `bson_typeid` 特化。

未满足条件时，文件不会生成 BSON 类型特征，并通过内部宏配合后续包含控制逻辑。

## 类型特化

特化位于：

```cpp
yggr::nsql_database_system
```

### Boost 集合大小类型

```cpp
template<>
struct bson_typeid<boost::serialization::collection_size_type>;
```

类型别名：

```cpp
typedef boost::serialization::collection_size_type type;
typedef boost::serialization::collection_size_type wrap_type;
```

### yggdrasil 集合大小类型

```cpp
template<>
struct bson_typeid<yggr::serialization::collection_size_type>;
```

类型别名：

```cpp
typedef yggr::serialization::collection_size_type type;
typedef yggr::serialization::collection_size_type wrap_type;
```

## BSON 类型映射

两种特化都根据对应序列化大小类型的字节数选择 BSON 类型：

| 类型大小 | BSON 类型 |
| --- | --- |
| `4` 字节 | `bson_typeid_def::E_BSON_TYPE_INT32` |
| `8` 字节 | `bson_typeid_def::E_BSON_TYPE_INT64` |
| 其他大小 | `bson_typeid_def::E_BSON_TYPE_EOD` |

Boost 类型的判断依据为：

```cpp
sizeof(ser_size_type)
```

yggdrasil 类型的判断依据为：

```cpp
sizeof(yggr::serialization::collection_size_type)
```

## `bson_typeid` 接口

每个特化提供静态常量：

```cpp
YGGR_STATIC_CONSTANT(
    bson_typeid_def::bson_typeid_type,
    value);
```

并提供两个调用运算符：

```cpp
u8 operator()(void) const;
u8 operator()(const type&) const;
```

两个运算符都会返回转换为 `u8` 的 BSON 类型值。

## 使用示例

```cpp
#include <yggr/serialization/collection_size_type.hpp>

void inspect_collection_size_type()
{
    typedef yggr::serialization::collection_size_type size_type;
    typedef yggr::nsql_database_system::bson_typeid<size_type> typeid_type;

    const typeid_type typeid_value;
    const yggr::u8 bson_type = typeid_value();

    (void)bson_type;
}
```

实际使用时，BSON 归档和类型特征系统通常会自动查询该特化，不需要业务代码手动调用。

## 类型选择规则

实现的选择逻辑等价于：

```text
sizeof(size type) == 4 -> BSON int32
sizeof(size type) == 8 -> BSON int64
其他大小             -> BSON EOD
```

`EOD` 表示当前大小没有对应的有效 BSON 类型映射，应视为不受支持的布局。

## 宏状态管理

文件使用内部宏：

```cpp
__YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_BSON_IMPL_HPP__
__YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_BSON_IMPL_HPP_BSON_ARCHIVE
```

这些宏用于控制 BSON 类型特征实现的重复展开，通常不应由业务代码直接操作。

## 注意事项

1. 该文件只注册 BSON 类型标识，不负责集合大小值的实际读写。
2. 只有 4 字节和 8 字节的集合大小类型分别映射到 BSON `int32` 和 `int64`。
3. 其他大小会映射为 `E_BSON_TYPE_EOD`，可能导致 BSON 归档不支持该类型。
4. Boost 类型和 yggdrasil 类型分别依据各自的底层大小类型进行判断。
5. BSON 类型大小和 C++ 类型大小必须保持归档双方兼容。
6. 相关类型特征只有在 `YGGR_HAS_BSON_OARCHIVE` 或 `YGGR_HAS_BSON_IARCHIVE` 启用时才会生成。
7. `value` 的具体类型和 BSON 常量定义由 `bson_type_traits.hpp` 提供。
8. 修改 `collection_size_type` 的底层表示后，应重新验证 BSON 类型映射。

## 相关组件

- `yggr/serialization/collection_size_type.hpp`
- `yggr/nsql_database_system/bson_type_traits.hpp`
- `yggr/serialization/array_bson_save_impl.hpp`
- `yggr/serialization/array_bson_load_impl.hpp`
- `yggr/archive/bson_oarchive.hpp`
- `yggr/archive/bson_iarchive.hpp`
- `boost/serialization/collection_size_type.hpp`
