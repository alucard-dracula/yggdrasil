# `seh_android.hpp`

## 概述

该头文件定义了 `yggr::seh::seh_android`，用于在 Android 平台封装结构化异常处理相关操作。

该类通过 `seh_env_android` 执行安全调用，并使用 `seh_helper_android` 及项目异常组件提供平台相关支持。

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

## 类型别名

```cpp
typedef u32 code_type;
typedef seh_env_android::seh_sys_codes_type seh_sys_codes_type;
```

`code_type` 为 `u32`，用于表示异常代码；`seh_sys_codes_type` 是 Android 异常环境提供的系统异常代码集合类型。

## 构造与析构

```cpp
seh_android(void);
~seh_android(void);
```

用于初始化和释放 Android 异常处理环境相关状态。具体实现位于对应源文件中。

## 安全调用接口

### 带返回值

```cpp
template<typename Ret, typename Handler>
bool safe_invoke(Ret& ret, const Handler& handler) const;
```

执行 `handler` 并将结果写入 `ret`。调用过程由 `seh_env_android::safe_invoke` 管理，并使用当前对象的异常过滤器、返回值清理器和默认修复器。

发生异常或调用失败时，返回值清理器返回值初始化后的 `Ret()`。

### 无返回值

```cpp
template<typename Handler>
bool safe_invoke(const Handler& handler) const;
```

执行不需要返回值的 `handler`，使用无返回值清理器和默认修复器。

## 异常代码管理

### `register_code`

```cpp
bool register_code(code_type code);
```

注册指定异常代码。具体行为由 Android 异常环境实现决定。

### `unregister_code`

```cpp
bool unregister_code(code_type code);
```

注销指定异常代码，返回底层操作是否成功。

## 内部回调

```cpp
template<typename Ret>
Ret ret_def_cleaner(void) const;

void nonret_def_cleaner(void) const;
bool def_fixer(void) const;
u32 filter(code_type code) const;
```

- `ret_def_cleaner()` 返回 `Ret()`；
- `nonret_def_cleaner()` 不执行操作；
- `def_fixer()` 当前固定返回 `false`；
- `filter()` 当前忽略异常代码并固定返回 `0`。

## 使用示例

```cpp
#include <yggr/seh/seh.hpp>

void invoke_android_safely()
{
    yggr::seh::seh_android seh;
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

无返回值调用：

```cpp
void invoke_without_result()
{
    yggr::seh::seh_android seh;

    seh.safe_invoke(
        []()
        {
            // 执行需要异常保护的操作
        });
}
```

异常代码管理：

```cpp
void configure_exception_code(yggr::seh::seh_android& seh)
{
    const yggr::seh::seh_android::code_type code = 1;

    seh.register_code(code);
    seh.unregister_code(code);
}
```

## 注意事项

1. 该类是 Android 平台专用实现。
2. 使用前应包含 `seh.hpp`，以确保 `YGGR_SEH_INCLUDE` 和相关环境配置已建立。
3. `safe_invoke()` 的实际异常捕获能力由 `seh_env_android` 决定。
4. 当前 `filter()` 固定返回 `0`，不能根据异常代码执行差异化筛选。
5. 当前 `def_fixer()` 固定返回 `false`。
6. 返回值重载要求 `Ret` 支持值初始化，即支持 `Ret()`。
7. `register_code()` 和 `unregister_code()` 的有效代码范围及线程安全性应以 Android 异常环境实现为准。
8. 构造函数、析构函数及异常代码管理函数只有声明，具体实现需链接对应库或源文件。

## 相关组件

- `yggr/seh/seh.hpp`
- `yggr/seh/seh_env_android.hpp`
- `yggr/seh/seh_helper_android.hpp`
- `yggr/exception/exception.hpp`
- `boost/thread/mutex.hpp`
