# proxy_unregister_msg.hpp — 中文文档

概览
- `proxy_unregister_msg` 表示代理端发起的注销请求消息，用以提交若干要注销的数据项（data_info）的集合。
- 支持序列化、移动/拷贝构造、赋值与交换，适用于通过网络传递注销请求。

模板参数
- `Attached_Data`：附加数据类型，默认 `u32`。
- `TaskDataInfo`：任务数据类型，默认 `task_center::default_task_data_info_type`。

类型别名
- `attached_data_type`：附加数据类型。
- `task_data_info_type`：任务数据类型。
- `this_type`：当前类类型。
- `unreg_set_type`：`yggr::unordered_set<data_info_type>`，保存要注销的数据项集合。

消息注册与序列化
- 使用宏 `YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (proxy_netdata_def::E_id_start + 5), 1)` 注册消息 ID 与版本（ID = `proxy_netdata_def::E_id_start + 5`，版本 = 1）。
- `serialize(Archive& ar, const u32 version)` 序列化 `_unreg_set`。

构造函数与生命周期
- 默认构造，支持以 `attached_data_type` 构造。
- 支持移动构造与拷贝构造，析构为默认实现。

赋值与交换
- 移动赋值使用 `copy_or_move_or_swap`，拷贝赋值为普通复制。
- 成员 `swap` 提供对象交换功能，并在 `swap_support` 命名空间中定义 `swap` 重载，以便与 `std::swap` / `boost::swap` 协同使用。

核心接口
- `void clear()`：清空注销集合与附加数据。
- `bool add_unreg_data(const data_info_type& data_info)`：向集合插入 `data_info`，返回是否插入成功（不存在则插入成功）。
- `bool rm_unreg_data(const data_info_type& data_info)`：从集合中删除 `data_info`，返回是否删除成功。
- 访问器：`unreg_set()` / `const unreg_set()` 获取集合引用；`attached_data()` / `const attached_data()` 访问附加数据。

成员变量
- `_unreg_set`：保存要注销的数据项集合。
- `_att_data`：附加数据。

交换支持
- 在 `yggr::proxy::proxy_msg::swap_support` 命名空间中定义 `swap(proxy_unregister_msg<...>& l, proxy_unregister_msg<...>& r)`，并通过 `using` 引入到 `yggr::proxy::proxy_msg`，以及 `std`/`boost` 命名空间。

依赖与包含
- `yggr/base/yggrdef.h`
- `yggr/utility/copy_or_move_or_swap.hpp`
- `yggr/task_center/task_info.hpp`
- `yggr/task_center/support/task_import.hpp`
- `yggr/proxy/proxy_msg/proxy_netdata_def.hpp`
- `yggr/serialization/access.hpp`
- `yggr/serialization/nvp.hpp`
- `yggr/serialization/unordered_set.hpp`

注意事项
- `unreg_set_type` 使用 `data_info_type` 作为集合元素类型，请确保 `data_info_type` 在 `yggr::unordered_set` 中可哈希和可比较（若需要自定义比较或哈希，请确认已提供相应实现）。
- `attached_data` 字段在序列化时并未显式写入（该类序列化函数只序列化 `_unreg_set`），但类定义确实包含 `_att_data` 成员；如果需要在通信中携带 `attached_data`，请确认序列化机制是否覆盖或扩展。

示例（伪代码）
- 构造并使用：
  - `proxy_unregister_msg<> msg;`
  - `msg.add_unreg_data(some_data_info);`
  - `auto &s = msg.unreg_set(); if(s.find(some_data_info) != s.end()) ...`

参考文件
- 源码： [proxy_unregister_msg.hpp](proxy_unregister_msg.hpp)
