# `task.hpp` 中文说明

## 用途

`yggr::task_center::task` 是任务数据对象，封装任务 ID、所有者信息、数据类型元信息、计算类型和实际数据。它提供数据访问接口、类型解析器定制点及基于任务 ID 的比较操作。

```cpp
#include <yggr/task_center/task.hpp>
```

## 模板参数

```cpp
template<
    typename TaskID,
    typename OwnerInfo,
    typename TaskDataInfo,
    typename DataContainer = yggr::any,
    template<typename> class Task_Inner_Data_Parser =
        yggr::task_center::support::task_inner_data_parser,
    template<typename> class Task_Owner_Info_Parser =
        yggr::task_center::support::task_owner_info_parser,
    template<typename> class Task_Data_Info_Parser =
        yggr::task_center::support::task_data_info_parser
>
class task;
```

| 参数 | 用途 |
| --- | --- |
| `TaskID` | 任务 ID 类型。 |
| `OwnerInfo` | 任务所有者信息类型。 |
| `TaskDataInfo` | 任务数据元信息配置类型。 |
| `DataContainer` | 实际数据容器，默认使用 `yggr::any`。 |
| `Task_Inner_Data_Parser` | 从数据容器中取得实际数据的解析器模板。 |
| `Task_Owner_Info_Parser` | 从所有者信息中取得所有者 ID 的解析器模板。 |
| `Task_Data_Info_Parser` | 解析数据 ID、版本和数据类型元信息的解析器模板。 |

任务从三个解析器导出 `owner_id_type`、`data_id_type`、`data_ver_type`、`data_info_type`、`class_info_type` 和 `class_name_type` 等类型别名。

## 构造任务

默认构造会初始化 ID、所有者信息和数据元信息，并将计算类型设为 `task_cal_type::E_CAL_FAST`。数据容器默认构造。

可以显式提供所有者信息、元信息、计算类型和数据容器：

```cpp
task(const owner_info_type& owner_info,
     const data_info_type& data_info,
     u32 cal_type,
     const data_container_type& data);

task(const id_type& id,
     const owner_info_type& owner_info,
     const data_info_type& data_info,
     u32 cal_type,
     const data_container_type& data);
```

还提供以数据对象构造的模板重载：

```cpp
template<typename Data>
task(const owner_info_type& owner_info, const Data& data);

template<typename Data>
task(const id_type& id, const owner_info_type& owner_info, const Data& data);
```

这两种构造方式通过 `Task_Data_Info_Parser::data_info<Data>()` 生成数据元信息，从 `data.cal_type()` 获取计算类型，并用 `data` 初始化数据容器。

## 数据与元信息访问

| 接口 | 说明 |
| --- | --- |
| `empty()` | 检查实际数据容器是否为空；不检查任务 ID 或其他元信息。 |
| `task_id()` | 返回任务 ID 的可写引用或常量引用。 |
| `copy_task_id()` | 返回任务 ID 的副本。 |
| `owner_info()` / `copy_owner_info()` | 访问或复制所有者信息。 |
| `owner_id()` / `copy_owner_id()` | 通过所有者解析器取得所有者 ID。 |
| `data_info()` / `copy_data_info()` | 访问或复制数据元信息。 |
| `data_id()` / `copy_data_id()` | 通过数据元信息解析器取得数据 ID。 |
| `data_ver()` / `copy_data_ver()` | 通过数据元信息解析器取得数据版本。 |
| `cal_type()` | 返回计算类型，类型为 `u32`。 |
| `type()` | 返回实际数据容器所报告的 `std::type_info`。 |
| `class_info()` | 返回数据容器所报告的 `class_info_type`。 |
| `class_name()` | 返回实际数据类型的运行时类型名。 |

非 `copy_` 访问器通常返回对内部成员的引用；`owner_id()` 和 `class_name()` 则返回解析器或容器提供的值。

## 访问实际数据

```cpp
template<typename T>
T* get();

template<typename T>
const T* get() const;
```

`get<T>()` 将数据容器交给 `Task_Inner_Data_Parser`，尝试取得其中类型为 `T` 的对象。若容器不保存该类型的数据，具体解析器会以空指针等方式表示无法取得；调用方应检查返回指针后再访问。

## 复制、移动与交换

任务支持复制构造、移动构造、复制赋值、移动赋值和 `swap()`。移动赋值使用 `copy_or_move_or_swap` 处理各成员，交换操作会交换 ID、所有者信息、数据元信息、计算类型及数据容器。

本头文件还为任务提供 `yggr::task_center::swap_support::swap`，并将相应 `swap` 引入 `yggr::task_center`、`std` 和 `boost` 命名空间，以配合相关泛型算法和交换调用。

## 类信息查询辅助类型

任务类型定义了一组 getter 类型别名，可用于泛型代码中读取任务字段：

| 别名 | 读取内容 |
| --- | --- |
| `task_id_getter` | `task_id()` |
| `owner_info_getter` | `owner_info()` |
| `owner_id_getter` | `owner_id()` |
| `data_info_getter` | `data_info()` |
| `data_id_getter` | `data_id()` |
| `data_ver_getter` | `data_ver()` |
| `class_info_getter` | 若任务提供匹配的 `class_info()` 则读取它，否则回退到 `typeid(T)`。 |
| `class_name_getter` | 若任务提供匹配的 `class_name()` 则读取它，否则回退到 `typeid(T).name()`。 |

`class_info_getter` 和 `class_name_getter` 使用编译期检测选择任务成员函数或类型信息回退路径。

## 比较操作

本头文件提供任务与任务 ID、任务与任务之间的 `==` 和 `!=` 比较：

- 任务与 ID 比较其 `task_id()`。
- 两个任务比较其任务 ID；相同对象地址也视为相等。
- `!=` 是对应相等判断的取反。

因此，任务内容、所有者、数据元信息和计算类型不同，并不会单独使两个任务在这些比较操作下不相等；比较依据是任务 ID。

## 示例

以下示例展示一种常见类型组合，具体 ID、所有者及数据元信息类型需按应用的解析器配置提供：

```cpp
typedef yggr::task_center::task<
    task_id_type,
    owner_info_type,
    task_data_info_type
> task_type;

task_type value;

if(!value.empty())
{
    const auto* payload = value.get<payload_type>();
    if(payload)
    {
        // 使用 payload
    }
}
```

默认数据容器是 `yggr::any`。如需使用其他数据容器或元信息格式，可替换相应模板参数，并确保解析器接口与任务构造及访问操作相匹配。
