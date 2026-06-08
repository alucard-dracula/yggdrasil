# default_proxy_msg_fixer.hpp — 中文文档

概览
- `default_proxy_msg_fixer` 是 `basic_proxy_msg_fixer` 的默认实现模板类，用于处理代理消息（注册、模式切换、注销）的接收、解析、调用和回报。
- 它将网络输入包解码为代理消息对象，调用基类的处理逻辑，然后将结果封装到网络输出包并通过回调返回。

模板参数
- `OwnerInfoContainer`：保存所有者信息的容器类型。
- `ProxyRepeater`：代理重复器类型。
- `NetInPacket`：网络输入包类型，用于从接收到的网络数据加载代理消息。
- `NetOutPacket`：网络输出包类型，用于将响应消息封装为可发送的网络数据。
- `AdapterMgr`：适配器管理器类型。
- `TaskDataInfo`：任务数据相关信息类型，默认为 `task_center::default_task_data_info_type`。

继承关系
- 继承自 `basic_proxy_msg_fixer<OwnerInfoContainer, ProxyRepeater, NetInPacket, NetOutPacket, AdapterMgr, TaskDataInfo>`。
- 通过继承复用了基类的注册、注销、清理和消息处理逻辑。

类型别名
- `owner_info_container_type`、`proxy_repeater_type`、`net_ipak_type`、`net_opak_type`、`task_data_info_type`、`proxy_fix_state_def_type` 等来自基类。
- `proxy_mode_def_type`：`proxy::proxy_mode_def`，表示代理模式相关状态。
- `proxy_register_msg_type`、`proxy_mode_change_msg_type`、`proxy_unregister_msg_type` 及其对应回报类型，均是默认无模板参数的代理消息类型。

主要方法
- `register_dispatchers(Tag, CtrlCenter&)`：调用基类的 `register_dispatchers`，将网络异常处理回调注册到系统控制中心。

- `register_cal_object(Action_Table&, const Recv_Handler&)`：
  - 将 `proxy_register_msg_type`、`proxy_mode_change_msg_type`、`proxy_unregister_msg_type` 注册为特殊数据处理动作。
  - 在收到对应消息时，绑定本类的 `cal_proxy_*_msg` 方法进行实际处理。

- `unregister_cal_object(Action_Table&)`：注销特殊消息处理动作。

- `clear()`：
  - 调用基类 `clear()` 清除路由器状态。
  - 然后调用重复器 `prepeater->clear()` 清除重复器中的数据。

消息处理方法
- `cal_proxy_register_msg(owner, cdt, prunner, handler)`：
  - 解析输入包 `cdt` 为 `proxy_register_msg_type`。
  - 调用基类 `cal_proxy_register_msg` 处理注册逻辑。
  - 若处理失败，将注册消息的原始注册映射拷贝到返回消息，并设置代理模式失败状态。
  - 将结果封装到 `net_opak_type`，调用 `handler` 返回。

- `cal_proxy_mode_change_msg(owner, cdt, prunner, handler)`：
  - 解析输入包为 `proxy_mode_change_msg_type`。
  - 调用基类 `cal_proxy_mode_change_msg` 来交换模式，并通过回调 `handler_mode_change` 发送结果。

- `cal_proxy_unregister_msg(owner, cdt, prunner, handler)`：
  - 解析输入包为 `proxy_unregister_msg_type`。
  - 调用基类 `cal_proxy_unregister_msg` 处理注销逻辑。
  - 将返回消息封装为 `net_opak_type` 并通过 `handler` 发送。

内部回调
- `handler_mode_change(owner, chg_owners, data_info, back_mode, att_data, prunner, basic_handler_holder)`：
  - 当模式切换处理完成后，根据变更所有者集合构造 `proxy_mode_change_back_msg_type`。
  - 使用传入的 `handler` 将回报结果发送出去。

依赖与包含
- `yggr/base/yggrdef.h`
- `yggr/utility/args_holder_nv.hpp`
- `yggr/iterator_ex/iterator.hpp`
- `yggr/proxy/proxy_ctrl/basic_proxy_msg_fixer.hpp`
- `yggr/proxy/proxy_mode/proxy_mode_def.hpp`

注意事项
- `cal_proxy_register_msg`、`cal_proxy_mode_change_msg`、`cal_proxy_unregister_msg` 依赖输入包的 `load` 方法正确解析消息数据，否则会直接返回并忽略本次请求。
- `handler_mode_change` 仅在 `chg_owners` 非空时发送回报，这意味着若没有代理变更则不会触发回调发送结果。
- `default_proxy_msg_fixer` 主要为默认代理消息流程提供通用实现，可根据需要继承或特化以支持更丰富的消息类型或自定义处理逻辑。

示例用法（伪代码）

  default_proxy_msg_fixer<MyOwnerInfoContainer, MyProxyRepeater, NetInPacket, NetOutPacket, MyAdapterMgr> fixer(repeater_ptr, adapter_mgr_ptr);
  fixer.register_cal_object(action_table, recv_handler);
  fixer.cal_proxy_register_msg(owner, in_packet, runner, handler);

参考文件
- 原始实现： [default_proxy_msg_fixer.hpp](default_proxy_msg_fixer.hpp)
