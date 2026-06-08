# proxy_register_msg.hpp — 中文文档

概览
- `proxy_register_msg` 表示代理注册请求消息，用于提交要注册的数据项及其期望的代理模式与计算类型。
- 支持序列化、移动/拷贝构造、赋值与交换，适用于网络传输场景。

模板参数
- `Attached_Data`：附加数据类型，默认 `u32`。
- `TaskDataInfo`：任务数据类型，默认 `task_center::default_task_data_info_type`。

类型别名
- `attached_data_type`：附加数据类型。
- `task_data_info_type`：任务数据类型。
- `this_type`：当前类类型。
- `reg_info_type`：`std::pair<u32, u32>`，表示 `<mode, cal_type>`。
- `reg_map_type`：`yggr::unordered_map<data_info_type, reg_info_type>`，保存注册映射。

消息注册与序列化
- 使用宏 `YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (proxy_netdata_def::E_id_start + 1), 1)` 注册消息 ID 与版本。
- `serialize(Archive& ar, const u32 version)` 序列化 `_reg_map` 与 `_att_data`。

构造函数
- 默认构造与使用 `attached_data` 构造函数均可用。
- 支持移动构造与拷贝构造。

赋值与交换
- 移动赋值使用 `copy_or_move_or_swap`，拷贝赋值按值复制。
- `swap` 支持对象间交换 `_reg_map` 与 `_att_data`。

核心接口
- `clear()`：清空注册映射与附加数据。
- `add_reg_data(const data_info_type& data_info, u32 mode, u32 cal_type)`：插入一条注册数据项。
- `add_reg_data(const data_info_type& data_info, const reg_info_type ref_info)`：插入已有的注册信息对。
- `template<typename T> bool add_reg_data(u32 mode)`：根据数据类型 `T` 的静态信息插入注册项（使用 `T::data_info()` 与 `T::cal_type()`）。
- `rm_reg_data(const data_info_type& data_info)`：按 `data_info` 删除注册项。
- `template<typename T> bool rm_reg_data()`：按数据类型模板删除注册项（使用 `T::data_info()`）。
- `reg_map()` / `const reg_map()`：访问注册映射。
- `attached_data()` / `const attached_data()`：访问附加数据。

成员变量
- `_reg_map`：保存数据项到注册信息的映射。
- `_att_data`：附加数据。

交换支持
- 在 `yggr::proxy::proxy_msg::swap_support` 命名空间中定义 `swap(proxy_register_msg<Attached_Data, TaskDataInfo>& l, proxy_register_msg<Attached_Data, TaskDataInfo>& r)`。
- 同时引入到 `yggr::proxy::proxy_msg`、`std` 和 `boost` 命名空间。

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
- `reg_map_type` 使用 `data_info_type` 作为键，确保 `data_info_type` 在 `yggr::unordered_map` 中可用。
- `add_reg_data<T>(mode)` 依赖 `T` 提供静态方法 `data_info()` 与 `cal_type()`。

参考文件
- 原始实现： [proxy_register_msg.hpp](proxy_register_msg.hpp)
