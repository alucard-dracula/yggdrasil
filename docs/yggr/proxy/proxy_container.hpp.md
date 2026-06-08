# proxy_container.hpp — 中文文档

概览
- `proxy_container` 是代理模式下的容器类模板，负责管理单个代理 ID 下的代理实例（所有者 ID 列表 / 模式数据存储）以及代理模式的创建与切换逻辑。
- 主要职责：维护代理模式对象、管理模式下的所有者集合、路由发送（send）到对应所有者，以及基于策略的模式创建/切换。

模板参数
- `ID`：容器的标识类型（`id_type`）。
- `Task`：任务/消息类型（`task_type`），用于发送时携带的数据；要求 `Task::owner_info_type` 存在且与 `ModeCreator::owner_info_type` 相同。
- `ModeTactics`：模式切换策略类型（`mode_tactics_type`），用于决定新旧模式时的选择规则（可调用，接受旧模式与目标模式并返回实际新模式）。
- `ModeCreator`：模式创建器类型（`mode_creator_type`），负责根据模式 ID 创建 `base_proxy_mode_type` 实例，并提供若干类型别名（如 `owner_info_type`, `owner_id_type`, `mode_data_store_type` 等）。

重要类型别名
- `owner_info_container_type` / `owner_info_type` / `owner_id_type`：由 `ModeCreator` 导出，表示所有者信息相关类型。
- `mode_data_store_type`：存储模式数据的类型，提供对所有者集合的增删查等操作。
- `base_proxy_mode_type`：代理模式接口类型。
- `interface_proxy_mode_type`：`shared_ptr<base_proxy_mode_type>`。
- `proxy_mode_wrap_type`：`safe_wrap<interface_proxy_mode_type>`，用于安全并发访问当前代理模式对象。

核心成员变量
- `_mode_tactics`：模式决策策略引用。
- `_mode_creator`：模式创建器引用。
- `_id`：容器 ID（`id_wrap_type`，safe_wrap 封装）。
- `_proxy_mode_wrap`：当前代理模式的线程安全包装（可加载/锁定以获取模式实例）。
- `_mode_data_store`：模式数据存储，维护所有者集合并提供并发安全的访问接口。

主要方法
- `id()`：返回容器 ID。
- `empty_of_proxy()`：判断是否有代理数据（调用 `_mode_data_store.empty()`）。

- 发送（路由）接口：
  - `template<typename Runner, typename Handler> void send(const owner_info_type& owner, const task_type& proxy_data, Runner* prunner, const Handler& handler, u32 cal_type) const`：
    - 加载当前代理模式 `_proxy_mode_wrap.load()`，若失败则直接返回。
    - 使用 `_mode_data_store.get_owner_info_container(owners, pimode, owner)` 获取目标所有者列表；若为空返回。
    - 调用 `handler(prunner, ..., owners, proxy_data.data_info(), cal_type, boost::ref(ref_data.org_buf()))` 发起回调（将 `proxy_data` 转为非 const 引用用于访问 organ buffer）。
  - 同名重载带 `Args& args` 参数：支持额外参数并通过 `args_holder` 封装传递给 `_mode_data_store.get_owner_info_container`。

- 模式管理接口：
  - `append_proxy(u32 mode, const owner_id_type& val)`：向 `_mode_data_store` 请求加入所有者（通过 `using_handler` 调用 `handler_append_proxy`）。
  - `exchange_mode(u32 mode)`：创建/交换为指定模式，内部调用 `create_mode(mode)`。
  - `remove_proxy(const owner_id_type& id)`：从 `_mode_data_store` 删除所有者 ID。
  - `get_proxy_list(Container& cont)` / `swap_proxy_list(Container& cont)`：获取或交换内部所有者集合。

内部行为与处理流程
- `handler_append_proxy(store_type& cont, u32 mode, const owner_id_type& val)`：
  - 若 `mode_data_store_type::s_is_exists_of_base(cont, val)` 已存在，则返回 `E_existsed`。
  - 否则调用 `create_mode(mode)` 创建（或切换到）目标模式；若创建成功则 `s_push_data_of_base(cont, val)` 将该所有者加入存储。

- `create_mode(u32 mode)`：通过 `_proxy_mode_wrap.locked_unsafe_using_handler(...)` 执行 `handler_creator_mode`，以线程安全方式创建/替换当前模式对象。

- `handler_creator_mode(value_ptr_type& ptr, const u32 mode)`：
  - 读取旧模式 `old_mode`（若存在）并通过 `_mode_tactics(old_mode, mode)` 计算 `new_mode`。若 `new_mode == 0` 返回拒绝。
  - 若 `old_mode == new_mode` 返回已接受状态。
  - 调用 `_mode_creator(new_mode)` 创建新的 `base_proxy_mode_type` 实例并包装为 `interface_proxy_mode_type`，再将其分配到 `ptr`。
  - 返回 `(new_mode, 状态)`，状态可能是 `E_changed`, `E_accepted`, `E_failed`, `E_refused` 等由 `proxy_fix_state_def` 定义。

断言与要求
- 使用 `BOOST_MPL_ASSERT` 校验 `Task::owner_info_type` 与 `ModeCreator::owner_info_type` 必须一致，且 `owner_info_type::owner_id_type` 与 `owner_id_type` 一致。
- `ModeCreator` 必须实现构造 `base_proxy_mode_type` 的操作符 `operator()(u32 mode)`，并提供 `store_type` 与 `mode_data_store_type` 等必要接口。
- `mode_data_store_type` 需要提供并发安全的 `get_owner_info_container`, `get_datas`, `swap_datas`, `erase`, `using_handler` 等静态/实例方法。

依赖列表
- `yggr/smart_ptr_ex/shared_ptr.hpp`
- `yggr/utility/swap.hpp`
- `yggr/utility/args_holder.hpp`
- `yggr/safe_container/safe_wrap.hpp`
- `yggr/safe_container/safe_multi_index_*`（多索引容器支持）
- `yggr/task_center/task_info_def.hpp`
- `yggr/proxy/proxy_fix_state_def.hpp`
- Boost: `enable_if`, `is_same`, `mpl::or_`, `range`/`value_type`, `multi_index` 等

注意事项
- `send` 方法中对 `proxy_data` 使用 `const_cast` 转为非常量以访问 `org_buf()`，调用方应注意线程安全和数据所有权问题。
- `handler_creator_mode` 在创建新模式时使用 `yggr_nothrow_new` 并捕获异常以避免抛出，返回失败状态；确保 `ModeCreator` 的 `operator()` 在异常安全方面可控。
- `proxy_mode_wrap_type` 的加载与锁定可能抛出或返回空指针，代码中对此已有捕获处理，但上层使用仍需注意并发语义。

示例（伪代码）
- 创建容器：
  - `proxy_container<MyID, MyTask, MyTactics, MyCreator> pc(tactics, creator, myid);`
- 添加代理并发送：
  - `pc.append_proxy(mode, owner_id);`
  - `pc.send(owner_info, task, runner_ptr, handler, cal_type);`

参考
- 源文件： `proxy_container.hpp`
