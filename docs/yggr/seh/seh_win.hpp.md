# `seh_win.hpp`

## 概述

该头文件定义了 `yggr::seh::seh_win`，用于在 Windows/MSVC 环境下提供高层结构化异常处理（SEH）封装。

该类在 `seh_env_win` 的基础上增加：

- 异常代码注册和注销；
- 异常调用栈采集；
- Windows 错误消息格式化；
- 栈溢出异常的专用处理线程；
- 调试版和发布版的异常处理策略。

## 编译与包含条件

完整实现只在 `_MSC_VER` 已定义时生效：

```cpp
#if defined(_MSC_VER)
```

同时要求先包含统一入口：

```cpp
#include <yggr/seh/seh.hpp>
```

直接包含本文件而未定义 `YGGR_SEH_INCLUDE` 会触发预处理器错误。

## 命名空间

```cpp
yggr::seh
```

## 类型与常量

### `code_type`

```cpp
typedef u32 code_type;
```

用于表示 Windows SEH 异常代码。

### `seh_sys_codes_type`

```cpp
typedef seh_env_win::seh_sys_codes_type seh_sys_codes_type;
```

表示 Windows 平台的系统异常代码集合。

### `E_limit_stack_overflow`

```cpp
YGGR_STATIC_CONSTANT(u32, E_limit_stack_overflow = 20);
```

栈溢出异常进行调用栈回溯时使用的最大回溯次数。

### `E_limit_normal`

```cpp
YGGR_STATIC_CONSTANT(u32, E_limit_normal = 0x7fffffff);
```

普通异常使用的最大回溯次数上限。

## 构造与析构

```cpp
seh_win(void);
~seh_win(void);
```

当前构造和析构函数为空。异常代码集合和互斥量由成员对象自动初始化和释放。

## 安全调用接口

### 带返回值

```cpp
template<typename Ret, typename Handler>
bool safe_invoke(Ret& ret, const Handler& handler) const;
```

执行 `handler`，并将结果写入 `ret`。内部调用 `seh_env_win::safe_invoke()`，绑定：

- `filter(code, exception_pointers)` 作为 SEH 过滤器；
- `ret_def_cleaner<Ret>()` 作为返回值清理器；
- `def_fixer()` 作为默认修复器。

### 无返回值

```cpp
template<typename Handler>
bool safe_invoke(const Handler& handler) const;
```

执行不需要返回值的 `handler`，使用 `nonret_def_cleaner()` 作为清理器。

两个重载的返回值均表示底层 `seh_env_win::safe_invoke()` 的执行结果。

## 异常代码管理

### `register_code`

```cpp
bool register_code(code_type code);
```

将异常代码加入内部线程安全集合 `_set`。返回插入操作的结果；如果代码已经存在，通常返回 `false`。

### `unregister_code`

```cpp
bool unregister_code(code_type code);
```

从 `_set` 中移除异常代码。代码存在并成功移除时返回 `true`，否则返回 `false`。

已注册代码用于 `throw_error()` 返回值的判断。

## 内部清理与修复回调

### `ret_def_cleaner`

```cpp
template<typename Ret>
Ret ret_def_cleaner(void) const;
```

返回 `Ret()`，用于异常路径中的返回值清理。

### `nonret_def_cleaner`

```cpp
void nonret_def_cleaner(void) const;
```

无返回值清理器，不执行实际操作。

### `def_fixer`

```cpp
bool def_fixer(void) const;
```

当前实现固定返回 `false`。

## 异常处理流程

### `filter`

```cpp
u32 filter(code_type code, LPEXCEPTION_POINTERS pep) const;
```

作为 `seh_env_win::safe_invoke()` 的 SEH 过滤器，处理流程如下：

1. 为栈溢出修复参数分配临时结构；
2. 保存当前对象、异常代码和异常指针；
3. 获取当前线程句柄；
4. 创建事件对象；
5. 创建辅助线程执行 `filter_handler()`；
6. 等待辅助线程完成；
7. 读取栈溢出修复结果；
8. 根据调试/发布配置返回 SEH 处理结果。

### `filter_handler`

```cpp
static DWORD WINAPI filter_handler(LPVOID params);
```

辅助线程入口。参数有效时调用当前对象的 `throw_error()`，将结果写入 `bfixed`，随后设置事件通知等待线程。

参数无效时直接返回 `0`。

### `throw_error`

```cpp
bool throw_error(
    code_type code,
    LPEXCEPTION_POINTERS pep,
    HANDLE hthread = 0) const;
```

采集并报告异常信息：

1. 使用 `_mutex` 串行保护调用栈采集；
2. 调用 `seh_helper_win::dump_call_stack()` 获取调用栈；
3. 栈溢出异常使用 `E_limit_stack_overflow`，其他异常使用 `E_limit_normal`；
4. 使用 `FormatMessage()` 获取系统错误文本；
5. 将异常代码、错误文本和调用栈组合成消息；
6. 调用 `exception::exception::throw_error()` 抛出项目异常；
7. 返回 `_set.is_exists(code)` 的结果。

## 异常处理策略

当 `filter()` 判断异常已经被辅助线程修复时：

- Debug 且定义 `YGGR_INCLUDE_SEH_DEBUG`：返回 `EXCEPTION_EXECUTE_HANDLER`，继续执行处理流程；
- Debug 且未定义 `YGGR_INCLUDE_SEH_DEBUG`：返回 `EXCEPTION_CONTINUE_SEARCH`，继续搜索处理器；
- Release：返回 `EXCEPTION_EXECUTE_HANDLER`。

当异常未被修复时：

- Debug：返回 `EXCEPTION_CONTINUE_SEARCH`；
- Release：返回 `EXCEPTION_EXECUTE_HANDLER`。

## 使用示例

```cpp
#include <yggr/seh/seh.hpp>

void invoke_with_windows_seh()
{
    yggr::seh::seh_win seh;
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

注册和注销异常代码：

```cpp
void configure_seh_codes(yggr::seh::seh_win& seh)
{
    seh.register_code(
        yggr::seh::seh_sys_codes_win::E_thread_access_violation);

    seh.unregister_code(
        yggr::seh::seh_sys_codes_win::E_thread_access_violation);
}
```

## 成员状态

```cpp
code_set_type _set;
mutable mutex_type _mutex;
```

- `_set`：保存已注册的异常代码，类型为线程安全无序集合；
- `_mutex`：保护调用栈采集等异常处理操作，允许在 `const` 成员函数中修改锁状态。

## 注意事项

1. 该实现依赖 MSVC 原生 SEH 和 Windows API，非 MSVC 环境不会提供完整定义。
2. 栈溢出异常使用辅助线程处理，目的是避免在当前线程栈耗尽时继续执行复杂处理。
3. `filter()`、`throw_error()` 涉及线程创建、事件等待、符号解析和字符串处理，应结合异常现场安全性评估。
4. `register_code()` 只影响内部异常代码集合，不会直接调用 Windows 全局异常注册 API。
5. `throw_error()` 返回值表示异常代码是否存在于 `_set`，但当前 `filter()` 主要根据 `bfixed` 和构建配置决定返回值。
6. `E_limit_stack_overflow` 较小，用于降低栈溢出异常处理期间的额外栈消耗。
7. 过滤器和修复器必须遵守 `seh_env_win` 对 SEH 回调签名的要求。
8. 具体调用栈符号解析依赖 `seh_helper_win`、DbgHelp 和调试符号配置。
9. 异常代码常量应使用 `seh_sys_codes_win` 提供的名称，避免直接依赖平台数值。

## 相关组件

- `yggr/seh/seh.hpp`
- `yggr/seh/seh_env_win.hpp`
- `yggr/seh/seh_helper_win.hpp`
- `yggr/seh/seh_sys_codes_win.hpp`
- `yggr/seh/ipps/seh_win.ipp`
- `yggr/safe_container/safe_unordered_set.hpp`
- `yggr/thread/boost_thread.hpp`
- Windows SEH API
