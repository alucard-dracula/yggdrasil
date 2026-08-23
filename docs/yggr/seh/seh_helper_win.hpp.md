# `seh_helper_win.hpp`

## 概述

该头文件定义了 `yggr::seh::seh_helper_win`，用于 Windows 平台的异常调用栈采集、符号格式化和当前进程机器类型查询。

类接口依赖 Windows API 和 DbgHelp：

- `windows.h`；
- `dbghelp.h`；
- `Tlhelp32.h`。

具体实现位于 `seh_helper_win.ipp`。

## 编译与包含条件

该文件要求先包含统一入口：

```cpp
#include <yggr/seh/seh.hpp>
```

直接包含本文件而未定义 `YGGR_SEH_INCLUDE` 会触发预处理器错误。

文件中的原始 MSVC 检查目前被注释，但接口和实现仍依赖 Windows API；实际使用应采用 Windows/MSVC 构建环境。

## 命名空间

```cpp
yggr::seh
```

## 类声明

```cpp
class seh_helper_win;
```

该类不保存实例状态，所有公开功能均为静态成员函数。

## 类型别名

```cpp
typedef ::yggr::list<std::string> dump_call_stack_type;
```

`dump_call_stack_type` 用于保存调用栈条目的文本描述，每个元素通常对应一个栈帧。

## 静态接口

### `get_this_machine_type`

```cpp
static WORD get_this_machine_type(void);
```

获取当前进程主模块的 PE 机器类型，用于后续 `StackWalk()` 栈回溯时选择机器架构。

返回值来自 PE 文件头的 `IMAGE_FILE_HEADER::Machine` 字段。

### `format_dump_call_stack_msg`

```cpp
static std::string format_dump_call_stack_msg(
    const dump_call_stack_type& call_stack);
```

将调用栈条目转换为多行字符串。

当前实现的格式规则为：

- 每个条目占一行；
- 第 `n` 个条目前添加 `n` 个制表符；
- 每行末尾追加换行符；
- 输出顺序与容器当前顺序一致。

### `dump_call_stack`

```cpp
static bool dump_call_stack(
    LPEXCEPTION_POINTERS ex_ptr,
    dump_call_stack_type& call_stack,
    HANDLE hprocess = 0,
    HANDLE hthread = 0,
    u32 limit = 0x7fffffff);
```

根据 Windows 异常指针采集调用栈，并写入 `call_stack`。

参数说明：

| 参数 | 说明 |
| --- | --- |
| `ex_ptr` | Windows 异常指针，必须包含有效的 `ContextRecord`。 |
| `call_stack` | 输出调用栈容器。 |
| `hprocess` | 可选进程句柄；为 `0` 时由实现选择当前进程。 |
| `hthread` | 可选线程句柄；为 `0` 时由实现选择当前线程。 |
| `limit` | 最大回溯次数，默认值为 `0x7fffffff`。 |

函数会初始化 DbgHelp 符号处理，执行内部栈回溯，最后清理符号处理状态。返回值表示初始化或清理流程的结果。

## 私有接口

### `handler_dump_call_stack`

```cpp
static void handler_dump_call_stack(
    PCONTEXT context_ptr,
    dump_call_stack_type& call_stack,
    HANDLE crash_hprocess = 0,
    HANDLE crash_hthread = 0,
    u32 limit = 0x7fffffff);
```

执行具体的栈回溯和符号解析，供 `dump_call_stack()` 调用。

该函数会：

- 清空 `call_stack`；
- 根据异常上下文初始化 `STACKFRAME`；
- 使用 `StackWalk()` 遍历栈帧；
- 使用 `SymFromAddr()` 获取函数名；
- 使用 `SymGetLineFromAddr()` 获取源文件和行号；
- 将解析结果或错误信息写入调用栈容器；
- 反转调用栈顺序。

### `check_seh_env_eha_set`

当定义 `YGGR_USE_SEH` 时提供：

```cpp
static void check_seh_env_eha_set(void);
```

当前实现通过断言检查并调用 `_set_se_translator(0)`。该辅助函数仅用于头文件中的 SEH 环境检查，不属于公开 API。

## 栈帧信息

每个栈帧可能包含：

- 函数名称；
- 指令地址；
- 源文件路径；
- 源代码行号；
- 符号解析或源位置查询失败时的 Windows 错误码和错误消息。

符号和源代码信息是否完整取决于模块加载状态、PDB 文件和 DbgHelp 配置。

## 使用示例

```cpp
#include <yggr/seh/seh_helper_win.hpp>

void collect_windows_stack(LPEXCEPTION_POINTERS exception_ptr)
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

## 与异常环境的关系

`seh_env_win` 的 SEH 过滤器可接收 `EXCEPTION_POINTERS`，然后将其传递给
`seh_helper_win::dump_call_stack()` 获取诊断调用栈。

典型流程为：

1. Windows SEH 产生异常；
2. 过滤器获得异常代码和异常指针；
3. 调用本辅助类采集调用栈；
4. 调用格式化函数生成文本；
5. 将文本写入日志或项目异常信息。

## 注意事项

1. `ex_ptr` 和 `ex_ptr->ContextRecord` 必须有效，否则可能导致访问冲突。
2. `dump_call_stack()` 依赖 DbgHelp 符号初始化和清理，调用时应注意线程同步和符号状态管理。
3. 缺少 PDB 或符号加载失败时，调用栈条目可能只包含地址和错误信息。
4. 当前实现的 `CONTEXT_AMD64` 栈帧初始化需要结合 `seh_helper_win.ipp` 检查；64 位回溯不可仅由头文件声明保证。
5. `handler_dump_call_stack()` 会清空调用者传入的 `call_stack`，并在完成后反转内容。
6. `limit` 过大可能增加异常处理耗时，应根据使用场景设置合理上限。
7. `check_seh_env_eha_set()` 仅在 `YGGR_USE_SEH` 下可用，并且当前会触发断言后重置 SEH 转换器。
8. 调用栈采集、符号解析和字符串分配通常不适合直接在严格的异步异常处理上下文中执行，应结合项目架构评估。
9. 该类是 Windows 平台辅助接口，其他平台应使用对应的 `seh_helper_*.hpp`。

## 相关组件

- `yggr/seh/seh.hpp`
- `yggr/seh/seh_env_win.hpp`
- `yggr/seh/seh_win.hpp`
- `yggr/seh/ipps/seh_helper_win.ipp`
- `yggr/seh/seh_sys_codes_win.hpp`
- Windows `windows.h`
- Windows `dbghelp.h`
- Windows `Tlhelp32.h`
