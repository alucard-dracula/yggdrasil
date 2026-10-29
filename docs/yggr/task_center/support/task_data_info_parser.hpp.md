# `task_data_info_parser.hpp` 中文说明

## 用途

本头文件定义任务数据类型元信息解析器 `yggr::task_center::support::task_data_info_parser<Info_Def>`。它根据数据类型 `T` 查询其数据 ID、版本、组合元信息、运行时类型信息及类名，并可从组合元信息中提取数据 ID 和版本。

```cpp
#include <yggr/task_center/support/task_data_info_parser.hpp>
```

## 元信息配置

`Info_Def` 提供以下类型别名：

| 别名 | 用途 |
| --- | --- |
| `data_id_type` | 数据 ID 类型。 |
| `data_ver_type` | 数据版本类型。 |
| `data_info_type` | 数据 ID 和版本组成的元信息类型。 |
| `class_info_type` | C++ 类型信息类型。 |
| `class_name_type` | 类名的表示类型。 |

默认的元信息配置可使用 `task_info_def.hpp` 中定义的 `default_task_data_info_type`。

## 数据类型 getter

```cpp
template<typename Info_Def, typename T>
struct task_data_info_getter;
```

通用实现要求数据类型 `T` 提供静态接口：

```cpp
T::data_id();
T::data_ver();
T::data_info();
T::class_info();
T::class_name();
```

并要求这些接口的返回类型分别与 `Info_Def` 对应的五个类型别名完全一致。类型不匹配时，编译期断言会报错。

## 解析器接口

```cpp
template<typename Info_Def>
struct task_data_info_parser;
```

解析器公开与 `Info_Def` 对应的类型别名，并提供以下静态模板接口：

```cpp
template<typename T> static data_id_type data_id();
template<typename T> static data_ver_type data_ver();
template<typename T> static data_info_type data_info();
template<typename T> static class_info_type class_info();
template<typename T> static class_name_type class_name();
```

这些接口将请求转发给 `task_data_info_getter<Info_Def, T>`。此外还提供从元信息对象读取 ID 和版本的重载：

```cpp
static const data_id_type& data_id(const data_info_type& info);
static const data_ver_type& data_ver(const data_info_type& info);
```

这两个访问器分别读取 `info.get<0>()` 和 `info.get<1>()`，因此 `data_info_type` 必须能以此方式访问其两个字段。

## 内置数据类型

头文件使用 `YGGR_PP_TASK_DATA_INFO_PARSER_DEF` 为常见基础类型生成 `task_data_info_getter` 特化。各内置类型的版本号均为 `1`：

| 数据类型 | ID 常量 |
| --- | --- |
| `char` | `task_data_id_def::E_data_id_char` |
| `s8`、`u8` | `E_data_id_s8`、`E_data_id_u8` |
| `wchar_t` | `E_data_id_wchar_t` |
| `s16`、`u16` | `E_data_id_s16`、`E_data_id_u16` |
| `s32`、`u32` | `E_data_id_s32`、`E_data_id_u32` |
| `s64`、`u64` | `E_data_id_s64`、`E_data_id_u64` |
| `f32`、`f64` | `E_data_id_f32`、`E_data_id_f64` |
| `c8`、`c16`、`c32` | `E_data_id_c8`、`E_data_id_c16`、`E_data_id_c32` |

`wchar_t`、`c8`、`c16`、`c32` 的映射会根据构建配置中相应的 `YGGR_NO_CWCHAR`、`YGGR_NO_CHAR8_T`、`YGGR_NO_CHAR16_T` 和 `YGGR_NO_CHAR32_T` 宏启用或排除。

生成的特化使用 `typeid(T)` 提供 `class_info()`，使用 `typeid(T).name()` 提供 `class_name()`。

## 自定义数据类型

对于未在头文件中预定义的类型，通用 getter 要求该类型自行提供上述静态元信息接口。也可以通过宏生成 getter 特化：

```cpp
YGGR_PP_TASK_DATA_INFO_PARSER_DEF(
    my_data_type,
    task_data_id_def::E_data_id_user,
    1)
```

该宏按 `Info_Def` 的类型定义返回值，并使用给定的数据类型、ID 和版本生成 `data_id()`、`data_ver()`、`data_info()`、`class_info()` 和 `class_name()`。使用前应确保该 ID 在应用中唯一，且位于项目为用户类型保留的 ID 范围内。

## 与其他任务组件的关系

`task` 可使用此解析器生成和读取其数据元信息；`task_data_send_typeid_getter` 和 `task_data_recv_typeid_getter` 则可将数据类型映射为发送元信息或接收类名。自定义类型应在使用这些接口前具备有效的 getter 或特化。
