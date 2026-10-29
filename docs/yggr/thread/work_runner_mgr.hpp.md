# `work_runner_mgr.hpp` 中文说明

## 用途

本头文件定义 `yggr::thread::work_runner<Data_Center>`，用于从数据中心获取条件任务、调度快速或慢速计算，并将计算结果写回数据中心。它还维护慢速任务等待队列及按任务 ID 提交的待取消请求。

```cpp
#include <yggr/thread/work_runner_mgr.hpp>
```

该实现依赖旧版 lock-free 队列和 CAS 接口；使用前需确认项目环境包含相应依赖。

## 模板参数与依赖

```cpp
template<class Data_Center>
class work_runner;
```

`Data_Center` 需提供任务 ID 和任务类型，并实现本类使用的条件任务读取及结果写入接口。任务类型需提供任务数据访问、ID、用户 ID 和快速/慢速条件类型等成员。计算操作还依赖源码中引用的 `base_runner_type`，它应提供计算及结果状态定义。

**源码完整性注意：**当前头文件的类模板只声明了一个模板参数 `Data_Center`，但使用了未在类内定义或声明的 `base_runner_type`；文件末尾的静态成员定义却使用 `work_runner<Data_Center, Base_Runner>` 两个模板参数。因而该文件当前内容存在明显的模板声明不一致，不能直接据此认为头文件可独立编译。应先确认是否缺少相关声明或该文件是否为未完成的旧实现，再依实际修复后的接口使用。

## 构造与生命周期

```cpp
work_runner(data_center_type& dc);
~work_runner();
```

构造时保存对数据中心的非拥有引用，因此数据中心必须比 runner 存活更久。析构函数调用 `clear()`，清除本地慢速任务列表和待取消任务 ID 队列。

该类私有继承 `boost::noncopyable`，不可复制。

## 任务驱动接口

源码意图提供一个无参数调用运算符，用于每轮调度任务：

1. 处理待取消任务 ID。
2. 如果本地慢速任务队列非空且有慢速任务槽位，则尝试执行队首任务。
3. 否则从数据中心提取条件任务。
4. 快速任务直接交给计算器；慢速任务在没有可用槽位时进入本地等待队列。

但当前源码将函数写为 `bool operator(void)`，而不是常规的 `bool operator()(void)`。调用接口的实际可用形式需以修正后的声明为准。

## 慢速任务并发控制

```cpp
static void init_slow_runner(u32 size);
static bool slow_runner_free();
static bool plus_slow_runner();
static void minus_slow_runner();
static bool slow_task_enable();
```

这些静态函数通过 `_slow_runner_size` 和 `_slow_runner` 维护慢速任务并发数量：

- `init_slow_runner(size)` 设置允许的慢速任务槽位数量。
- `slow_runner_free()` 检查当前计数是否小于限制。
- `plus_slow_runner()` 在有容量时尝试增加计数。
- `minus_slow_runner()` 在计数大于零时尝试减少计数。
- `slow_task_enable()` 检查慢速任务限制是否大于零。

计数是类模板静态成员，因此意图上由相同模板实例的 runner 共享。实现使用 CAS 更新共享计数，调用方需在合适的生命周期节点配对增加和减少操作。

## 取消排队任务

```cpp
void post_kill_task_id(const task_id_type& task_id);
```

该接口将任务 ID 加入 lock-free 队列。后续调度时，runner 会在本地慢速等待列表中查找并移除匹配任务。该请求不会中断已经开始计算的任务，也不会直接从数据中心删除任务。

## 结果处理

计算完成后，结果回调按状态写回数据中心：

- `E_CAL_CONDITION`：调用 `push_condition(task_id, uid, state, result)`。
- `E_CAL_RESULT`：调用 `push_result(task_id, uid, state, result)`。

计算出错时，代码尝试取得全局 `exception::Exception` 实例，并将错误投递给它；如果没有取得实例，则不会执行该投递。

## 清理

```cpp
void clear();
```

清理本地慢速等待队列和待取消 ID 队列。此接口不清除数据中心队列，也不表示正在运行的计算已停止。

## 使用前核对

由于当前文件存在模板参数、基类类型和调用运算符声明不一致的问题，建议在使用前核对：

- `base_runner_type` 应由何处定义以及如何传入。
- 类模板是否应包含 `Base_Runner` 参数。
- 调用运算符是否缺少 `operator()`。
- 慢速任务计数的初始化和递减是否在完整调度流程中正确配对。

本说明记录的是头文件中可见的设计意图；这些接口在源码问题修正并通过目标编译配置验证前，不应视为可直接使用的稳定 API。
