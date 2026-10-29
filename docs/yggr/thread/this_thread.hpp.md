# `this_thread.hpp` 中文说明

## 用途

本头文件定义线程类型适配模板 `yggr::thread::this_thread<Thread>`，通过统一的静态接口查询当前线程 ID、休眠和让出执行权。头文件还会在自身末尾引入 Boost.Thread 特化。

```cpp
#include <yggr/thread/this_thread.hpp>
```

## 通用模板

```cpp
template<typename Thread>
class this_thread;
```

`this_thread` 私有继承 `nonable::noncreateable`，不能创建对象；所有操作均通过静态成员函数调用。`Thread` 类型需提供线程 ID 类型和当前线程操作：

| 接口 | 要求 |
| --- | --- |
| `Thread::id` | 线程 ID 类型。 |
| `Thread::this_thread_id()` | 返回当前线程 ID。 |
| `Thread::sleep(time)` | 使当前线程休眠。 |
| `Thread::yield()` | 让出执行机会。 |

公开别名 `id_type` 对应 `Thread::id`。

## 接口

### 当前线程 ID

```cpp
static const id_type id();
```

调用 `Thread::this_thread_id()` 并返回当前线程的 ID。

### 相对休眠

```cpp
template<typename Time>
static void sleep(const Time& step);

template<typename Time>
static void sleep(const Time& start, const Time& step);
```

- 单参数重载默认构造一个 `Time`，累加 `step` 后调用 `Thread::sleep()`。
- 双参数重载计算 `start + step`，并将结果传给 `Thread::sleep()`。

参数类型需支持相应的默认构造、加法和 `+=` 操作，并符合底层线程类型的休眠接口要求。

### 指定时间休眠

```cpp
template<typename Time>
static void sleep_to(const Time& tm);
```

将 `tm` 直接传给 `Thread::sleep()`。其表示时长还是绝对时间，由底层线程类型及时间类型的约定决定。

### 让出执行权

```cpp
static void yield();
```

调用 `Thread::yield()`，提示调度器当前线程可以让出处理器执行机会。

## Boost.Thread 特化

在通用模板定义之后，本头文件设置 `YGGR_THIS_THREAD_INCLUDE` 并包含 `boost_this_thread.hpp`，由此提供 `this_thread<boost::thread>` 特化。该特化将 ID、休眠和让出操作转发到 Boost.Thread，并使用 `boost::this_thread::get_id()` 获取当前线程 ID。

直接包含本头文件即可使用该特化；不要单独包含 `boost_this_thread.hpp`，因为后者要求此头文件先行包含。

## 使用示例

```cpp
typedef yggr::thread::this_thread<boost::thread> this_thread_type;

this_thread_type::id_type current_id = this_thread_type::id();
this_thread_type::yield();
```

具体休眠调用应传入与所用线程实现兼容的时间类型。
