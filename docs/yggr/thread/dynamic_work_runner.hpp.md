# `dynamic_work_runner.hpp` 中文说明

## 用途

`yggr::thread::dynamic_work_runner<Task_Center, ID_Parser, Mutex, ActionTable>` 是动态工作运行器。它从任务中心获取条件任务，按注册 ID 查找并执行对应计算器；支持快速/慢速计算任务、结果回传，以及按任务 ID、所有者信息或数据元信息请求取消待执行任务。

```cpp
#include <yggr/thread/dynamic_work_runner.hpp>
```

该类不可复制、不可移动。运行器对象通常由调度循环反复调用其 `operator()()` 驱动。

## 模板参数

| 参数 | 说明 |
| --- | --- |
| `Task_Center` | 任务中心类型，需提供 `task_type`、条件/结果保存器标记及 `get_task()`、`send()` 等接口。 |
| `ID_Parser` | 动作 ID 解析器，传给 `ActionTable` 用于计算器注册 ID 和任务查找 ID。 |
| `Mutex` | 安全容器使用的互斥类型，默认 `void`。 |
| `ActionTable` | 动作表模板，默认是 `yggr::thread::action_table`。 |

运行器从任务类型导出 `task_type`、`task_id_type`、`owner_info_type`、`data_info_type` 和 `task_data_container_type` 等别名。

## 共享信息与计算器注册

### 创建共享信息

```cpp
static shared_info_ptr_type
create_work_runner_shared_info(size_type slow_runner_size = 0);
```

创建并返回多个工作运行器可共享的 `shared_info`。参数指定允许并行运行慢速任务的数量，默认 `0` 表示未启用慢速任务槽位。

共享信息内部包含动作表和慢速运行计数。慢速计数通过原子变量管理；任务执行前占用槽位，运行器销毁或清理时会释放其共享信息引用。

### 注册和注销计算器

`shared_info` 提供模板接口：

```cpp
template<typename Calculator>
void register_calculator(Calculator& calculator);

template<typename Calculator>
void unregister_calculator(Calculator& calculator);

void clear();
```

- `register_calculator()` 要求计算器的所有者信息容器元素类型与运行器 `owner_info_type` 相同，并将计算器对象注册到共享动作表。
- `unregister_calculator()` 将注销请求转发给计算器。
- `clear()` 清空共享动作表中的全部计算器。

多个运行器实例若共享同一个 `shared_info`，也共享其中的动作表和慢速任务计数。

## 构造与生命周期

```cpp
dynamic_work_runner(const task_center_type& task_center,
                    const shared_info_ptr_type& shared_info);
~dynamic_work_runner();
```

构造时保存任务中心的引用和共享信息指针。传入的任务中心必须比运行器存活更久；共享信息可由多个运行器共同持有。

析构函数调用 `clear()`：释放当前运行器持有的共享信息引用，并清除本运行器中的待执行任务及取消请求队列。

## 驱动任务执行

```cpp
bool operator()();
```

每次调用会先处理排队的取消请求，然后尝试执行任务：

1. 若共享信息存在且有可用慢速任务槽位，则尝试从本地慢速任务队列取出一个任务并运行。
2. 如果没有执行慢速任务，则尝试从任务中心提取一个条件任务。
3. 返回值表示本次是否从任务中心取得条件任务，或取出并处理了本地慢速任务；无任务时返回 `false`。

快速任务会直接进入动作表执行。慢速任务在没有可用槽位时暂存在本地队列，之后由运行器再次调用时尝试调度。

任务为空或共享信息指针不存在时，不会调用动作表执行任务。

## 动作执行与结果回传

共享动作表根据任务 ID 查找计算器并执行。任务的实际数据通过 `task_real_data_getter` 获取，调用处理器时会传入所有者信息、数据引用、运行器指针和结果回调。

### `fix_result()`

```cpp
template<typename Owners>
void fix_result(u32 state,
                const Owners& owners,
                const data_info_type& data_info,
                u32 cal_type,
                task_data_container_type& result);
```

根据 `state` 将结果发送回任务中心：

- `runtime_task_type::E_CAL_CONDITION`：对每个所有者发送条件任务。
- `runtime_task_type::E_CAL_RESULT`：对每个所有者发送结果任务。
- 所有者集合为空时不发送。
- 未识别的 `state` 不会进入上述两种发送分支。

发送内容包括所有者、数据元信息、计算类型和结果数据容器。

当启用 `YGGR_USE_SEH` 时，动作表可注册异常处理器。计算器错误回调收到非空错误后，会通过 `exception::exception::throw_error()` 抛出对应错误；正常结果则进入 `fix_result()`。

## 取消待执行任务

```cpp
template<typename Tag, typename ID>
void post_kill_task(ID&& id);

template<typename Tag, typename ID>
void post_kill_task(const ID& id);
```

该接口不立即删除任务，而是将筛选值加入对应的线程安全队列。`Tag` 决定按哪种任务索引处理请求：

| 标签 | 匹配字段 |
| --- | --- |
| `tag_task_id_type` | 任务 ID。 |
| `tag_task_owner_info_type` | 所有者信息。 |
| `tag_task_data_info_type` | 数据元信息索引。 |

取消请求在下一次 `operator()()` 驱动时处理；它针对本运行器本地待执行队列中的任务，不会自动从任务中心的保存器中撤销任务，也不会中断已经开始执行的任务。每项请求会通过对应的 MultiIndex 查找匹配项并移出待执行队列。

**接口类型注意：**数据元信息取消队列的元素类型是 `data_info_type`，但当前头文件中 `tag_task_data_info_type` 对应的 `prv_post_kill_task()` 重载参数声明为 `task_id_type`。若两种类型不相同，调用该标签的 `post_kill_task()` 可能无法编译，或需依赖二者之间的转换；使用前应确认具体任务类型和重载匹配情况。

## 队列与清理接口

```cpp
void clear_tasks();
void clear();
```

- `clear_tasks()` 清空本地慢速待执行任务和所有取消请求队列。
- `clear()` 先释放本实例持有的共享信息指针，再调用 `clear_tasks()`。

运行器内部任务列表维护多个索引：按顺序排列任务、按任务 ID 唯一索引，并按所有者信息和数据元信息建立非唯一索引。

## 使用示意

```cpp
typedef yggr::thread::dynamic_work_runner<
    my_task_center,
    my_id_parser
> runner_type;

runner_type::shared_info_ptr_type shared =
    runner_type::create_work_runner_shared_info(2);

runner_type runner(task_center, shared);

// 调度循环中反复调用：
bool did_work = runner();
```

实际任务中心和 ID 解析器类型应由应用提供。若需要执行慢速任务，创建共享信息时应配置合适的槽位数，并确保运行器调度循环持续调用。

## 注意事项

- 慢速任务槽位由共享信息原子计数。当前执行路径先占用槽位，再尝试从本地慢速任务队列取任务；若队列为空，代码没有回滚该次占用。本文件虽提供原子递减辅助函数，但任务执行和结果回调路径中没有调用它。需核实完整调度流程的计数释放，否则可能耗尽槽位，导致慢速任务无法继续调度。
- `post_kill_task<Tag>()` 是延迟取消请求，不保证任务一定仍在队列中；任务可能已被其他执行路径取出。
- 任务中心以引用方式存储在运行器中，必须保证其生命周期覆盖运行器的整个使用期。
- `clear()` 会使当前实例不再持有共享动作表，但不会主动清空其他仍持有同一 `shared_info` 的运行器所共享的动作表。
