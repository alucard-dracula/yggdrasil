# proxy_register_back_msg.hpp — 中文文档

概览
- `proxy_register_back_msg` 表示代理注册响应消息，用于向调用方返回注册结果、模式和状态信息。
- 该消息类支持序列化、移动/拷贝构造、赋值和交换操作，适合网络传输与异步处理。

模板参数
- `Attached_Data`：附加数据类型，默认 `u32`。
- `TaskDataInfo`：任务数据类型，默认 `task_center::default_task_data_info_type`。

类型别名
- `attached_data_type`：附加数据类型。
- `task_data_info_type`：任务数据类型。
- `this_type`：当前类类型。
- `reg_back_info_type`：`std::pair<u32, u32>`，表示 `<mode, state>`。
- `reg_back_map_type`：`yggr::unordered_map<data_info_type, reg_back_info_type>`，保存每个数据项的注册返回信息。

消息注册与序列化
- 使用宏 `YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (proxy_netdata_def::E_id_start + 2), 1)` 注册消息 ID 和版本。
- 序列化函数 `serialize(Archive& ar, const u32 version)` 包括：
  - `_reg_back_map`
  - `_att_data`

构造函数
- 默认构造函数：初始化附加数据。
- `proxy_register_back_msg(const attached_data_type& att_data)`：使用附加数据构造。
- 支持移动构造与拷贝构造。

赋值与交换
- 移动赋值：使用 `copy_or_move_or_swap` 移动 `_reg_back_map` 和 `_att_data`。
- 拷贝赋值：按值复制。
- `swap`：支持与另一个同类型对象交换。

核心接口
- `set_item_state(u32 state)`：遍历 `_reg_back_map`，将所有条目的状态字段更新为 `state`。
- `set_item_state(const data_info_type& data_info, u32 state)`：仅更新指定数据项的状态，返回是否成功。
- `clear()`：清空 `_reg_back_map` 和 `_att_data`。
- `add_reg_back_data(const data_info_type& data_info, u32 mode, u32 state)`：插入模式和状态信息。
- `add_reg_back_data(const data_info_type& data_info, const reg_back_info_type& reg_back_info)`：插入已有的返回信息对。
- `rm_reg_back_data(const data_info_type& data_info)`：删除指定数据项的返回信息。
- `reg_back_map()` / `const reg_back_map()`：访问注册返回映射。
- `attached_data()` / `const attached_data()`：访问附加数据。

成员变量
- `_reg_back_map`：保存每个数据项对应的注册结果与状态。
- `_att_data`：附加数据。

交换支持
- 在 `yggr::proxy::proxy_msg::swap_support` 命名空间中定义 `swap(proxy_register_back_msg<Attached_Data, TaskDataInfo>& l, proxy_register_back_msg<Attached_Data, TaskDataInfo>& r)`。
- 同时将该 `swap` 引入到 `yggr::proxy::proxy_msg`、`std` 和 `boost` 命名空间。

依赖与包含
- `yggr/base/yggrdef.h`
- `yggr/utility/copy_or_move_or_swap.hpp`
- `yggr/task_center/task_info.hpp`
- `yggr/task_center/support/task_import.hpp`
- `yggr/proxy/proxy_msg/proxy_netdata_def.hpp`
- `yggr/serialization/access.hpp`
- `yggr/serialization/nvp.hpp`
- `yggr/serialization/unordered_map.hpp`

注意事项
- `reg_back_map` 的值对 `reg_back_info_type` 中，第一个元素表示代理模式，第二个元素表示处理状态。
- `set_item_state` 对所有条目统一设置状态，适合整体失败或统一结果场景。
- `add_reg_back_data` 插入失败时返回 `false`，通常表示同一 `data_info` 已经存在。

参考文件
- 原始实现： [proxy_register_back_msg.hpp](proxy_register_back_msg.hpp)
