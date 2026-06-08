# proxy_dispatcher.hpp — 中文文档

概览
- `proxy_dispatcher` 是代理层的调度器模板类，负责将任务根据 `id` 分派到对应的 `proxy_container`（每个 `proxy_container` 管理特定 ID 的代理实例与所有者集合）。
- 它维护 `dispatch_map`（ID -> `proxy_container`）与 `proxy_val_map`（proxy value -> 所属 `proxy_container` 集合），支持注册、注销、清理、模式切换以及发送（dispatch）到代理或回传给后端。

模板参数
- `ID_Parser`: ID 解析器类型，需提供静态接口 `get_reg_id<RealData>()`（获取注册 ID）与 `get_task_id(task)`（从任务提取任务 ID）。
- `ProxyContainer`: 管理单个 ID 下代理实例的容器类型（通常为 `proxy_container<...>`），应导出若干类型别名（例如 `id_type`, `task_type`, `owner_info_type`, `value_type`, `fix_state_def_type` 等）。

重要类型别名
- `proxy_container_ptr_type`：`shared_ptr<ProxyContainer>`。
- `task_type`：任务类型，由 `ProxyContainer` 导出。
- `owner_info_type` / `owner_info_container_type`：所有者信息及其容器类型。
- `proxy_val_type`：代理值类型（`ProxyContainer::value_type`），用于按代理值索引与查找。
- `dispatch_map_type`：线程安全的 `safe_unordered_map<id_type, proxy_container_ptr_type>`，管理 ID 到容器的映射。
- `proxy_val_map_type`：键为 `proxy_val_type` 值为 `dispatch_set_type`（`unordered_set<proxy_container_ptr_type>`）的映射，用于根据代理值查找关联容器集合。

主要功能
- 注册与交换模式：
  - `register_dispatch`：将 `proxy_val` 注册到指定 `id` 的 `proxy_container`，若对应容器不存在则创建；并在 `_proxy_val_map` 中记录引用关系。支持以模板 `RealData` 自动解析 `id` 的重载。
  - `exchange_mode`：请求指定 `id` 的容器切换到新模式（委托 `proxy_container::exchange_mode`），当模式确实变更时可触发用户提供的 `handler` 回调。

- 注销与清理：
  - `remove_dispatch`：从指定 `id` 的容器移除 `proxy_val`，若容器变空则触发 `handler` 并从 `_dis_map` 删除容器，并从 `_proxy_val_map` 清理引用。
  - `clear_dispatch_of_id` / `clear_dispatch_of_proxy`：按 ID 或代理值清理注册信息（支持可选回调用于销毁时额外处理）。
  - `clear`：原子地交换出当前 `_dis_map` 与 `_proxy_val_map` 并返回，以便外部批量处理与清理。

- 发送(dispatch)逻辑：
  - `send(const owner_info_type& owner, const task_type& proxy_data, Runner*, Handler, u32 cal_type)`：根据 `owner` 判断数据应发送到后端还是转发给代理：
    - `is_proxy_data(owner)` 为 true 时调用 `send_to_back`（回传给后端）；否则调用 `send_to_proxy`（根据 `proxy_data` 的任务 ID 找到对应 `proxy_container` 并调用其 `send`）。
  - `send_to_back`：将 `owner` 中的顶层信息移除后封装为 `owners` 并直接调用 `handler` 返回结果（用于回传）。
  - `send_to_proxy`：通过 `ID_Parser::get_task_id` 获取 `id`，从 `_dis_map` 查找容器并调用容器的 `send`。

内部辅助与并发处理
- `_dis_map` 与 `_proxy_val_map` 均为线程安全容器（`safe_unordered_map`），很多操作使用 `find` / `using_handler` 提供的回调在内部的 base 映射上执行以避免持有锁的复杂性。
- 当为新的 `id` 创建 `proxy_container` 时，使用 `construct_shared` + `yggr_nothrow_new` 包裹，并在发生异常时返回失败状态（`fix_state_def` 中定义的错误码）。
- `_proxy_val_map` 的值为 `dispatch_set_type`，即一组 `proxy_container` 的弱引用集合（实际为 `shared_ptr`），用于管理哪个容器包含某个 `proxy_val`。

错误/边界情况处理
- 在 `handler_register_dispatch` 中，如果 `_proxy_val_map.find` 失败且在创建新容器时出现异常，会回滚并返回 `E_failed`。
- 多处使用 `assert` 与异常捕获（`::yggr::stl_exception`）来保证在 STL 分配/赋值失败时能安全返回。
- `send` 系列函数对查找失败或空指针有早期返回保护，避免解引用空指针。

依赖与包含
- `yggr` 公共组件：`bind`, `shared_ptr`, `stl_exception`, `move`, `mplex/static_assert`, `args_holder_nv`, `task_info_def`。
- 容器：`yggr/container/unordered_set.hpp`, `yggr/container/vector.hpp`。
- 并发/安全容器：`yggr/safe_container/safe_unordered_map.hpp`。
- 其他：Boost 类型工具与 range。

注意事项
- `proxy_dispatcher` 假设 `ProxyContainer` 有一致的类型定义和线程安全语义（如 `send`, `append_proxy`, `exchange_mode`, `remove_proxy`, `swap_proxy_list` 等）。
- 回调 `handler` 的类型必须匹配库使用的 `args_holder` 封装方式，注册/清理时的回调会被 `args_holder_nv_ref_wrap` 包装传入内部函数。
- 在 `send_to_back` 中对 `owner` 使用 `pop()` 与 `boost::move`，会修改本地 `owner` 的副本，确保传入的 `owner` 是按值传递或调用方允许该副本被移动。

示例（伪代码）
- 注册：
  - `dispatcher.register_dispatch<RealData>(mode, proxy_val, change_handler);`
- 发送：
  - `dispatcher.send(owner_info, task, runner, handler, cal_type);`
- 清理：
  - `dispatcher.clear_dispatch_of_proxy(proxy_val, cleanup_handler);`

参考
- 源码： `proxy_dispatcher.hpp`
