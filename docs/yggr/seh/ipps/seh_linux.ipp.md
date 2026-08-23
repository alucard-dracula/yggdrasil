# `seh_linux.ipp`

## 概述

该实现文件提供 `yggr::seh::seh_linux` 的 Linux 平台实现，负责初始化接口以及异常/信号代码的注册和注销。

信号处理由 `seh_env_linux::handler_recv_signal` 承担，本文件通过标准 `signal()` 将指定信号连接到该处理函数。

## 包含与编译条件

文件内部定义：

```cpp
#define YGGR_SEH_INCLUDE
```

随后包含：

```cpp
#include <yggr/seh/seh_linux.hpp>
```

该文件是实现文件，通常由构建系统编译，不应在业务源文件中直接包含。

## 命名空间

```cpp
yggr::seh
```

## 实现接口

### 构造函数

```cpp
seh_linux::seh_linux(void);
```

当前实现为空，不执行额外初始化。

### 析构函数

```cpp
seh_linux::~seh_linux(void);
```

当前实现为空，不执行额外清理。

### `register_code`

```cpp
bool seh_linux::register_code(code_type code);
```

使用标准 `signal()` 将指定信号代码绑定到 Linux 异常环境处理函数：

```cpp
signal(code, &seh_env_type::handler_recv_signal);
```

当前实现无条件返回 `true`，不检查 `signal()` 的返回结果。

### `unregister_code`

```cpp
bool seh_linux::unregister_code(code_type code);
```

使用标准 `signal()` 将指定信号代码恢复为默认处理方式：

```cpp
signal(code, SIG_DFL);
```

当前实现无条件返回 `true`，不检查 `signal()` 的返回结果。

## 使用示例

```cpp
#include <yggr/seh/seh_linux.hpp>

void configure_linux_signals()
{
    yggr::seh::seh_linux seh;
    const yggr::seh::seh_linux::code_type code = 1;

    seh.register_code(code);
    seh.unregister_code(code);
}
```

通常应通过 `seh_linux::safe_invoke()` 配合 Linux 异常环境使用，具体安全调用流程请参考对应头文件和 `seh_env_linux` 实现。

## 处理流程

### 注册

1. 接收调用者提供的信号代码；
2. 调用标准 `signal()`；
3. 设置 `seh_env_linux::handler_recv_signal` 为信号处理函数；
4. 返回 `true`。

### 注销

1. 接收调用者提供的信号代码；
2. 调用标准 `signal()`；
3. 将处理函数设置为 `SIG_DFL`；
4. 返回 `true`。

## 与异常环境的关系

注册后的信号由 `seh_env_linux::handler_recv_signal()` 处理。该处理函数负责：

- 获取当前线程 ID；
- 采集并格式化调用栈；
- 记录操作系统信号代码；
- 抛出项目异常；
- 通过 `siglongjmp` 返回安全调用现场。

## 注意事项

1. 当前实现忽略 `signal()` 的返回结果，因此返回 `true` 不代表底层注册或注销一定成功。
2. `code` 必须是 Linux 平台支持的有效信号代码；传入无效代码时，平台行为应以 C/C++ 运行库为准。
3. 注册信号处理器会改变当前进程对相应信号的处理方式，可能影响其他代码或线程。
4. 注销后会恢复 `SIG_DFL`，不会恢复调用前可能存在的自定义处理器。
5. 信号处理函数必须遵守平台对异步信号安全操作的限制。
6. 构造和析构当前没有额外逻辑，信号注册需要显式调用 `register_code()`。
7. 该文件依赖 `seh_linux.hpp`、`seh_env_linux` 和平台提供的 `signal()`/`SIG_DFL` 定义。

## 相关组件

- `yggr/seh/seh_linux.hpp`
- `yggr/seh/seh_env_linux.hpp`
- `yggr/seh/seh_helper_linux.hpp`
- `yggr/seh/ipps/seh_env_linux.ipp`
