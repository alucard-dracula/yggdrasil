# basic_proxy_msg_fixer.hpp — 中文文档

概览
- `basic_proxy_msg_fixer` 是基于 `basic_proxy_route_fixer` 的消息修复/路由辅助模板类，负责处理代理（proxy）相关的注册、模式切换、注销等消息，以及在出错或清理时对代理容器和适配器进行相应处理。
- 该类将路由器、重复器（repeater）和适配器管理器连接起来，为上层提供集中化的消息处理和错误修复逻辑。

模板参数
- `OwnerInfoContainer`：表示所有者信息的容器类型（例如包含 owner 信息的集合）。
- `ProxyRepeater`：代理重复器类型，用于管理各代理的数据注册/注销等。
- `NetInPacket`：网络输入包类型（`net_ipak_type`）。
- `NetOutPacket`：网络输出包类型（`net_opak_type`）。
- `AdapterMgr`：适配器管理器类型（`adapter_mgr_type`）。
- `TaskDataInfo`：任务数据相关信息类型，默认使用 `task_center::default_task_data_info_type`。

继承与重要类型
- 继承：`public basic_proxy_route_fixer<ProxyRepeater>`（因此可复用路由/重复器相关操作）。
- 重要类型别名包括：
  - `owner_info_container_type`：`OwnerInfoContainer`。
  - `proxy_repeater_type`、`proxy_router_type`、`proxy_val_type` 等来自基类。
  - `adapter_mgr_ptr_type`、`adapter_mgr_weak_ptr_type`：适配器管理器的 smart/weak 指针类型。

主要功能
- `register_dispatchers(CtrlCenter&)`：向系统控制中心注册一组错误处理回调（例如网络异常相关错误码），在捕获这些异常时调用 `handler_fix_network_exception` 修复或清理相关代理容器。

- `cal_proxy_register_msg(...)`：处理 `proxy_register_msg`，核心流程：
  1. 获取 `adapter_mgr`、`prepeater`、`prouter`，若任一不存在则返回 `false`。
  2. 遍历注册映射，调用 `prouter->register_dispatch` 注册分发，并通过 `handler_get_proxy_list_of_register` 获取当前代理列表。
  3. 根据返回状态（如 `E_accepted` / `E_changed`），在重复器中注册数据并在适配器管理器中注册适配器。
  4. 向 `back_msg` 添加注册结果。

- `cal_proxy_mode_change_msg(...)`：处理 `proxy_mode_change_msg`，调用路由器的 `exchange_mode` 并通过回调收集变更后的所有者列表，随后调用 `handler_mode_chg` 回调通知上层。

- `cal_proxy_unregister_msg(...)`：处理注销消息，遍历注销集合并调用 `prouter->remove_dispatch` 来移除分发，同时在 `unreg_back_msg` 中记录结果。

- `clear()`：清空路由器中的所有代理容器，通过 `prouter->clear` 并绑定 `handler_clear_proxy_container` 进行逐项清理（包括从重复器和适配器管理器中注销）。

回调处理函数（内部）
- `handler_fix_network_exception(const proxy_val_type&)`：当检测到网络异常（在 `register_dispatchers` 注册的错误码中）时，调用路由器的 `clear_dispatch_of_proxy` 并绑定 `handler_remove_proxy_container` 做清理。

- `handler_remove_proxy_container(const proxy_container_ptr_type&)`：在代理容器被移除时，从重复器注销数据并从适配器管理器中注销适配器。

- `handler_clear_proxy_container(const proxy_container_ptr_type&)`：清理时使用，调用重复器的 `clear()` 并从适配器管理器注销对应适配器。

- `handler_get_proxy_list_of_register` 与 `handler_get_proxy_list_of_exchange_mode`：用于从 `proxy_container` 中提取当前代理列表并填充 `owner_info_container_type`，供上层回调使用。

并发与生命周期注意事项
- 使用 `ref_count_info_locker` 对关键操作加锁（基于基类的引用计数信息），并通过 `adapter_mgr_weak_ptr_type` 保存适配器管理器的弱指针以避免循环引用。
- 在多个方法中使用 `base_type::_repeater_wrap.get_shared_ptr()` 获取重复器和路由器指针，请确保在调用这些方法时对象仍然有效。

依赖与包含
- 本文件依赖并包含若干模块：
  - `yggr/move/move.hpp`、`yggr/smart_ptr_ex/*`、`yggr/container/*`、`yggr/iterator_ex/iterator.hpp`、`yggr/proxy/*` 等。

示例用法（伪代码）

  // 假设已有 prepeater 和 adapter_mgr
  basic_proxy_msg_fixer<MyOwnerInfoContainer, MyProxyRepeater, NetIn, NetOut, MyAdapterMgr> fixer(prepeater, adapter_mgr_ptr);
  fixer.cal_proxy_register_msg(owner, reg_msg, back_msg, handler_mode_chg);

安全与扩展建议
- 本类将路由器与适配器管理器耦合到消息处理流程中；如果需要更灵活的测试或替换，可以通过抽象适配器管理器或注入替代实现来解耦。
- 注意 `register_dispatchers` 中对错误码的硬编码（如 104/121/0x60009004 等），这些与平台/环境有关，修改时请确认跨平台兼容性。

参考文件
- 原始实现： [basic_proxy_msg_fixer.hpp](basic_proxy_msg_fixer.hpp)
