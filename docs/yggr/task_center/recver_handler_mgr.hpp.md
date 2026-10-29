# `recver_handler_mgr.hpp` 中文说明

## 用途

`yggr::task_center::recver_handler_mgr<Mark, Task, Mutex>` 是一种按任务所有者 ID 管理接收处理器的管理器。注册处理器时将其与 `Task::owner_id_type` 关联；推送任务时，根据任务的 `owner_id()` 查找对应处理器并执行。

```cpp
#include <yggr/task_center/recver_handler_mgr.hpp>
```

该管理器不维护任务队列，也不负责链接到其他管理器。其核心职责是保存和调用接收处理器。

## 模板参数

```cpp
template<typename Mark, typename Task, typename Mutex = void>
class recver_handler_mgr;
```

| 参数 | 说明 |
| --- | --- |
| `Mark` | 标记类型，作为管理器类型的一部分；本实现不直接使用其值。 |
| `Task` | 任务类型，必须提供 `owner_id_type` 类型别名及 `owner_id()` 成员函数。 |
| `Mutex` | 传递给 `safe_unordered_map` 的互斥类型，默认是 `void`，由安全容器按其约定处理默认配置。 |

该类私有继承 `nonable::noncopyable` 和 `nonable::nonmoveable`，因此管理器不可复制、不可移动。

## 类型别名

| 别名 | 定义或用途 |
| --- | --- |
| `mark_type` | 对应模板参数 `Mark`。 |
| `task_type` | 对应模板参数 `Task`。 |
| `owner_id_type` | `Task::owner_id_type`，处理器映射的键类型。 |
| `init_type` | 内部初始化标签 `recver_handler_mgr_tag_init`。 |
| `init_tag_type` | `task_center::type_traits::nonmust_need_init`。 |

管理器提供默认构造函数和接受 `init_type` 的构造函数。两个构造函数均不额外初始化接收器状态。

## 接收器注册与移除

### 注册

```cpp
template<typename Handler>
bool hold_recv(const owner_id_type& id, const Handler& handler);
```

为 `id` 创建接收器，并将其插入内部映射。接收器在处理任务时执行：

```cpp
handler(true, task);
```

返回值表示 ID 有效、接收器创建成功且映射插入成功。相同 ID 已经存在时，映射插入不会替换原有处理器，操作会失败并返回 `false`。接口使用断言检查 ID；调用方应传入有效 ID。

### 移除

```cpp
bool stop_recv(const owner_id_type& id);
```

删除指定所有者 ID 对应的接收器。返回值表示 ID 有效且映射中存在并删除了该项。该接口同样通过断言检查传入 ID。

## 任务分发

```cpp
bool push(const task_type& val) const;
```

`push()` 从任务读取 `val.owner_id()`，在接收器映射中查找该 ID：

- 找到有效处理器时，调用 `handler(true, val)` 并返回 `true`。
- 没有对应接收器时返回 `false`。

`push()` 只负责向已注册接收器分发，不会保存或重试任务。

## 状态与清理

```cpp
bool empty() const;
void clear();
```

- `empty()` 判断接收器映射是否为空。
- `clear()` 清空所有接收器。

析构函数通过断言要求映射为空。因此销毁管理器前，应先调用 `stop_recv()` 移除接收器，或调用 `clear()` 清空映射。

## 不支持的操作

```cpp
static bool need_link();

template<typename Handler>
void link(const Handler& handler);

void drop_link();
```

- `need_link()` 固定返回 `false`，表示此管理器不需要链接。
- `link()` 和 `drop_link()` 是不支持的操作；其实现通过 `assert(false)` 表明误调用。在禁用断言的构建中不要依赖这些函数执行任何操作。

以下任务清理接口当前为空实现，不会删除接收器或改变任务状态：

```cpp
template<typename IDs>
void drop_task(const IDs& ids);

template<typename IDs, typename KeyGetter>
void drop_task(const IDs& ids, const KeyGetter& key_getter);

void clear_tasks();
```

## 使用示例

以下示例展示注册、按任务所有者 ID 分发和清理的基本顺序。`task_type` 需由应用提供 `owner_id_type` 和 `owner_id()`：

```cpp
struct recv_mark {};

// task_type 和 owner_id_type 由应用定义。
using manager_type =
    yggr::task_center::recver_handler_mgr<recv_mark, task_type>;

void example(const typename manager_type::owner_id_type& owner,
             const task_type& task)
{
    manager_type manager;

    manager.hold_recv(owner, [](bool accepted, const task_type& value) {
        // 处理 value；accepted 在此管理器中为 true。
    });

    manager.push(task);
    manager.stop_recv(owner);
}
```

示例中的处理器必须可按 `void(bool, const task_type&)` 的形式调用。实际任务类型及所有者 ID 类型应替换为项目使用的定义。
