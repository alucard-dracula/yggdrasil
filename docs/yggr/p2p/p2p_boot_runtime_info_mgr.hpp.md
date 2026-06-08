**概述**
- **描述：** `p2p_boot_runtime_info_mgr` 是用于 P2P 引导阶段的运行时信息管理器。它负责根据引导请求生成对等端引导信息、维护本地 peer 映射、修正用户数据，并将结果填充到当前节点和其他节点的返回数据结构中。
- **头文件：** [yggr/p2p/p2p_boot_runtime_info_mgr.hpp](yggr/p2p/p2p_boot_runtime_info_mgr.hpp)

**模板参数**
- `Owner_Container`：所有者信息容器类型，用于存储当前网络中的其它节点信息。
- `P2P_Boot_Request`：引导请求类型，要求包含 `socket_info_type` 与本地套接字信息访问器。
- `P2P_Boot_Request_Back`：引导请求返回类型，要求包含 `peer_boot_info_type` 以及 `peer_boot_infos()` 容器访问器。
- `Peer_Session_ID_Generator`：对等节点会话 ID 生成器类型，用于生成源/目标间唯一会话 ID。
- `P2P_Peer_Boot_Info_Generator`：对等节点引导信息生成器类型，默认值为 `peer_boot_info_generator<P2P_Boot_Request_Back::peer_boot_info_type>`。
- `User_Data_Fixer`：用户数据修正器类型，默认值为 `p2p_default_boot_user_data_fixer<Owner_Container, P2P_Boot_Request, P2P_Boot_Request_Back>`。

**类型别名**
- `owner_info_container_type`：所有者信息容器类型。
- `p2p_boot_request_type`：引导请求类型。
- `p2p_boot_request_back_type`：引导请求返回类型。
- `peer_session_id_generator_type`：会话 ID 生成器类型。
- `p2p_peer_boot_info_generator_type`：对等节点引导信息生成器类型。
- `user_data_fixer_type`：用户数据修正器类型。
- `owner_type`：容器中单个所有者元素类型。
- `owner_id_type`：所有者 ID 类型。
- `socket_info_type`：套接字信息类型。
- `peer_boot_info_type`：对等节点引导信息类型。
- `p2p_boot_request_back_map_type`：从所有者到引导返回信息映射类型。

**构造函数**
- 支持可变参数构造，参数会直接转发给 `_user_data_fixer`。
- 在不支持 C++11 可变参数模板时，通过预处理宏生成多个重载版本。

**主要方法**
- `void fix_boot_request(const owner_type& owner, const socket_info_type& src_nat_socket_info, const p2p_boot_request_type& p2p_boot_request, owner_info_container_type& owners_self, p2p_boot_request_back_type& back_self, p2p_boot_request_back_map_type& back_others)`
  - 检查 `_user_data_fixer.fix_boot_request_enable(owner, p2p_boot_request)` 是否允许处理。
  - 清空 `owners_self` 和 `back_others`，确保旧数据不会混入。
  - 在 `_p2p_peer_mapping` 中添加本地映射：`owner`、源 NAT 信息与本地套接字信息。
  - 计算其他所有者 `owners_other`，如果为空则直接返回。
  - 对每个其他所有者生成会话 ID，并对可用映射结果生成对等节点引导信息。
  - 将自己和其他节点的引导信息分别追加到 `back_self` 和 `back_others[*i]`。
  - 最后调用 `_user_data_fixer.fix_boot_request()` 对生成结果进行用户数据修正。

- `inline void drop_peer(const owner_id_type& owner_id)`
  - 从 `_p2p_peer_mapping` 删除指定 `owner_id` 的映射，并通知 `_user_data_fixer` 删除相关信息。

- `inline void drop_peer(const owner_type& owner)`
  - 同上，使用 `owner.owner_id()` 删除映射和用户数据。

- `inline user_data_fixer_type& user_data_fixer()` / `inline const user_data_fixer_type& user_data_fixer() const`
  - 访问用户数据修正器实例。

**内部行为**
- `_p2p_peer_mapping`：维护 `owner_type` 与对应套接字信息之间的映射关系，用于定位其他节点的 NAT/local 端点。
- `_user_data_fixer`：提供两阶段修正：先判断是否允许处理请求，再对最终生成的 `owners_self`、`back_self` 和 `back_others` 进行修正。
- `peer_boot_info_gen`：用于生成对等节点的 boot info 对，结果包含 `first` 和 `second` 两个方向的信息。
- IDs 生成逻辑：使用 `id_gen(owner.owner_id(), (*i).owner_id())` 生成对等会话 ID，并在结果为默认 `owner_id_type()` 时跳过。

**备注**
- 该管理器适合在引导请求处理流程中使用，负责将单个 boot request 拆分并分发给本节点与其它节点。
- `owners_self` 与 `back_others` 在方法开始时会被临时清空，以保证清理旧状态，并在生成后仅保留当前请求相关数据。

**成员变量**
- `user_data_fixer_type _user_data_fixer`：用户数据修正器。
- `p2p_peer_mapping_type _p2p_peer_mapping`：P2P peer 映射表。

**相关文件**
- 源文件： [yggr/p2p/p2p_boot_runtime_info_mgr.hpp](yggr/p2p/p2p_boot_runtime_info_mgr.hpp)
- 依赖：`yggr/p2p/protocol/p2p_peer_boot_info.hpp`、`yggr/p2p/protocol/p2p_encryption_peer_boot_info.hpp`、`yggr/p2p/p2p_peer_mapping.hpp`、`yggr/p2p/p2p_default_boot_user_data_fixer.hpp`

**作者 / 许可证**
- 源文件头部包含版权与 MIT 风格许可声明，详情请参见源文件顶部。
