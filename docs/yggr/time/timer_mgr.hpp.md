# `timer_mgr.hpp` 中文说明

## 用途

本头文件定义 `yggr::time::timer_mgr`，在后台线程中轮询并执行定时任务。管理器将任务加入安全队列，再把任务同步到内部任务表；任务回调返回状态值，以决定本轮是否执行成功、暂不执行或从任务表移除。

```cpp
#include <yggr/time/timer_mgr.hpp>
```

## 模板参数与类型

```cpp
template<
    typename Task_ID,
    typename Thread_Config = yggr::thread::boost_thread_config_type,
    typename Inner_Process_ID = void,
    std::size_t nid = 0>
class timer_mgr;
```

| 参数 | 含义 |
| --- | --- |
| `Task_ID` | 定时任务 ID 类型；用于生成、查找和移除任务。 |
| `Thread_Config` | 线程配置，提供线程、线程组和时间类型；默认使用 `boost_thread_config_type`。 |
| `Inner_Process_ID` | 内部进程 ID 类型。为 `void` 时使用普通管理器；指定其他类型时启用按进程 ID 分组的共享管理器。 |
| `nid` | 保留在模板接口中的 `std::size_t` 参数。 |

管理器的等待间隔类型为 `Thread_Config::time_type`。默认构造使用线程配置提供的默认睡眠时间，也可以通过构造函数传入轮询间隔。

## 任务回调约定

添加的任务包必须可调用，且调用结果应可转换为 `yggr::u32`。返回值使用 `timer_task_state` 中的状态：

| 状态 | 含义 |
| --- | --- |
| `E_timer_task_ran` | 本次任务执行；增加本轮执行计数。 |
| `E_timer_task_not_ran` | 本轮不执行；保留任务。 |
| `E_timer_task_remove` | 从管理器任务表中移除任务。 |

返回未知状态会在调试构建中触发断言，并将该任务安排移除。任务的触发条件和重复周期由回调自身决定；管理器负责轮询调用，而不替任务包解释到期时间。

## 普通管理器

当 `Inner_Process_ID` 为 `void` 时，使用普通管理器特化：

```cpp
yggr::time::timer_mgr<task_id_type> mgr;
```

主要接口：

| 接口 | 说明 |
| --- | --- |
| `add_task<Timer_Task_Pak>(args...)` | 用给定参数构造任务包，生成任务 ID 并排入待处理队列。也可传入任务包的左值或右值。添加失败时返回默认构造的 `task_id_type`。 |
| `remove_task(id)` | 将 ID 排入待移除队列；实际从任务表移除发生在管理线程下一轮同步时。 |
| `exit_task(id)` | 从待添加队列、待移除队列和任务表中立即清理该 ID。 |
| `clear()` | 清空待添加、待移除和当前任务表。 |
| `start()` | 启动后台轮询线程；已运行时再次调用不重复启动。 |
| `stop()` | 停止轮询并等待线程退出。 |
| `join()` | 等待管理线程退出。 |
| `is_running()` | 查询后台线程运行状态。 |
| `get_delegate()` | 返回指向管理器自身的指针，便于与共享管理器接口统一使用。 |

## 按进程 ID 共享的管理器

指定 `Inner_Process_ID` 后，管理器通过 `get_delegate(id)` 为每个进程 ID 提供一个委托对象。委托对象拥有自己的待添加/待移除队列，任务由同一个管理线程轮询执行。

```cpp
yggr::time::timer_mgr<task_id_type, thread_config, process_id_type> mgr;

mgr.start();
auto delegate = mgr.get_delegate(process_id);
task_id_type id = delegate->add_task<MyTimerTask>(/* 构造参数 */);
delegate->remove_task(id);
```

委托对象主要接口：

- `add_task<Timer_Task_Pak>(args...)`：向该委托的队列添加任务并返回任务 ID。
- `remove_task(id)`：排队请求移除任务。
- `exit_task(id)`：从委托队列以及管理器任务表中清理该任务。
- `clear()`：清空委托队列、从管理器注销该委托，并清理该委托拥有的任务。

通过 `get_delegate(id)` 获取的委托由管理器持有共享指针。模块或进程退出时应显式调用委托的 `clear()` 释放登记关系和它拥有的任务；管理器析构时会断言内部任务表为空。停止管理器线程和清理委托任务是不同操作，销毁前应分别完成。

## 执行流程与线程行为

后台线程循环执行以下工作：

1. 将待添加任务并入活动任务表，并处理待移除 ID。
2. 遍历活动任务并调用任务包。
3. 按返回状态保留或移除任务。
4. 若本轮没有任务报告 `E_timer_task_ran`，让出线程并按轮询间隔休眠；若有任务报告已运行，则继续下一轮。

因此，持续返回 `E_timer_task_ran` 的任务会使管理器连续轮询，不会触发该轮的休眠。回调应根据自身调度逻辑合理返回状态，避免不必要的忙循环。若启用 `YGGR_USE_SEH` 且未定义 `YGGR_TIMER_TASK_NOT_USE_SEH`，执行任务的路径会使用项目的 SEH 安全调用包装。

## 使用示例

下面的任务包在每次调用时报告已运行；实际应用通常应在任务包内部判断到期时间，并在未到期时返回 `E_timer_task_not_ran`：

```cpp
struct MyTimerTask
{
    yggr::u32 operator()(void) const
    {
        // 执行到期工作
        return yggr::time::timer_task_state::E_timer_task_ran;
    }
};

typedef yggr::time::timer_mgr<unsigned long long> timer_mgr_type;

timer_mgr_type mgr;
timer_mgr_type::task_id_type id = mgr.add_task<MyTimerTask>();
mgr.start();

mgr.remove_task(id);
mgr.stop();
```

## 注意事项

- `remove_task()` 是异步排队移除；如果需要直接清理队列和任务表，使用 `exit_task()`。调用这些操作时仍应协调任务正在执行的并发访问。
- 应在管理器析构前调用 `stop()`，确保后台线程已退出。普通管理器的基类析构会断言管理器不处于运行状态。
- 使用共享管理器时，应在所属模块退出时对委托调用 `clear()`，并在销毁管理器前确认任务表已清空。
- 当前委托类中，针对任务包左值和右值的两个 `add_task` 重载将 `timer_task` 的第一个模板实参写为任务包类型，而不是管理器的 `task_id_type`；这与可变参数重载的写法不一致。使用这些重载前应核对具体任务包与 ID 类型是否兼容。
- 若任务创建或队列插入失败，接口以默认构造的 ID 表示失败；调用方应按项目使用的 ID 约定检查返回值。
