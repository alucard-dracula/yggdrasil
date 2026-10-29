# `action_table.hpp` 中文说明

## 用途

`yggr::thread::action_table<Task_Type, Work_Runner, ID_Parser, Mutex>` 是工作运行器使用的任务动作注册表。它根据任务 ID 解析器生成的注册 ID 查找计算器，并将任务中的实际数据、所有者信息和运行器传给对应处理函数。

```cpp
#include <yggr/thread/action_table.hpp>
```

注册表以 `safe_unordered_map` 保存“注册 ID -> 计算器”映射。任务执行时，若找不到对应计算器则不执行处理函数。

## 模板参数

| 参数 | 说明 |
| --- | --- |
| `Task_Type` | 被执行任务的类型。实际数据通过 `task_real_data_getter<Task_Type>` 获取。 |
| `Work_Runner` | 工作运行器类型，作为计算器调用处理函数时的运行器参数。 |
| `ID_Parser` | 解析任务注册 ID 和任务 ID 的策略类型，需提供 `id_type`、`get_reg_id<T>()` 和 `get_task_id<Task_Type>(task)`。 |
| `Mutex` | 传递给安全映射的互斥类型，默认值为 `void`。 |

公开类型别名为 `task_type`、`task_real_data_getter_type`、`work_runner_type`、`id_parser_type` 和 `tpl_arg_mutex_type`。

## 注册计算器

### 按数据类型注册

```cpp
template<typename Real_Data_Type, typename Recv_Handler, typename Handler>
bool register_calculator(const Recv_Handler& recv_handler,
                         const Handler& handler);
```

使用 `ID_Parser::get_reg_id<Real_Data_Type>()` 生成映射键。任务执行时，计算器通过 `task_real_data_getter<Task_Type>` 取得 `Real_Data_Type` 数据，并调用：

```cpp
handler(task.owner_info(), boost::ref(real_data), &runner, recv_handler);
```

其中 `recv_handler` 会作为回调/返回处理器传递给 `handler`。

### 使用独立数据容器类型注册

```cpp
template<typename Real_Data_Type, typename Container,
         typename Recv_Handler, typename Handler>
bool register_calculator_of_container(const Recv_Handler& recv_handler,
                                      const Handler& handler);
```

此重载以 `Real_Data_Type` 生成注册 ID，但计算器尝试从任务中取得的实际对象类型为 `Container`。适用于注册键所代表的逻辑数据类型与任务中保存的具体包装/容器类型不同的场景。尽管模板参数名为 `Container`，它必须是 `task_real_data_getter<Task_Type>` 能够实际取出的数据类型。

### 按显式注册 ID 注册

```cpp
template<typename Container, typename Recv_Handler, typename Handler>
bool register_calculator(const calculator_id_type& id,
                         const Recv_Handler& recv_handler,
                         const Handler& handler);
```

此重载使用调用方提供的 `id` 作为映射键，并以 `Container` 作为任务中读取的实际数据类型。

以上注册函数在构造计算器后向映射插入键值；返回值表示计算器创建并插入是否成功。已有相同键时，插入不会替换原计算器，调用返回 `false`。

## SEH 异常处理

定义 `YGGR_USE_SEH` 时，额外提供接受 `SEH_Handler` 的注册重载。计算器会在安全调用包装中执行实际处理：

- 成功执行时调用常规 `Handler`。
- 安全调用报告失败时，尝试取得对应的常量数据指针，并调用 `SEH_Handler(task.owner_info(), data_ptr)`。

SEH 重载同样支持按数据类型、按独立容器类型和按显式注册 ID 注册。未定义 `YGGR_USE_SEH` 时，不提供这些带异常处理器的重载。

未启用 `YGGR_USE_SEH` 时，处理函数直接调用；其异常处理行为由调用方及工作运行器决定。

## 注销

```cpp
template<typename Real_Data_Type>
bool unregister_calculator();

bool unregister_calculator(const calculator_id_type& id);
```

- 模板重载使用 `ID_Parser::get_reg_id<Real_Data_Type>()` 生成键。
- 显式 ID 重载直接使用传入的 `id`。
- 返回值表示映射中是否存在并删除了对应计算器。

## 执行任务

```cpp
void run_task(work_runner_type& runner, task_type& task) const;
```

执行流程：

1. 通过 `ID_Parser::get_task_id<task_type>(task)` 取得任务对应的注册键。
2. 从线程安全映射中查找计算器。
3. 找到非空计算器时调用其 `run_handler(runner, task)`。
4. 未找到计算器时直接返回，不调用处理器。

计算器通过 `task_real_data_getter<task_type>` 获取其注册的数据类型。任务类型需要有适用的 `task_real_data_getter` 特化；数据不存在或类型不匹配时，常规处理函数不会被调用。

## 清空

```cpp
void clear();
```

清空全部注册项。之后调用 `run_task()` 时，所有任务都不会匹配到已注册的计算器，直到重新注册。

## 使用示意

```cpp
typedef yggr::thread::action_table<
    my_task,
    my_work_runner,
    my_action_id_parser
> action_table_type;

action_table_type table;

table.register_calculator<my_payload>(
    recv_handler,
    call_handler);

table.run_task(runner, task);
table.unregister_calculator<my_payload>();
```

示例中的任务、运行器、ID 解析器和处理器由应用提供。`call_handler` 应可接收所有者信息、实际数据引用、运行器指针和接收处理器；注册和注销使用的类型必须对应同一注册键。

## 注意事项

- 注册键由 `ID_Parser` 定义；注册和执行必须遵循同一套映射规则。
- `register_calculator_of_container` 中注册键类型和实际读取类型是两个独立模板参数，使用时需确认二者的对应关系。
- 注册表使用安全无序映射，但实际回调中处理器和任务数据的生命周期仍需满足工作运行器的调用约定。
- `clear()` 会移除所有计算器，不等同于等待正在执行的任务完成。
