# `ctrl_center_controller_group.hpp` 中文说明

## 用途

`yggr::system_controller::detail::controller_group` 是控制中心内部使用的处理器组。每组分别维护两类处理器集合：

- **控制器集合**：通过 `ctrl()` 触发。
- **分发器集合**：通过 `dispatch()` 触发。

组本身负责把调用转交给对应的 `dispatcher_container`，而处理器类型管理、插入和删除由容器负责。

```cpp
#include <yggr/system_controller/detail/ctrl_center_controller_group.hpp>
```

此头文件位于 `detail` 目录，属于实现细节；通常应通过公开接口 `yggr::system_controller::ctrl_center` 使用控制器功能。

## 类型别名

| 别名 | 来源 | 用途 |
| --- | --- | --- |
| `dispatcher_container_type` | `dispatcher_container` | 内部处理器容器类型。 |
| `handler_id_type` | `dispatcher_container_type::handler_id_type` | 处理器类型标识；对应 `const std::type_info*`。 |
| `param_type` | `dispatcher_container_type::param_type` | 内部参数载体类型。 |
| `handler_wrap_type` | `dispatcher_container_type::handler_wrap_type` | 处理器包装类型；对应 `yggr::any`。 |

## 状态管理接口

```cpp
bool empty() const;
void clear();
void clear_controller();
void clear_dispatcher();
```

- `empty()` 仅当控制器集合和分发器集合都为空时返回 `true`。
- `clear()` 清空两类集合。
- `clear_controller()` 只清空控制器集合。
- `clear_dispatcher()` 只清空分发器集合。

## 调用接口

```cpp
template<typename... Args>
void ctrl(const Args&... args) const;

template<typename... Args>
void dispatch(const Args&... args) const;
```

`ctrl(args...)` 将参数转交给控制器容器；`dispatch(args...)` 将参数转交给分发器容器。两者都不会在本类中直接执行处理器逻辑，而是由底层容器查找并调用已注册的处理器。

当未定义 `YGGR_NO_CXX11_VARIADIC_TEMPLATES` 时使用可变参数模板；禁用可变参数模板的平台则通过预处理器按项目支持的参数数量生成重载。

## 注册与注销接口

```cpp
template<typename Ctrl>
bool append_controller(const handler_wrap_type& handler_wrap);

template<typename Dis>
bool append_dispatcher(const handler_wrap_type& handler_wrap);

bool erase_controller(handler_id_type id,
                      const handler_wrap_type& handler_wrap);

bool erase_dispatcher(handler_id_type id,
                      const handler_wrap_type& handler_wrap);
```

- `append_controller<Ctrl>()` 将指定控制器类型对应的处理器加入控制器集合。
- `append_dispatcher<Dis>()` 将指定分发器类型对应的处理器加入分发器集合。
- `erase_controller()` 按处理器类型 ID 和包装对象，从控制器集合中删除匹配处理器。
- `erase_dispatcher()` 按处理器类型 ID 和包装对象，从分发器集合中删除匹配处理器。
- 返回值表示底层插入或删除操作的结果。

## 内部结构

`controller_group` 持有两个独立的 `dispatcher_container_type` 成员：`_ctrl_cont` 和 `_dis_cont`。这使两种处理器可以分别注册、调用、清空和注销；调用顺序由外层控制中心决定，本类的 `ctrl()` 与 `dispatch()` 是分开的入口。

## 与控制中心的关系

`ctrl_center` 按模块和代码组合定位处理器组，再通过组接口分发。每个组中的控制器和分发器分别保存在独立容器中；控制中心执行匹配项时，先调用控制器，再调用分发器。该分组类不负责模块 ID 或代码匹配。
