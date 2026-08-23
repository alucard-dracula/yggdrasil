# `export.hpp`

## 概述

该头文件为 Boost.Serialization 的类导出宏提供 yggdrasil 统一命名封装。

文件不实现新的序列化导出机制，而是将 `YGGR_CLASS_EXPORT*` 宏直接映射到
`BOOST_CLASS_EXPORT*` 宏，便于项目代码使用统一的前缀。

## 头文件

```cpp
#include <yggr/serialization/export.hpp>
```

## 依赖

```cpp
#include <yggr/base/yggrdef.h>
#include <boost/serialization/export.hpp>
```

## 宏映射

| yggdrasil 宏 | Boost.Serialization 宏 |
| --- | --- |
| `YGGR_CLASS_EXPORT` | `BOOST_CLASS_EXPORT` |
| `YGGR_CLASS_EXPORT_GUID` | `BOOST_CLASS_EXPORT_GUID` |
| `YGGR_CLASS_EXPORT_KEY` | `BOOST_CLASS_EXPORT_KEY` |
| `YGGR_CLASS_EXPORT_KEY2` | `BOOST_CLASS_EXPORT_KEY2` |
| `YGGR_CLASS_EXPORT_IMPLEMENT` | `BOOST_CLASS_EXPORT_IMPLEMENT` |

具体定义：

```cpp
#define YGGR_CLASS_EXPORT BOOST_CLASS_EXPORT
#define YGGR_CLASS_EXPORT_GUID BOOST_CLASS_EXPORT_GUID
#define YGGR_CLASS_EXPORT_KEY BOOST_CLASS_EXPORT_KEY
#define YGGR_CLASS_EXPORT_KEY2 BOOST_CLASS_EXPORT_KEY2
#define YGGR_CLASS_EXPORT_IMPLEMENT BOOST_CLASS_EXPORT_IMPLEMENT
```

## 宏说明

### `YGGR_CLASS_EXPORT`

```cpp
YGGR_CLASS_EXPORT(Type)
```

导出指定类型，适用于需要通过 Boost.Serialization 处理多态类型的场景。

### `YGGR_CLASS_EXPORT_GUID`

```cpp
YGGR_CLASS_EXPORT_GUID(Type, "guid")
```

为类型注册指定 GUID，使序列化数据中的类型标识更加稳定和明确。

### `YGGR_CLASS_EXPORT_KEY`

```cpp
YGGR_CLASS_EXPORT_KEY(Type)
```

声明指定类型的导出键，通常用于头文件中的导出声明。

### `YGGR_CLASS_EXPORT_KEY2`

```cpp
YGGR_CLASS_EXPORT_KEY2(Type, key)
```

使用指定键声明类型导出信息。具体参数要求由 Boost.Serialization 宏定义决定。

### `YGGR_CLASS_EXPORT_IMPLEMENT`

```cpp
YGGR_CLASS_EXPORT_IMPLEMENT(Type)
```

生成指定类型的导出实现，通常应放在一个源文件中，避免多个编译单元重复定义。

## 使用示例

头文件中声明导出键：

```cpp
#include <yggr/serialization/export.hpp>

YGGR_CLASS_EXPORT_KEY(my_namespace::my_type)
```

源文件中生成导出实现：

```cpp
#include <yggr/serialization/export.hpp>

YGGR_CLASS_EXPORT_IMPLEMENT(my_namespace::my_type)
```

使用自定义 GUID：

```cpp
YGGR_CLASS_EXPORT_GUID(
    my_namespace::my_type,
    "my_namespace.my_type")
```

## 与 `yggr::any` 的关系

`yggr/serialization/any.hpp` 使用本文件提供的导出宏，为
`yggr::any::holder<T>` 构造更具体的导出宏：

```cpp
#define YGGR_ANY_EXPORT(__type__) \
    YGGR_CLASS_EXPORT(yggr::any::holder<__type__>)
```

因此，`export.hpp` 是 any 类型导出和其他项目序列化导出的基础宏适配层。

## 注意事项

1. 这些宏只是 Boost.Serialization 宏的别名，具体行为、参数格式和链接要求由 Boost.Serialization 决定。
2. `YGGR_CLASS_EXPORT_IMPLEMENT` 通常只应在一个源文件中使用，避免重复定义。
3. 导出声明、GUID 和实现必须对同一类型保持一致。
4. 模板类型导出时，应确认 Boost.Serialization 对模板实例化和导出位置的要求。
5. 导出宏通常用于多态序列化；普通非多态类型不一定需要显式导出。
6. 该文件的 `yggr::serialization` 命名空间为空，不提供额外类或函数。
7. 使用本文件前应确保 Boost.Serialization 的导出头文件和相关库配置正确。

## 相关组件

- `yggr/serialization/any.hpp`
- `yggr/serialization/nvp.hpp`
- `yggr/any_val/any.hpp`
- `boost/serialization/export.hpp`
