**概述**
- **描述：** `p2p_default_peer_user_data_fixer` 是 P2P 引导流程中针对对等节点返回数据的默认用户数据修正器模板。它提供是否启用修正的默认判断、参数占位方法，以及删除节点信息的空实现。
- **头文件：** [yggr/p2p/p2p_default_peer_user_data_fixer.hpp](yggr/p2p/p2p_default_peer_user_data_fixer.hpp)

**模板参数**
- `Owner`：所有者类型，表示当前节点或对等节点的信息类型。
- `P2P_Boot_Request_Back`：引导请求返回类型，要求包含 `peer_boot_info_cont_type`、`peer_boot_info_type` 和 `empty()` 方法。
- `P2P_Boot_Start`：表示对等节点引导启动数据类型。

**类型别名**
- `Owner_type`：等同于模板参数 `Owner`。
- `p2p_boot_request_back_type`：等同于 `P2P_Boot_Request_Back`。
- `p2p_boot_start_type`：等同于 `P2P_Boot_Start`。
- `owner_id_type`：从 `owner_type` 提取的 owner_id 类型。
- `peer_boot_info_cont_type`：`P2P_Boot_Request_Back` 中的 peer boot info 容器类型。
- `peer_boot_info_type`：`P2P_Boot_Request_Back` 中的 peer boot info 类型。

**构造与析构**
- 默认构造函数 `p2p_default_peer_user_data_fixer()`。
- 默认析构函数 `~p2p_default_peer_user_data_fixer()`。

**主要方法**
- `inline bool fix_boot_request_back_enable(const owner_type& owner, const p2p_boot_request_back_type& boot_request_back)`
  - 默认实现：`return !boot_request_back.empty();`
  - 用于判断是否对当前 `boot_request_back` 进行修正。默认当 `boot_request_back` 非空时才启用。

- `inline void fix_boot_request_back(const owner_type& owner, peer_boot_info_cont_type& out_peer_info_cont, p2p_boot_start_type& out_boot_start)`
  - 默认实现为空。
  - 子类可以重载该方法，在生成对等节点引导信息后填充或修改 `out_peer_info_cont` 与 `out_boot_start`。

- `inline void remove_info(const owner_type& owner)`
  - 默认实现为空。
  - 子类可重载以在某个节点被删除时清理额外状态或缓存。

**用途与扩展**
- 该类作为默认实现，适用于不需要对 peer 返回数据做进一步处理的场景。
- 可通过继承并重载方法实现自定义逻辑，如对返回的 `peer_boot_info_cont_type` 进行过滤、补充、或设置启动参数。

**适用场景**
- 在 P2P 引导返回阶段，对每个对等节点返回信息进行二次处理前，先判断是否启用修正。
- 作为 `p2p_boot_runtime_info_mgr` 或其他引导流程组件的默认用户数据修正器。

**相关文件**
- 源文件： [yggr/p2p/p2p_default_peer_user_data_fixer.hpp](yggr/p2p/p2p_default_peer_user_data_fixer.hpp)

**作者 / 许可证**
- 源文件头部包含版权与 MIT 风格许可声明，详情请参见源文件顶部。
