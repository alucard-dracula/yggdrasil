# `seh_sys_codes_darwin.hpp`

## 概述

该头文件定义了 `yggr::seh::seh_sys_codes_darwin`，用于统一表示 Darwin 平台的系统信号异常代码。

类中通过匿名枚举将项目常量映射到 C 标准库提供的 `SIG*` 信号宏，供
`seh_env_darwin` 和 `seh_darwin` 的异常处理流程使用。

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

## 类型声明

```cpp
class seh_sys_codes_darwin;
```

该类没有实例状态，主要作为系统信号代码常量的命名空间使用。

## 信号代码

| 常量 | 映射值 | 说明 |
| --- | --- | --- |
| `E_SIGILL` | `SIGILL` | 非法指令信号。源码注释标注为栈溢出相关，但具体语义由 Darwin 平台定义。 |
| `E_SIGABRT` | `SIGABRT` | 异常终止信号。 |
| `E_SIGBUS` | `SIGBUS` | 总线错误信号。 |
| `E_SIGFPE` | `SIGFPE` | 算术错误信号，例如除零。 |
| `E_SIGSEGV` | `SIGSEGV` | 段错误信号，常见于非法内存访问。 |
| `E_SIGSYS` | `SIGSYS` | 无效系统调用信号。 |
| `E_SIGIOT` | `SIGIOT` | IOT 异常信号。 |
| `E_SIGXCPU` | `SIGXCPU` | CPU 时间限制超限信号。 |
| `E_SIGXFSZ` | `SIGXFSZ` | 文件大小限制超限信号。 |

对应定义：

```cpp
enum
{
    E_SIGILL = SIGILL,
    E_SIGABRT = SIGABRT,
    E_SIGBUS = SIGBUS,
    E_SIGFPE = SIGFPE,
    E_SIGSEGV = SIGSEGV,
    E_SIGSYS = SIGSYS,
    E_SIGIOT = SIGIOT,
    E_SIGXCPU = SIGXCPU,
    E_SIGXFSZ = SIGXFSZ
};
```

`SIGQUIT` 和 `SIGTRAP` 在当前文件中以注释形式保留，未定义对应项目常量。

## 特殊常量

### `E_unknown`

```cpp
E_unknown = 0xffffffff
```

表示无法确定具体系统信号的未知异常代码。安全调用环境在找不到当前线程的信号记录时，会将该值传给过滤器。

### `E_compile_u32`

```cpp
E_compile_u32 = 0xffffffff
```

与 `E_unknown` 使用相同数值，通常用于表达需要符合无符号 32 位常量的编译期占位值。

## 使用示例

```cpp
#include <yggr/seh/seh_sys_codes_darwin.hpp>

void check_darwin_signal(int code)
{
    if(code == yggr::seh::seh_sys_codes_darwin::E_SIGSEGV)
    {
        // 处理非法内存访问信号
    }

    if(code == yggr::seh::seh_sys_codes_darwin::E_unknown)
    {
        // 处理未知信号
    }
}
```

注册信号时可将项目常量传给 `seh_darwin::register_code()`：

```cpp
#include <yggr/seh/seh_darwin.hpp>
#include <yggr/seh/seh_sys_codes_darwin.hpp>

void register_darwin_signals(yggr::seh::seh_darwin& seh)
{
    seh.register_code(
        yggr::seh::seh_sys_codes_darwin::E_SIGSEGV);
    seh.register_code(
        yggr::seh::seh_sys_codes_darwin::E_SIGFPE);
}
```

## 与安全调用环境的关系

`seh_env_darwin` 使用该类型作为 `seh_sys_codes_type`：

```cpp
typedef seh_sys_codes_darwin seh_sys_codes_type;
```

当信号处理器记录不到当前线程的信号码时，环境会使用：

```cpp
seh_sys_codes_darwin::E_unknown
```

通知过滤器当前异常代码未知。

## 注意事项

1. 常量值直接来自 Darwin 系统的 `SIG*` 宏，不应假设不同操作系统中的数值完全一致。
2. `E_SIGILL` 等源码注释只是项目作者对信号用途的提示，实际信号语义应以 Darwin/POSIX 文档为准。
3. `E_unknown` 与 `E_compile_u32` 当前数值相同，比较或序列化时应结合使用场景区分其语义。
4. `SIGQUIT` 和 `SIGTRAP` 当前未导出为项目常量。
5. 注册信号处理器会改变当前进程的信号处理行为，使用前应评估对其他线程和组件的影响。
6. 该文件依赖 `csignal` 和项目基础类型定义，并要求通过 `seh.hpp` 建立包含配置。

## 相关组件

- `yggr/seh/seh.hpp`
- `yggr/seh/seh_darwin.hpp`
- `yggr/seh/seh_env_darwin.hpp`
- `yggr/seh/seh_sys_codes_android.hpp`
- `csignal`
