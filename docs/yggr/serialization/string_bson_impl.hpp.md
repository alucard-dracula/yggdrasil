# `string_bson_impl.hpp`

## 概述

该头文件是字符串 BSON 序列化实现的条件编译控制头文件。它本身不定义序列化函数，而是根据 BSON 输入或输出归档是否可用，设置一个临时控制宏，供相关头文件的重新包含逻辑使用。

## 头文件

```cpp
#include <yggr/serialization/string_bson_impl.hpp>
```

通常不需要直接调用该文件中的 API。它由字符串序列化相关头文件或 BSON 实现重新包含流程间接使用。

## 编译条件

当定义以下任一宏时，文件启用其主 include guard：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HAS_BSON_IARCHIVE
```

这表示 BSON 输出归档或 BSON 输入归档至少有一种可用：

```cpp
#if defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HAS_BSON_IARCHIVE)
#   define __YGGR_SERIALIZATION_STRING_BSON_IMPL_HPP__
```

如果两个宏都未定义，文件不会定义主 include guard，而是定义：

```cpp
YGGR_SERIALIZATION_STRING_BSON_IMPL_HPP_ARCHIVE
```

## 重新包含机制

`YGGR_SERIALIZATION_STRING_BSON_IMPL_HPP_ARCHIVE` 是一次性的内部控制标记。它用于通知 `yggr/serialization/detail/bson_impl_reinclue.hpp`：当前字符串 BSON 实现需要通过以下头文件重新进入普通字符串序列化定义：

```cpp
#include <yggr/serialization/string.hpp>
```

该设计允许 BSON 归档宏在合适的时机启用字符串相关序列化代码，同时避免普通头文件 include guard 阻止必要的重新包含。

## 宏状态总结

| 条件 | 设置的宏 | 含义 |
| --- | --- | --- |
| `YGGR_HAS_BSON_OARCHIVE` 或 `YGGR_HAS_BSON_IARCHIVE` 已定义 | `__YGGR_SERIALIZATION_STRING_BSON_IMPL_HPP__` | BSON 归档支持已启用，正常完成该头文件的包含保护 |
| 两个 BSON 归档宏均未定义 | `YGGR_SERIALIZATION_STRING_BSON_IMPL_HPP_ARCHIVE` | 暂存重新包含请求，等待 BSON 归档环境建立后处理 |

如果此前残留了 `YGGR_SERIALIZATION_STRING_BSON_IMPL_HPP_ARCHIVE`，文件会先将其取消定义，再根据当前 BSON 归档宏重新判断状态。

## 依赖与注意事项

- 该文件依赖 `YGGR_HAS_BSON_OARCHIVE` 和 `YGGR_HAS_BSON_IARCHIVE` 的定义状态。
- `__YGGR_SERIALIZATION_STRING_BSON_IMPL_HPP__` 和 `YGGR_SERIALIZATION_STRING_BSON_IMPL_HPP_ARCHIVE` 均属于内部实现宏，不应由业务代码主动依赖或修改。
- 该文件不直接提供字符串序列化接口；实际字符串序列化定义位于 `yggr/serialization/string.hpp` 及其相关实现中。
- 修改宏定义顺序可能改变 `string.hpp` 的重新包含时机，因此调整时应同时检查 BSON 归档头文件和 `detail/bson_impl_reinclue.hpp` 的包含流程。
