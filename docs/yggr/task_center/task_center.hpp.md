# `task_center.hpp` 中文说明

## 用途

`yggr::task_center::task_center<Task_Creator, Cdt_Saver, Rst_Saver>` 将任务创建、保存、提取和接收处理器管理封装在一个接口中。它根据 `Mark` 类型选择条件任务保存器或结果任务保存器，并将对应操作转发给所选保存器。

```cpp
#include <yggr/task_center/task_center.hpp>
```

## 模板参数

```cpp
template<typename Task_Creator, typename Cdt_Saver, typename Rst_Saver>
class task_center;
```

| 参数 | 说明 |
| --- | --- |
| `Task_Creator` | 任务创建器，需提供 `task_type` 类型别名，并能根据发送参数或已有任务创建任务。 |
| `Cdt_Saver` | 条件任务保存器，需提供任务、标记及初始化相关类型，以及本类转发调用的保存器操作。 |
| `Rst_Saver` | 结果任务保存器，接口要求与 `Cdt_Saver` 对应；还需支持接收器注册与移除接口。 |

该类以 `protected` 方式继承两个保存器，并通过自己的公开成员函数提供统一访问接口。

## 关键类型与编译期约束

| 别名 | 来源 |
| --- | --- |
| `task_creator_type`、`task_type` | `Task_Creator` 及其 `task_type`。 |
| `owner_id_type` | `task_type::owner_id_type`。 |
| `cdt_saver_type`、`rst_saver_type` | 对应的保存器模板参数。 |
| `cdt_value_type`、`rst_value_type` | 对应保存器的 `task_type`。 |
| `cdt_mark_type`、`rst_mark_type` | 对应保存器的 `mark_type`。 |
| `cdt_saver_init_type`、`rst_saver_init_type` | 对应保存器的 `init_type`。 |

编译期断言要求：

- 条件和结果保存器的 `mark_type` 不同。
- `Cdt_Saver::task_type` 与 `Task_Creator::task_type` 相同。
- 默认构造函数要求两个保存器的 `init_tag_type` 都是 `nonmust_need_init`。

结果保存器的任务类型需满足本类实际调用所需的接口和类型兼容性。

## 构造与生命周期

```cpp
task_center();
task_center(const cdt_saver_init_type& cdt_init,
            const rst_saver_init_type& rst_init);
~task_center();
```

- 默认构造仅适用于声明为无需显式初始化的保存器；编译期会检查其 `init_tag_type`。
- 带初始化参数的构造函数分别用初始化对象构造两个保存器，然后调用内部初始化逻辑。若保存器的 `need_link()` 返回 `true`，会建立保存器之间的回调链接。
- 析构时会对需要链接的保存器调用 `drop_link()`。

使用带初始化参数的构造方式时，应保证保存器支持对应的初始化构造和链接接口。

## 标记与保存器选择

本类根据 `Mark` 选择目标保存器：

| 标记 | 选择的保存器 |
| --- | --- |
| `type_traits::mark_saver_condition` | `Cdt_Saver` |
| `type_traits::mark_saver_result` | `Rst_Saver` |

其他未特化的标记不能通过该选择器解析。`send`、`empty`、`get_task` 和 `drop_task` 均使用此规则选择保存器。

## 发送任务

### 根据参数创建任务

```cpp
template<typename Mark, typename... Args>
void send(Args&&... args);
```

此重载创建 `Task_Creator`，将参数转发给任务创建器生成任务。任务非空时，将任务移动并推送到由 `Mark` 选出的保存器；空任务不会推送。

### 发送已有任务

```cpp
template<typename Mark>
void send(task_type& task);
```

此重载通过任务创建器处理已有任务；创建结果非空时，将其推送到对应保存器。此接口接收非常量左值引用。

不支持可变参数模板的配置下，第一种形式由预处理器生成固定参数数量范围内的重载。

## 接收器管理

```cpp
template<typename Handler>
bool hold_recv(const owner_id_type& id, const Handler& handler);

bool stop_recv(const owner_id_type& id);
```

这两个接口直接转发到 `Rst_Saver`：

- `hold_recv()` 将接收器与所有者 ID 关联。
- `stop_recv()` 移除该 ID 对应的接收器。

接收器实际如何调用、重复 ID 如何处理，由结果保存器实现决定。

## 查询、提取和删除任务

```cpp
template<typename Mark>
bool empty() const;

template<typename Mark, typename Handler>
bool get_task(const Handler& handler);

template<typename Mark, typename IDs>
void drop_task(const IDs& ids);

template<typename Mark, typename IDs, typename KeyGetter>
void drop_task(const IDs& ids, const KeyGetter& key_getter);
```

- `empty<Mark>()` 查询所选保存器是否为空。
- `get_task<Mark>(handler)` 调用所选保存器的 `pop(handler)`，返回值由该操作给出。
- 两个 `drop_task<Mark>()` 重载将任务删除请求转发给所选保存器；第二个重载同时传递键提取器。

具体的任务匹配方式、提取语义和删除规则由保存器定义。

## 全局清理

```cpp
void drop_all_tasks();
void clear();
```

- `drop_all_tasks()` 分别调用两个保存器的 `clear_tasks()`，用于清除任务。
- `clear()` 分别调用两个保存器的 `clear()`，其具体清理范围由保存器实现定义，可能比清除任务更广。

## 使用流程示意

保存器和任务创建器由应用按所需策略提供。一般使用流程如下：

```cpp
// task_center_type 由 Task_Creator、Cdt_Saver 和 Rst_Saver 组成。
task_center_type center(cdt_init, rst_init);

center.send<yggr::task_center::type_traits::mark_saver_condition>(
    /* 创建任务所需参数 */);

const bool no_pending_tasks =
    center.empty<yggr::task_center::type_traits::mark_saver_condition>();

center.drop_all_tasks();
```

示例中的 `task_center_type`、初始化对象和创建任务所需参数应替换为项目实际定义。若使用默认构造，则两个保存器必须满足默认构造所要求的 `nonmust_need_init` 约束。
