# `boost_thread_config.hpp` 中文说明

## 用途

本头文件定义 `yggr::thread::boost_thread_config_type`，将 Boost.Thread 的线程、线程组和 Yggdrasil 时间类型组合为统一的线程配置类型。

```cpp
#include <yggr/thread/boost_thread_config.hpp>
```

## 配置内容

配置使用以下类型：

| 配置项 | 类型 |
| --- | --- |
| 线程类型 | `boost::thread` |
| 线程组类型 | `boost::thread_group` |
| 时间类型 | `yggr::time::time` |

当未定义 `YGGR_TPL_CONFIG_USING_TPL` 时，使用 `YGGR_PP_THREAD_CONFIG_DECL` 宏声明 `boost_thread_config_type`；定义该宏时，则直接将其定义为：

```cpp
typedef thread_config<
    boost::thread,
    boost::thread_group,
    time::time
> boost_thread_config_type;
```

两种分支都表达相同的配置组合，区别在于项目线程配置的声明方式。

## 依赖

该头文件包含 Boost.Thread 适配、通用 `thread_config` 定义以及 Yggdrasil 时间类型。使用前应确保项目已配置相应的 Boost.Thread 依赖。

## 使用示例

```cpp
typedef yggr::thread::boost_thread_config_type thread_config_type;

// 将 thread_config_type 用作依赖线程配置的组件的配置类型。
```

本头文件只提供类型别名，不会创建线程、线程组或启动任何线程。
