# `timer_task.hpp` 中文说明

## 用途

本头文件定义定时任务的抽象基类 `base_timer_task<Task_ID>` 和具体包装类 `timer_task<Task_ID, Time_Task_Pak>`。包装类保存一个可调用任务包，并将任务 ID 和回调执行统一到管理器所需的虚接口中。

```cpp
#include <yggr/time/detail/timer_task.hpp>
```

类型位于 `yggr::time::detail` 命名空间，通常由 `timer_mgr` 创建和调用，不需要应用代码直接管理。

## `base_timer_task<Task_ID>`

抽象基类保存任务 ID，并定义多态执行接口：

| 接口 | 说明 |
| --- | --- |
| `base_timer_task(const task_id_type& id)` | 初始化任务 ID。 |
| `id() const` | 返回保存的 ID 常量引用。 |
| `do_timer() const` | 纯虚函数，执行任务并返回 `yggr::u32` 状态。 |

析构函数为虚函数，确保通过基类指针销毁派生任务时正确调用析构。

## `timer_task<Task_ID, Time_Task_Pak>`

具体任务包装类继承 `base_timer_task<Task_ID>`，保存类型为 `Time_Task_Pak` 的可调用对象。

构造函数接受任务 ID 以及以下任一方式提供的任务包：

- 通过可变参数转发，直接构造任务包；
- 传入任务包右值并移动保存；
- 传入任务包常量左值并复制保存。

该类不可复制。任务包成员声明为 `mutable`，因此 `do_timer() const` 可以调用任务包的非 `const` 调用运算符。

```cpp
yggr::u32 operator()(void)
{
    // 执行或判断定时工作
    return yggr::time::timer_task_state::E_timer_task_ran;
}
```

任务包调用的返回值由 `do_timer()` 直接返回，应符合 `timer_task_state` 约定：

- `E_timer_task_ran`：本轮任务已运行；
- `E_timer_task_not_ran`：本轮未运行但保留；
- `E_timer_task_remove`：请求管理器移除任务。

## 使用关系示例

通常由 `timer_mgr` 通过 `add_task` 创建包装实例；概念上等价于：

```cpp
typedef yggr::time::detail::timer_task<task_id_type, MyTimerTask> task_type;
task_type task(id, MyTimerTask(/* 参数 */));
yggr::u32 result = task.do_timer();
```

## 注意事项

- `Time_Task_Pak` 必须可调用，且调用结果应可转换为 `yggr::u32`。
- 包装对象不可复制；需要在安全队列或管理器之间传递时，应遵循其共享指针和移动构造路径。
- 回调是否执行工作、是否重复以及何时返回移除状态，均由任务包自身决定；此包装类不实现计时周期或到期判断。
- 本文件属于 `detail` 实现接口。业务代码优先通过 `timer_mgr` 添加、移除和调度任务。
