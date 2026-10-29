# `timer_task_state.hpp` 中文说明

## 用途

本头文件将内部类型 `yggr::time::detail::timer_task_state` 引入公开命名空间 `yggr::time`，为定时任务回调提供统一的状态常量。

```cpp
#include <yggr/time/timer_task_state.hpp>
```

## 状态常量

| 常量 | 值 | 含义 |
| --- | ---: | --- |
| `E_timer_task_state` | `0` | 状态基值，仅供计算其他状态值使用，不应作为回调结果返回。 |
| `E_timer_task_ran` | `1` | 任务本轮已执行。 |
| `E_timer_task_not_ran` | `2` | 任务本轮未执行，但继续保留。 |
| `E_timer_task_remove` | `3` | 请求管理器移除该任务。 |

状态常量类型为 `yggr::u32`，通过 `YGGR_STATIC_CONSTANT` 声明。该头文件仅提供类型别名和常量，不负责执行任务或改变任务状态。

## 使用示例

定时任务回调应返回上述状态之一，供 `timer_mgr` 决定是否保留或移除任务：

```cpp
yggr::u32 operator()(void) const
{
    if(is_due())
    {
        run_task();
        return yggr::time::timer_task_state::E_timer_task_ran;
    }

    return yggr::time::timer_task_state::E_timer_task_not_ran;
}
```

当任务不再需要时，回调可返回 `E_timer_task_remove`。不要返回保留用的基值 `E_timer_task_state` 或未定义的状态值。
