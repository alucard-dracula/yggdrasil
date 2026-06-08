# proxy_mode_change_back_msg.hpp — 中文文档

概览
- `proxy_mode_change_back_msg` 是一个代理模式切换回调消息类，用于携带模式切换结果以及附加数据返回给调用方。
- 该类支持序列化、移动/拷贝构造、赋值、交换操作，并通过 `YGGR_FAST_CAL_TASK_IMPORT_TPL` 宏注册消息类型与版本信息。

模板参数
- `Attached_Data`：附加数据类型，默认 `u32`。
- `TaskDataInfo`：任务数据类型，默认 `task_center::default_task_data_info_type`。

类型别名
- `attached_data_type`：附件数据类型。
- `task_data_info_type`：任务数据类型。
- `this_type`：当前类类型。

消息导入与序列化
- 使用宏 `YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (proxy_netdata_def::E_id_start + 4), 1)` 注册消息 ID 和版本。
- 通过 `serialize(Archive& ar, const u32 version)` 方法序列化：
  - `_data_info`
  - `_now_mode`
  - `_att_data`

构造函数
- 默认构造函数：`proxy_mode_change_back_msg()`，初始化 `_now_mode` 和 `_att_data`。
- 构造函数 `proxy_mode_change_back_msg(const data_info_type& data_info, u32 now_mode)`：使用数据描述和当前模式值。
- 构造函数 `proxy_mode_change_back_msg(const data_info_type& data_info, u32 now_mode, const attached_data_type& att_data)`：额外接受附加数据。
- 移动构造函数和拷贝构造函数均支持对应初始化。

赋值与交换
- 移动赋值运算符：使用 `copy_or_move_or_swap` 对 `_data_info`、`_now_mode` 和 `_att_data` 进行移动。
- 拷贝赋值运算符：将成员逐个赋值。
- `swap(BOOST_RV_REF(this_type) right)` 和 `swap(this_type& right)`：支持对象间交换，`swap(this_type& right)` 使用成员 `swap` 和 `yggr::swap`。

访问器
- `const u32& now_mode() const` 和 `u32& now_mode()`：获取或修改当前模式值。
- `data_info_type& now_data_info()` 和 `const data_info_type& now_data_info() const`：获取或修改消息数据描述。
- `attached_data_type& attached_data()` 和 `const attached_data_type& attached_data() const`：获取或修改附加数据。

成员变量
- `_data_info`：消息数据描述信息。
- `_now_mode`：当前模式 ID。
- `_att_data`：附加数据。

交换支持
- 在 `yggr::proxy::proxy_msg::swap_support` 命名空间中定义 `swap(proxy_mode_change_back_msg<Attached_Data, TaskDataInfo>& l, proxy_mode_change_back_msg<Attached_Data, TaskDataInfo>& r)`。
- 同时在 `yggr::proxy::proxy_msg`、`std` 和 `boost` 命名空间中引入该 `swap`，使得通用 `swap` 调用可用。

依赖与包含
- `yggr/base/yggrdef.h`
- `yggr/utility/copy_or_move_or_swap.hpp`
- `yggr/task_center/task_info.hpp`
- `yggr/task_center/support/task_import.hpp`
- `yggr/proxy/proxy_msg/proxy_netdata_def.hpp`
- `yggr/serialization/access.hpp`
- `yggr/serialization/nvp.hpp`

注意事项
- `now_mode` 表示当前代理模式，通常由代理模式切换处理结果设置。
- `attached_data` 可用于携带附加信息，例如原始请求中的关联参数或状态。
- 该消息类设计为可序列化且可移动，以便在网络传输和异步处理场景中高效使用。

参考文件
- 原始实现： [proxy_mode_change_back_msg.hpp](proxy_mode_change_back_msg.hpp)
