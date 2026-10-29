# `tags.hpp` 中文说明

## 用途

本头文件在 `yggr::task_center::type_traits` 命名空间中定义任务中心使用的空标签类型，用于在编译期区分保存器的初始化要求和任务保存类别。

```cpp
#include <yggr/task_center/type_traits/tags.hpp>
```

这些结构体不包含数据或成员函数，其类型本身用于模板参数和编译期判断。

## 初始化标签

| 标签 | 含义 |
| --- | --- |
| `must_need_init` | 保存器需要显式初始化。 |
| `nonmust_need_init` | 保存器不要求显式初始化，可使用相应的默认构造路径。 |

任务中心可根据保存器的 `init_tag_type` 判断其初始化约束。例如，`task_center` 的默认构造要求相关保存器使用 `nonmust_need_init`；需要显式初始化的保存器则应通过提供初始化对象的构造方式使用。

## 保存器类别标签

| 标签 | 含义 |
| --- | --- |
| `mark_saver_condition` | 标识条件任务保存器。 |
| `mark_saver_result` | 标识结果任务保存器。 |

`task_center` 根据这些标记类型选择对应的保存器，以区分条件任务与结果任务的发送、查询、提取及删除操作。

## 使用示例

```cpp
typedef yggr::task_center::type_traits::mark_saver_condition
    condition_mark;

typedef yggr::task_center::type_traits::nonmust_need_init
    saver_init_tag;
```

标签通常由保存器的 `mark_type`、`init_tag_type` 等类型别名公开，并由任务中心内部用于编译期分派；一般无需在运行时创建或操作这些类型的对象。
