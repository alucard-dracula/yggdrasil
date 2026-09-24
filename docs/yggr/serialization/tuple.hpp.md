# `tuple.hpp`

## 概述

该头文件为 tuple 和 cons 类型提供 Boost.Serialization 支持，采用递归方式处理每个元素。支持的类型包括：

- `boost::tuples::cons`；
- `yggr::tuples::cons`；
- `boost::tuples::tuple`；
- `yggr::tuples::tuple`；
- 在 C++11 tuple 可用时的 `std::tuple`。

序列化数据包含元素数量 `count`，每个元素使用名称 `item` 写入或读取。

## 头文件

```cpp
#include <yggr/serialization/tuple.hpp>
```

主要依赖包括：

```cpp
#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/tuple_ex/tuple.hpp>
#include <boost/serialization/split_free.hpp>
```

## 内部处理器

### `cons_io_helper`

`yggr::serialization::cons_io_helper<T>` 是 cons 和 tuple 的递归处理器：

- 对普通 cons，处理当前节点的 head，然后递归处理 tail；
- 对以 `yggr::tuples::null_type` 结尾的 cons，处理最后一个 head 后结束；
- 对 `boost::tuples::tuple` 和 `yggr::tuples::tuple`，将 tuple 转换为其继承的 cons 类型后复用相同逻辑；
- 对 `yggr::tuples::null_type`，不执行任何读写并直接返回归档对象。

每个元素都使用以下命名值对名称：

```cpp
YGGR_SERIALIZATION_NAME_NVP("item", value)
```

### `std_tuple_io_helper`

当未定义 `YGGR_NO_CXX11_HDR_TUPLE` 时，`std_tuple_io_helper<Tuple, I, ISize>` 使用索引递归访问 `std::tuple`：

```cpp
YGGR_SERIALIZATION_NAME_NVP("item", (::yggr::get<I>(t)))
```

当 `I` 到达 `ISize` 时，偏特化停止递归。

## 保存格式

所有支持的 tuple/cons 类型在保存时都会先写入元素数量：

```cpp
yggr::ser_size_type count = ...;
ar << YGGR_SERIALIZATION_NVP(count);
```

随后按顺序写入每个元素，元素名称均为 `item`。不同 tuple 类型的长度通过对应的 `length` trait 获取：

- `boost::tuples::length<T>::value`；
- `yggr::tuples::length<T>::value`。

`std::tuple` 使用 `yggr::tuples::length<std::tuple<...>>::value` 获取长度。

## 加载格式

加载时先读取 `count`：

```cpp
yggr::ser_size_type count = 0;
ar >> YGGR_SERIALIZATION_NVP(count);
```

然后使用断言检查归档中的元素数量是否与目标类型的静态长度一致：

```cpp
assert(count == tuple_length);
```

数量检查通过后，处理器按 tuple 元素顺序读取所有 `item` 值。

注意：该数量检查使用 `assert`，在禁用断言的构建配置中不会提供运行时校验。

## 序列化接口

所有接口位于 `boost::serialization` 命名空间中，并通过 `split_free` 分离保存和加载操作。

### `boost::tuples::cons` 与 `yggr::tuples::cons`

```cpp
template<typename Archive, typename HH, typename TT>
void save(Archive& ar, const cons<HH, TT>& value, unsigned int version);

template<typename Archive, typename HH, typename TT>
void load(Archive& ar, cons<HH, TT>& value, unsigned int version);

template<typename Archive, typename HH, typename TT>
void serialize(Archive& ar, cons<HH, TT>& value, unsigned int version);
```

### `boost::tuples::tuple` 与 `yggr::tuples::tuple`

两种 tuple 均根据 `BOOST_TUPLE_LIMIT_LENGTH()` 或 `YGGR_TUPLE_LIMIT_LENGTH()` 展开模板参数，并提供对应的 `save`、`load` 和 `serialize` 函数。

```cpp
template<typename Archive, ...>
void save(Archive& ar, const tuple<...>& value, unsigned int version);

template<typename Archive, ...>
void load(Archive& ar, tuple<...>& value, unsigned int version);

template<typename Archive, ...>
void serialize(Archive& ar, tuple<...>& value, unsigned int version);
```

### `std::tuple`

当未定义 `YGGR_NO_CXX11_HDR_TUPLE` 时，提供变参模板接口：

```cpp
template<typename Archive, typename ...Args>
void save(Archive& ar, const std::tuple<Args...>& value, unsigned int version);

template<typename Archive, typename ...Args>
void load(Archive& ar, std::tuple<Args...>& value, unsigned int version);

template<typename Archive, typename ...Args>
void serialize(Archive& ar, std::tuple<Args...>& value, unsigned int version);
```

## 实现级别

文件将相关类型注册为 `object_serializable`：

```cpp
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    2, boost::tuples::cons, object_serializable)

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    2, yggr::tuples::cons, object_serializable)

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    BOOST_TUPLE_LIMIT_LENGTH(), boost::tuples::tuple, object_serializable)

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    YGGR_TUPLE_LIMIT_LENGTH(), yggr::tuples::tuple, object_serializable)
```

在 C++11 tuple 可用时，`std::tuple` 使用：

```cpp
YGGR_SERIALIZATION_VARIADIC_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    YGGR_TUPLE_LIMIT_LENGTH(), std::tuple, object_serializable)
```

## 长度限制与兼容性

- Boost tuple 的最大模板参数数量由 `BOOST_TUPLE_LIMIT_LENGTH()` 控制。
- Yggdrasil tuple 和 `std::tuple` 的最大长度由 `YGGR_TUPLE_LIMIT_LENGTH()` 控制。
- 定义 `YGGR_NO_CXX11_HDR_TUPLE` 后，不会生成 `std::tuple` 相关处理器、序列化函数和实现级别注册。
- 归档中的 `count` 必须与目标 tuple 的静态元素数量一致。

## BSON 集成

文件末尾包含：

```cpp
#include <yggr/serialization/tuple_bson_impl.hpp>
```

该 BSON 头文件负责在 BSON 归档启用时注册 tuple 的 BSON 类型特征。具体 BSON 类型注册和条件编译逻辑见 `tuple_bson_impl.hpp`。
