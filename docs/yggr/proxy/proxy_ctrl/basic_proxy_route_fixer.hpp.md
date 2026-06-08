# basic_proxy_route_fixer.hpp — 中文文档

概览
- `basic_proxy_route_fixer` 是一个基础路由修复器辅助类，用于包装代理重复器（`ProxyRepeater`）并提供线程安全的注册、注销、路由和特殊数据管理接口。
- 它通过 `boost::enable_shared_from_raw` 支持从 `this` 获取共享指针，并使用 `safe_wrap` 将重复器对象安全包装保存。

模板参数
- `ProxyRepeater`：代理重复器类型。该类型须提供路由器类型 `router_type`、路由器指针类型 `router_ptr_type`，以及注册/注销数据、路由和特殊数据管理接口。

类型别名
- `proxy_repeater_type`：模板参数 `ProxyRepeater`。
- `proxy_repeater_wrap_type`：对重复器类型的安全包装类型 `yggr::safe_container::safe_wrap<proxy_repeater_type>`。
- `proxy_repeater_ptr_type`：包装类型中保存的指针类型。
- `proxy_router_type`：`proxy_repeater_type::router_type`。
- `proxy_router_ptr_type`：`proxy_repeater_type::router_ptr_type`。

主要行为
- 构造函数 `basic_proxy_route_fixer(const proxy_repeater_ptr_type& prepeater)`：接收 `ProxyRepeater` 指针并保存到 `_repeater_wrap`。
- `void join()`：等待引用计数 `_count` 为空，通常用于确保当前对象在所有挂起操作结束后再退出。
- `proxy_repeater_ptr_type get_proxy_repeater() const`：获取当前安全包装的重复器共享指针。

数据注册/注销接口
- `register_repeater_data` / `register_repeater_special_data`：
  - 有多个重载，可按 `RealData`、`DataInfo`、`Fix_Handler`、`cal_t` 和（可选的）`Seh_Handler` 注册不同类型的修复处理器。
  - 这些方法均先加锁 `_count`，然后从 `_repeater_wrap` 获取重复器指针，再委托给重复器执行实际注册。
- `unregister_repeater_data` / `unregister_repeater_special_data`：
  - 支持按类型模板或按 `DataInfo` 实例注销注册项。
- `append_special_data` / `remove_special_data`：
  - 通过重复器附加或移除“特殊数据”，通常用于内部状态或额外路由信息管理。

异常处理支持
- 当启用 `YGGR_USE_SEH` 时，额外提供 `register_repeater_data` / `register_repeater_special_data` 重载，允许传入结构化异常处理器 `Seh_Handler`。

路由接口
- `route(owner, proxy_data, prunner, handler, cal_type) const`：
  - 在 `_count` 上加锁后，从 `_repeater_wrap` 获取重复器并调用其 `route` 方法。
- `route(owner, proxy_data, prunner, handler, cal_type, args) const`：
  - 带额外 `args` 参数的路由版本，也会委托给重复器的相应 `route` 方法。

并发与生命周期
- 成员 `_count` 用于引用计数保护，并在每次访问重复器时加锁，避免对象在并发调用期间被销毁。
- `_repeater_wrap` 封装重复器指针，提供线程安全访问。

依赖与包含
- 头文件依赖：
  - `yggr/base/yggrdef.h`
  - `yggr/base/ref_count_info.h`
  - `yggr/func/foo_t_info.hpp`
  - `yggr/safe_container/safe_wrap.hpp`
  - `boost/smart_ptr/enable_shared_from_raw.hpp`
  - `boost/utility/enable_if.hpp`

注意事项
- `basic_proxy_route_fixer` 仅提供通用包装与委托逻辑，实际行为由模板参数 `ProxyRepeater` 的实现决定。
- 由于使用了 `boost::enable_shared_from_raw`，对象需通过 `boost::shared_ptr` 或兼容方式管理，以便 `shared_from_raw` 正常工作。

示例用法（伪代码）

  typedef basic_proxy_route_fixer<MyRepeater> route_fixer_type;
  route_fixer_type fixer(repeater_ptr);
  fixer.register_repeater_data<MyData>(fix_handler);
  fixer.route(owner, proxy_data, runner, handler, cal_type);

参考文件
- 原始实现： [basic_proxy_route_fixer.hpp](basic_proxy_route_fixer.hpp)
