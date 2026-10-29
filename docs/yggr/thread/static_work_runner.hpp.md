# `static_work_runner.hpp` 中文说明

## 用途

`yggr::thread::static_work_runner<Task, ID_Parser, Mutex, ActionTable>` 是静态工作运行器。它接收一个任务，按任务 ID 从共享动作表中查找计算器并执行；计算完成后，可通过已链接的处理器将结果任务回传给调用方。

```cpp
#include <yggr/thread/static_work_runner.hpp>
```

该运行器不可复制、不可移动。它不负责维护任务队列；调用方通过 `operator()(task)` 提交任务。

## 模板参数

| 参数 | 说明 |
| --- | --- |
| `Task` | 任务类型，需提供所有者信息、数据元信息和数据容器类型。 |
| `ID_Parser` | 任务/计算器 ID 解析器，传递给动作表用于查找注册项。 |
| `Mutex` | 安全容器的互斥类型，默认 `void`。 |
| `ActionTable` | 动作表模板，默认 `yggr::thread::action_table`。 |

运行器定义 `tag_type` 为 `tag_static_work_runner`，供其他适配器检查该运行器类别。

## 共享信息及计算器管理

### 创建共享信息

```cpp
static shared_info_ptr_type create_work_runner_shared_info();
```

创建共享信息对象。一个或多个运行器可持有该信息，以共享内部动作表。

### `shared_info` 接口

```cpp
void run_task(work_runner_type& runner, task_type& task) const;

template<typename Calculator>
void register_calculator(Calculator& calculator);

template<typename Calculator>
void unregister_calculator(Calculator& calculator);

void clear();
```

- `run_task()` 将任务委托给动作表执行。
- `register_calculator()` 将计算器注册到动作表，并要求计算器所有者信息容器的元素类型与 `Task::owner_info_type` 一致。
- `unregister_calculator()` 将注销操作转发给计算器。
- `clear()` 清空共享动作表中的计算器。

运行器通过 `safe_wrap` 持有共享信息。构造时必须提供有效的共享信息指针；代码用断言检查该指针。

## 构造与初始化

```cpp
static_work_runner(const init_type& shared_info);
```

`init_type` 是共享信息安全包装所使用的共享指针类型。创建方式示意：

```cpp
typedef yggr::thread::static_work_runner<
    my_task,
    my_id_parser
> runner_type;

runner_type::init_type shared =
    runner_type::create_work_runner_shared_info();

runner_type runner(shared);
```

实际任务类型和 ID 解析器由应用提供。

## 提交任务

```cpp
bool operator()(task_type& task);
```

调用时运行器尝试执行非空任务：若能从共享包装中取得共享信息，则由共享动作表查找并运行对应计算器。该接口在执行路径结束后返回 `true`；返回值不表示是否找到匹配的计算器或计算是否成功。

## 动作结果与回传链接

静态运行器的 `need_link()` 固定返回 `true`，表示它需要结果回传处理器。链接接口为：

```cpp
template<typename Handler>
void link(const Handler& handler);

void drop_link();
```

- `link()` 将处理器包装并保存。
- `drop_link()` 清除已保存的回传处理器。
- `clear()` 释放当前运行器持有的共享信息，并清除回传处理器。

计算器成功返回结果时，运行器要求状态为 `runtime_task_type::E_CAL_RESULT`，并为每个 owner 创建结果任务，再调用已链接的处理器。此实现通过断言要求 owner 集合非空且状态为结果类型；如果回传处理器不存在，结果不会被转交。

计算错误非空时，结果回调会调用 `exception::exception::throw_error()` 抛出对应错误。

## 结果接口

```cpp
template<typename Owners>
void fix_result(u32 state,
                const Owners& owners,
                const data_info_type& data_info,
                u32 cal_type,
                task_data_container_type& result);
```

该接口将结果交给内部结果处理逻辑。对于本静态运行器，结果处理逻辑预期 `state` 为结果类型，并通过已链接的处理器向每个 owner 回传结果任务。

## 清理接口

```cpp
void clear_tasks() const;
void clear();
```

- `clear_tasks()` 在当前实现中为空操作；该运行器没有自己的任务队列。
- `clear()` 清除本运行器持有的共享信息引用，并清除回传链接。它不会调用 `shared_info::clear()`，因此不会清空其他运行器仍共享的动作表。

## 使用注意

- 必须先创建并注册计算器，再提交需要该计算器处理的任务。
- 在使用结果回传功能前，应调用 `link()` 设置处理器，并确保其生命周期和调用约定满足使用要求。
- 多个运行器共享同一 `shared_info` 时，清空某个运行器本身不会清除共享动作表；如需清除计算器，应显式对共享信息调用 `clear()`。
- `operator()` 的 `true` 仅表示该接口完成调用流程，不应作为“任务成功执行”的状态判断。
