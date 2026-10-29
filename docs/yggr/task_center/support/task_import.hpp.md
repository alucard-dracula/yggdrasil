# `task_import.hpp` 中文说明

## 用途

本头文件提供一组宏，用于为任务数据类导入统一的类型元信息接口：数据 ID、数据版本、数据描述、运行时类型信息、类名和计算类型。宏通常在任务数据类的类定义内部使用。

```cpp
#include <yggr/task_center/support/task_import.hpp>
```

默认的元信息类型来自 `task_data_info_parser<BaseType>`；`BaseType` 通常是 `task_info_def.hpp` 中定义的任务数据元信息配置类型，需提供解析器所要求的类型别名。

## 主要宏

### 通用计算类别

```cpp
YGGR_CAL_TASK_IMPORT(BaseType, ThisType, DataID, DataVer, CalType)
YGGR_CAL_TASK_IMPORT_TPL(BaseType, ThisType, DataID, DataVer, CalType)
```

这两个宏在类中声明：

- 由 `task_data_info_parser<BaseType>` 提供的 `data_id_type`、`data_ver_type`、`data_info_type`、`class_info_type` 和 `class_name_type` 类型别名。
- 静态常量 `E_pak_id` 和 `E_now_ver`，分别保存数据 ID 和版本。
- 静态函数 `data_id()`、`data_ver()`、`data_info()`、`class_info()`、`class_name()` 和 `cal_type()`。

`YGGR_CAL_TASK_IMPORT` 用于普通类型；`YGGR_CAL_TASK_IMPORT_TPL` 用于依赖模板参数的类型别名场景，在相关类型前增加 `typename`。

### 快速和慢速计算类别

```cpp
YGGR_FAST_CAL_TASK_IMPORT(BaseType, ThisType, DataID, DataVer)
YGGR_FAST_CAL_TASK_IMPORT_TPL(BaseType, ThisType, DataID, DataVer)

YGGR_SLOW_CAL_TASK_IMPORT(BaseType, ThisType, DataID, DataVer)
YGGR_SLOW_CAL_TASK_IMPORT_TPL(BaseType, ThisType, DataID, DataVer)
```

这组宏是通用宏的便捷包装，计算类型分别固定为：

- `YGGR_FAST_CAL_TASK_IMPORT*`：`task_cal_type::E_CAL_FAST`。
- `YGGR_SLOW_CAL_TASK_IMPORT*`：`task_cal_type::E_CAL_SLOW`。

### 继承类型信息

```cpp
YGGR_INHERIT_CAL_TASK_IMPORT(ThisType)
```

该宏只声明当前类的 `class_info()` 和 `class_name()`，适用于已有数据 ID、版本和计算类型接口，只需要为派生类型提供自身运行时类型信息的场景。宏不会自动继承或生成这些其他接口。

## 使用示例

```cpp
class example_task_data
{
public:
    YGGR_FAST_CAL_TASK_IMPORT(
        yggr::task_center::default_task_data_info_type,
        example_task_data,
        yggr::task_center::task_data_id_def::E_data_id_user,
        1)
};
```

模板类可使用 `_TPL` 版本：

```cpp
template<typename Value>
class example_task_data_tpl
{
public:
    YGGR_SLOW_CAL_TASK_IMPORT_TPL(
        yggr::task_center::default_task_data_info_type,
        example_task_data_tpl<Value>,
        yggr::task_center::task_data_id_def::E_data_id_user + 1,
        1)
};
```

示例中的数据 ID 仅供说明；实际项目应为不同数据类型分配稳定且不重复的 ID，并按数据格式变更策略维护版本号。

## 参数与生成接口

| 参数 | 含义 |
| --- | --- |
| `BaseType` | 数据 ID、版本及类信息类型的配置类型，用于实例化 `task_data_info_parser`。 |
| `ThisType` | 当前任务数据类类型，用于生成 `typeid(ThisType)` 类信息。模板类中应传入完整特化类型。 |
| `DataID` | 当前数据类型的 ID，转换为配置中的 `data_id_type`。 |
| `DataVer` | 当前数据类型的版本，转换为配置中的 `data_ver_type`。 |
| `CalType` | 计算类型值；通常使用 `task_cal_type` 中的快速或慢速常量。 |

`data_info()` 无参重载返回由数据 ID 和版本构成的 `data_info_type`；同时还生成接受 `const ThisType&` 的重载，并返回同一元信息。`class_info()` 返回 `typeid(ThisType)`，`class_name()` 返回其运行时类型名。

## 注意事项

- 宏会生成 `public` 成员声明，应放在类定义内部；不要在同一个类中重复展开会生成相同成员的宏。
- `BaseType` 必须与使用该类的任务数据解析器配置一致。
- 数据 ID 和版本值必须能转换为配置中对应的类型。
- `typeid(...).name()` 返回的名称由 C++ 实现决定，不保证跨编译器或跨平台稳定；需要稳定外部名称时，应另行设计应用层映射。
