# `seh_linux.hpp`

## 概述

该头文件定义了 `yggr::seh::seh_linux`，用于在 Linux 平台封装基于信号的安全调用。

该类将用户处理器转发给 `seh_env_linux`，并自动绑定异常过滤器、返回值清理器和默认修复器。

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

## 类型别名

```cpp
typedef u32 code_type;
typedef seh_env_linux::seh_sys_codes_type seh_sys_codes_type;
```

- `code_type`：异常或信号代码类型，实际为 `u32`；
- `seh_sys_codes_type`：由 Linux 异常环境提供的系统异常代码集合类型。

## 构造与析构

```cpp
seh_linux(void);
~seh_linux(void);
```

构造和析构函数用于初始化及释放 Linux 异常处理对象相关状态，具体实现位于
`seh_linux.ipp`。

## 安全调用接口

### 带返回值

```cpp
template<typename Ret, typename Handler>
bool safe_invoke(Ret& ret, const Handler& handler) const;
```

执行 `handler` 并将结果写入 `ret`。内部调用：

```cpp
seh_env_linux::safe_invoke(
    ret,
    handler,
    filter,
    ret_def_cleaner<Ret>,
    def_fixer);
```

发生信号或调用失败时，返回值清理器会生成值初始化后的 `Ret()`。

### 无返回值

```cpp
template<typename Handler>
bool safe_invoke(const Handler& handler) const;
```

执行不需要返回值的 `handler`，内部使用 `nonret_def_cleaner()` 作为默认清理器。

两个重载的返回值均表示底层 `seh_env_linux::safe_invoke()` 的执行结果。

## 信号代码管理

### `register_code`

```cpp
bool register_code(code_type code);
```

注册指定的 Linux 信号代码。具体实现通过标准 `signal()` 将信号转发至
`seh_env_linux::handler_recv_signal`。

### `unregister_code`

```cpp
bool unregister_code(code_type code);
```

注销指定的 Linux 信号代码，具体实现通过 `signal()` 将处理函数恢复为
`SIG_DFL`。

## 内部回调

### `ret_def_cleaner`

```cpp
template<typename Ret>
Ret ret_def_cleaner(void) const;
```

返回 `Ret()`，用于带返回值的安全调用失败时清理结果。

### `nonret_def_cleaner`

```cpp
void nonret_def_cleaner(void) const;
```

无返回值清理器，不执行实际操作。

### `def_fixer`

```cpp
bool def_fixer(void) const;
```

当前实现固定返回 `false`，表示默认修复流程未报告成功。

### `filter`

```cpp
u32 filter(code_type code) const;
```

当前实现忽略传入的 `code`，固定返回 `0`。差异化的信号过滤策略需要额外实现。

## 使用示例

```cpp
#include <yggr/seh/seh.hpp>

void invoke_linux_safely()
{
    yggr::seh::seh_linux seh;
    int result = 0;

    const bool success = seh.safe_invoke(
        result,
        []() -> int
        {
            return 42;
        });

    (void)success;
    (void)result;
}
```

无返回值调用：

```cpp
void invoke_without_result()
{
    yggr::seh::seh_linux seh;

    seh.safe_invoke(
        []()
        {
            // 执行需要信号保护的操作
        });
}
```

注册和注销信号代码：

```cpp
void configure_signal_code(yggr::seh::seh_linux& seh)
{
    const yggr::seh::seh_linux::code_type code = 1;

    seh.register_code(code);
    seh.unregister_code(code);
}
```

## 调用关系

`seh_linux` 与平台实现的关系如下：

1. `register_code()` 将信号绑定到 `seh_env_linux::handler_recv_signal()`；
2. `safe_invoke()` 将处理器和内部回调传入 `seh_env_linux`；
3. 信号处理器采集调用栈并记录信号代码；
4. `seh_env_linux` 根据跳转结果执行过滤、清理和修复逻辑。

## 注意事项

1. 该类是 Linux 平台专用实现。
2. 使用前应包含 `seh.hpp`，以确保 `YGGR_SEH_INCLUDE` 和相关环境配置已建立。
3. `safe_invoke()` 的实际信号捕获和跳转能力由 `seh_env_linux` 决定。
4. 当前 `filter()` 固定返回 `0`，不能根据异常代码执行差异化处理。
5. 当前 `def_fixer()` 固定返回 `false`。
6. 返回值重载要求 `Ret` 支持值初始化，即支持 `Ret()`。
7. 注册信号会改变当前进程的信号处理方式，注销后恢复 `SIG_DFL`，不会恢复之前的自定义处理器。
8. 当前 `register_code()` 和 `unregister_code()` 实现无条件返回 `true`，不检查底层 `signal()` 的返回值。
9. 构造函数、析构函数及信号代码管理函数的具体实现需链接对应的 Linux 实现文件。

## 相关组件

- `yggr/seh/seh.hpp`
- `yggr/seh/seh_env_linux.hpp`
- `yggr/seh/seh_helper_linux.hpp`
- `yggr/seh/ipps/seh_linux.ipp`
- `yggr/exception/exception.hpp`
