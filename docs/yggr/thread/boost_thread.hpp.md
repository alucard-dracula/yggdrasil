# `boost_thread.hpp` 中文说明

## 用途

本头文件是项目对 Boost.Thread 的包含适配层。它处理部分平台兼容依赖，并包含 Boost.Thread 主头文件，使项目其他线程组件可以统一通过 Yggdrasil 路径引入 Boost.Thread。

```cpp
#include <yggr/thread/boost_thread.hpp>
```

## 包含内容

头文件主要执行以下操作：

- 包含 Yggdrasil 基础定义 `yggr/base/yggrdef.h`。
- 在 Windows 平台且未定义 `BOOST_USE_WINDOWS_H` 时，包含 `yggr/network/socket_conflict_fixer.hpp`，用于处理网络相关头文件与 Windows 定义的冲突。
- 对 `BOOST_VERSION >= 106500` 的 Boost 版本，额外包含 `boost/detail/lightweight_mutex.hpp`。
- 最后包含 `<boost/thread.hpp>`，提供 Boost.Thread 接口。

## 平台与版本说明

- Windows 平台的冲突处理由 `YGGR_AT_WINDOWS` 和 `BOOST_USE_WINDOWS_H` 宏控制。
- `BOOST_VERSION` 用于决定是否单独包含 lightweight mutex 头文件；低于 `106500` 时跳过该包含。
- 此文件只负责头文件适配，不定义新的线程类或线程函数，也不创建、启动或管理线程。

## 使用建议

依赖 Boost.Thread 的 Yggdrasil 组件可包含此头文件，以复用项目统一的平台适配逻辑。直接使用 Boost.Thread 的类型时，仍需按项目构建配置链接相应依赖库。
