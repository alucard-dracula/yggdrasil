# `seh_env_darwin.hpp`

## 概述

该头文件定义了 `yggr::seh::seh_env_darwin`，用于在 Darwin 平台建立基于信号和 `sigsetjmp/siglongjmp` 的安全调用环境。

该类负责：

- 为当前线程创建或复用跳转缓冲区；
- 执行带返回值或无返回值的安全调用；
- 在信号处理后恢复安全调用流程；
- 保存线程最近接收的信号代码；
- 调用异常过滤器、清理器和默认修复器。

类中的状态和操作接口均为静态成员，所有实例共享同一进程级环境。

## 包含方式

该文件要求先包含统一入口：

```cpp
#include <yggr/seh/seh.hpp>
```

直接包含本文件而未定义 `YGGR_SEH_INCLUDE` 会触发预处理器错误。

## 命名空间

```cpp
yggr::seh
```

## 类型别名与常量

```cpp
typedef seh_sys_codes_darwin seh_sys_codes_type;
typedef sigjmp_buf jmp_array_type;
typedef int jmp_buf_val_type;
typedef int* jmp_buf_type;
typedef boost::shared_ptr<jmp_buf_val_type> jmp_buf_ptr_type;
```

跳转缓冲区长度常量为：

```cpp
#if defined(_JBLEN)
E_JMP_BUF_LEN = _JBLEN + 1;
#else
E_JMP_BUF_LEN = sizeof(jmp_array_type) / sizeof(jmp_buf_val_type);
#endif
```

线程 ID 类型来自 `thread::boost_thread_config_type`，映射容器使用 yggdrasil 的 `unordered_map`。

## 安全调用接口

### 带返回值

```cpp
template<typename Ret, typename Handler,
         typename Filter_Handler,
         typename Clean_Handler,
         typename FixHandler>
static bool safe_invoke(
    Ret& ret,
    const Handler& handler,
    const Filter_Handler& filter,
    const Clean_Handler& cleaner,
    const FixHandler& fixer);
```

处理流程：

1. 获取当前线程 ID；
2. 查找当前线程已有的跳转缓冲区；
3. 如果不存在，则分配 `E_JMP_BUF_LEN` 个 `int` 并加入 `_jmp_buf_map`；
4. 调用 `sigsetjmp()` 建立恢复点；
5. 正常返回时执行 `handler()`，将结果写入 `ret`，并返回 `true`；
6. 由信号处理器跳回时执行 `cleaner()`，将清理结果写入 `ret`；
7. 查询当前线程最近的信号代码并调用 `filter()`；
8. 返回 `fixer()` 的结果。

`Ret` 必须支持由 `handler()` 和 `cleaner()` 返回的值。

### 无返回值

```cpp
template<typename Handler,
         typename Filter_Handler,
         typename Clean_Handler,
         typename FixHandler>
static bool safe_invoke(
    const Handler& handler,
    const Filter_Handler& filter,
    const Clean_Handler& cleaner,
    const FixHandler& fixer);
```

执行流程与带返回值版本一致，但正常路径调用 `handler()`，信号恢复路径调用 `cleaner()`，不处理返回值。

## 跳转缓冲区管理

当前线程的跳转缓冲区保存在：

```cpp
static jmp_buf_map_type _jmp_buf_map;
```

键为线程 ID，值为 `boost::shared_ptr<int>`。访问由 `_jmp_buf_mutex` 保护。

如果无法分配缓冲区、缓冲区为空或插入映射失败，`safe_invoke()` 会返回 `false`。

## 信号状态管理

最近接收的信号代码保存在：

```cpp
static now_signal_map_type _now_sig_map;
```

键为线程 ID，值为信号整数代码。访问由 `_signal_mutex` 保护。

如果找不到当前线程的信号记录，安全调用会将：

```cpp
seh_sys_codes_type::E_unknown
```

传给过滤器。

## `handler_recv_signal`

```cpp
static void handler_recv_signal(int code);
```

该函数由 `seh_darwin::register_code()` 注册为信号处理器，具体实现位于
`seh_env_darwin.ipp`。

信号处理器通常会：

1. 获取当前线程 ID；
2. 采集并格式化调用栈；
3. 抛出项目异常；
4. 将信号代码写入 `_now_sig_map`；
5. 使用当前线程的跳转缓冲区调用 `siglongjmp()`。

## 同步机制

类中使用三个互斥量：

| 成员 | 保护内容 |
| --- | --- |
| `_jmp_buf_mutex` | `_jmp_buf_map` 和跳转缓冲区状态。 |
| `_signal_mutex` | `_now_sig_map` 和最近信号代码。 |
| `_call_stack_mutex` | 调用栈采集过程，由信号处理实现使用。 |

互斥量类型来自 `thread::mutex_def_helper<boost::mutex>`。

## 使用示例

```cpp
#include <yggr/seh/seh_env_darwin.hpp>

void invoke_with_darwin_environment()
{
    int result = 0;

    const bool success =
        yggr::seh::seh_env_darwin::safe_invoke(
            result,
            []() -> int
            {
                return 42;
            },
            [](int)
            {
                return 0u;
            },
            []() -> int
            {
                return 0;
            },
            []() -> bool
            {
                return false;
            });

    (void)success;
    (void)result;
}
```

实际项目中通常不直接调用该类，而是通过 `seh_darwin::safe_invoke()` 使用。

## 调用结果语义

- 正常执行 `handler()`：将结果写入 `ret`，返回 `true`；
- 跳转恢复后：先执行清理器，再执行过滤器和修复器，最终返回 `fixer()` 的结果；
- 跳转缓冲区分配或初始化失败：返回 `false`；
- 当前线程没有信号记录：过滤器收到 `E_unknown`。

当前实现调用了 `filter()`，但没有使用其返回值决定最终结果；信号恢复路径最终返回 `fixer()` 的结果。

## 注意事项

1. 该类是 Darwin 平台专用实现，依赖 `signal.h` 和 `setjmp.h`。
2. 该类使用进程级静态映射，不同 `seh_env_darwin` 实例之间共享状态。
3. 跳转缓冲区按线程 ID 保存；信号必须在建立对应安全调用环境的线程中触发。
4. `siglongjmp()` 依赖有效的 `sigsetjmp()` 恢复点，错误使用可能导致未定义行为。
5. `safe_invoke()` 的处理器、清理器、过滤器和修复器必须满足对应的可调用对象签名。
6. Darwin 实现使用 `int` 存储跳转缓冲区元素，并根据 `_JBLEN` 或 `sigjmp_buf` 大小计算缓冲区长度。
7. `filter()` 的返回值当前不会直接影响 `safe_invoke()` 的最终返回值。
8. 信号处理、调用栈采集和线程同步约束应结合 Darwin 运行环境及项目整体异常处理设计评估。
9. 具体异常代码定义由 `seh_sys_codes_darwin` 提供。

## 相关组件

- `yggr/seh/seh.hpp`
- `yggr/seh/seh_darwin.hpp`
- `yggr/seh/seh_helper_darwin.hpp`
- `yggr/seh/seh_sys_codes_darwin.hpp`
- `yggr/seh/ipps/seh_env_darwin.ipp`
- `yggr/thread/mutex_def_helper.hpp`
