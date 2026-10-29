# `action_id_parser.hpp` 中文说明

## 用途

本头文件定义 `yggr::thread::action_helper::action_id_parser<Reg_ID_Parser, Task_ID_Parser>`，组合“注册类型 ID 解析器”和“任务 ID 解析器”，为动作表提供统一的 ID 类型及获取注册键、任务键的接口。

```cpp
#include <yggr/thread/action_helper/action_id_parser.hpp>
```

## 模板参数

| 参数 | 说明 |
| --- | --- |
| `Reg_ID_Parser` | 注册 ID 解析器，需提供 `result_type`，并支持无参数模板调用 `operator()<T>()`。 |
| `Task_ID_Parser` | 任务 ID 解析器，需提供 `result_type`，并支持以任务对象调用 `operator()(task)`。 |

解析器的 `result_type` 可为值类型或带 `const`、引用限定的类型；本模板会移除顶层 `const` 和引用，生成规范化的 `id_type`。

## 编译期类型约束

`Reg_ID_Parser::result_type` 与 `Task_ID_Parser::result_type` 去除 `const` 和引用后的类型必须一致，或满足源码断言所检查的 `const` 类型兼容关系。若两种解析器产生的 ID 类型不兼容，实例化时会触发编译期断言。

公开别名：

```cpp
typedef reg_id_type id_type;
```

`id_type` 即规范化后的注册 ID 类型，用作动作表的索引键。

## 获取注册 ID

```cpp
template<typename T>
static typename Reg_ID_Parser::result_type get_reg_id();
```

该函数默认构造 `Reg_ID_Parser`，再调用其 `operator()<T>()`，返回类型 `T` 对应的注册 ID。

## 获取任务 ID

```cpp
template<typename Task>
static typename Task_ID_Parser::result_type
get_task_id(const Task& task);
```

该函数默认构造 `Task_ID_Parser`，并将任务对象传给解析器的 `operator()(task)`，取得任务对应的查找 ID。

## 解析器实现要求

两个解析器均在调用时默认构造，因此需支持默认构造。示意：

```cpp
struct my_reg_id_parser
{
    typedef my_id_type result_type;

    template<typename T>
    result_type operator()() const;
};

struct my_task_id_parser
{
    typedef my_id_type result_type;

    template<typename Task>
    result_type operator()(const Task& task) const;
};

typedef yggr::thread::action_helper::action_id_parser<
    my_reg_id_parser,
    my_task_id_parser
> my_action_id_parser;
```

两个解析器的返回 ID 类型必须与动作表注册、任务执行时使用的键类型相匹配。

## 与动作表的关系

动作表通过 `get_reg_id<RealData>()` 生成计算器注册键，通过 `get_task_id(task)` 取得任务查找键。二者必须表示同一套 ID 空间，才能在任务执行时找到已注册的计算器。
