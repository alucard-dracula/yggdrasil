# `task_info_def.hpp` 中文说明

## 用途

本头文件定义任务计算类型、运行时任务类型，以及描述任务数据类型信息的模板 `task_data_info`。这些定义供任务中心及任务数据元信息解析相关功能使用。

```cpp
#include <yggr/task_center/task_info_def.hpp>
```

## 计算类型

`task_cal_type` 提供计算速度类别常量：

| 常量 | 数值 | 含义 |
| --- | ---: | --- |
| `E_CAL_FAST` | `1` | 快速计算类别。 |
| `E_CAL_SLOW` | `2` | 慢速计算类别。 |

## 运行时任务类型

`runtime_task_type` 提供运行时任务类别常量：

| 常量 | 数值 | 含义 |
| --- | ---: | --- |
| `E_CAL_CONDITION` | `1` | 条件任务。 |
| `E_CAL_RESULT` | `2` | 结果任务。 |

## 任务数据类型信息

```cpp
template<typename ID, typename Ver, typename ClassName = yggr::string>
struct task_data_info;
```

该模板为某种任务数据定义其 ID、版本和类名类型：

| 类型别名 | 定义 | 用途 |
| --- | --- | --- |
| `data_id_type` | `ID` | 数据类型 ID。 |
| `data_ver_type` | `Ver` | 数据格式或类型版本。 |
| `data_info_type` | `packet::packet_info<data_id_type, data_ver_type>` | 由数据 ID 和版本组成的数据描述信息包。 |
| `class_info_type` | `const std::type_info&` | C++ 运行时类型信息的引用类型。 |
| `class_name_type` | `ClassName` | 数据类名的表示类型，默认是 `yggr::string`。 |

## 默认类型

```cpp
typedef task_data_info<u32, u16, yggr::string>
    default_task_data_info_type;
```

默认数据描述使用 `u32` 数据 ID、`u16` 版本号和 `yggr::string` 类名。

## 使用示例

```cpp
typedef yggr::task_center::task_data_info<
    yggr::u32,
    yggr::u16
> my_task_data_info;

my_task_data_info::data_id_type data_id = 1;
my_task_data_info::data_ver_type version = 1;
my_task_data_info::class_name_type class_name("example");
```

`data_info_type` 可用于将数据 ID 与版本组合成数据元信息；实际序列化和解析行为由相关任务数据支持组件实现。
