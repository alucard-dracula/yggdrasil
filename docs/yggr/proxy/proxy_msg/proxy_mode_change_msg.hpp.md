# proxy_mode_change_msg.hpp — 中文文档

概览
- `proxy_mode_change_msg` 是一个代理模式切换请求消息类，用于携带需要切换到的目标模式数据。
- 该类支持序列化、移动/拷贝构造、赋值以及交换操作，适用于网络传输和异步消息处理。

模板参数
- `Attached_Data`：附加数据类型，默认 `u32`。
- `TaskDataInfo`：任务数据类型，默认 `task_center::default_task_data_info_type`。

类型别名
- `attached_data_type`：附加数据类型。
- `task_data_info_type`：任务数据类型。
- `this_type`：当前消息类类型。
- `chg_map_type`：从 `data_info_type` 到 `u32` 的映射类型，用于保存每个数据项的目标模式。

消息导入与序列化
- 使用宏 `YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (proxy_netdata_def::E_id_start + 3), 1)` 注册消息 ID 和版本号。
- `serialize(Archive& ar, const u32 version)` 序列化以下成员：
  - `_chg_map`
  - `_att_data`

构造函数
- 默认构造函数：初始化 `attached_data`。
- `proxy_mode_change_msg(const attached_data_type& att_data)`：使用附加数据初始化。
- 支持移动构造与拷贝构造。

赋值与交换
- 移动赋值：通过 `copy_or_move_or_swap` 移动 `_chg_map` 和 `_att_data`。
- 拷贝赋值：按值复制 `_chg_map` 和 `_att_data`。
- `swap`：支持与另一个同类型消息对象交换状态。

核心接口
- `bool add_chg_data(const data_info_type& data_info, u32 mode)`：
  - 向 `_chg_map` 插入数据描述和对应模式编号。
  - 插入成功返回 `true`，若已存在则返回 `false`。
- `bool rm_chg_data(const data_info_type& data_info)`：
  - 从 `_chg_map` 删除指定数据描述。
  - 删除成功返回 `true`。
- `chg_map_type& chg_map()` / `const chg_map_type& chg_map() const`：获取模式切换映射。
- `attached_data_type& attached_data()` / `const attached_data_type& attached_data() const`：获取附加数据。

成员变量
- `_chg_map`：映射每个数据项到目标模式 ID 的集合。
- `_att_data`：附加数据，用于携带额外上下文。

交换支持
- 在 `yggr::proxy::proxy_msg::swap_support` 命名空间中定义 `swap(proxy_mode_change_msg<Attached_Data, TaskDataInfo>& l, proxy_mode_change_msg<Attached_Data, TaskDataInfo>& r)`。
- 同时将该 `swap` 引入到 `yggr::proxy::proxy_msg`、`std` 和 `boost` 命名空间，使通用交换可用。

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
- `chg_map_type` 使用 `data_info_type` 作为键，因此 `data_info_type` 必须支持哈希或比较（由 `yggr::unordered_map` 要求）。
- `attached_data` 可用于携带与模式切换请求相关的额外上下文信息。
- 此消息类型通常与 `proxy_mode_change_back_msg` 配对使用，前者发送请求，后者返回结果。

参考文件
- 原始实现： [proxy_mode_change_msg.hpp](proxy_mode_change_msg.hpp)
