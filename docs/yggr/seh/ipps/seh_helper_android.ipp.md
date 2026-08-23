# `seh_helper_android.ipp`

## 概述

该实现文件提供 `yggr::seh::seh_helper_android` 的 Android 平台实现，用于采集和格式化调用栈信息。

实现根据 Android API 可用性分为两个分支：

- Android API 33 及以上：使用 `backtrace()` 和 `backtrace_symbols()` 获取调用栈；
- 低于 API 33：不执行实际采集，返回兼容性提示或失败状态。

## 包含与编译条件

文件定义：

```cpp
#define YGGR_SEH_INCLUDE
```

并包含：

```cpp
#include <yggr/seh/seh_helper_android.hpp>
```

该文件是实现文件，通常由构建系统编译，不应在业务源文件中直接包含。

## 命名空间

```cpp
yggr::seh
```

## API 33 及以上实现

该分支由以下条件编译：

```cpp
#if __BIONIC_AVAILABILITY_GUARD(33)
```

### `format_dump_call_stack_msg`

```cpp
std::string format_dump_call_stack_msg(
    const dump_call_stack_type& call_stack);
```

将调用栈条目格式化为多行字符串。

格式化规则：

- 每个调用栈条目占一行；
- 第 `n` 个条目前添加 `n` 个制表符；
- 每行末尾追加换行符；
- 条目顺序与传入容器当前顺序一致。

### `dump_call_stack`

```cpp
bool dump_call_stack(dump_call_stack_type& call_stack);
```

使用固定大小为 1024 的地址数组采集当前调用栈：

```cpp
void* arr[1024] = {0};
int size = backtrace(arr, 1024);
char** strs = reinterpret_cast<char**>(
    backtrace_symbols(arr, size));
```

处理流程：

1. 调用 `backtrace()` 获取地址列表；
2. 调用 `backtrace_symbols()` 将地址转换为符号字符串；
3. 当符号数组为空或采集数量为零时返回 `false`；
4. 将每条符号字符串追加到 `call_stack`；
5. 释放 `backtrace_symbols()` 返回的字符串数组；
6. 将调用栈容器反转；
7. 返回 `true`。

调用栈反转后，调用者看到的顺序与原始采集顺序相反。

## 低于 API 33 的实现

该分支用于不满足 Android API 33 可用性条件的环境。

### `format_dump_call_stack_msg`

```cpp
std::string format_dump_call_stack_msg(
    const dump_call_stack_type& call_stack);
```

不格式化传入的调用栈，而是返回固定的兼容性提示字符串，提示当前 Android 最低 SDK 版本不满足调用栈 API 要求。

### `dump_call_stack`

```cpp
bool dump_call_stack(dump_call_stack_type& call_stack);
```

当前实现直接返回 `false`，不会修改 `call_stack`。

## 与异常环境的关系

`seh_env_android::handler_recv_signal()` 会调用：

```cpp
seh_helper_type::dump_call_stack(call_stack);
seh_helper_type::format_dump_call_stack_msg(call_stack);
```

采集和格式化结果随后被拼接到异常消息中，用于记录操作系统信号代码和调用栈信息。

## 使用示例

```cpp
#include <yggr/seh/seh_helper_android.hpp>

void collect_android_call_stack()
{
    yggr::seh::seh_helper_android::dump_call_stack_type call_stack;

    if(yggr::seh::seh_helper_android::dump_call_stack(call_stack))
    {
        const std::string message =
            yggr::seh::seh_helper_android::format_dump_call_stack_msg(
                call_stack);
        (void)message;
    }
}
```

## 注意事项

1. API 33 及以上分支最多采集 1024 个栈帧。
2. `backtrace_symbols()` 返回的内存由实现释放，当前代码在完成复制后调用 `free()`。
3. 当调用栈采集失败时，API 33 及以上分支返回 `false`。
4. 低于 API 33 时，`dump_call_stack()` 始终返回 `false`，格式化函数返回固定提示文本。
5. `format_dump_call_stack_msg()` 会按照调用栈索引增加缩进，输出格式依赖容器中的条目顺序。
6. 符号解析结果依赖 Android 运行库、可执行文件符号和当前构建配置，可能只包含地址或不完整符号。
7. 该实现通常由信号处理路径调用，调用栈采集、字符串构造和内存分配是否满足异步信号安全要求，应结合项目整体异常处理设计评估。
8. 该文件只提供 Android 实现；其他平台应使用对应的 `seh_helper_*.ipp` 文件。

## 相关组件

- `yggr/seh/seh_helper_android.hpp`
- `yggr/seh/seh_env_android.hpp`
- `yggr/seh/ipps/seh_env_android.ipp`
- `yggr/seh/seh_android.hpp`
