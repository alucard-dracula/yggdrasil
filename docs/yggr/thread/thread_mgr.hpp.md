# `thread_mgr.hpp` 中文说明

## 用途

`yggr::thread::thread_mgr<Work_Thread, Mutex>` 管理一组工作线程对象，提供线程添加、启动、查询、停止、中断及等待结束等操作。

```cpp
#include <yggr/thread/thread_mgr.hpp>
```

线程对象以 `shared_ptr` 保存于内部向量中。管理器不可复制；默认使用 `boost::shared_mutex` 保护线程集合，可通过模板参数指定其他互斥量配置。

## 模板参数与类型

| 参数/别名 | 说明 |
| --- | --- |
| `Work_Thread` | 工作线程类型，需提供线程配置类型、ID 类型及启动/停止/等待等接口。 |
| `Mutex` | 保护线程集合的互斥类型，默认 `boost::shared_mutex`。 |
| `work_thread_type` | `Work_Thread`。 |
| `thread_config_type` | `Work_Thread::thread_config_type`。 |
| `time_type` | 线程配置中的时间类型。 |
| `id_type` | 工作线程的 ID 类型。 |
| `work_thread_ptr_type` | 工作线程的共享指针类型。 |
| `trd_vector_type` | 工作线程指针容器类型。 |

读操作使用 `mutex_def_helper<Mutex>::read_lock_type`，写操作使用 `write_lock_type`。

## 添加与查询线程

```cpp
thread_mgr& add_thread();
thread_mgr& add_thread(const time_type& tm_step);

size_type size() const;
id_type get_id(size_type idx) const;

template<typename Container>
Container& get_id(Container& cont) const;
```

- `add_thread()` 创建默认配置的工作线程并加入管理器。
- `add_thread(tm_step)` 使用指定时间步长构造工作线程。
- 两个添加接口返回管理器自身，可链式调用。
- `size()` 返回当前线程对象数量。
- `get_id(idx)` 返回指定位置线程的 ID；索引越界或线程指针为空时返回默认构造的 `id_type`。
- 批量 `get_id(cont)` 将所有非空线程的 ID 插入到调用方提供的容器末尾，并返回该容器的引用。

## 启动线程

### 启动一个可用线程

```cpp
template<typename Runner, typename... Args>
size_type startup(Args&&... args);

template<typename Runner>
size_type startup(shared_ptr<Runner> runner);
```

从线程集合中查找第一个非空且处于停止状态的工作线程，使用 `Runner` 启动它。启动参数会转发给工作线程；也可以传入 runner 共享指针。

### 按索引启动

```cpp
template<typename Runner, typename... Args>
size_type startup(size_type idx, Args&&... args);

template<typename Runner, typename... Args>
size_type startup(size_type idx,
                  shared_ptr<Runner> runner,
                  Args&&... args);
```

只尝试启动指定索引处处于停止状态的线程。索引无效、指针为空或线程未停止时不启动。

### 启动所有可用线程

```cpp
template<typename Runner, typename... Args>
size_type startup_all(Args&&... args);

template<typename Runner>
size_type startup_all(shared_ptr<Runner> runner);
```

遍历线程集合，对所有非空且已停止的工作线程启动 runner，并返回成功启动的线程数量。

上述启动接口返回 `size_type`：单线程启动成功返回 `1`，未启动返回 `0`；`startup_all()` 返回成功启动的数量。未启用 C++11 可变参数模板时，参数重载由预处理器按项目支持的参数数量生成。

## 等待线程结束

```cpp
void join(size_type idx);
void join_all();
```

- `join(idx)` 等待指定索引处的非空线程结束。
- `join_all()` 依次等待所有非空线程结束。

索引越界时 `join()` 不执行操作。调用 `join_all()` 时，线程集合的写锁会在等待期间保持，因此其他需要访问或修改线程集合的操作可能被阻塞。

## 停止与中断

```cpp
void stop(size_type idx);
void stop(const id_type& id);
void stop_all();

void interrupt(size_type idx);
void interrupt_all();
```

- `stop(idx)` 请求指定索引线程停止。
- `stop(id)` 按 ID 查找并请求相应线程停止。
- `stop_all()` 请求所有非空线程停止。
- `interrupt(idx)` 中断指定索引线程。
- `interrupt_all()` 中断所有非空线程。

索引越界或未找到 ID 时不执行操作。停止和中断的具体响应方式由 `Work_Thread` 实现决定；它们不等同于等待线程结束，需要等待时调用 `join()` 或 `join_all()`。

## 生命周期与同步

- 管理器析构时清空线程指针容器；析构函数本身不显式调用 `stop()`、`interrupt()` 或 `join()`。
- 启动、查询及线程集合操作由管理器内部互斥量保护。
- 操作过程中管理器持有集合锁，并在部分接口中调用工作线程方法。避免工作线程回调同时重入管理器并请求同一集合锁，以免发生锁等待问题。
- `get_id(cont)` 先在读锁保护下复制线程指针列表，再遍历快照取得 ID。

## 使用示意

```cpp
typedef yggr::thread::thread_mgr<my_work_thread> thread_mgr_type;

thread_mgr_type manager;
manager.add_thread()
       .add_thread();

const yggr::size_type started =
    manager.startup_all<my_runner>(runner_arg);

manager.stop_all();
manager.join_all();
```

`my_work_thread` 和 `my_runner` 需满足工作线程的启动及执行接口要求。若 runner 需要共享所有权，可使用接受 `shared_ptr<Runner>` 的启动重载。
