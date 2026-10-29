# `boost_this_thread.hpp` 中文说明

## 用途

本头文件为 `this_thread<boost::thread>` 提供特化，将当前线程 ID、休眠和让出执行权等操作转发到 Boost.Thread。

```cpp
#include <yggr/thread/this_thread.hpp>
```

不要直接包含 `boost_this_thread.hpp`。该头文件要求先包含 `this_thread.hpp`，否则 `YGGR_THIS_THREAD_INCLUDE` 未定义时会触发编译错误。主头文件会按项目组织方式引入相应实现。

## 类型

```cpp
yggr::thread::this_thread<boost::thread>
yggr::thread::default_this_thread_type
```

特化私有继承 `nonable::noncreateable`，不能创建对象；其静态成员函数通过类名直接调用。

| 别名 | 定义 |
| --- | --- |
| `id_type` | `boost::thread::id`，即 Boost.Thread 的线程 ID 类型。 |
| `default_this_thread_type` | `this_thread<boost::thread>`。 |

## 接口

### 当前线程 ID

```cpp
static id_type id();
```

返回 `boost::this_thread::get_id()` 的结果。

### 相对休眠

```cpp
template<typename Time>
static void sleep(const Time& step);

template<typename Time, typename TimeStep>
static void sleep(const Time& start, const TimeStep& step);
```

- 单参数重载默认构造一个 `Time`，将 `step` 累加到该对象后传给 `boost::thread::sleep()`。
- 双参数重载将 `start + step` 的结果传给 `boost::thread::sleep()`。

这些接口的参数类型需满足对应的默认构造、加法及 Boost.Thread 时间参数要求。

### 指定时间休眠

```cpp
template<typename Time>
static void sleep_to(const Time& tm);
```

将 `tm` 直接传给 `boost::thread::sleep()`。具体时间点或时长语义取决于所使用的 Boost.Thread 版本和 `Time` 类型。

### 让出执行权

```cpp
static void yield();
```

转发到 `boost::thread::yield()`，提示调度器当前线程可以让出处理器执行机会。

## 使用示例

```cpp
typedef yggr::thread::default_this_thread_type this_thread_type;

this_thread_type::id_type current_id = this_thread_type::id();
this_thread_type::yield();
```

具体休眠调用应使用与项目 Boost.Thread 版本兼容的时间类型。

## 平台注意事项

在 MSVC 下，本头文件会临时禁用警告 4624，并在声明结束后恢复原警告状态。该特化依赖 Boost.Thread；项目需已正确配置 Boost.Thread 及其所需的构建依赖。
