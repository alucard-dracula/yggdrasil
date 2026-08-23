# `seh_helper_android.hpp`

## 概述

该头文件定义了 `yggr::seh::seh_helper_android`，用于 Android 平台的调用栈采集和格式化。

类本身只声明静态辅助接口，具体实现位于 `seh_helper_android.ipp`。

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

## 类声明

```cpp
class seh_helper_android;
```

该类不保存实例状态，所有功能均通过静态成员函数提供。

## 类型别名

```cpp
typedef ::yggr::list<std::string> dump_call_stack_type;
```

`dump_call_stack_type` 用于保存调用栈中的符号文本，每个元素通常代表一个栈帧。

## 静态接口

### `format_dump_call_stack_msg`

```cpp
static std::string format_dump_call_stack_msg(
    const dump_call_stack_type& call_stack);
```

将调用栈容器格式化为字符串。

具体缩进规则和输出格式由 `seh_helper_android.ipp` 中的实现决定。当前实现会按栈帧顺序逐行输出，并使用递增数量的制表符表示层级。

### `dump_call_stack`

```cpp
static bool dump_call_stack(
    dump_call_stack_type& call_stack);
```

采集当前执行线程的调用栈，并将栈帧信息写入 `call_stack`。

返回值表示采集是否成功。具体采集能力受 Android API 版本和运行时符号解析支持影响。

## 使用示例

```cpp
#include <yggr/seh/seh_helper_android.hpp>

void collect_call_stack()
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

## 与异常环境的关系

`seh_env_android::handler_recv_signal()` 使用该类生成异常诊断信息：

1. 调用 `dump_call_stack()` 获取调用栈；
2. 调用 `format_dump_call_stack_msg()` 转换为文本；
3. 将调用栈文本与 Android 信号代码组合到异常消息中。

## 平台实现说明

对应实现文件会根据 Android API 可用性选择实现：

- API 33 及以上：使用 `backtrace()` 和 `backtrace_symbols()` 采集调用栈；
- 低于 API 33：调用栈采集返回失败，格式化函数返回兼容性提示文本。

## 注意事项

1. 该类是 Android 平台专用辅助类。
2. 使用前应通过 `seh.hpp` 建立必要的包含配置。
3. `dump_call_stack()` 会修改传入的调用栈容器，调用者应提供可写对象。
4. 符号解析结果依赖 Android 运行库、构建符号和当前 API 支持，可能不完整。
5. 采集失败时应检查返回值，不应假定 `call_stack` 一定包含有效栈帧。
6. 具体输出格式、最大栈帧数量和低版本行为以 `seh_helper_android.ipp` 的实现为准。
7. 调用栈采集通常发生在异常处理路径，使用时应评估 Android 信号处理上下文中的安全性和性能影响。

## 相关组件

- `yggr/seh/seh.hpp`
- `yggr/seh/seh_env_android.hpp`
- `yggr/seh/ipps/seh_helper_android.ipp`
- `yggr/charset/string.hpp`
- `yggr/container/list.hpp`
- `execinfo.h`
