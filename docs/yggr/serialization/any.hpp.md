# `any.hpp`

## 概述

该头文件为 `yggr::any::holder<T>` 提供 Boost.Serialization 导出相关的宏封装，用于支持 `yggr::any` 中保存类型的序列化和多态导出。

当前文件本身不定义额外的序列化类或函数，`yggr::serialization` 命名空间为空。

## 头文件

```cpp
#include <yggr/serialization/any.hpp>
```

## 依赖

该文件依赖以下组件：

- `yggr/serialization/export.hpp`：提供类导出相关宏；
- `yggr/any_val/any.hpp`：提供 `yggr::any::holder<T>`；
- `yggr/serialization/string.hpp`：字符串序列化支持；
- `yggr/serialization/utf8_string.hpp`：UTF-8 字符串序列化支持；
- `yggr/charset/string.hpp` 和 `yggr/charset/utf8_string.hpp`：字符串类型定义；
- `yggr/ppex/concat.hpp`：预处理器连接工具。

## 导出宏

### `YGGR_ANY_EXPORT`

```cpp
#define YGGR_ANY_EXPORT(__type__) \
    YGGR_CLASS_EXPORT(yggr::any::holder<__type__>)
```

为指定类型的 `yggr::any::holder<__type__>` 生成类导出声明。

适用于需要在序列化代码中导出某个具体类型的场景。

### `YGGR_ANY_EXPORT_DECLEAR`

```cpp
#define YGGR_ANY_EXPORT_DECLEAR(__type__) \
    YGGR_CLASS_EXPORT_KEY(yggr::any::holder<__type__>)
```

为指定类型的 `yggr::any::holder<__type__>` 生成导出键声明。

注意：宏名称中的 `DECLEAR` 是当前源代码中的既有拼写，应按该名称调用。

### `YGGR_ANY_EXPORT_IMPL`

```cpp
#define YGGR_ANY_EXPORT_IMPL(__type__) \
    YGGR_CLASS_EXPORT_IMPLEMENT(yggr::any::holder<__type__>)
```

为指定类型的 `yggr::any::holder<__type__>` 生成导出实现。

该宏通常应在一个源文件中使用，避免同一导出实现被多个编译单元重复定义。

## 使用方式

以下示例展示宏的调用形式：

```cpp
#include <yggr/serialization/any.hpp>

YGGR_ANY_EXPORT_DECLEAR(my_namespace::my_type)
```

在对应的实现源文件中：

```cpp
#include <yggr/serialization/any.hpp>

YGGR_ANY_EXPORT_IMPL(my_namespace::my_type)
```

在适合直接导出类型的场景，也可以使用：

```cpp
YGGR_ANY_EXPORT(my_namespace::my_type)
```

具体声明与实现的组织方式应遵循项目对 `YGGR_CLASS_EXPORT` 系列宏的约定。

## 类型映射

宏会将用户类型转换为以下 holder 类型：

```cpp
yggr::any::holder<__type__>
```

例如：

```cpp
YGGR_ANY_EXPORT(std::string)
```

等价于对以下类型执行导出宏：

```cpp
YGGR_CLASS_EXPORT(yggr::any::holder<std::string>)
```

## 内置类型导出

文件中保留了一组内置类型导出示例，但当前全部被注释，没有产生实际编译效果。涉及类型包括：

- `char`；
- `wchar_t`；
- `yggr::s8`、`yggr::u8`；
- `yggr::s16`、`yggr::u16`；
- `yggr::s32`、`yggr::u32`；
- `yggr::s64`、`yggr::u64`；
- `yggr::f32`、`yggr::f64`；
- `std::string`；
- `boost::container::string`；
- `yggr::charset::utf8_string`；
- Android 条件下的 `long int` 和 `unsigned long int`。

`wchar_t` 的原注释还包含 Android 和 MinGW 条件限制。

## 序列化关系

该文件只负责导出宏，不负责定义具体的 `save`、`load` 或 `serialize` 函数。被导出的 `yggr::any::holder<T>` 是否可以正确序列化，还取决于：

1. `T` 是否满足项目序列化要求；
2. `yggr::any::holder<T>` 是否提供对应序列化支持；
3. 导出声明和实现是否位于正确的编译单元；
4. 序列化库和链接配置是否正确。

## 注意事项

1. `YGGR_ANY_EXPORT_DECLEAR` 的拼写是源码中的公共宏名称，不应自行改写为 `DECLARE` 版本。
2. `YGGR_ANY_EXPORT_IMPL` 通常只应在一个源文件中使用，避免重复符号定义。
3. 当前文件中的内置类型导出代码全部被注释，不会自动导出任何内置类型。
4. 使用宏前应确保目标类型可以作为 `yggr::any::holder<T>` 的合法模板参数。
5. 导出声明和实现必须使用完全一致的类型，包括命名空间、限定符和平台条件。
6. 具体的导出宏行为由 `yggr/serialization/export.hpp` 决定。
7. 字符串类型的序列化能力由对应的字符串序列化头文件提供，并不由本文件单独实现。

## 相关组件

- `yggr/any_val/any.hpp`
- `yggr/serialization/export.hpp`
- `yggr/serialization/string.hpp`
- `yggr/serialization/utf8_string.hpp`
- `yggr/charset/string.hpp`
- `yggr/charset/utf8_string.hpp`
