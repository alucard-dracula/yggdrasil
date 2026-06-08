# proxy_mode_reduce.hpp — 中文文档

概览
- `proxy_mode_reduce` 是 `basic_proxy_mode` 的一种广播/转发模式实现。
- 它将请求转发到 `ModeDataStore` 中的所有可用目标，并将每个目标作为独立的所有者信息条目添加到结果容器中。

模板参数
- `ModeDataStore`：模式数据存储类型。
  - 该类型必须定义 `owner_info_container_type`、`owner_info_type`、`store_type`、`ex_args_type` 和 `vector_store_type`。
  - 需要提供静态函数 `s_get_vector_store_of_base(oid_cont)` 来从 `store_type` 中获取目标向量。

继承关系
- 继承自 `basic_proxy_mode<ModeDataStore>`。
- 静态常量 `E_mode` 为 `proxy_mode_def::E_proxy_mode_reduce`。

类型别名
- `mode_data_store_type`：来自基类的模式数据存储类型。
- `owner_info_type`：所有者信息类型。
- `owner_info_container_type`：所有者信息容器类型。
- `store_type`：模式存储类型。
- `ex_args_type`：扩展参数类型。
- `vector_store_type`：来自 `ModeDataStore` 的目标向量存储类型。

主要逻辑
- `pro_get_owner_info_container` 实现广播发送：
  1. 从 `oid_cont` 获取目标向量 `vt`。
  2. 如果 `vt` 为空，则返回原始容器 `cont` 不做修改。
  3. 遍历 `vt` 中的每个目标 ID。
  4. 对每个目标构造 `owner_info_type info(owner_info)`，调用 `info.push(*i)` 将该目标追加进去。
  5. 将 `info` 移动插入结果容器 `cont`。
  6. 返回 `cont`。

行为说明
- 此模式会将请求发送给所有目标，适合“广播”或“全量转发”场景。
- 因为每个目标都会生成一个独立的 `owner_info_type` 条目，最终输出容器将包含多个结果。

依赖与包含
- `yggr/proxy/proxy_mode/proxy_mode_def.hpp`
- `yggr/proxy/proxy_mode/basic_proxy_mode.hpp`
- `yggr/iterator_ex/iterator.hpp`
- `yggr/move/move.hpp`
- `boost/range/functions.hpp`

注意事项
- 该模式不使用扩展参数 `ex_args_type`，其 `pro_get_owner_info_container` 中的参数 `pargs_holder` 会被忽略。
- 如果 `vt` 为空，则不会更改输出容器 `cont`。
- `info.push(*i)` 依赖 `owner_info_type` 支持将目标信息追加到自身。

示例用法（伪代码）

  proxy_mode_reduce<MyModeDataStore> mode;
  owner_info_container_type owners;
  mode.get_owner_info_container(owners, owner_info, store);

参考文件
- 原始实现： [proxy_mode_reduce.hpp](proxy_mode_reduce.hpp)
