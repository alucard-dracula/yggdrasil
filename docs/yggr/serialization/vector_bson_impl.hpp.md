# `vector_bson_impl.hpp`

## 概述

该头文件为多种动态数组容器接入 BSON 序列化支持。它本身不直接实现元素的 BSON 保存和加载，而是完成以下适配工作：

- 为 `std::vector`、Boost.Container vector 和 Yggdrasil 内部 vector 注册 BSON 类型 ID；
- 将这些容器统一映射为 BSON 数组类型；
- 根据 BSON 输入/输出归档是否可用控制注册时机；
- 在文件末尾接入 `liner_container_bson_impl.hpp`，由后者提供线性容器的具体 BSON 读写实现。

## 头文件

```cpp
#include <yggr/serialization/vector_bson_impl.hpp>
```

该文件通常由以下通用 vector 序列化头文件间接包含：

```cpp
#include <yggr/serialization/vector.hpp>
```

## 编译条件

主实现仅在以下任一宏定义时启用：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HAS_BSON_IARCHIVE
```

启用时定义主 include guard：

```cpp
__YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP__
```

如果两个宏都未定义，则不注册 BSON 类型，而是设置内部待处理宏：

```cpp
YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP_ARCHIVE
```

当后续重新包含且 BSON 归档条件满足时，该待处理宏会被取消定义。

## BSON 类型注册依赖

启用实现时包含：

```cpp
#include <yggr/nsql_database_system/bson_type_traits.hpp>
```

该依赖提供：

```cpp
bson_typeid_def::E_BSON_TYPE_ARRAY
```

所有注册的 vector 类型都映射为 BSON 数组类型。

## 容器注册

注册宏的形式为：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    template_parameter_count,
    ContainerType,
    bson_typeid_def::E_BSON_TYPE_ARRAY)
```

注册项如下：

| 容器 | 模板参数数量 | 条件 |
| --- | ---: | --- |
| `std::vector` | `2` | 始终在 BSON 条件满足时注册 |
| `boost::container::vector` | `2` | `BOOST_VERSION < 106700` |
| `boost::container::vector` | `3` | `BOOST_VERSION >= 106700` |
| `yggr::container::detail::vector` | `2` | `BOOST_VERSION >= 106700` |

对应源码逻辑为：

```cpp
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    2, std::vector, bson_typeid_def::E_BSON_TYPE_ARRAY)

#if (BOOST_VERSION < 106700)
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    2, boost::container::vector, bson_typeid_def::E_BSON_TYPE_ARRAY)
#else
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    3, boost::container::vector, bson_typeid_def::E_BSON_TYPE_ARRAY)
YGGR_PP_CONTAINER_BSON_TYPEID_IMPL(
    2, yggr::container::detail::vector,
    bson_typeid_def::E_BSON_TYPE_ARRAY)
#endif
```

## Boost 版本兼容

Boost.Container 在不同版本中的 `vector` 模板参数数量不同：

- Boost 1.66 及更早版本使用 2 个模板参数；
- Boost 1.67 及更新版本使用 3 个模板参数。

因此 BSON 类型注册必须与当前 Boost 版本的模板签名匹配。新版本分支还注册 `yggr::container::detail::vector`，以支持项目自己的 vector 实现。

## BSON 实现接入

主 include guard 结束后，文件包含：

```cpp
#include <yggr/serialization/liner_container_bson_impl.hpp>
```

该头文件负责线性容器的 BSON 元素处理。当前文件只负责把已注册的 vector 类型连接到通用线性容器 BSON 流程。

典型处理链如下：

```text
vector.hpp
  -> 线性容器通用 save/load
  -> vector_bson_impl.hpp
      -> vector BSON 类型 ID 注册
      -> liner_container_bson_impl.hpp
          -> BSON 元素保存/加载
```

## 与 `vector.hpp` 的关系

`vector.hpp` 负责：

- 注册 `is_vector` 类型特征；
- 通过 `YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD` 生成通用序列化函数；
- 根据 Boost 版本注册不同 vector 模板参数数量；
- 设置容器的 `object_serializable` 实现级别；
- 在末尾包含本文件。

本文件负责：

- 为相同容器注册 BSON 数组类型 ID；
- 接入线性容器 BSON 实现。

## 宏状态总结

| 条件 | 行为 |
| --- | --- |
| `YGGR_HAS_BSON_OARCHIVE` 或 `YGGR_HAS_BSON_IARCHIVE` 已定义 | 定义主 include guard，注册 vector BSON 类型并接入线性容器 BSON 实现 |
| 两个 BSON 归档宏均未定义 | 设置 `YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP_ARCHIVE`，暂不注册类型 |
| `BOOST_VERSION < 106700` | `boost::container::vector` 按 2 个模板参数注册 |
| `BOOST_VERSION >= 106700` | `boost::container::vector` 按 3 个模板参数注册，并注册 Yggdrasil detail vector |

## 注意事项

- 本文件只负责 BSON 类型注册和实现接入，不直接遍历 vector 或读写元素。
- 所有 vector 类型都注册为 BSON 数组，而不是 BSON 对象。
- 模板参数数量必须与当前库版本中的容器模板签名一致。
- `YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP_ARCHIVE` 和主 include guard 属于内部控制宏，业务代码不应手动定义或取消定义。
- 具体 BSON 字段格式、索引处理和元素版本逻辑由 `liner_container_bson_impl.hpp` 及其相关加载/保存实现负责。
