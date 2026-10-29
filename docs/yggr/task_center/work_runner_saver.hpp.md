# `work_runner_saver.hpp` 中文说明

## 用途

`yggr::task_center::work_runner_saver<Mark, WorkRunner>` 将静态工作运行器包装成任务中心可使用的保存器接口。它继承 `WorkRunner`，并把任务提交、链接和清理操作转发给基类。

```cpp
#include <yggr/task_center/work_runner_saver.hpp>
```

## 模板参数与约束

```cpp
template<typename Mark, typename WorkRunner>
class work_runner_saver;
```

| 参数 | 说明 |
| --- | --- |
| `Mark` | 任务中心使用的保存器标记类型。 |
| `WorkRunner` | 工作运行器类型，必须是静态工作运行器，并提供本包装器所调用的任务、初始化、提交和清理接口。 |

编译期断言要求 `WorkRunner::tag_type` 与 `yggr::thread::type_traits::tag_static_work_runner` 相同。

公开别名包括 `mark_type`、`work_runner_type`、`base_type`、`task_type` 和 `init_type`。其 `init_tag_type` 为 `task_center::type_traits::must_need_init`，表示任务中心默认构造路径不适用，需使用初始化参数构造。

## 构造

```cpp
work_runner_saver();
explicit work_runner_saver(const init_type& init);
```

默认构造函数通过 `assert(false)` 标记为不支持；应传入基类所需的 `init_type`，由该初始化对象构造 `WorkRunner`。

## 任务提交

```cpp
bool push(task_type&& val);
bool push(task_type& val);
```

两个重载均将任务传递给基类的函数调用运算符 `WorkRunner::operator()(task)`，并返回基类给出的结果。右值重载通过可移动引用接收任务；非常量左值重载直接传递任务引用。

该包装器没有实现独立任务队列，任务调度及处理行为由 `WorkRunner` 决定。

## 链接接口

```cpp
static bool need_link();

template<typename Handler>
void link(const Handler& handler);

void drop_link();
```

这些接口直接转发到 `WorkRunner` 对应接口：

- `need_link()` 返回 `WorkRunner::need_link()` 的结果。
- `link(handler)` 将处理器交给基类建立链接。
- `drop_link()` 请求基类移除链接。

链接是否需要建立及其具体语义由工作运行器实现决定。

## 清理与删除任务

```cpp
template<typename IDs>
void drop_task(const IDs& ids);

template<typename IDs, typename KeyGetter>
void drop_task(const IDs& ids, const KeyGetter& key_getter);

void clear_tasks();
void clear();
```

- 两个 `drop_task()` 重载都是空操作，不会根据 ID 或键提取器删除任务。
- `clear_tasks()` 将调用转发给 `WorkRunner::clear_tasks()`。
- `clear()` 将调用转发给 `WorkRunner::clear()`。

不要依赖 `drop_task()` 从该保存器中移除任务；是否可取消或清理任务取决于 `WorkRunner` 支持的其他接口。

## 与 `task_center` 的配合

该适配器满足任务中心保存器所需的标记、任务类型、初始化和任务提交接口。`task_center` 可按 `Mark` 选择此保存器并调用 `push()`；任务实际如何进入静态工作运行器由基类实现负责。

## 使用示意

```cpp
typedef yggr::task_center::work_runner_saver<
    my_task_mark,
    my_static_work_runner
> runner_saver_type;

runner_saver_type saver(runner_saver_type::init_type(/* runner 初始化参数 */));
saver.push(task);
```

`my_static_work_runner` 必须满足静态工作运行器标签约束，并提供匹配的 `init_type`、任务类型及相关基类接口。示例中的初始化参数需替换为具体运行器所需内容。
