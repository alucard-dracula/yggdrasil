# `seh_helper_win.ipp`

## 概述

该实现文件提供 `yggr::seh::seh_helper_win` 的 Windows 平台实现，用于获取当前 PE 模块架构信息、采集异常调用栈并格式化调用栈消息。

实现依赖 Windows DbgHelp API，包括：

- `SymInitialize()` 和 `SymCleanup()`；
- `StackWalk()`；
- `SymFromAddr()`；
- `SymGetLineFromAddr()`；
- `FormatMessage()`。

## 包含与编译条件

文件定义：

```cpp
#define YGGR_SEH_INCLUDE
```

随后包含：

```cpp
#include <yggr/seh/seh_helper_win.hpp>
```

在 `_MSC_VER` 下还会包含 Visual Leak Detector：

```cpp
#include <vld.h>
```

该文件是实现文件，通常由构建系统编译，不应在业务源文件中直接包含。

## 命名空间

```cpp
yggr::seh
```

## `get_this_machine_type`

```cpp
WORD seh_helper_win::get_this_machine_type(void);
```

获取当前进程主模块的 PE 机器类型。

处理流程：

1. 调用 `GetModuleHandle(0)` 获取当前进程主模块句柄；
2. 将模块地址解释为 `IMAGE_DOS_HEADER`；
3. 根据 `e_lfanew` 定位 PE 文件头中的 `IMAGE_FILE_HEADER`；
4. 返回 `IMAGE_FILE_HEADER::Machine`。

返回值可传递给 `StackWalk()`，用于选择对应的机器架构回溯方式。

## `format_dump_call_stack_msg`

```cpp
std::string seh_helper_win::format_dump_call_stack_msg(
    const dump_call_stack_type& call_stack);
```

将调用栈条目格式化为多行字符串。

格式化规则：

- 每个条目占一行；
- 第 `n` 个条目前添加 `n` 个制表符；
- 每行末尾追加换行符；
- 输出顺序与传入容器的当前顺序一致。

示例输出形式：

```text
frame_0
	frame_1
		frame_2
```

## `dump_call_stack`

```cpp
bool seh_helper_win::dump_call_stack(
    LPEXCEPTION_POINTERS ex_ptr,
    dump_call_stack_type& call_stack,
    HANDLE hprocess = 0,
    HANDLE hthread = 0,
    u32 limit = 0x7fffffff);
```

根据 Windows 异常指针采集调用栈。

参数说明：

| 参数 | 说明 |
| --- | --- |
| `ex_ptr` | Windows 异常指针，提供异常上下文。必须包含有效的 `ContextRecord`。 |
| `call_stack` | 输出调用栈容器。 |
| `hprocess` | 可选进程句柄。为 `0` 时使用当前进程句柄。 |
| `hthread` | 可选线程句柄。为 `0` 时使用当前线程句柄。 |
| `limit` | 最大回溯迭代次数，默认值为 `0x7fffffff`。 |

处理流程：

1. 使用 `GetCurrentProcess()` 获取当前进程句柄；
2. 调用 `SymInitialize()` 初始化符号处理；
3. 调用 `handler_dump_call_stack()` 执行实际栈回溯；
4. 调用 `SymCleanup()` 清理符号处理状态；
5. 返回 `SymCleanup()` 的结果。

如果 `SymInitialize()` 失败，函数立即返回 `false`。

## `handler_dump_call_stack`

```cpp
void seh_helper_win::handler_dump_call_stack(
    PCONTEXT context_ptr,
    dump_call_stack_type& call_stack,
    HANDLE crash_hprocess = 0,
    HANDLE crash_hthread = 0,
    u32 limit = 0x7fffffff);
```

执行具体的 Windows 栈回溯、符号解析和源代码位置查询。

### 初始化栈帧

函数首先清空 `call_stack`，然后根据编译条件初始化 `STACKFRAME`：

- `CONTEXT_i386` 或 `CONTEXT_i486`：使用 `Eip`、`Esp` 和 `Ebp` 初始化程序计数器、栈指针和帧指针；
- `CONTEXT_AMD64`：当前分支没有执行栈帧字段初始化；
- 其他架构：编译时触发错误，要求补充实现。

### 句柄选择

- 使用调用者提供的 `crash_hprocess`，否则使用 `GetCurrentProcess()`；
- 使用调用者提供的 `crash_hthread`，否则使用 `GetCurrentThread()`。

### 符号和源代码信息

函数为每个栈帧执行：

1. `StackWalk()` 获取下一帧；
2. `SymFromAddr()` 解析函数名；
3. `SymGetLineFromAddr()` 查询源文件和行号；
4. 将函数名、地址、错误码、错误消息或源位置追加到字符串；
5. 将字符串加入 `call_stack`。

成功解析函数时，输出类似：

```text
[ foo_name = function_name ]
```

成功获取源位置时，输出类似：

```text
[ source file : file.cpp, sourec line : 42 ]
```

函数名或源位置解析失败时，会使用 `GetLastError()` 和 `FormatMessage()` 生成系统错误信息。

循环结束后，函数会反转 `call_stack`，使调用者获得反向排列的调用栈。

## 使用示例

```cpp
#include <yggr/seh/seh_helper_win.hpp>

void collect_windows_call_stack(LPEXCEPTION_POINTERS exception_ptr)
{
    yggr::seh::seh_helper_win::dump_call_stack_type call_stack;

    if(yggr::seh::seh_helper_win::dump_call_stack(
        exception_ptr,
        call_stack))
    {
        const std::string message =
            yggr::seh::seh_helper_win::format_dump_call_stack_msg(
                call_stack);
        (void)message;
    }
}
```

## 符号处理资源

`dump_call_stack()` 在开始回溯前调用 `SymInitialize()`，完成后调用 `SymCleanup()`。

符号解析依赖当前进程的模块信息和调试符号。缺少 PDB 或符号加载失败时，函数仍可能返回调用栈，但条目中会包含地址和系统错误信息。

## 注意事项

1. `ex_ptr` 和 `ex_ptr->ContextRecord` 必须有效，否则回溯过程可能访问无效内存。
2. 当前 `CONTEXT_AMD64` 分支没有初始化 `STACKFRAME` 的寄存器字段，64 位 Windows 栈回溯可能无法正常工作；使用前应验证或补充该分支实现。
3. `limit` 只是最大循环次数，实际帧数还受 `StackWalk()` 成功与帧指针状态限制。
4. `handler_dump_call_stack()` 会先清空 `call_stack`，与 Linux/Darwin 辅助实现的行为不同。
5. `dump_call_stack()` 使用当前进程句柄初始化符号处理，即使调用者提供了其他进程句柄；跨进程回溯时应确认符号环境是否正确。
6. `SymInitialize()`、符号解析和 `FormatMessage()` 的结果依赖 Windows 版本、模块加载状态和调试符号配置。
7. 源码输出文本中的 `sourec line` 是现有实现中的拼写形式，使用者如依赖文本解析应注意这一点。
8. DbgHelp 的符号处理状态通常需要按进程串行化；并发调用时应遵守项目上层的同步约定。
9. 该实现通常用于异常处理路径，符号解析和字符串分配是否适合异常现场，应结合整体异常处理设计评估。

## 相关组件

- `yggr/seh/seh_helper_win.hpp`
- `yggr/seh/seh_env_win.hpp`
- `yggr/seh/seh_win.hpp`
- Windows DbgHelp API
- `dbghelp.h`
