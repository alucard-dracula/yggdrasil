# `ctrl_center.hpp` 中文说明

## 用途

`yggr::system_controller::ctrl_center` 是系统控制器的注册与分发中心。它按照“代码 + 类别代码”查找匹配项，并支持把处理器按模块 ID 分组注册。一次分发会依次调用匹配组中的控制器和分发器。

头文件位置：`yggr/system_controller/ctrl_center.hpp`

```cpp
#include <yggr/system_controller/ctrl_center.hpp>
```

## 类型

| 类型 | 定义及用途 |
| --- | --- |
| `code_type` | `u32`，事件/系统代码及类别代码的类型。 |
| `ctrl_id_type` | `u64`，由代码和类别代码组合得到的查找 ID。 |
| `modules_id_type` | `yggr::ids::inner_process_id`，注册所属模块的 ID。 |
| `handler_id_type` | `const std::type_info*`，用于按处理器类型标识注册项。 |
| `param_type` | `utility::basic_args_holder`，内部参数载体。 |
| `handler_wrap_type` | `yggr::any`，内部保存处理器对象。 |

## 注册键与模块

注册项以 `(modules_id, ctrl_id)` 为唯一键，其中 `ctrl_id` 由两个 32 位代码拼接：

```text
ctrl_id = (code << 32) | class_code
```

同一模块和同一代码组合共享一个处理器组；不同模块可以分别注册相同的代码组合。调用 `ctrl(code, class_code, ...)` 时，会查找所有模块中该代码组合对应的组并分发。

不指定模块 ID 的注册接口使用默认构造的 `modules_id_type`。带显式 `mid` 的重载可将处理器归属到指定模块。

## 主要接口

### 分发

```cpp
void ctrl(const code_type& code,
          const code_type& class_code,
          const Args&... args) const;
```

按照代码组合查找处理器组。如果没有匹配项则直接返回；有匹配项时，先对每个组调用控制器，再对每个组调用分发器。`args...` 会传给注册的处理器。

### 注册控制器和分发器

```cpp
template<typename Handler>
bool reg_ctrl(const code_type& code,
              const code_type& class_code,
              const Handler& handler);

template<typename Handler>
bool reg_ctrl(const modules_id_type& mid,
              const code_type& code,
              const code_type& class_code,
              const Handler& handler);

template<typename Handler>
bool reg_dis(const code_type& code,
             const code_type& class_code,
             const Handler& handler);

template<typename Handler>
bool reg_dis(const modules_id_type& mid,
             const code_type& code,
             const code_type& class_code,
             const Handler& handler);
```

- `reg_ctrl` 注册控制器；`reg_dis` 注册分发器。二者保存在不同的处理器集合中。
- 带 `mid` 的重载指定模块；不带 `mid` 的重载使用默认模块 ID。
- 返回值表示注册操作是否成功。无效的处理器包装、无法建立处理器组或底层容器操作失败时，可能返回 `false`。

### 注销处理器

```cpp
template<typename Handler>
bool unreg_ctrl(const code_type& code,
                const code_type& class_code,
                const Handler& handler);

template<typename Handler>
bool unreg_ctrl(const modules_id_type& mid,
                const code_type& code,
                const code_type& class_code,
                const Handler& handler);

template<typename Handler>
bool unreg_dis(const code_type& code,
               const code_type& class_code,
               const Handler& handler);

template<typename Handler>
bool unreg_dis(const modules_id_type& mid,
               const code_type& code,
               const code_type& class_code,
               const Handler& handler);
```

注销时需提供注册时对应的模块 ID、代码、类别代码和处理器。处理器的具体类型用于定位处理器集合，处理器对象的相等性用于匹配待删除项；使用不同类型或不相等的对象不能删除原注册项。成功删除后，如果该模块/代码组合的控制器和分发器均为空，对应处理器组也会从中心移除。

```cpp
bool unreg_dis(const modules_id_type& mid);
```

虽然接口名为 `unreg_dis`，此重载会从中心移除指定模块的全部处理器组，因此该模块的控制器和分发器注册都会一并删除。

### 批量注册、注销与清空

```cpp
template<typename Sys_Obj>
ctrl_center& register_controller(Sys_Obj& obj);

template<typename Dis_Obj>
ctrl_center& register_dispatchers(Dis_Obj& obj);

template<typename Sys_Obj>
ctrl_center& unregister_controller(Sys_Obj& obj);

template<typename Dis_Obj>
ctrl_center& unregister_dispatchers(Dis_Obj& obj);

ctrl_center& unregister_module();
void clear();
```

- `register_controller`、`register_dispatchers` 和对应的注销接口委托给对象的 `register_controller<Tag, ctrl_center>` 或 `register_dispatchers<Tag, ctrl_center>` 成员函数，返回 `*this` 以支持链式调用。
- 这些接口也提供显式 `Tag` 的模板重载，用于指定对象内部使用的操作标签。
- `unregister_module()` 从 `ptr_single<modules_id_type>` 获取当前模块 ID，并移除该模块的全部处理器组（包括控制器和分发器）。
- `clear()` 清空中心中的所有模块、控制器和分发器注册。

## 使用示例

处理器应能以分发时传入的参数调用。以下示例注册一个接收 `int` 参数的控制器：

```cpp
#include <yggr/system_controller/ctrl_center.hpp>

struct example_handler
{
    void operator()(int value) const
    {
        // 处理 value
    }
};

void example()
{
    yggr::system_controller::ctrl_center center;
    const yggr::system_controller::ctrl_center::code_type code = 0x7001;
    const yggr::system_controller::ctrl_center::code_type class_code = 1;

    center.reg_ctrl(code, class_code, example_handler());
    center.ctrl(code, class_code, 42);
    center.unreg_ctrl(code, class_code, example_handler());
}
```

也可以注册分发器：

```cpp
center.reg_dis(code, class_code, example_handler());
center.ctrl(code, class_code, 42);
center.unreg_dis(code, class_code, example_handler());
```

## 注册标签与 `CTRL_HANDLER`

类中公开了 `tag_reg_ctrl`、`tag_reg_dis`、`tag_unreg_ctrl` 和 `tag_unreg_dis` 类型别名，供具有模板化注册/注销成员函数的系统对象使用。注册标签会先检查 `ptr_single<modules_id_type>` 中是否存在当前模块 ID：存在时调用带模块 ID 的接口，不存在时使用默认模块 ID。

头文件还定义了 `CTRL_HANDLER(tag, ctrl_obj, code, class_code, handler)` 宏，通过 `obj_creator<tag>::create()` 创建标签对象并调用它。调试构建中调用结果会用 `assert` 检查；非调试构建中宏直接返回调用结果。

## 注意事项

- `code` 和 `class_code` 均为 32 位代码，分发时两者必须与注册时一致。
- 控制器和分发器是两类独立注册项；`ctrl()` 会按“先控制器、后分发器”的顺序执行。
- 处理器必须支持以 `ctrl()` 传入的参数进行调用。具体的参数封装和调用适配由内部控制器实现。
- `unregister_module()` 依赖 `ptr_single<modules_id_type>` 提供的当前模块 ID；未设置时会使用默认构造的模块 ID。
