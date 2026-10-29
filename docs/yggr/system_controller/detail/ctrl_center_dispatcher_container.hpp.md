# `ctrl_center_dispatcher_container.hpp` 中文说明

## 用途

`yggr::system_controller::detail::dispatcher_container` 是控制中心内部的处理器容器。它按处理器类型保存多态控制器对象，并在收到分发参数时，逐个调用对应的控制器。

```cpp
#include <yggr/system_controller/detail/ctrl_center_dispatcher_container.hpp>
```

该头文件位于 `detail` 目录，属于实现细节。应用代码通常通过 `yggr::system_controller::ctrl_center` 注册、注销和调用处理器。

## 类型别名

| 别名 | 来源 | 用途 |
| --- | --- | --- |
| `handler_id_type` | `ctrl_center_types_def::handler_id_type` | 处理器类型 ID，定义为 `const std::type_info*`。 |
| `param_type` | `ctrl_center_types_def::param_type` | 内部调用参数载体。 |
| `handler_wrap_type` | `ctrl_center_types_def::handler_wrap_type` | 处理器包装类型，定义为 `yggr::any`。 |

内部容器 `_cont` 是一个 `safe_unordered_map`，以处理器类型 ID 为键，以 `base_controller` 智能指针为值。每个值指向对应 `Handler` 实例化的控制器对象；实际处理器对象保存在该控制器内部。

## 分发接口

```cpp
template<typename... Args>
void dispatch(const Args&... args) const;
```

`dispatch()` 的工作流程如下：

1. 创建本地控制器指针列表，并根据映射大小预留空间。
2. 通过安全容器的 `using_handler()` 收集当前控制器。
3. 遍历本地列表，跳过空指针。
4. 对每个有效控制器调用 `base_controller::ctrl(args...)`。

具体参数的封装、类型匹配和处理器调用由 `base_controller` 及其派生类负责。未定义 `YGGR_NO_CXX11_VARIADIC_TEMPLATES` 时使用可变参数模板；否则通过预处理器生成支持的参数数量范围内的重载。

## 状态管理接口

```cpp
bool empty() const;
void clear();
```

- `empty()` 在按类型保存的控制器映射为空时返回 `true`。
- `clear()` 清空映射及其控制器对象引用。

## 注册接口

```cpp
template<typename Ctrl>
bool append_dispatcher(const handler_wrap_type& handler_wrap);
```

`Ctrl` 是控制器实现类型，应具有 `handler_type` 类型别名。此接口使用 `typeid(Ctrl::handler_type)` 的地址作为键，并通过安全容器的 `using_handler()` 执行插入操作：

- 若该类型的控制器已存在，则调用其 `insert(handler_wrap)`，将处理器加入已有控制器。
- 若尚不存在，则以 `handler_wrap` 构造新的 `Ctrl`，并将其存为 `base_controller` 指针。

返回值表示插入或加入处理器是否成功。访问底层安全映射时若发生 `yggr::stl_exception`，内部插入回调返回 `false`。

## 注销接口

```cpp
bool erase_dispatcher(handler_id_type id,
                      const handler_wrap_type& handler_wrap);
```

该接口按处理器类型 ID 查找对应控制器，再委托内部删除回调处理包装中的处理器。实际删除规则由 `base_controller` 的派生类实现；相关实现可能在处理器集合变空后移除对应类型项。

## 内部辅助函数

以下函数是容器的实现细节：

```cpp
static bool handler_append_dispatcher(...);
static bool handler_erase_dispatcher(...);
static ctrl_cont_type& handler_get_dispatchers(...);
```

- `handler_append_dispatcher<Ctrl>()` 获取或创建某种处理器类型的控制器，并插入处理器包装。
- `handler_erase_dispatcher()` 处理已找到项的删除操作。
- `handler_get_dispatchers()` 将控制器映射中的控制器指针收集到临时列表，供 `dispatch()` 遍历。

## 与处理器组的关系

`controller_group` 分别持有用于控制器和分发器的 `dispatcher_container`。这两个容器的实现相同，但由不同的组成员独立管理；组的 `ctrl()` 和 `dispatch()` 分别将参数转交给对应容器。
