# `work_thread.hpp` 中文说明

## 用途

`yggr::thread::work_thread<Thread_Config>` 封装一个工作线程及其运行状态。它负责创建底层线程、反复调用 runner、在 runner 暂无工作时让出并休眠，以及响应停止和中断请求。

```cpp
#include <yggr/thread/work_thread.hpp>
```

工作线程不可复制、不可移动。默认配置为 `boost_thread_config_type`。

## 模板参数与类型

```cpp
template<typename Thread_Config = yggr::thread::boost_thread_config_type>
class work_thread;
```

`Thread_Config` 需提供线程类型、时间类型和线程 ID 类型。工作线程公开以下别名：

| 别名 | 来源 |
| --- | --- |
| `thread_config_type` | `Thread_Config` |
| `thread_type` | 配置中的线程类型 |
| `time_type` | 配置中的时间类型 |
| `thread_ptr_type` | 底层线程的共享指针类型 |
| `id_type` | 配置中的线程 ID 类型 |

## 状态常量与查询

| 常量 | 含义 |
| --- | --- |
| `E_thread_state_start` | 状态值基点，数值为 `0`。 |
| `E_thread_stop` | 停止状态，数值为 `1`。 |
| `E_thread_free` | 空闲状态，数值为 `2`。 |
| `E_thread_busy` | 忙碌状态，数值为 `3`。 |

查询接口：

```cpp
id_type get_id() const;
bool is_free() const;
bool is_stop() const;
bool is_busy() const;
bool is_running() const;
```

- `get_id()` 返回底层线程 ID；线程指针为空时返回默认构造的 ID。
- `is_free()`、`is_busy()` 根据原子状态值判断。
- `is_stop()` 在状态为停止或底层线程指针为空时返回 `true`。
- `is_running()` 仅判断状态值是否不等于停止状态；它不等同于检查底层线程是否仍可加入（joinable）。

## 构造

```cpp
work_thread();
explicit work_thread(const time_type& tm_step);
```

默认构造使用配置中的默认休眠步长；另一个构造函数接受自定义空闲轮询步长。新建对象初始状态为停止，运行标志为 `false`。

## 启动 runner

```cpp
template<typename Runner, typename... Args>
bool start(Args&&... args);

template<typename Runner>
bool start(yggr::shared_ptr<Runner> runner_ptr);
```

第一种形式用转发参数构造 `Runner` 并放入共享指针；第二种形式接收已有 runner 指针。两者都会创建底层线程，在新线程中执行内部 `run()` 循环。

内部 `run()` 通过编译期断言要求 `Runner::tag_type` 等于 `tag_dynamic_work_runner`。若 runner 指针为空，会触发断言并返回。启动的 runner 每轮通过 `(*runner_ptr)()` 执行：

- 返回 `true` 时继续立即执行下一轮。
- 返回 `false` 时将状态设为空闲，调用线程 `yield()`，并休眠 `_tm_step` 后再继续。

**返回值注意：**`start()` 在状态切换失败或运行标志设置失败时也返回 `true`；创建底层线程后则返回线程指针是否非空。因此返回 `true` 不一定代表本次调用新启动了线程。调用前应先检查 `is_stop()` 等状态，并不要把该返回值当作“新线程确实启动”的唯一判据。

## 停止、打断与等待

```cpp
void stop();
void interrupt();
void join();
```

- `stop()` 尝试将运行标志从 `true` 设为 `false`；若成功则调用 `join()` 等待线程结束。
- `interrupt()` 仅在运行标志为真且底层线程存在时，向底层线程发出中断请求。
- `join()` 在底层线程存在且可加入时等待其结束；否则直接返回。

`stop()` 依赖 runner 的调用或休眠能够返回以观察运行标志。若工作函数长时间阻塞，停止请求可能无法立即完成；是否使用 `interrupt()` 取决于底层线程和 runner 的中断处理逻辑。

## 线程 ID 比较

提供 `work_thread` 与 `id_type`、以及两个 `work_thread` 对象之间的 `==` 和 `!=` 非成员运算符。比较按以下规则进行：

- 线程对象与 ID 比较底层线程 ID。
- 两个工作线程对象在对象地址相同、底层线程共享指针相同，或线程 ID 相同时视为相等。

对于未启动或底层线程指针为空的对象，`get_id()` 返回默认 ID；应避免仅凭默认 ID 判断不同的未启动对象身份。

## 生命周期与实现注意事项

- 析构函数本身为空，不会主动调用 `stop()` 或 `join()`。销毁对象前应确保工作线程已经停止并完成等待。
- 工作循环中的 runner 类型检查要求 `tag_dynamic_work_runner`，因此需使用满足该标签约束的 runner。
- `_sat` 和 `_brun` 使用原子变量管理状态与运行标志；休眠步长决定 runner 返回无工作时的轮询间隔。
- 如果线程或 runner 构造/启动过程中失败，调用方应检查状态和线程对象，并按应用的线程生命周期策略处理。

## 使用示意

```cpp
typedef yggr::thread::work_thread<> work_thread_type;

work_thread_type worker;
worker.start<my_dynamic_runner>(runner_arg);

// 需要停止时：
worker.stop();
worker.join();
```

`my_dynamic_runner` 需定义匹配的 `tag_type`，并提供可调用且返回布尔值的 `operator()()`；具体类型还需满足内部调用约定。
