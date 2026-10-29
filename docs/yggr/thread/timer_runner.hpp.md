# `timer_runner.hpp` 中文说明

## 用途

`yggr::thread::timer_runner<Timer_Mgr>` 是对定时器管理器的轻量包装器。每次调用 runner 时，它调用一次定时器管理器，并返回 `false`。

```cpp
#include <yggr/thread/timer_runner.hpp>
```

本头文件只声明包装器本身，没有包含其依赖类型的定义。使用前应先包含提供 `nonable::noncopyable` 及具体 `Timer_Mgr` 定义的头文件。

## 模板参数

| 参数 | 说明 |
| --- | --- |
| `Timer_Mgr` | 定时器管理器类型，需提供可调用的 `operator()()`。 |

公开别名 `timer_mgr_type` 对应模板参数 `Timer_Mgr`。

## 构造与生命周期

```cpp
explicit timer_runner(timer_mgr_type& timer_mgr);
```

构造函数保存定时器管理器的引用，不复制或拥有管理器。被引用的管理器必须比 `timer_runner` 存活更久。

`timer_runner` 私有继承 `nonable::noncopyable`，不可复制；析构函数不执行额外清理。

## 调用接口

```cpp
bool operator()();
```

调用时按以下顺序执行：

1. 调用 `_timer_mgr()`。
2. 返回 `false`。

该返回值不是定时器管理器执行结果，而是 runner 的固定返回值。管理器的返回值（若有）会被忽略。

## 使用示意

```cpp
timer_mgr_type timer_mgr(/* 初始化参数 */);
yggr::thread::timer_runner<timer_mgr_type> runner(timer_mgr);

bool result = runner(); // 执行 timer_mgr() 后返回 false
```

示例中的 `timer_mgr_type` 应替换为项目使用的定时器管理器类型。
