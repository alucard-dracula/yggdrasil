# `detail/timer_task_state.hpp` 中文说明

## 用途

本头文件定义内部状态类型 `yggr::time::detail::timer_task_state`，用于表示定时任务回调的执行结果。公开头文件 `yggr/time/timer_task_state.hpp` 会将该类型引入 `yggr::time` 命名空间。

```cpp
#include <yggr/time/detail/timer_task_state.hpp>
```

## 状态常量

类型通过 `YGGR_STATIC_CONSTANT` 定义以下 `yggr::u32` 常量：

| 常量 | 值 | 含义 |
| --- | ---: | --- |
| `E_timer_task_state` | `0` | 状态基值，仅供构造其他状态值使用；源码注明不应使用。 |
| `E_timer_task_ran` | `1` | 任务本轮已执行。 |
| `E_timer_task_not_ran` | `2` | 任务本轮未执行，但仍保留在管理器中。 |
| `E_timer_task_remove` | `3` | 请求管理器移除该任务。 |

数值由基值递增得到，调用方应使用具名常量而非依赖硬编码数值。

## 使用示例

```cpp
yggr::u32 operator()(void) const
{
    if(is_due())
    {
        run_task();
        return yggr::time::detail::timer_task_state::E_timer_task_ran;
    }

    return yggr::time::detail::timer_task_state::E_timer_task_not_ran;
}
```

任务不再需要时，可返回 `E_timer_task_remove`。`timer_mgr` 会根据回调返回值决定是否将任务从活动任务表移除；不要将保留用的基值或未定义状态作为回调结果。

## 注意事项

- 该类型位于 `detail` 命名空间，业务代码通常应包含公开头文件并使用 `yggr::time::timer_task_state`。
- 本头文件只声明状态常量，不执行任务，也不直接操作任务管理器。
