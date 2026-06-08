# proxy_unregister_back_msg.hpp — 中文文档

概览
- `proxy_unregister_back_msg` 表示代理注销（unregister）操作的回执/响应消息，用于通知对应的数据项是否成功被注销并携带可选附加数据。
- 该消息支持序列化、移动/拷贝构造、赋值与交换，适用于网络通信场景。

模板参数
- `Attached_Data`：附加数据类型，默认 `u32`。
- `TaskDataInfo`：任务数据类型，默认 `task_center::default_task_data_info_type`。

类型别名
- `attached_data_type`：附加数据类型。
- `task_data_info_type`：任务数据类型。
- `this_type`：当前类类型。
- `unreg_back_map_type`：`yggr::unordered_map<data_info_type, bool>`，保存注销结果映射（data_info -> state）。

消息注册与序列化
- 使用宏 `YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (proxy_netdata_def::E_id_start + 6), 1)` 注册消息 ID 与版本（ID = `proxy_netdata_def::E_id_start + 6`，版本 = 1）。
- `serialize(Archive& ar, const u32 version)` 序列化 `_unreg_back_map` 与 `_att_data`。

构造函数与生命周期
- 默认构造，接受 `attached_data_type` 的构造函数。
- 支持从 `::yggr::unordered_set<data_info_type>` 与 `state` 和 `attached_data` 构造（声明中预留构造签名，但实现里未对集合参数进行直接填充）。
- 支持移动构造与拷贝构造，析构为默认实现。

赋值与交换
- 移动赋值使用 `copy_or_move_or_swap`，拷贝赋值为普通复制。
- 提供成员 `swap`，可与 `std::swap` / `boost::swap` 协同使用（在 `swap_support` 命名空间中提供 swap 重载并引入到 `std` 与 `boost`）。

核心接口
- `void set_item_state(bool state)`：将 `_unreg_back_map` 中所有项的状态设置为 `state`。
- `bool set_item_state(const data_info_type& data_info, bool state)`：查找单个 `data_info` 并设置其状态，返回是否存在并设置成功。
- `template<typename DataInfoContainer> void set_item_state(const DataInfoContainer& cont, bool state)`：以容器 `cont` 中的 `data_info` 填充 `_unreg_back_map`，并将每项设为 `state`（会先清空当前映射）。
- `void clear()`：清空映射和附加数据。
- `bool add_unreg_back_data(const data_info_type& data_info, bool right)`：插入单个注销结果，返回是否插入成功。
- `bool rm_unreg_back_data(const data_info_type& data_info)`：按键删除注销结果。
- 访问器：`unreg_back_map()` / `const unreg_back_map()`、`attached_data()` / `const attached_data()`。

成员变量
- `_unreg_back_map`：`unreg_back_map_type`，保存每个 `data_info` 的注销结果（true/false）。
- `_att_data`：`attached_data_type`，携带的附加数据。

交换支持
- 在 `yggr::proxy::proxy_msg::swap_support` 命名空间中定义 `swap(proxy_unregister_back_msg<...>& l, proxy_unregister_back_msg<...>& r)`，并通过 `using` 引入到 `yggr::proxy::proxy_msg`，以及 `std`/`boost` 命名空间。

依赖与包含
- `yggr/base/yggrdef.h`
- `yggr/utility/copy_or_move_or_swap.hpp`
- `yggr/container/clear.hpp`
- `yggr/container/unordered_set.hpp`
- `yggr/task_center/task_info.hpp`
- `yggr/task_center/support/task_import.hpp`
- `yggr/proxy/proxy_msg/proxy_netdata_def.hpp`
- `yggr/serialization/access.hpp`
- `yggr/serialization/nvp.hpp`
- `yggr/serialization/unordered_map.hpp`

注意事项
- `set_item_state(const ::yggr::unordered_set<data_info_type>&, ...)` 风格的构造在声明处存在，但实际构造函数体内未对传入集合进行自动填充，使用者如果需要可通过 `set_item_state(container, state)` 明确设置。
- `unreg_back_map_type` 使用 `data_info_type` 作为键，请确保 `data_info_type` 对应的哈希/等价比较在 `yggr::unordered_map` 可用。
- `template<typename DataInfoContainer>` 的重载依赖 boost::range 接口（支持 `begin`/`end`）。

示例（伪代码）
- 构造并插入：

  - 创建消息：`proxy_unregister_back_msg<> msg;`
  - 插入结果：`msg.add_unreg_back_data(some_data_info, true);`
  - 访问映射：`auto &m = msg.unreg_back_map(); if(m[some_data_info]) ...`

参考文件
- 源码： [proxy_unregister_back_msg.hpp](proxy_unregister_back_msg.hpp)
