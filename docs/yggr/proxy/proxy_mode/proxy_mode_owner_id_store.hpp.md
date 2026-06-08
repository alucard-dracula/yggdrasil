# proxy_mode_owner_id_store.hpp — 中文文档

概览
- `proxy_mode_owner_id_store` 是一个用于保存并查询代理所有者 ID 的线程安全存储类。
- 它基于 `safe_multi_index_container`，同时支持按哈希索引快速查找和按顺序访问。

模板参数
- `OwnerInfoContainer`：所有者信息容器类型。
- `ExArgs`：扩展参数类型，默认 `void`。

类型别名
- `owner_info_container_type`：`OwnerInfoContainer`。
- `owner_info_type`：容器元素类型。
- `owner_id_type`：所有者 ID 类型。
- `owner_id_midx_type`：多索引容器类型，包含哈希索引 `tag_hash` 和随机访问索引 `tag_vt`。
- `safe_store_type`：`owner_id_midx_type`。
- `store_type`：内部多索引容器的基础类型。
- `hash_store_type`：基于 `tag_hash` 的索引类型。
- `vector_store_type`：基于 `tag_vt` 的随机访问索引类型。
- `ex_args_type`：扩展参数类型。

主要功能
- `empty()`：判断存储是否为空。
- `erase(const owner_id_type& id)`：按 ID 从哈希索引删除元素。

- `get_owner_info_container(owner_info_container_type& cont, ProxyModePtr pimode, const owner_info_type& owner) const`：
  - 在内部通过 `_owner_id_midx.using_handler` 调用 `handler_get_owner_info_container`。
  - `handler_get_owner_info_container` 会将存储 `cont` 传给 `pimode->get_owner_info_container` 进行代理模式查询。

- `get_owner_info_container(owner_info_container_type& cont, ProxyModePtr pimode, const owner_info_type& owner, Args& args) const`：
  - 带额外参数 `args` 的版本。
  - 使用 `utility::args_holder_nv_ref_wrap` 和 `utility::args_holder_trans` 将参数封装为 `basic_args_holder_nv`。

数据访问接口
- `get_datas(Container& cont) const`：返回当前存储中按随机访问索引顺序的 ID 列表，并交换到传入容器中。
- `get_datas(store_type& cont) const`：将内部存储拷贝到传入的基础类型容器。
- `get_datas(owner_id_midx_type& cont) const`：直接将当前存储赋值给传入容器。
- `swap_datas(Container& cont)`：与传入容器交换数据，支持任意容器类型。
- `swap_datas(store_type& cont) const` 和 `swap_datas(owner_id_midx_type& cont) const`：交换整个底层存储。

处理器接口
- `using_handler(const Handler& handler)`：对内部多索引容器执行自定义处理器，可用于直接访问存储数据。

静态辅助函数
- `s_is_exists_of_base(const store_type& cont, const owner_id_type& id)`：检查 ID 是否存在于哈希索引中。
- `s_push_data_of_base(store_type& cont, const owner_id_type& id)`：向向量索引末尾追加 ID。
- `s_get_hash_store_of_base(store_type& cont)`：获取哈希索引引用。
- `s_get_vector_store_of_base(store_type& cont)`：获取随机访问索引引用。

内部回调
- `handler_get_owner_info_container`：用于不带额外参数的代理模式查询。
- `handler_get_owner_info_container_args`：用于带额外参数的代理模式查询。

依赖与包含
- `yggr/base/yggrdef.h`
- `yggr/bind/bind.hpp`
- `yggr/utility/args_holder_nv.hpp`
- `yggr/func/foo_t_info.hpp`
- `yggr/safe_container/safe_multi_index_random_access_index.hpp`
- `yggr/safe_container/safe_multi_index_hashed_index.hpp`
- `yggr/safe_container/safe_multi_index_container.hpp`
- `boost/range/value_type.hpp`
- `boost/range/functions.hpp`

注意事项
- `OwnerInfoContainer` 中元素必须定义 `owner_id_type`。
- `get_owner_info_container` 依赖传入的 `ProxyModePtr` 对象有效，否则直接返回原容器。
- 由于使用多索引容器，既能按 ID 高效查找，也能保留顺序访问能力。

示例用法（伪代码）

  proxy_mode_owner_id_store<MyOwnerInfoContainer> store;
  store.get_owner_info_container(out_cont, mode_ptr, owner);

参考文件
- 原始实现： [proxy_mode_owner_id_store.hpp](proxy_mode_owner_id_store.hpp)
