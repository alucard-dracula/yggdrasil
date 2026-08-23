# `seh_env_win.hpp`

## 概述

该头文件定义了 `yggr::seh::seh_env_win`，用于在 Windows/MSVC 环境下封装原生结构化异常处理（SEH）。

实现使用 MSVC 专有语法：

- `__try`：建立异常保护区域；
- `__finally`：无论正常返回还是异常，都执行清理逻辑；
- `__except`：接收并处理结构化异常。

## 编译条件

该头文件的完整实现只在 `_MSC_VER` 已定义时生效：

```cpp
#if defined(_MSC_VER)
```

同时要求先通过统一入口包含相关 SEH 配置：

```cpp
#include <yggr/seh/seh.hpp>
```

直接包含本文件而未定义 `YGGR_SEH_INCLUDE` 会触发预处理器错误。

非 MSVC 编译器不会获得该类的实现定义。

## 命名空间

```cpp
yggr::seh
```

## 类型别名

```cpp
typedef seh_sys_codes_win seh_sys_codes_type;
```

`seh_sys_codes_type` 表示 Windows 平台的系统异常代码集合，由
`seh_sys_codes_win.hpp` 提供。

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

调用流程：

1. 进入外层 `__try`；
2. 在内层 `__try` 中执行 `handler()`；
3. 正常完成时将结果写入 `ret`，并将成功标记设为 `true`；
4. 内层 `__finally` 无论是否发生异常都会执行 `cleaner()`，并将结果写入 `ret`；
5. 如果发生 SEH 异常，外层 `__except` 调用：

   ```cpp
   filter(GetExceptionCode(), GetExceptionInformation())
   ```

6. 异常被过滤器接收后，返回 `fixer()` 的结果；
7. 未进入异常处理分支时，返回正常执行成功标记。

`Ret` 必须同时兼容 `handler()` 和 `cleaner()` 的返回值。

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

该重载与带返回值版本的控制流一致，但不会保存处理器或清理器的返回值：

- 正常路径执行 `handler()`，然后返回 `true`；
- 清理阶段执行 `cleaner()`；
- 异常路径调用过滤器和 `fixer()`。

## SEH 回调参数

过滤器接收两个 Windows SEH 参数：

```cpp
filter(
    GetExceptionCode(),
    GetExceptionInformation());
```

参数含义：

| 参数 | 说明 |
| --- | --- |
| `GetExceptionCode()` | 当前结构化异常的系统异常代码。 |
| `GetExceptionInformation()` | 当前异常的 `EXCEPTION_POINTERS` 信息，可用于访问异常记录和处理器上下文。 |

过滤器的返回值遵循 MSVC `__except` 过滤表达式语义，由过滤器决定异常是否由当前处理块处理。

## 清理与修复顺序

正常执行时：

```text
handler() -> cleaner() -> 返回 true
```

发生 SEH 异常时：

```text
handler() -> cleaner() -> filter(code, information) -> fixer()
```

其中 `cleaner()` 位于 `__finally` 中，因此即使 `handler()` 抛出结构化异常，也会先执行清理逻辑。

## 使用示例

```cpp
#include <yggr/seh/seh.hpp>

void invoke_with_windows_seh()
{
    int result = 0;

    const bool success =
        yggr::seh::seh_env_win::safe_invoke(
            result,
            []() -> int
            {
                return 42;
            },
            [](unsigned long code, EXCEPTION_POINTERS* information)
            {
                (void)code;
                (void)information;
                return EXCEPTION_EXECUTE_HANDLER;
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

无返回值调用：

```cpp
void invoke_without_result()
{
    const bool success =
        yggr::seh::seh_env_win::safe_invoke(
            []()
            {
                // 执行需要 SEH 保护的操作
            },
            [](unsigned long code, EXCEPTION_POINTERS* information)
            {
                (void)code;
                (void)information;
                return EXCEPTION_EXECUTE_HANDLER;
            },
            []()
            {
                // 执行清理
            },
            []() -> bool
            {
                return false;
            });

    (void)success;
}
```

实际项目通常通过 `seh_win::safe_invoke()` 使用该环境类，而不是直接调用它。

## 返回值语义

- `handler()` 正常完成：执行清理器后返回 `true`；
- 发生 SEH 且过滤器选择当前处理块：执行清理器后返回 `fixer()` 的结果；
- 过滤器决定继续搜索其他异常处理器时，控制权由 Windows SEH 运行时继续处理；
- `bright` 仅在 `handler()` 正常完成并执行到成功标记设置处时为 `true`。

## 注意事项

1. 该实现依赖 MSVC 的 `__try`、`__finally` 和 `__except`，不适用于普通非 MSVC 编译器。
2. `cleaner()` 位于 `__finally` 中，会在正常路径和 SEH 异常路径执行。
3. 过滤器应正确处理 `GetExceptionInformation()` 返回的异常指针，避免访问无效上下文。
4. 过滤器返回值必须符合 MSVC `__except` 过滤表达式要求，例如 `EXCEPTION_EXECUTE_HANDLER` 或 `EXCEPTION_CONTINUE_SEARCH`。
5. 带返回值版本要求清理器返回可赋值给 `Ret` 的结果。
6. 该类不提供异常代码注册接口；Windows SEH 由编译器和操作系统异常分发机制管理。
7. `seh_env_win.hpp` 只声明和定义头文件内的模板逻辑，相关平台头文件和实现需满足项目构建配置。
8. 头文件包含 `socket_conflict_fixer.hpp` 和 Windows API，使用时应确保 Windows SDK 与项目依赖配置完整。

## 相关组件

- `yggr/seh/seh.hpp`
- `yggr/seh/seh_win.hpp`
- `yggr/seh/seh_sys_codes_win.hpp`
- `yggr/seh/ipps/seh_win.ipp`
- `yggr/network/socket_conflict_fixer.hpp`
- Windows `windows.h`
- MSVC Structured Exception Handling
