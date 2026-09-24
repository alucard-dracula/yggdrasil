# `tuple_bson_impl.hpp`

## 概述

该头文件为 tuple 类型提供 BSON 类型特征注册，并根据 BSON 输入或输出归档是否启用控制这些注册代码是否参与编译。

支持的 tuple 类型包括：

- `boost::tuples::tuple`；
- `yggr::tuples::tuple`；
- 在 C++11 tuple 可用时的 `std::tuple`。

## 头文件

```cpp
#include <yggr/serialization/tuple_bson_impl.hpp>
```

## 编译条件

只有定义以下任一宏时，BSON tuple 类型注册才会启用：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HAS_BSON_IARCHIVE
```

启用后，文件会定义主 include guard：

```cpp
__YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP__
```

如果两个 BSON 归档宏均未定义，则不执行 tuple 类型注册，而是设置内部控制宏：

```cpp
YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP_ARCHIVE
```

该宏用于记录待处理状态，供后续 BSON 实现重新包含流程使用。

## 依赖

启用 BSON 支持时，文件包含：

```cpp
#include <yggr/nsql_database_system/bson_type_traits.hpp>
#include <yggr/serialization/nvp.hpp>
```

其中 `bson_type_traits.hpp` 提供 BSON 类型特征注册所需的宏和类型定义。

## BSON 类型注册

### Boost tuple

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    BOOST_TUPLE_LIMIT_LENGTH(),
    boost::tuples::tuple,
    bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT)
```

### Yggdrasil tuple

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    YGGR_TUPLE_LIMIT_LENGTH(),
    yggr::tuples::tuple,
    bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT)
```

### `std::tuple`

当未定义 `YGGR_NO_CXX11_HDR_TUPLE` 时，注册 C++11 标准 tuple：

```cpp
YGGR_PP_VARIADIC_CONTAINER_BSON_TYPEID_IMPL(
    YGGR_TUPLE_LIMIT_LENGTH(),
    std::tuple,
    bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT)
```

## BSON 类型值

上述三类 tuple 均注册为：

```cpp
bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT
```

这表示 tuple 在 BSON 外部类型识别中使用拼接对象形式，而不是普通 BSON document 类型。源代码中的注释特别说明，外部类型不使用 `E_BSON_TYPE_DOCUMENT`，因为这里对应的是 C BSON 的处理方式。

## Tuple 长度限制

注册宏的第一个参数分别使用：

- `BOOST_TUPLE_LIMIT_LENGTH()`：Boost tuple 的最大长度；
- `YGGR_TUPLE_LIMIT_LENGTH()`：Yggdrasil tuple 及 `std::tuple` 的最大长度。

实际可用的 tuple 元素数量受这些宏和底层预处理器实现限制影响。

## 内部宏状态清理

当 BSON 归档宏已定义时，如果此前存在：

```cpp
YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP_ARCHIVE
```

文件会先将其取消定义，再完成当前 BSON tuple 注册，避免旧的待处理状态残留。

## 使用注意事项

- 本文件注册的是 BSON 类型特征，不直接提供 tuple 的 `save`、`load` 或 `serialize` 函数。
- `std::tuple` 支持依赖 C++11 tuple 头文件可用；定义 `YGGR_NO_CXX11_HDR_TUPLE` 后不会生成其注册代码。
- BSON 输出或输入归档宏必须在包含本文件前正确设置，否则只会记录内部待处理宏。
- `__YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP__` 和 `YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP_ARCHIVE` 均属于内部实现细节，业务代码不应直接依赖。
