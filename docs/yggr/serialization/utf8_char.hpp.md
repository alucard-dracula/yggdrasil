# `utf8_char.hpp`

## 概述

该头文件为 `yggr::charset::utf8_char_impl` 提供通用的 Boost.Serialization 支持。它在 `boost::serialization` 命名空间中定义 `save`、`load` 和 `serialize` 自由函数，并根据 `YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE` 选择 UTF-8 字符的归档方式。

主要行为如下：

- 默认情况下，直接对字符类型的底层字符串表示执行原语保存和加载；
- 定义 `YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE` 时，通过 `std::basic_string<Char>` 临时对象完成保存和加载；
- 使用 `boost::serialization::split_free` 分离保存与加载；
- 将 `utf8_char_impl` 的实现级别设置为 `object_serializable`；
- 在文件末尾接入 BSON 专用实现。

源码中的主 include guard 名称为 `__YGGR_SERIALIZATION_UTF8_STRING_HPP__`，虽然当前文件名和适配类型是 `utf8_char`，这里按源码实际名称记录。

## 头文件

```cpp
#include <yggr/serialization/utf8_char.hpp>
```

## 依赖

主要依赖包括：

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_char.hpp>
#include <yggr/serialization/string.hpp>
#include <boost/archive/detail/oserializer.hpp>
#include <boost/archive/detail/iserializer.hpp>
#include <boost/serialization/collections_save_imp.hpp>
#include <boost/serialization/collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>
```

其中：

- `yggr/charset/utf8_char.hpp` 提供被序列化的 `utf8_char_impl` 类型；
- `serialization/string.hpp` 提供字符串相关归档基础；
- Boost 的输入/输出归档和 `split_free` 依赖支持自由函数序列化。

## 支持的类型

序列化函数适用于：

```cpp
yggr::charset::utf8_char_impl<Char, Traits, Alloc>
```

该类型提供 `base_type` 和 `org_str()`，本文件通过它们访问 UTF-8 字符的底层字符串表示。

| 参数 | 含义 |
| --- | --- |
| `Char` | 字符类型 |
| `Traits` | 字符 traits 类型 |
| `Alloc` | 分配器类型 |

## 直接原语序列化路径

当未定义以下宏时，使用直接原语路径：

```cpp
YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE
```

### `save`

`save` 获取对象的底层字符串：

```cpp
typedef yggr::charset::utf8_char_impl<
    Char, Traits, Alloc> char_type;
typedef typename char_type::base_type base_type;

const base_type& base = value.org_str();
archive::save_access::save_primitive(ar, base);
```

该路径不复制到临时字符串，直接将 `org_str()` 返回的底层表示交给归档访问器。

### `load`

`load` 获取底层字符串的可写引用，并直接加载原语：

```cpp
base_type& base = value.org_str();
archive::load_access::load_primitive(ar, base);
```

## 非原语兼容路径

当定义以下宏时启用兼容路径：

```cpp
YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE
```

### `save`

实现先将底层字符串复制到标准字符串：

```cpp
typedef std::basic_string<Char> store_type;
store_type temporary(base.begin(), base.end());
archive::save_access::save_primitive(ar, temporary);
```

这使归档访问器处理标准 `std::basic_string<Char>`，而不是直接处理 `utf8_char_impl` 的底层字符串类型。

### `load`

实现先加载临时标准字符串，再将内容复制回 UTF-8 字符对象：

```cpp
typedef std::basic_string<Char> load_type;
load_type temporary;
archive::load_access::load_primitive(ar, temporary);
base.assign(temporary.begin(), temporary.end());
```

该路径会产生临时字符串和一次内容复制，但可用于底层字符串不被当前归档识别为原语的环境。

## `serialize`

无论采用哪条 `save`/`load` 路径，`serialize` 都使用 Boost.Serialization 的自由函数拆分：

```cpp
template<
    typename Archive,
    typename Char,
    typename Traits,
    typename Alloc>
inline void serialize(
    Archive& ar,
    yggr::charset::utf8_char_impl<Char, Traits, Alloc>& value,
    const unsigned int version)
{
    boost::serialization::split_free(ar, value, version);
}
```

归档方向由 `split_free` 决定：输出时调用 `save`，输入时调用 `load`。

## 实现级别

文件包含：

```cpp
#include <yggr/serialization/detail/container_implementation_level_def.hpp>
```

并将 `utf8_char_impl` 的实现级别设置为 `object_serializable`：

```cpp
YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(
    3,
    yggr::charset::utf8_char_impl,
    object_serializable)
```

其中 `3` 表示 `utf8_char_impl` 模板参数数量。

## BSON 集成

主 include guard 结束后，文件包含：

```cpp
#include <yggr/serialization/utf8_string_bson_impl.hpp>
```

该 BSON 实现为 `utf8_char_impl` 提供 BSON 输入和输出归档的专用 `save`/`load` 函数，并将操作委托给 BSON 归档的原语访问器。

虽然包含文件名使用 `utf8_string_bson_impl.hpp`，其实际实现针对的是 `utf8_string_impl`；当前源码的最后包含关系应结合项目的 BSON 重新包含机制一并维护。

## 序列化流程

默认直接原语路径：

```text
utf8_char_impl
  -> org_str()
  -> archive::save_access::save_primitive / load_primitive
```

非原语兼容路径：

```text
utf8_char_impl
  -> org_str()
  -> std::basic_string<Char> 临时对象
  -> save_primitive / load_primitive
  -> assign 回 utf8_char_impl
```

## 注意事项

- `YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE` 会改变保存和加载的中间类型及复制行为。
- `version` 参数用于匹配 Boost.Serialization 接口，本文件不根据版本执行分支。
- `utf8_char_impl` 必须提供 `base_type`、`org_str()` 和可用的 `assign` 操作。
- `save` 和 `load` 使用底层字符串表示，字符编码合法性由 `utf8_char_impl` 和相关字符类型实现负责。
- 当前文件的 include guard 使用 `UTF8_STRING` 名称，修改时应避免与 `utf8_string.hpp` 产生冲突。
- BSON 专用行为由文件末尾包含的 BSON 实现负责；修改包含关系时应同步检查对应 BSON 头文件和重新包含控制宏。
