# `thread_config.hpp` 中文说明

## 用途

本头文件定义线程配置模板 `yggr::thread::thread_config<Thread, Thread_Group, Time>`，并提供宏 `YGGR_PP_THREAD_CONFIG_DECL` 用于生成具名配置类。配置统一描述线程类型、线程组、时间类型、默认休眠步长及建议线程数。

```cpp
#include <yggr/thread/thread_config.hpp>
```

配置类继承或依赖的线程操作通过 `this_thread<Thread>` 提供。

## `thread_config` 模板

```cpp
template<typename Thread, typename Thread_Group, typename Time>
class thread_config;
```

公开类型别名：

| 别名 | 定义 |
| --- | --- |
| `thread_type` | `Thread` |
| `id_type` | `Thread::id` |
| `thread_group_type` | `Thread_Group` |
| `time_type` | `Time` |
| `this_thread_type` | `this_thread<thread_type>` |

### 默认休眠步长

```cpp
static const time_type& default_sleep_time_step();
```

返回静态保存的默认时间步长，其秒数为 `0`，纳秒数为 `33,000,000`（约 33 毫秒）。秒数常量在定义 `BOOST_NO_INT64_T` 时使用 `s32`，否则使用 `s64`。

### 并发度

```cpp
static u32 hardware_concurrency();
static u32 recommended_number_of_threads();
```

- `hardware_concurrency()` 返回 `boost::thread::hardware_concurrency()`。
- `recommended_number_of_threads()` 按 `(hardware_concurrency() << 1) + 2` 计算建议线程数，即硬件并发数的两倍再加二。

这只是项目提供的建议值；硬件并发数可能为零，调用方应结合实际工作负载及资源限制决定线程数量。

## 生成具名配置类

```cpp
YGGR_PP_THREAD_CONFIG_DECL(Name, Thread, ThreadGroup, Time)
```

该宏在 `yggr::thread` 命名空间作用域生成具名配置类，并提供与 `thread_config` 相同的类型别名、默认休眠步长、硬件并发数及建议线程数接口。宏参数分别指定配置名、线程类型、线程组类型和时间类型。

通常可直接使用已有的具体配置别名，例如 `boost_thread_config.hpp` 中定义的 `boost_thread_config_type`；需要自定义线程实现时，可通过 `thread_config` 或该宏组合相应类型。

## 使用示例

```cpp
typedef yggr::thread::thread_config<
    my_thread,
    my_thread_group,
    my_time
> my_thread_config;

my_thread_config::id_type id;
const my_thread_config::time_type& step =
    my_thread_config::default_sleep_time_step();

yggr::u32 thread_count =
    my_thread_config::recommended_number_of_threads();
```

`my_thread` 需提供嵌套类型 `id`；线程操作由匹配的 `this_thread<my_thread>` 适配。

## 平台注意事项

- 头文件包含 `boost/thread/scoped_thread.hpp`，并调用 Boost.Thread 查询硬件并发度。
- 在 MSVC 下会临时禁用警告 4624，并在文件末尾恢复原警告设置。
