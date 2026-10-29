# `task_creator.hpp` 中文说明

## 用途

`yggr::task_center::task_creator<ID_Generator, Task>` 是任务创建器。它使用 ID 生成器为新任务分配任务 ID，并支持从构造参数创建任务，或为已有的无 ID 任务补充分配 ID。

```cpp
#include <yggr/task_center/task_creator.hpp>
```

## 模板参数与类型约束

```cpp
template<typename ID_Generator, typename Task>
class task_creator;
```

| 参数 | 说明 |
| --- | --- |
| `ID_Generator` | ID 生成器，需提供 `id_type` 类型，并可通过 `operator()()` 生成 ID。 |
| `Task` | 任务类型，需提供 `id_type`，且支持以生成的 ID 和任务构造参数构造任务。 |

编译期断言要求 `Task::id_type` 与 `ID_Generator::id_type` 完全相同，否则模板实例化失败。

公开类型别名：

- `id_gener_type`：对应 `ID_Generator`。
- `task_type`：对应 `Task`。

## 创建新任务

```cpp
template<typename... Args>
task_type operator()(Args&&... args) const;
```

调用时先从内部 ID 生成器取得一个新 ID，再将 ID 和转发后的参数传给任务构造函数：

实现中会调用 `task_type(_id_gener(), boost::forward<Args>(args)...)`。

因此，`Task` 必须提供与该调用相兼容的构造方式。未启用 C++11 可变参数模板时，接口通过预处理器生成项目设定参数范围内的重载。

## 为已有任务分配 ID

```cpp
task_type& operator()(task_type& task) const;
```

该重载用于尚未分配任务 ID 的已有任务：

1. 通过断言要求 `task.task_id()` 尚未设置。
2. 调用 ID 生成器取得新 ID。
3. 使用 `copy_or_move_or_swap` 将生成的 ID 写入任务。
4. 返回原任务的引用。

调用方应确保任务确实没有 ID。断言在关闭断言的构建中不会阻止对已有 ID 的任务再次调用此接口，因此应避免违反该前置条件。

## 使用示例

以下示例展示类型组合方式。`id_generator` 和 `my_task` 由应用提供，并需满足前述类型及构造要求：

```cpp
using creator_type =
    yggr::task_center::task_creator<id_generator, my_task>;

void example()
{
    creator_type creator;

    my_task task = creator(/* 任务构造参数 */);
    creator(task); // 为无 ID 的 task 分配 ID
}
```

## 与 `task_center` 的关系

`task_center` 通过 `Task_Creator` 创建要保存的新任务，也可使用它处理已有任务并补充分配 ID。该创建器只负责生成任务及 ID，不负责任务队列、保存策略或任务分发。
