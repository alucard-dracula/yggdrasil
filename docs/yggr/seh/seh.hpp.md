# `seh.hpp`

## 概述

该头文件是 yggdrasil SEH 模块的统一入口，负责：

- 检查目标平台和 `YGGR_USE_SEH` 配置；
- 选择并包含 Windows、Linux、Android 或 Darwin 平台实现；
- 定义通用的 `yggr::seh::seh<Base>` 包装类；
- 根据当前平台提供统一别名 `yggr::seh::seh_type`。

调用方通常只需要包含该文件，不需要直接包含平台专用的 `seh_*.hpp` 文件。

## 头文件

```cpp
#include <yggr/seh/seh.hpp>
```

## 启用条件

SEH 实现需要同时满足：

```cpp
#if defined(_MSC_VER) || defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN)
#if defined(YGGR_USE_SEH)
```

如果未定义 `YGGR_USE_SEH`，该文件不会启用 SEH 类型实现。

MinGW 和 Cygwin 被明确禁止：

```cpp
#if defined(__MINGW32__) || defined(__CYGWIN__)
#error "seh must use to system compiler"
#endif
```

## 平台选择

在 `YGGR_USE_SEH` 生效时，文件按以下条件选择平台实现：

| 条件 | 包含的实现 | `seh_type` |
| --- | --- | --- |
| `WIN32`、`WIN64` 或 `WINDOWS` | `seh_win.hpp` | `seh<seh_win>` |
| `YGGR_AT_ANDROID` | `seh_android.hpp` | `seh<seh_android>` |
| `YGGR_AT_LINUX` | `seh_linux.hpp` | `seh<seh_linux>` |
| `YGGR_AT_DARWIN` | `seh_darwin.hpp` | `seh<seh_darwin>` |

如果没有匹配的平台宏，预处理器会报告错误，要求补充对应平台实现。

## 通用类 `seh<Base>`

```cpp
template<typename Base>
class seh : public Base;
```

`seh<Base>` 继承平台实现类 `Base`，并通过 `ptr_single` 管理一个共享的实现实例。

### 类型别名

```cpp
typedef typename base_type::seh_sys_codes_type seh_sys_codes_type;
typedef typename base_type::code_type code_type;
typedef typename this_single_type::obj_ptr_type obj_ptr_type;
typedef const obj_ptr_type obj_cptr_type;
```

其中 `Base` 通常是 `seh_win`、`seh_linux`、`seh_android` 或 `seh_darwin`。

## 实例管理接口

### `init_ins`

```cpp
static void init_ins(void);
static void init_ins(const obj_ptr_type& pobj);
```

初始化全局单例实例：

- 无参版本使用默认构造方式初始化；
- 带指针版本使用调用者提供的对象指针初始化。

### `get_ins`

```cpp
static obj_ptr_type get_ins(void);
```

返回当前全局实例的智能指针。如果尚未初始化，可能返回空指针。

### `uninstall`

```cpp
static void uninstall(void);
```

卸载当前全局实例。

## 异常代码管理接口

### `s_register_code`

```cpp
static bool s_register_code(code_type code);
```

获取全局实例并调用其 `register_code(code)`。

如果当前实例不存在，返回 `false`。

### `s_unregister_code`

```cpp
static bool s_unregister_code(code_type code);
```

获取全局实例并调用其 `unregister_code(code)`。

如果当前实例不存在，返回 `false`。

## 安全调用接口

### 带返回值

```cpp
template<typename Ret, typename Handler>
static bool s_safe_invoke(Ret& ret, const Handler& handler);
```

执行 `handler` 并将结果写入 `ret`。

行为如下：

- 如果全局实例存在，调用实例的 `safe_invoke(ret, handler)`；
- 如果全局实例不存在，直接调用 `handler()`，将结果写入 `ret` 并返回 `true`。

### 无返回值

```cpp
template<typename Handler>
static bool s_safe_invoke(const Handler& handler);
```

执行不需要返回值的 `handler`。

行为如下：

- 如果全局实例存在，调用实例的 `safe_invoke(handler)`；
- 如果全局实例不存在，直接调用 `handler()` 并返回 `true`。

因此，未初始化全局实例时，`s_safe_invoke()` 不提供平台异常保护，只执行普通调用。

## 平台类型别名

```cpp
typedef seh<seh_win> seh_type;
typedef seh<seh_linux> seh_type;
typedef seh<seh_android> seh_type;
typedef seh<seh_darwin> seh_type;
```

实际生效的定义取决于编译平台：

```cpp
yggr::seh::seh_type
```

可用于编写不依赖具体平台实现类名称的代码。

## 使用示例

```cpp
#include <yggr/seh/seh.hpp>

void initialize_seh()
{
    yggr::seh::seh_type::init_ins();
    yggr::seh::seh_type::s_register_code(
        yggr::seh::seh_type::seh_sys_codes_type::E_unknown);
}
```

安全执行带返回值的操作：

```cpp
void invoke_safely()
{
    int result = 0;

    const bool success = yggr::seh::seh_type::s_safe_invoke(
        result,
        []() -> int
        {
            return 42;
        });

    (void)success;
    (void)result;
}
```

完成后卸载：

```cpp
void shutdown_seh()
{
    yggr::seh::seh_type::uninstall();
}
```

## 典型调用流程

```text
包含 seh.hpp
    -> 检查平台和 YGGR_USE_SEH
    -> 选择平台 Base 类型
    -> init_ins() 创建全局实例
    -> s_register_code() 注册异常/信号代码
    -> s_safe_invoke() 执行安全调用
    -> uninstall() 释放全局实例
```

## 注意事项

1. 必须定义 `YGGR_USE_SEH` 才会启用平台 SEH 实现。
2. MinGW 和 Cygwin 被明确禁止使用该模块，Windows 下应使用系统编译器。
3. `seh_type` 是平台相关别名，具体可用的异常代码类型和注册语义由平台实现决定。
4. 未调用 `init_ins()` 时，`s_safe_invoke()` 会直接执行处理器，不提供异常保护。
5. `s_register_code()` 和 `s_unregister_code()` 在全局实例为空时返回 `false`。
6. 全局实例通过 `ptr_single` 管理，初始化、替换和卸载应遵守项目对单例和线程安全的约定。
7. Windows 平台使用原生 MSVC SEH；Linux、Android 和 Darwin 平台使用信号与跳转缓冲区机制，异常语义并不完全相同。
8. 平台宏未正确配置时会触发编译错误，而不是自动回退到无保护调用。
9. 具体安全调用回调、异常代码和调用栈行为请参考对应平台的 `seh_*.hpp`、`seh_env_*.hpp` 和辅助类文档。

## 相关组件

- `yggr/seh/seh_win.hpp`
- `yggr/seh/seh_linux.hpp`
- `yggr/seh/seh_android.hpp`
- `yggr/seh/seh_darwin.hpp`
- `yggr/seh/seh_env_win.hpp`
- `yggr/seh/seh_env_linux.hpp`
- `yggr/seh/seh_env_android.hpp`
- `yggr/seh/seh_env_darwin.hpp`
- `yggr/base/ptr_single.hpp`
