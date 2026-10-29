# `base_timer_mgr.hpp` 中文说明

## 用途

本头文件定义内部基类 `yggr::time::detail::base_timer_mgr<Task_ID, Thread_Config>`，为 `timer_mgr` 提供共享的线程状态、任务表、轮询间隔和任务 ID 生成逻辑。它属于实现细节，通常通过公开的 `yggr/time/timer_mgr.hpp` 使用定时器管理功能，而不直接实例化该基类。

```cpp
#include <yggr/time/detail/base_timer_mgr.hpp>
```

## 模板参数与类型

| 参数或别名 | 含义 |
| --- | --- |
| `Task_ID` / `task_id_type` | 定时任务的 ID 类型。 |
| `Thread_Config` / `thread_config_type` | 线程配置，提供线程类型、线程组类型和时间类型。 |
| `task_state_type` | 定时任务回调状态类型 `timer_task_state`。 |
| `time_type` | `Thread_Config::time_type`，用于表示管理器轮询休眠间隔。 |

内部线程类型根据 `Thread_Config` 的 `thread_type` 和 `thread_group_type` 定义，并通过 `thread::this_thread` 执行线程让步或休眠。

## 内部任务结构

- `_task_map` 是按任务 ID 索引的安全映射，保存指向 `base_timer_task` 的共享指针。
- `timer_task_queue_type` 和 `timer_task_id_queue_type` 是用于缓冲待添加任务及待移除 ID 的安全队列类型，供派生管理器实现异步队列处理。
- `_tm_step` 是线程安全包装的轮询间隔。
- `_brun` 是原子运行标记，`_trd_group` 保存管理器的工作线程组。

这些队列类型和字段是受保护的实现成员，供 `timer_mgr` 等派生类协调添加、移除和执行任务。

## 生命周期与线程控制

```cpp
base_timer_mgr();
explicit base_timer_mgr(const time_type& tm_step);

bool is_running() const;
void join();
void stop();
```

- 默认构造将运行状态设为停止，并以 `Thread_Config` 提供的默认秒和纳秒值初始化轮询间隔。
- 带参构造使用调用方指定的 `tm_step`。
- `is_running()` 读取原子运行标记。
- `join()` 等待线程组中的线程结束。
- `stop()` 通过原子比较交换请求停止；若运行标记已不是运行状态，则直接返回，否则等待线程组退出。
- 析构函数断言管理器已停止。该基类不可复制、不可移动。

线程启动由派生管理器负责；基类本身不提供 `start()`。

## 受保护的任务操作

| 函数 | 说明 |
| --- | --- |
| `pro_add_task(ptask)` | 按任务对象的 `id()` 将共享任务指针插入 `_task_map`。 |
| `pro_remove_task(id)` | 从 `_task_map` 删除指定 ID。 |
| `gen_id()` | 通过静态 `ids::id_generator<Task_ID>` 生成任务 ID。 |
| `handler_equal_id_of_task(task, id)` | 判断任务指针有效且其 ID 与给定 ID 相等。 |
| `handler_equal_id_of_task_id(task_id, id)` | 比较两个任务 ID 是否相等。 |

添加和移除任务的公开语义由派生类决定；此基类仅提供操作底层任务表的受保护接口。

## 注意事项

- 这是 `detail` 命名空间中的基类，不是完整可用的任务管理器；应优先使用 `timer_mgr` 的公开接口。
- 销毁对象前必须确保运行线程已停止并退出，否则析构断言会失败。
- `stop()` 只负责请求停止并等待线程，不会清空 `_task_map` 或派生类的待处理队列。
- `Task_ID` 和 `Thread_Config` 必须满足 ID 生成器及线程配置所需的类型要求。
