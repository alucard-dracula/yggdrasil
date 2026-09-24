# `max_min_undef.ipp`

## 概述

该文件是用于清理 Microsoft 编译环境中 `min` 和 `max` 宏的预处理器片段。它不定义 C++ 类型、函数或类，也不提供运行时逻辑。

主要用途是避免 Windows/Microsoft 头文件定义的宏与标准库或项目代码中的：

```cpp
std::min
std::max
```

以及同名函数、模板或成员名称发生宏替换冲突。

## 文件类型

文件扩展名为：

```text
.ipp
```

它通常作为实现片段被其他源文件或头文件包含，而不是作为独立编译单元使用。

## 处理条件

整个清理逻辑受以下宏控制：

```cpp
#ifdef _MSC_VER
...
#endif
```

只有在 `_MSC_VER` 已定义时执行清理。该宏通常由 Microsoft Visual C++ 编译器定义。

在 GCC、Clang、MinGW 或其他未定义 `_MSC_VER` 的环境中，文件不执行任何操作。

## 清理的宏

### `min`

如果当前已经定义 `min` 宏：

```cpp
#ifdef min
#undef min
#endif
```

文件会取消该宏定义。

### `max`

如果当前已经定义 `max` 宏：

```cpp
#ifdef max
#undef max
#endif
```

文件会取消该宏定义。

## 使用示例

典型使用方式为：

```cpp
#include <windows.h>
#include <yggr/support/max_min_undef.ipp>

int value = std::min(left, right);
int limit = std::max(value, other);
```

包含 Windows 相关头文件后，`min` 和 `max` 可能已经被定义为宏；包含本文件可以恢复这些名称作为普通 C++ 标识符使用。

## 重复包含行为

该文件没有自己的 include guard，但每个宏清理操作都由 `#ifdef` 保护：

```cpp
#ifdef min
#undef min
#endif
```

因此重复包含不会产生额外错误。第一次包含清理宏，之后再次包含时由于宏已不存在，不执行任何操作。

## 设计特点

- 仅影响预处理器宏，不影响 `std::min` 或 `std::max` 的函数实现；
- 仅在 `_MSC_VER` 环境中生效；
- 只清理 `min` 和 `max`，不处理其他 Windows 宏；
- 不包含任何头文件；
- 不需要命名空间或运行时初始化。

## 使用限制与注意事项

- 该文件会主动取消当前作用域中的 `min` 和 `max` 宏，使用前应确认调用方不依赖这些宏。
- 它不会恢复被取消宏定义的原始内容，也不会重新定义兼容版本。
- 如果必须同时使用 Windows 宏形式和 C++ 函数形式，应在包含本文件前后明确控制宏状态。
- 该文件主要针对 MSVC/Windows 头文件冲突；非 MSVC 环境中包含它不会产生清理效果。
