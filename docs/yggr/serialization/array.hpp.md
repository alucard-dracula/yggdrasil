# `array.hpp`

## 概述

该头文件是 yggdrasil 数组序列化的统一入口，负责引入：

- yggdrasil 数组容器定义；
- Boost.Serialization 的原生数组支持；
- Boost `array` 兼容支持；
- BSON 数组序列化实现。

业务代码通常只需要包含该文件，即可获得项目数组序列化所需的基础声明和兼容处理。

## 头文件

```cpp
#include <yggr/serialization/array.hpp>
```

## 依赖

该文件包含：

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/container/array.hpp>
#include <boost/serialization/array.hpp>
#include <yggr/serialization/array_bson_impl.hpp>
```

其中：

- `yggr/container/array.hpp`：提供项目数组容器支持；
- `boost/serialization/array.hpp`：提供原生数组序列化基础设施；
- `array_bson_impl.hpp`：提供 BSON 输入/输出归档的数组实现。

## Boost 版本兼容

当 Boost 版本不低于 1.64 时，文件会额外处理 Boost 数组序列化头文件：

```cpp
#if !(BOOST_VERSION < 106400)
```

如果已经定义 `BOOST_SERIALIZATION_ARRAY_HPP`，文件会先取消该宏，再包含：

```cpp
#include <boost/serialization/boost_array.hpp>
```

随后确保 `BOOST_SERIALIZATION_ARRAY_HPP` 被定义。

该逻辑用于兼容不同 Boost 版本中原生数组和 `boost::array` 序列化头文件的包含保护行为。

## 支持的数组类型

通过所包含的序列化实现，通常支持：

- C/C++ 原生数组 `T[N]`；
- `boost::array<T, N>`；
- `std::array<T, N>`，具体取决于编译器、Boost 版本和 BSON 实现条件；
- yggdrasil 容器数组类型，具体能力由 `yggr/container/array.hpp` 决定。

## BSON 支持

文件最后包含：

```cpp
#include <yggr/serialization/array_bson_impl.hpp>
```

该包含位于主 `__YGGR_SERIALIZATION_ARRAY_HPP__` 保护块之后，因此 BSON 实现会按照其自身的宏和保护逻辑独立处理。

BSON 数组支持由以下宏控制：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HAS_BSON_IARCHIVE
```

- 输出归档宏启用 BSON 数组保存；
- 输入归档宏启用 BSON 数组加载和相关类型识别。

## 使用示例

```cpp
#include <yggr/serialization/array.hpp>

void prepare_array_serialization()
{
    int values[3] = {1, 2, 3};
    boost::array<int, 3> boost_values = {{1, 2, 3}};

    (void)values;
    (void)boost_values;
}
```

使用 BSON 归档时，具体字段布局和 `skip_auto_id` 行为请参考：

```text
yggr/serialization/array_bson_impl.hpp
yggr/serialization/array_bson_save_impl.hpp
yggr/serialization/array_bson_load_impl.hpp
```

## 包含保护说明

主头文件使用：

```cpp
#ifndef __YGGR_SERIALIZATION_ARRAY_HPP__
#define __YGGR_SERIALIZATION_ARRAY_HPP__
```

Boost 兼容宏 `BOOST_SERIALIZATION_ARRAY_HPP` 会根据 Boost 版本和已有定义进行调整。

由于 `array_bson_impl.hpp` 位于主 include guard 之后，它不完全受主头文件保护；该文件通过自身的内部宏避免 BSON 实现重复展开。

## 注意事项

1. 该文件主要负责包含和兼容处理，不定义数组序列化算法本身。
2. `boost::array` 的可用性和头文件行为受 Boost 版本影响。
3. BSON 数组实现是否启用取决于 `YGGR_HAS_BSON_OARCHIVE` 和 `YGGR_HAS_BSON_IARCHIVE`。
4. 数组元素类型仍必须满足对应归档的序列化要求。
5. 使用 BSON 输入和输出时，应确认数组字段布局与对应实现约定一致。
6. `std::array` 的支持还取决于标准库和项目对 C++11 数组头文件的配置。
7. 该文件会调整 Boost.Serialization 的 include guard 宏，项目升级 Boost 版本时应验证相关兼容逻辑。

## 相关组件

- `yggr/base/yggrdef.h`
- `yggr/container/array.hpp`
- `yggr/serialization/array_bson_impl.hpp`
- `yggr/serialization/array_bson_save_impl.hpp`
- `yggr/serialization/array_bson_load_impl.hpp`
- `boost/serialization/array.hpp`
- `boost/serialization/boost_array.hpp`
