# `task_real_data_getter.hpp` 中文说明

## 用途

本头文件声明 `yggr::task_center::support::task_real_data_getter<Task>`，作为从任务对象获取实际数据的扩展点。通用模板不提供可用实现；应由具体任务类型或其他头文件提供特化。

```cpp
#include <yggr/task_center/support/task_real_data_getter.hpp>
```

## 通用模板

在非 MSVC 编译器下，通用模板仅作前置声明：

```cpp
template<typename Task>
struct task_real_data_getter;
```

因此直接实例化通用模板时会因类型不完整而无法使用。需要对 `Task` 提供特化，并实现所需的数据访问接口。

在 MSVC 下，头文件提供一个带编译期断言失败的定义，以明确禁止误用；其成员模板只声明、不定义：

```cpp
template<typename Real_Data>
Real_Data* operator()(task_type& task) const;

template<typename Real_Data>
const Real_Data* operator()(const task_type& task) const;
```

这些接口仅作特化形式的示意，通用模板本身不可用。

## `task` 的特化

包含 `yggr/task_center/task.hpp` 后，该头文件会为 `yggr::task_center::task<...>` 提供专用特化。它通过任务的 `get<Real_Data>()` 取得数据：

- 非常量任务返回 `Real_Data*`。
- 常量任务返回 `const Real_Data*`。

这使泛型代码可以通过 `task_real_data_getter<Task>` 访问 `task` 内部的数据，而无需依赖其具体数据容器。

## 使用示意

```cpp
typedef yggr::task_center::support::task_real_data_getter<my_task>
    real_data_getter;

my_task& task = /* 已创建的任务 */;
payload_type* payload =
    real_data_getter().template operator()<payload_type>(task);
```

使用前应确认 `my_task` 已有对应的 `task_real_data_getter` 特化，并检查返回指针是否为空。直接包含本头文件但未提供特化时，不能通过该接口取得数据。

## 设计说明

源码注释指出，通用模板没有直接写成始终失败的 `BOOST_MPL_ASSERT(false)` 或 `static_assert(false)`，以兼顾编译器支持。MSVC 分支通过依赖模板参数的断言标记误用；其他编译器保留未定义的主模板，等待合法特化。
