# `utf8_char_bson_impl.hpp`

## 概述

该头文件为 `yggr::charset::utf8_char_impl` 提供 BSON 输入和输出归档支持。它在 `boost::serialization` 命名空间中定义针对 BSON 归档的 `save` 和 `load` 自由函数，并将实际数据处理委托给归档访问器的原语接口：

- BSON 输出归档调用 `archive::save_access::save_primitive`；
- BSON 输入归档调用 `archive::load_access::load_primitive`。

文件不负责 UTF-8 字符编码转换，也不实现字符类型本身；它只是把该类型接入 BSON 原语序列化路径。

## 头文件

```cpp
#include <yggr/serialization/utf8_char_bson_impl.hpp>
```

## 支持的类型

序列化函数适用于以下模板类型：

```cpp
yggr::charset::utf8_char_impl<Char, Traits, Alloc>
```

模板参数含义由 `utf8_char_impl` 本身决定：

| 参数 | 含义 |
| --- | --- |
| `Char` | 字符类型 |
| `Traits` | 字符 traits 类型 |
| `Alloc` | 分配器类型 |

## 命名空间

实现位于：

```cpp
boost::serialization
```

归档类型使用 Yggdrasil 的 BSON 归档模板：

```cpp
yggr::archive::bson_oarchive<nld>
yggr::archive::bson_iarchive<skip_auto_id>
```

其中：

- `nld` 表示 BSON 输出归档的模板参数；
- `skip_auto_id` 表示 BSON 输入归档是否跳过自动 ID。

## BSON 输出归档

当定义以下宏时启用输出函数：

```cpp
YGGR_HAS_BSON_OARCHIVE
```

函数签名为：

```cpp
template<
    bool nld,
    typename Char,
    typename Traits,
    typename Alloc>
inline void save(
    yggr::archive::bson_oarchive<nld>& ar,
    const yggr::charset::utf8_char_impl<
        Char, Traits, Alloc>& value,
    const unsigned int version);
```

实现只调用：

```cpp
archive::save_access::save_primitive(ar, value);
```

因此 UTF-8 字符的 BSON 表示和具体原语写入规则由 BSON 输出归档及其访问器决定。本函数的 `version` 参数当前不参与额外分支处理。

## BSON 输入归档

当定义以下宏时启用输入函数：

```cpp
YGGR_HAS_BSON_IARCHIVE
```

函数签名为：

```cpp
template<
    bool skip_auto_id,
    typename Char,
    typename Traits,
    typename Alloc>
inline void load(
    yggr::archive::bson_iarchive<skip_auto_id>& ar,
    yggr::charset::utf8_char_impl<
        Char, Traits, Alloc>& value,
    const unsigned int version);
```

实现只调用：

```cpp
archive::load_access::load_primitive(ar, value);
```

该函数将 BSON 输入中的原语值恢复到目标 `utf8_char_impl` 对象中。同样，`version` 参数当前不用于版本分支。

## 条件编译与重新包含标记

文件分别为 BSON 输出和输入归档维护内部状态宏。

### 输出归档状态

启用 `YGGR_HAS_BSON_OARCHIVE` 后，先定义：

```cpp
__YGGR_SERIALIZATION_UTF8_CHAR_BSON_IMPL_HPP_OARCHIVE__
```

如果未启用输出归档，则设置：

```cpp
YGGR_SERIALIZATION_UTF8_CHAR_BSON_IMPL_HPP_OARCHIVE
```

### 输入归档状态

启用 `YGGR_HAS_BSON_IARCHIVE` 后，先定义：

```cpp
__YGGR_SERIALIZATION_UTF8_CHAR_BSON_IMPL_HPP_IARCHIVE__
```

如果未启用输入归档，则设置：

```cpp
YGGR_SERIALIZATION_UTF8_CHAR_BSON_IMPL_HPP_IARCHIVE
```

### 总体 include guard

当带双下划线后缀的输出和输入状态宏同时存在时，文件定义：

```cpp
__YGGR_SERIALIZATION_UTF8_CHAR_BSON_IMPL_HPP__
```

并取消定义：

```cpp
__YGGR_SERIALIZATION_UTF8_CHAR_BSON_IMPL_HPP_OARCHIVE__
__YGGR_SERIALIZATION_UTF8_CHAR_BSON_IMPL_HPP_IARCHIVE__
```

源码中还分别检查并尝试取消定义不带末尾双下划线的宏名称：

```cpp
YGGR_SERIALIZATION_UTF8_CHAR_BSON_IMPL_HPP_OARCHIVE
YGGR_SERIALIZATION_UTF8_CHAR_BSON_IMPL_HPP_IARCHIVE
```

这些宏名称属于现有重新包含控制机制，修改时需要同时检查对应的字符串 BSON 实现和相关包含流程。

## 序列化流程

输出流程：

```text
utf8_char_impl
  -> boost::serialization::save
  -> bson_oarchive
  -> archive::save_access::save_primitive
```

输入流程：

```text
bson_iarchive
  -> boost::serialization::load
  -> archive::load_access::load_primitive
  -> utf8_char_impl
```

该文件不使用 `split_free`，因为它分别直接提供了 BSON 输入和输出方向所需的自由函数。

## 注意事项

- `YGGR_HAS_BSON_OARCHIVE` 和 `YGGR_HAS_BSON_IARCHIVE` 决定对应的 `save`/`load` 是否参与编译。
- 该文件只处理 BSON 原语归档，不提供文本、二进制或其他归档类型的通用实现。
- UTF-8 字符的实际编码合法性和 BSON 字段表示由 `utf8_char_impl` 及 BSON 归档访问器负责。
- `version` 参数当前仅用于匹配 Boost.Serialization 接口，函数体不根据版本执行逻辑。
- 输出/输入状态宏和主 include guard 都是内部实现细节，业务代码不应手动定义、取消定义或依赖其状态。
