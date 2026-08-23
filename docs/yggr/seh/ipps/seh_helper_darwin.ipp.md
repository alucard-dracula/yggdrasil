# `seh_helper_darwin.ipp`

## 概述

该实现文件提供 `yggr::seh::seh_helper_darwin` 的 Darwin 平台调用栈采集和格式化实现。

实现直接使用系统调用栈接口：

- `backtrace()`：采集栈帧地址；
- `backtrace_symbols()`：将地址转换为符号字符串。

## 包含与编译条件

文件定义：

```cpp
#define YGGR_SEH_INCLUDE
```

并包含：

```cpp
#include <yggr/seh/seh_helper_darwin.hpp>
```

该文件是实现文件，通常由构建系统编译，不应在业务源文件中直接包含。

## 命名空间

```cpp
yggr::seh
```

## `format_dump_call_stack_msg`

```cpp
std::string seh_helper_darwin::format_dump_call_stack_msg(
    const dump_call_stack_type& call_stack);
```

将调用栈条目格式化为多行文本。

格式化规则：

- 每个调用栈条目占一行；
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
bool seh_helper_darwin::dump_call_stack(
    dump_call_stack_type& call_stack);
```

采集当前调用栈并追加到 `call_stack`。

### 处理流程

1. 创建最多容纳 1024 个地址的数组；
2. 调用 `backtrace(arr, 1024)` 获取栈帧地址；
3. 调用 `backtrace_symbols(arr, size)` 获取符号字符串数组；
4. 当符号数组为空或栈帧数量为零时返回 `false`；
5. 将每条符号字符串追加到 `call_stack`；
6. 调用 `free(strs)` 释放符号数组；
7. 反转 `call_stack`；
8. 返回 `true`。

该函数不会在采集前清空调用者传入的 `call_stack`，因此已有内容会保留并参与最终反转。

## 与异常环境的关系

`seh_env_darwin::handler_recv_signal()` 会调用这两个辅助函数：

```cpp
seh_helper_type::dump_call_stack(call_stack);
seh_helper_type::format_dump_call_stack_msg(call_stack);
```

采集结果会被格式化并拼接到异常消息中，同时记录 Darwin 信号代码。

## 使用示例

```cpp
#include <yggr/seh/seh_helper_darwin.hpp>

void collect_darwin_call_stack()
{
    yggr::seh::seh_helper_darwin::dump_call_stack_type call_stack;

    if(yggr::seh::seh_helper_darwin::dump_call_stack(call_stack))
    {
        const std::string message =
            yggr::seh::seh_helper_darwin::format_dump_call_stack_msg(
                call_stack);
        (void)message;
    }
}
```

## 资源管理

`backtrace_symbols()` 返回的符号数组由当前实现通过 `free()` 释放。函数只将符号内容复制到 `call_stack`，不会把该数组的所有权交给调用者。

## 注意事项

1. 当前实现最多采集 1024 个栈帧。
2. 采集失败时返回 `false`；成功时返回 `true`。
3. 调用者传入的 `call_stack` 不会在函数开始时清空。
4. 函数完成后会反转整个 `call_stack`，因此调用前已有的条目也会受到影响。
5. 符号解析结果依赖 Darwin 运行库、可执行文件符号和构建配置，可能只包含地址或不完整的符号信息。
6. 该实现通常由信号处理路径调用，调用栈采集、字符串构造和内存分配是否满足异步信号安全要求，应结合项目整体异常处理设计评估。
7. 该文件只提供 Darwin 实现；Android、Linux 等平台应使用对应的 `seh_helper_*.ipp` 文件。

## 相关组件

- `yggr/seh/seh_helper_darwin.hpp`
- `yggr/seh/seh_env_darwin.hpp`
- `yggr/seh/ipps/seh_env_darwin.ipp`
- `yggr/seh/seh_darwin.hpp`
