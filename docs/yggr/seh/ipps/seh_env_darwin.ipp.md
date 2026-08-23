# `seh_env_darwin.ipp`

## 概述

该实现文件提供 `yggr::seh::seh_env_darwin` 的 Darwin 平台静态成员定义和信号处理实现。

主要职责包括：

- 初始化异常环境使用的静态互斥量；
- 初始化线程跳转缓冲区映射和当前信号映射；
- 接收信号并记录相关线程信息；
- 采集调用栈并转换为项目异常；
- 使用 `siglongjmp` 返回安全调用现场。

## 包含与编译条件

文件内部定义：

```cpp
#define YGGR_SEH_INCLUDE
```

随后包含：

```cpp
#include <yggr/seh/seh_env_darwin.hpp>
```

该文件是实现文件，通常由构建系统编译，不应在业务源文件中直接包含。

## 命名空间

```cpp
yggr::seh
```

## 静态成员定义

### `_jmp_buf_mutex`

```cpp
seh_env_darwin::mutex_type seh_env_darwin::_jmp_buf_mutex;
```

保护线程跳转缓冲区映射 `_jmp_buf_map` 的访问。

### `_signal_mutex`

```cpp
seh_env_darwin::mutex_type seh_env_darwin::_signal_mutex;
```

保护当前信号映射 `_now_sig_map` 的访问。

### `_call_stack_mutex`

```cpp
seh_env_darwin::mutex_type seh_env_darwin::_call_stack_mutex;
```

保护调用栈采集过程，避免并发采集造成冲突。

### `_jmp_buf_map`

```cpp
seh_env_darwin::jmp_buf_map_type seh_env_darwin::_jmp_buf_map;
```

保存线程 ID 到跳转缓冲区的映射，用于信号处理后跳回触发安全调用的位置。

### `_now_sig_map`

```cpp
seh_env_darwin::now_signal_map_type seh_env_darwin::_now_sig_map;
```

保存线程 ID 到最近接收信号代码的映射。

## `handler_recv_signal`

```cpp
static void handler_recv_signal(int code);
```

该函数是 Darwin 信号处理入口，由 `seh_darwin::register_code()` 通过标准 `signal()` 注册。

### 处理流程

1. 获取当前线程 ID：

   ```cpp
   trd_id_type tid = this_thread_type::id();
   ```

2. 在 `_call_stack_mutex` 写锁保护下采集当前调用栈；
3. 将操作系统信号代码和调用栈格式化为错误消息；
4. 调用 `exception::exception::throw_error(code, message)` 抛出项目异常；
5. 在 `_signal_mutex` 写锁保护下，将 `code` 保存到 `_now_sig_map[tid]`；
6. 在 `_jmp_buf_mutex` 写锁保护下，通过当前线程的跳转缓冲区调用 `siglongjmp(..., 1)`。

## 调用栈错误信息

错误消息包含：

```text
os_code = <signal code>
<formatted call stack>
```

调用栈由 `seh_helper_darwin::dump_call_stack()` 采集，并由
`seh_helper_darwin::format_dump_call_stack_msg()` 格式化。

## 与安全调用的关系

该实现依赖 `seh_env_darwin` 头文件中定义的安全调用流程：

- `seh_darwin::safe_invoke()` 设置当前线程的跳转环境；
- `register_code()` 将信号转发到 `handler_recv_signal()`；
- `handler_recv_signal()` 记录信号并跳回安全调用代码；
- 安全调用环境根据跳转结果执行过滤器、清理器和默认修复器。

## 依赖组件

| 组件 | 用途 |
| --- | --- |
| `seh_env_darwin` | 保存异常环境的静态状态和跳转映射。 |
| `seh_helper_darwin` | 采集和格式化调用栈。 |
| `exception::exception` | 抛出 yggdrasil 项目异常。 |
| `boost::thread::mutex` 相关类型 | 保护共享映射和调用栈采集。 |
| `siglongjmp` | 从信号处理函数跳回安全调用现场。 |

## 注意事项

1. `handler_recv_signal()` 会修改全局静态映射，所有相关访问必须遵守当前线程同步约定。
2. 信号处理函数中执行调用栈采集、字符串流操作和异常抛出，是否满足目标 Darwin 运行环境的异步信号安全要求，取决于项目运行时设计。
3. `_jmp_buf_map`、`_now_sig_map` 和互斥量是进程级静态状态，不同 `seh_env_darwin` 实例之间共享。
4. `siglongjmp` 要求对应线程的跳转缓冲区已经正确建立；否则可能导致未定义行为。
5. 信号处理函数通过线程 ID选择跳转缓冲区，因此注册和触发应发生在一致的线程上下文中。
6. 该文件只提供静态成员定义和信号处理函数实现，安全调用的其他流程位于 `seh_env_darwin.hpp` 或相关实现文件中。

## 相关组件

- `yggr/seh/seh_env_darwin.hpp`
- `yggr/seh/seh_darwin.hpp`
- `yggr/seh/seh_helper_darwin.hpp`
- `yggr/exception/exception.hpp`
