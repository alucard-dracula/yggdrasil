# `ctrl_center_controller.hpp` 中文说明

## 用途

本头文件定义系统控制中心内部的处理器抽象和模板实现，负责保存同类型处理器、校验处理器包装、匹配分发参数并调用处理器。主要类型位于 `yggr::system_controller::detail` 命名空间：

- `base_controller`：处理器调用与管理的抽象接口。
- `controller<Handler>`：针对具体处理器类型的实现。
- `handler_caller`：按处理器参数数量和分发参数类型执行调用。
- `base_controller_handler_wrap_op_helper`：用于比较包装后的处理器。

```cpp
#include <yggr/system_controller/detail/ctrl_center_controller.hpp>
```

该头文件位于 `detail` 目录，通常不应作为应用层直接使用的 API；处理器注册和分发建议通过 `yggr::system_controller::ctrl_center` 完成。

## 参数与处理器包装

`base_controller` 使用 `ctrl_center_types_def` 定义的类型：

| 类型 | 用途 |
| --- | --- |
| `param_type` | `utility::basic_args_holder`，封装一次分发的参数。 |
| `handler_wrap_type` | `yggr::any`，保存具体处理器对象。 |

调用 `base_controller::ctrl(args...)` 时，可变参数会先封装为 `packet::packet_info<Args...>`，再转换为 `param_type` 并交给虚函数 `pro_ctrl()`。禁用 C++11 可变参数模板时，头文件使用预处理器生成有限参数数量的重载。

## `base_controller` 抽象接口

`base_controller` 是多态基类，定义处理器实现必须提供的操作：

```cpp
virtual bool insert(const handler_wrap_type& handler_wrap) = 0;
virtual std::size_t erase(const handler_wrap_type& handler_wrap) = 0;
virtual void clear() = 0;

virtual const std::type_info& type() const = 0;
virtual bool empty() const = 0;

template<typename... Args>
void ctrl(const Args&... args) const;
```

`ctrl()` 负责构造参数包并转调受保护的纯虚函数 `pro_ctrl(const param_type&)`。`type()` 返回所管理处理器类型的 `type_info`；`empty()`、`insert()`、`erase()` 和 `clear()` 分别用于查询、修改和清理处理器集合。

该头文件还定义了 `base_ctrl_ptr_type`，即指向 `base_controller` 的 `shared_ptr`，供内部容器通过基类指针保存不同模板实例。

## `controller<Handler>`

`controller<Handler>` 继承 `base_controller`，内部用 `safe_vector<yggr::any>` 保存 `Handler` 实例。模板通过 `func::foo_t_info<Handler>` 获取处理器的参数数量和参数类型信息，并据此确定用于接收分发数据的 `packet_info` 类型。

### 构造

提供三种构造方式：

```cpp
controller(Handler&& handler);
controller(const Handler& handler);
controller(const handler_wrap_type& handler_wrap);
```

前两种直接保存处理器对象（右值形式会移动保存）；第三种从 `any` 包装中保存处理器。

### 管理操作

- `insert(handler_wrap)`：仅接受内部实际保存类型为当前 `Handler` 的包装。若集合中已存在相等处理器则不重复插入；成功加入时返回 `true`。
- `erase(handler_wrap)`：按当前处理器类型和处理器相等性删除匹配项，返回删除数量（实现中为 `0` 或 `1`）。
- `clear()`：清空处理器集合。
- `type()`：返回 `typeid(Handler)`。
- `empty()`：判断集合是否为空。

包装中实际保存的类型与当前 `Handler` 不完全相同时会被拒绝，插入返回 `false`，删除返回 `0`。

### 处理器调用

`pro_ctrl()` 会先将安全向量内容复制到本地容器，然后调用 `handler_caller`。该快照使本次遍历基于取出的处理器列表进行；之后按 `Handler` 的参数数量和类型处理参数，并对集合中的处理器逐个调用。

- **零参数处理器**：不需要从参数载体提取参数，直接逐个调用 `operator()()`。
- **一个或多个参数的处理器**：将参数载体转换为相应的 `packet_info` 类型；转换失败时不调用处理器，直接返回。成功后使用 `caller::to_params_call` 将参数展开并调用每个处理器。

处理器签名应与注册后分发时提供的参数类型和数量兼容。处理器的参数数量不得超过 `YGGR_PP_FOO_DEFAULT_PARAMS_LEN`，否则模板静态断言会导致编译失败。

## 比较辅助类

```cpp
template<typename Handler>
class base_controller_handler_wrap_op_helper;
```

该辅助对象保存待比较的 `handler_wrap_type` 引用，可作为查找谓词比较两个包装中的 `Handler` 对象。比较时优先识别相同对象地址；若对象是不同实例，则通过 `Handler` 的相等比较判断是否为同一处理器。使用涉及该比较的插入/删除路径时，处理器类型需支持相应的相等比较。

## 参数转换配置

宏 `YGGR_CTRL_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST` 控制多参数处理器的参数载体转换方式：

- 未定义时默认为 `0`，使用 `args_holder_ref_wrap_dynamic_cast`。
- 定义为非零时使用 `args_holder_ref_wrap_static_cast`。

此设置影响 `handler_caller` 对参数包类型的转换。零参数处理器不需要执行该转换。

## 调用流程概览

1. 上层通过 `ctrl_center` 按代码及模块找到处理器组。
2. 处理器容器将调用参数传给对应的 `base_controller::ctrl()`。
3. 参数被封装为 `packet_info`，并转为 `param_type`。
4. `controller<Handler>::pro_ctrl()` 取得处理器集合快照。
5. `handler_caller` 按处理器签名匹配并逐一调用处理器。
