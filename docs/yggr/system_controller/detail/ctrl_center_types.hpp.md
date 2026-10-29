# `ctrl_center_types.hpp` 中文说明

## 用途

本头文件定义系统控制中心内部共享的类型集合 `yggr::system_controller::detail::ctrl_center_types_def`。控制中心、控制器及处理器容器使用这些别名，统一代码 ID、模块 ID、处理器标识和参数包装的类型。

```cpp
#include <yggr/system_controller/detail/ctrl_center_types.hpp>
```

此头文件位于 `detail` 目录，主要供系统控制器内部实现使用。

## 类型别名

| 别名 | 定义 | 用途 |
| --- | --- | --- |
| `code_type` | `u32` | 事件代码和类别代码。 |
| `ctrl_id_type` | `u64` | 组合后的控制器查找 ID。 |
| `modules_id_type` | `yggr::ids::inner_process_id` | 注册项所属的模块 ID。 |
| `handler_id_type` | `const std::type_info*` | 处理器类型标识，供按类型索引处理器。 |
| `param_type` | `utility::basic_args_holder` | 分发调用的参数载体。 |
| `handler_wrap_type` | `yggr::any` | 保存类型擦除后的处理器对象。 |
| `bad_cast_type` | `utility::bad_args_holder_cast` | 参数载体类型转换失败时对应的异常类型。 |

## 类型之间的关系

- `code_type` 表示独立代码值；控制中心使用代码和类别代码生成 `ctrl_id_type` 进行查找。
- `modules_id_type` 与控制器 ID 一起用于区分不同模块下的注册。
- `handler_id_type` 标识处理器的 C++ 类型；处理器对象本身则保存在 `handler_wrap_type` 中。
- 调用参数通过 `param_type` 在通用控制器接口间传递；具体处理器根据自身签名将参数载体转换为相应的参数包。

## 使用范围

`ctrl_center_types_def` 提供的是实现层的基础类型定义，而不是独立的注册或分发 API。通常使用 `yggr::system_controller::ctrl_center` 的公开接口即可，无需在业务代码中直接操作这些别名。
