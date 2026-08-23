# `collection_size_type.hpp`

## 概述

该头文件定义了 `yggr::serialization::collection_size_type`，用于表示集合序列化过程中的元素数量。

该类型是对 Boost.Serialization `ser_size_type` 的轻量包装，主要用于为 yggdrasil 的序列化实现提供统一的集合大小类型和 BSON 类型适配。

## 头文件

```cpp
#include <yggr/serialization/collection_size_type.hpp>
```

## 命名空间

```cpp
yggr::serialization
```

## 类声明

```cpp
class collection_size_type;
```

内部底层类型：

```cpp
typedef ser_size_type base_type;
base_type t;
```

`base_type` 来自 `boost/serialization/collection_size_type.hpp`。

## 构造函数

### 默认构造

```cpp
collection_size_type();
```

将内部集合大小初始化为 `0`。

### 从 `std::size_t` 构造

```cpp
explicit collection_size_type(const std::size_t& value);
```

使用指定的 `std::size_t` 值初始化集合大小。该构造函数是显式的，不支持隐式从 `std::size_t` 转换。

### 拷贝构造

```cpp
collection_size_type(const collection_size_type& other);
```

复制另一个集合大小对象的内部值。

## 赋值操作

### 同类型赋值

```cpp
collection_size_type& operator=(
    const collection_size_type& rhs);
```

将另一个 `collection_size_type` 的内部值复制到当前对象。

### 从 `unsigned int` 赋值

```cpp
collection_size_type& operator=(const unsigned int& rhs);
```

将 `unsigned int` 值赋给内部底层类型。

## 类型转换

### 转换为底层值

```cpp
operator base_type() const;
```

以值形式返回内部集合大小，主要用于文本输出或需要底层值的场景。

### 转换为底层引用

```cpp
operator base_type&();
```

返回内部底层值的可修改引用，主要用于文本输入或需要直接写入底层值的场景。

该转换会暴露内部存储，应注意避免绕过类型本身的约束。

## 比较操作

### 相等比较

```cpp
bool operator==(const collection_size_type& rhs) const;
```

比较两个对象的内部集合大小是否相等。

### 小于比较

```cpp
bool operator<(const collection_size_type& rhs) const;
```

按内部集合大小执行小于比较。

## Boost.Serialization 特征

该类型被声明为原始序列化类型：

```cpp
BOOST_CLASS_IMPLEMENTATION(
    yggr::serialization::collection_size_type,
    primitive_type)
```

同时被声明为按位可序列化类型：

```cpp
BOOST_IS_BITWISE_SERIALIZABLE(
    yggr::serialization::collection_size_type)
```

这些声明使 Boost.Serialization 能够将其作为简单值类型处理。

## BSON 支持

文件在主 include guard 之后包含：

```cpp
#include <yggr/serialization/collection_size_type_bson_impl.hpp>
```

该实现为 `yggr::serialization::collection_size_type` 注册 BSON 类型标识：

- 底层大小为 4 字节时映射为 BSON `int32`；
- 底层大小为 8 字节时映射为 BSON `int64`；
- 其他大小映射为 `EOD`。

具体规则由 `collection_size_type_bson_impl.hpp` 决定。

## 使用示例

```cpp
#include <yggr/serialization/collection_size_type.hpp>

void use_collection_size()
{
    yggr::serialization::collection_size_type count(
        static_cast<std::size_t>(3));

    yggr::serialization::collection_size_type other;
    other = static_cast<unsigned int>(3);

    if(count == other)
    {
        const boost::serialization::ser_size_type value = count;
        (void)value;
    }
}
```

## 序列化用途

该类型通常用于保存集合或数组的元素数量，例如：

```cpp
yggr::serialization::collection_size_type count(element_count);
ar << count;
```

在 BSON 数组实现中，它可以作为数组元素数量字段的序列化值，具体字段名称和归档格式由 BSON 数组实现决定。

## 注意事项

1. 该类型底层使用 Boost.Serialization 的 `ser_size_type`，具体宽度取决于当前 Boost 配置。
2. `std::size_t` 构造函数是 `explicit`，不会自动进行隐式构造。
3. 类型提供到 `base_type` 的隐式转换，可能允许在输出或表达式中发生自动类型转换。
4. 非常量转换运算符会返回内部底层值的可修改引用，文本输入归档依赖该行为。
5. 只实现了 `==` 和 `<` 比较，没有直接提供其他关系运算符。
6. 按位可序列化声明要求该类型适合按位复制；修改内部表示后应重新评估该特征。
7. BSON 类型映射依赖底层类型大小，跨平台或跨配置归档时应确保双方兼容。
8. 文件末尾的 BSON 实现包含位于主 include guard 之外，由被包含实现自己的宏负责重复包含控制。

## 相关组件

- `yggr/base/yggrdef.h`
- `yggr/serialization/collection_size_type_bson_impl.hpp`
- `boost/serialization/collection_size_type.hpp`
- `boost/serialization/level.hpp`
- `boost/serialization/is_bitwise_serializable.hpp`
