# proxy_mode_source_hash.hpp — 中文文档

概览
- `proxy_mode_source_hash` 是 `basic_proxy_mode` 的一种按源哈希分配模式实现。
- 它根据请求者的 `owner_id` 计算哈希值，并从目标列表中选取一个目标，保证相同来源具有一致的路由目标。

模板参数
- `ModeDataStore`：模式数据存储类型。
  - 该类型必须定义 `owner_info_container_type`、`owner_info_type`、`store_type`、`ex_args_type` 和 `vector_store_type`。
  - 需要提供静态函数 `s_get_vector_store_of_base(oid_cont)` 来从 `store_type` 中获取目标向量。

继承关系
- 继承自 `basic_proxy_mode<ModeDataStore>`。
- 静态常量 `E_mode` 为 `proxy_mode_def::E_proxy_mode_source_hash`。

类型别名
- `mode_data_store_type`：来自基类的模式数据存储类型。
- `owner_info_type`：所有者信息类型。
- `owner_info_container_type`：所有者信息容器类型。
- `store_type`：模式存储类型。
- `ex_args_type`：扩展参数类型。
- `vector_store_type`：来自 `ModeDataStore` 的目标向量存储类型。
- `owner_id_type`：`owner_info_type::owner_id_type`。

主要逻辑
- `pro_get_owner_info_container` 实现源哈希路由：
  1. 从 `oid_cont` 获取目标向量 `vt`。
  2. 若 `vt` 为空，则直接返回 `cont` 不做修改。
  3. 从 `owner_info.owner_id()` 获取源 ID `src_id`，并断言该 ID 非零。
  4. 使用 `boost::hash<owner_id_type>` 对 `src_id` 进行哈希计算。
  5. 通过 `hasher(src_id) % boost::size(vt)` 计算目标索引 `use_idx`。
  6. 构造 `owner_info_type info(owner_info)`，将目标 `vt[use_idx]` 追加到 `info` 中。
  7. 将 `info` 移动插入到 `cont` 末尾并返回。

行为说明
- 相同的源 ID 会被哈希到同一个目标，从而提供一致性路由。
- 目标列表为空时不会修改 `cont`。
- 该模式适合需要根据源分布将请求稳定分配到目标的场景。

依赖与包含
- `yggr/proxy/proxy_mode/proxy_mode_def.hpp`
- `yggr/proxy/proxy_mode/basic_proxy_mode.hpp`
- `yggr/iterator_ex/iterator.hpp`
- `yggr/move/move.hpp`
- `boost/range/functions.hpp`
- `boost/functional/hash/hash.hpp`
- `cassert`

注意事项
- `owner_id` 必须非零，否则断言失败。
- 如果 `ModeDataStore` 中的 `vector_store_type` 为空，函数不会添加任何目标。
- 该模式忽略扩展参数 `pargs_holder`。

示例用法（伪代码）

  proxy_mode_source_hash<MyModeDataStore> mode;
  owner_info_container_type owners;
  mode.get_owner_info_container(owners, owner_info, store);

参考文件
- 原始实现： [proxy_mode_source_hash.hpp](proxy_mode_source_hash.hpp)
