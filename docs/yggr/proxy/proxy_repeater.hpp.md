# proxy_repeater.hpp — 中文文档

概览
- `proxy_repeater` 是一个模板化的“转发/重放”组件，用于在代理框架中将接收到的 `ProxyData` 按规则路由到目标所有者集合，并支持注册/注销计算器（calculator）回调。
- 它基于 `detail::basic_repeater` 提供的基础路由与共享信息，增加了参数构建器、特殊数据过滤与注册帮助器等功能。

模板参数
- `SharedInfoPtr`：共享信息指针类型（通常为 `shared_ptr` 指向上下文/路由等）。
- `ProxyData`：代理数据类型，需定义 `data_info_type`。
- `OwnerInfoContainer`：所有者信息容器类型，元素类型为 `owner_info_type`。
- `Error`：错误类型（库内部使用）。
- `Router`：路由器类型（由 base repeater 使用）。
- `ArgsBuilder`：参数构造器类型，默认 `proxy_args::default_args_builder<owner_info_type, ProxyData>`，用于基于不同规则构造路由参数。
- `Special_Data_Filter`：特殊数据过滤器类型，默认 `proxy_special_data_filter<ProxyData, void>`，用于控制哪些数据需要注册/处理。

主要类型别名
- `data_info_type`：来自 `ProxyData::data_info_type`。
- `owner_info_type`：`OwnerInfoContainer` 的元素类型。
- `base_type`：`detail::basic_repeater<SharedInfoPtr, Router>`。
- `args_builder_ptr_type`：参数构造器的共享指针从 `args_builder_wrap_type` 获取。

功能摘要
- 注册/注销计算器（calculator）回调：
  - 提供多种重载的 `register_repeater_data` 接口，支持按照类型 `RealData` 自动注册、按 `data_info` 注册、带固定处理器（fix handler）或 SEH 处理器等变体。
  - `unregister_repeater_data` 提供按类型或按 `data_info` 注销。
  - 注册过程通过内部 `reg_cal_obj_helper` 模板结构将回调绑定到 `handler_proxy_data`，使收到计算结果时能调用 `proxy_repeater` 的路由逻辑。

- 特殊数据管理：
  - 通过 `_sp_data_filter` 提供 `append_special_data`, `remove_special_data`, `clear` 等方法控制哪些数据项走特殊处理流程。

- 参数构建与路由：
  - `route` / `handler_proxy_data` 方法使用 `_args_builder_wrap` 中的构造器来将 `ProxyData` 转换为适用于 `base_type::get_router()` 的参数，然后调用传入的 `handler`。
  - 支持变参的 `modify_args_build_rule` 与 `check_args_build_rule`（C++11 可变模板或宏展开的场景）以定制参数构造规则。

设计要点与实现细节
- `reg_cal_obj_helper` 与 `unreg_cal_obj_helper`：一组模板化辅助结构，用于适配不同注册签名（带/不带 RealData、Fix_Handler、Seh_Handler 等），并封装对 `Action_Table` 的调用。
- 使用 `proxy_special_data_filter` 在注册前进行过滤，避免不必要的注册。
- 基于 `args_builder_wrap_type`（线程安全 wrapper）获取 `args_builder_ptr_type` 后通过 `route` 方法将路由请求转发到 `base_type` 的路由器。
- 继承 `boost::enable_shared_from_this` 以便在绑定回调时安全地传递 `shared_from_this()`。

依赖与包含
- `yggr` 内部组件：`detail/basic_repeater.hpp`, `proxy_args/default_args_builder.hpp`, `proxy_special_data_filter.hpp`，以及 `safe_wrap` 与 `args_holder` 等实用工具。
- Boost：`enable_shared_from_this`, `unordered_set`, `type_traits`, `range` 功能。

注意事项
- `register_repeater_data` 的模板多样性使接口非常灵活，但调用者需确保提供的 `Fix_Handler`（及可选 `Seh_Handler`）签名与 `Action_Table` 的 `register_calculator`/`register_calculator_of_container` 接口一致。
- `_args_builder_wrap` 为空时 `handler_proxy_data` 将不执行 routing；确保在构造时注入 `args_builder`（构造函数接受 `pargs_builder`）。
- `proxy_repeater` 主要作为中间层，不直接处理计算逻辑，而是将计算器结果通过回调路由到最终处理者。

示例（伪代码）
- 创建并注册：
  - `auto repeater = construct_shared<proxy_repeater>(siptr, prouter, pargs_builder);`
  - `repeater->register_repeater_data<MyRealData>(fix_handler);`
- 路由调用：
  - `repeater->route(owner, proxy_data, runner, handler, cal_t);`

参考
- 源文件： `proxy_repeater.hpp`
