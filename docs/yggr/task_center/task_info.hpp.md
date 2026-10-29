# `task_info.hpp` 中文说明

## 用途

本头文件定义任务数据类型信息的发送/接收查询器，以及用于任务回调的宏。查询器通过模板参数指定的数据解析器，获取数据类型对应的元信息或类名。

```cpp
#include <yggr/task_center/task_info.hpp>
```

## 发送类型信息查询器

```cpp
template<
    typename TaskDataInfo,
    template<typename> class Parser =
        yggr::task_center::support::task_data_info_parser
>
struct task_data_send_typeid_getter;
```

该类型用于取得类型 `T` 的数据元信息：

```cpp
template<typename T>
result_type operator()() const;
```

`operator()<T>()` 转发到 `Parser<TaskDataInfo>::data_info<T>()`。其中：

- `task_data_info_type` 是 `TaskDataInfo`。
- `parser_type` 是 `Parser<TaskDataInfo>`。
- `result_type` 是解析器的 `data_info_type`。

## 接收类型信息查询器

```cpp
template<
    typename TaskDataInfo,
    template<typename> class Parser =
        yggr::task_center::support::task_data_info_parser
>
struct task_data_recv_typeid_getter;
```

该类型用于取得类型 `T` 对应的类名：

```cpp
template<typename T>
result_type operator()() const;
```

`operator()<T>()` 转发到 `Parser<TaskDataInfo>::class_name<T>()`。其中 `result_type` 是解析器的 `class_name_type`。

两个查询器默认使用 `support::task_data_info_parser`，也可以通过第二个模板参数替换为提供相应静态模板函数和类型别名的解析器。

## 回调宏 `YGGR_PP_TASK_CENTER_BACK_TASK`

```cpp
YGGR_PP_TASK_CENTER_BACK_TASK(
    handler, runner, err, run_type, uids, val)
```

该宏展开为一个代码块，执行以下操作：

1. 将 `val` 包装到临时 `yggr::any` 对象中。
2. 调用 `handler`，并传入运行器、错误信息、转换为 `u32` 的运行类型、UID 集合引用、任务数据元信息、计算类型，以及临时 `any` 的引用包装。

概念上的调用参数为：

```cpp
handler(
    runner,
    err,
    static_cast<yggr::u32>(run_type),
    boost::cref(uids),
    val.data_info(),
    val.cal_type(),
    boost::ref(tmp_any));
```

宏使用 `val.data_info()` 和 `val.cal_type()`，因此 `val` 必须提供这些成员；`handler` 也必须能接收上述参数。临时 `yggr::any` 的引用只在宏展开的当前代码块执行期间有效，不应保存其引用供后续使用。

## 使用示例

```cpp
typedef yggr::task_center::default_task_data_info_type task_info_type;

yggr::task_center::task_data_send_typeid_getter<task_info_type> send_info;
yggr::task_center::task_data_recv_typeid_getter<task_info_type> recv_info;

// 对已注册的任务数据类型 T 查询发送元信息和接收类名：
// auto data_info = send_info.template operator()<T>();
// auto class_name = recv_info.template operator()<T>();
```

具体可查询的类型及其映射关系由 `task_data_info_parser` 的注册和解析规则决定。
