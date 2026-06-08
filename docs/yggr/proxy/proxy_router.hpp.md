# proxy_router.hpp — 中文文档

概览
- `proxy_router` 是对 `Dispatcher` 的轻量封装，作为路由层的适配器，它直接调用底层 `Dispatcher` 的发送接口将 `task` 路由到代理或回传后端。
- 设计为模板继承：`proxy_router<Dispatcher>` 继承自 `Dispatcher`，因此可以在不改变 `Dispatcher` 实现的前提下提供统一的路由接口。

类型别名
- `base_type`：模板参数 `Dispatcher`。
- `task_type`：由 `Dispatcher` 导出的任务类型。
- `owner_info_type`：由 `Dispatcher` 导出的所有者信息类型。
- `dispatch_id_type`：由 `Dispatcher` 导出的 ID 类型（`id_type`）。

主要方法
- `route_to_proxy(owner, proxy_data, prunner, handler, cal_type)`：将数据路由到代理端，内部调用 `base_type::send_to_proxy`。
- `route_to_proxy`（带 `Args` 重载）：支持附加参数并调用 `base_type::send_to_proxy(..., args)`。
- `route_to_back(owner, proxy_data, prunner, handler, cal_type)`：将数据回传给后端，调用 `base_type::send_to_back`。
- `route(owner, proxy_data, prunner, handler, cal_type)`：根据 `Dispatcher` 的策略决定转发到代理或回传后端，调用 `base_type::send`。

设计要点
- `proxy_router` 不包含路由逻辑本身，仅作为统一适配层；实际的发送/查找逻辑由 `Dispatcher`（如 `proxy_dispatcher`）实现。
- 通过继承 `Dispatcher`，可在需要时直接访问 `Dispatcher` 的保护成员或重载行为。

依赖
- 无额外成员变量，仅依赖 `Dispatcher` 提供的类型与接口。

示例（伪代码）
- 使用示例：
  - `proxy_router<MyDispatcher> router;`
  - `router.route(owner, task, runner, handler, cal_type);`

参考
- 源文件： `proxy_router.hpp`
