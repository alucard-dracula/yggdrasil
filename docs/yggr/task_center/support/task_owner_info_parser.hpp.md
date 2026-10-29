# `task_owner_info_parser.hpp` 中文说明

## 用途

本头文件定义 `yggr::task_center::support::task_owner_info_parser<T>`，用于从任务所有者信息中提取所有者 ID。`task` 类可通过该解析器获得自身的 `owner_id()`。

```cpp
#include <yggr/task_center/support/task_owner_info_parser.hpp>
```

## 通用解析器

```cpp
template<typename T>
struct task_owner_info_parser;
```

通用实现要求 `T` 提供嵌套类型 `owner_id_type` 和成员函数 `owner_id()`：

```cpp
typedef typename T::owner_id_type owner_id_type;

static owner_id_type owner_id(const T& value);
```

解析器通过调用 `value.owner_id()` 返回所有者 ID。自定义所有者信息类型可按此约定实现接口，或为该类型提供解析器特化。

## 内置值类型特化

宏 `YGGR_PP_TASK_OWNER_INFO_PARSER_OF_VALUE(Type)` 为可直接充当所有者 ID 的值类型生成特化。此时 `owner_id_type` 就是该类型本身，解析器直接返回输入值的常量引用。

头文件已为以下类型提供特化：

- `char`
- `yggr::s8`、`yggr::u8`
- `yggr::s16`、`yggr::u16`
- `yggr::s32`、`yggr::u32`
- `yggr::s64`、`yggr::u64`
- `yggr::ids::inner_process_id`
- `yggr::ids::uuid`

因此这些类型可直接用作任务所有者信息和所有者 ID，无需包装成带有 `owner_id()` 成员函数的结构体。

## 使用示例

### 所有者信息结构体

```cpp
struct owner_info
{
    typedef yggr::u64 owner_id_type;

    yggr::u64 owner_id() const
    {
        return id;
    }

    yggr::u64 id;
};

typedef yggr::task_center::support::task_owner_info_parser<owner_info>
    owner_parser;

owner_info info;
owner_parser::owner_id_type id = owner_parser::owner_id(info);
```

### 直接使用内置 ID 类型

```cpp
typedef yggr::task_center::support::task_owner_info_parser<yggr::u64>
    owner_parser;

yggr::u64 owner_id = 42;
const yggr::u64& id = owner_parser::owner_id(owner_id);
```

## 与 `task` 的关系

`task<TaskID, OwnerInfo, ...>` 使用 `task_owner_info_parser<OwnerInfo>` 定义 `owner_id_type`，并通过解析器实现 `owner_id()` 和 `copy_owner_id()`。若使用自定义 `OwnerInfo`，其解析器返回类型需与任务其他组件预期的所有者 ID 类型一致。
