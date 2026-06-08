# proxy_mode_blanace.hpp — 中文文档

概览
- `proxy_mode_blanace` 是 `basic_proxy_mode` 的一种负载均衡模式实现。
- 它基于 `ModeDataStore` 中的 `vector_store_type`，在多个可用代理目标中以轮询方式选择一个并返回给调用者。

模板参数
- `ModeDataStore`：模式数据存储类型。
  - 该类型必须定义 `owner_info_container_type`、`owner_info_type`、`store_type`、`ex_args_type` 以及 `vector_store_type`。
  - 需要提供静态函数 `s_get_vector_store_of_base(oid_cont)` 来从 `store_type` 中提取目标列表。

继承关系
- 继承自 `basic_proxy_mode<ModeDataStore>`。
- 当前类型的静态模式常量为 `E_mode = proxy_mode_def::E_proxy_mode_blanace`。

类型别名
- `mode_data_store_type`：来自基类的模式数据存储类型。
- `owner_info_type`：对应所有者信息类型。
- `owner_info_container_type`：所有者信息容器类型。
- `store_type`：对应模式存储类型。
- `ex_args_type`：扩展参数类型。

私有成员
- `vector_store_type`：从 `ModeDataStore` 派生的向量存储类型。
- `idx_type`：`boost::atomic<std::size_t>`，用于保存当前轮询索引。
- `_now_idx`：可变原子索引，用于在并发情况下安全地轮询目标。

主要逻辑
- `pro_get_owner_info_container` 实现负载均衡选择：
  1. 通过 `mode_data_store_type::s_get_vector_store_of_base(oid_cont)` 获取目标向量 `vt`。
  2. 如果 `vt` 为空，直接返回不修改 `cont`。
  3. 否则使用原子索引 `_now_idx` 和 `compare_exchange_weak` 计算 `use_idx = now_idx % vt.size()`，并将索引自增到下一轮。
  4. 将当前 `owner_info` 复制为 `info`，并调用 `info.push(vt[use_idx])` 将选定目标添加到所有者信息中。
  5. 将 `info` 移动插入到 `cont` 末尾，并返回 `cont`。

线程与并发
- 使用 `boost::atomic<std::size_t>` 作为轮询索引，保证在并发调用时选择目标的一致性。
- `compare_exchange_weak` 循环实现了原子递增，并在索引值发生竞争时重试。

依赖与包含
- `yggr/proxy/proxy_mode/proxy_mode_def.hpp`
- `yggr/proxy/proxy_mode/basic_proxy_mode.hpp`
- `yggr/iterator_ex/iterator.hpp`
- `yggr/move/move.hpp`
- `boost/atomic.hpp`
- `boost/range/functions.hpp`

注意事项
- 该模式仅在 `ModeDataStore` 提供的目标列表不为空时生效；否则返回空容器。
- `info.push(vt[use_idx])` 依赖于 `owner_info_type` 提供 `push` 方法，将选择的代理目标追加到 `owner_info` 中。
- 轮询索引可能随着线程竞争而跨多个调用递增，不保证顺序严格连续，但保证原子安全性。

示例用法（伪代码）

  proxy_mode_blanace<MyModeDataStore> mode;
  owner_info_container_type owners;
  mode.get_owner_info_container(owners, owner_info, store);

参考文件
- 原始实现： [proxy_mode_blanace.hpp](proxy_mode_blanace.hpp)
