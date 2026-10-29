# `task_saver.hpp` 中文说明

## 用途

`yggr::task_center::task_saver<Mark, Task, Container, Store_OP>` 是基于容器的任务保存器。它使用 `Store_OP` 将任务转换为容器保存的数据，并提供任务入队、取出、删除及清空等操作。

```cpp
#include <yggr/task_center/task_saver.hpp>
```

该保存器不负责接收处理器回调或与其他保存器建立链接。实际可用的任务容器和存储操作由模板参数决定。

## 模板参数

```cpp
template<typename Mark,
         typename Task,
         typename Container,
         typename Store_OP>
class task_saver;
```

| 参数 | 说明 |
| --- | --- |
| `Mark` | 标记类型，用于将保存器与任务中心的条件/结果保存器等角色关联。 |
| `Task` | 保存和取出的任务类型。 |
| `Container` | 底层缓冲容器，需支持本类使用的 `push`、`pop`、`empty`、`size` 和 `clear` 操作。 |
| `Store_OP` | 存储操作策略，需提供 `get_store_data()` 以及相应的 `remove()` 操作。 |

该类私有继承 `nonable::noncopyable` 和 `nonable::nonmoveable`，因此不可复制、不可移动。

## 类型别名与初始化

| 别名 | 定义 |
| --- | --- |
| `mark_type` | `Mark` |
| `task_type` | `Task` |
| `container_type` | `Container` |
| `store_op_type` | `Store_OP` |
| `init_type` | 内部初始化标签 `detail::task_saver_init` |
| `init_tag_type` | `type_traits::nonmust_need_init` |

保存器提供默认构造函数，以及接受 `init_type` 的构造函数。两者均不执行额外初始化；`init_type` 用于满足任务中心的保存器初始化接口约定。

## 保存任务

```cpp
void push(Task&& task);
void push(const Task& task);
```

两个重载都会忽略空任务。非空任务由 `Store_OP::get_store_data()` 转换为容器存储的数据，再推入底层缓冲：

- 右值重载将任务移动给存储操作。
- 常量左值重载将任务以常量引用传给存储操作。

具体存储形式由 `Store_OP` 决定。

## 取出任务

```cpp
template<typename Handler>
bool pop(const Handler& handler);
```

`pop()` 尝试从缓冲区取出一个 `Task`。取出成功后调用 `handler(task)` 并返回 `true`；缓冲为空或无法取出时返回 `false`。取出操作先于处理器回调执行，因此处理器异常退出时，该任务已经从缓冲中移除。

## 查询与删除

```cpp
bool empty() const;
size_type size() const;

template<typename IDs>
void drop_task(const IDs& ids);

template<typename IDs, typename KeyGetter>
void drop_task(const IDs& ids, const KeyGetter& key_getter);
```

- `empty()` 和 `size()` 分别查询底层缓冲是否为空及其当前大小。
- 两个 `drop_task()` 重载创建存储操作对象，并将删除请求委托给 `Store_OP::remove()`。
- 第二个重载同时传入 `key_getter`，由存储策略决定如何从缓冲数据中提取匹配键。

删除匹配规则及 `IDs`、`KeyGetter` 的具体要求由 `Store_OP` 实现定义。

## 清空

```cpp
void clear_tasks();
void clear();
```

两者在本实现中都直接调用底层容器的 `clear()`，清空当前缓冲中的全部任务。

## 链接接口

```cpp
static bool need_link();

template<typename Handler>
void link(const Handler& handler);

void drop_link();
```

- `need_link()` 固定返回 `false`，表示此保存器不需要链接。
- `link()` 和 `drop_link()` 不受支持，其实现通过 `assert(false)` 标记误用；调用方不应依赖它们执行任何操作。

## 与 `task_center` 的关系

`task_center` 根据标记类型选择保存器，并将创建的非空任务通过 `push()` 保存。之后可以通过 `empty()` 查询、通过 `get_task()` 间接调用 `pop()` 取出任务，也可以使用 `drop_task()` 删除匹配任务。队列结构和任务匹配方式分别由 `Container` 与 `Store_OP` 提供。
