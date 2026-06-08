**概述**
- **描述：** `p2p_peer_runtime_info_mgr` 是 P2P 对等节点运行时信息管理器，用于在接收对等节点引导返回数据时修正用户数据、创建会话启动信息，并支持对不同引导信息类型的加密方式自动适配。
- **头文件：** [yggr/p2p/p2p_peer_runtime_info_mgr.hpp](yggr/p2p/p2p_peer_runtime_info_mgr.hpp)

**模板参数**
- `Owner`：所有者类型，要求包含 `owner_id_type`。
- `Conn`：连接对象类型。
- `P2P_Boot_Request_Back`：引导请求返回类型，要求包含 `peer_boot_info_cont_type` 与 `peer_boot_info_type`。
- `P2P_Boot_Start`：启动数据类型。
- `Packets_Checker`：数据包校验器类型。
- `Packets_Crypher`：数据包加密器类型。
- `User_Data_Fixer`：用户数据修正器类型，默认 `p2p_default_peer_user_data_fixer<Owner, P2P_Boot_Request_Back, P2P_Boot_Start>`。

**类型别名**
- `owner_type`：`Owner`。
- `conn_type`：`Conn`。
- `p2p_boot_request_back_type`：`P2P_Boot_Request_Back`。
- `p2p_boot_start_type`：`P2P_Boot_Start`。
- `packets_checker_type`：`Packets_Checker`。
- `packets_crypher_type`：`Packets_Crypher`。
- `user_data_fixer_type`：`User_Data_Fixer`。
- `owner_id_type`：来自 `owner_type::owner_id_type`。
- `peer_boot_info_cont_type`：来自 `p2p_boot_request_back_type::peer_boot_info_cont_type`。
- `peer_boot_info_type`：来自 `p2p_boot_request_back_type::peer_boot_info_type`。

**内部类型**
- `session_id_creator_type`：根据 `peer_boot_info_type` 提取会话 ID 的对象。
- `session_checker_creator_type`：根据 `peer_boot_info_type` 创建 `packets_checker_type` 的对象。
- `p2p_session_creator_type`：组合 `session_creator`，用于创建会话并集成 ID、校验器与加密器。

**构造函数**
- 支持可变参数构造，将参数转发给 `_user_data_fixer`。
- 在不支持 C++11 可变参数模板的环境下，通过宏展开实现多个构造重载。
- 默认析构函数。

**主要方法**
- `inline void fix_boot_request_back(const owner_type& owner, const p2p_boot_request_back_type& p2p_boot_request_back, peer_boot_info_cont_type& peer_boot_info_cont, p2p_boot_start_type& boot_start)`
  - 若 `_user_data_fixer.fix_boot_request_back_enable(owner, p2p_boot_request_back)` 返回 false，则直接返回。
  - 将 `p2p_boot_request_back.peer_boot_infos()` 的内容交换到临时容器 `ret_peer_boot_info_cont`。
  - 调用 `_user_data_fixer.fix_boot_request_back(owner, ret_peer_boot_info_cont, boot_start)` 执行用户数据修正。
  - 将修正后的 `ret_peer_boot_info_cont` 交换回输出容器 `peer_boot_info_cont`。

- `inline void drop_peer(const owner_id_type& owner_id)`
  - 调用 `_user_data_fixer.remove_info(owner_id)` 删除与该节点相关的信息。

- `inline void drop_peer(const owner_type& owner)`
  - 通过 `owner.owner_id()` 调用 `_user_data_fixer.remove_info()`。

**辅助结构体**
- `detail::p2p_peer_runtime_info_mgr_session_id_creator`：根据 `peer_boot_info_type` 提取会话 ID。
- `detail::p2p_peer_runtime_info_mgr_session_checker_creator`：根据 `peer_boot_info_type` 创建校验器共享指针。
- `detail::p2p_peer_runtime_info_mgr_session_crypher_creator`：
  - 对于普通 `p2p_peer_boot_info`，创建无参加密器。
  - 对于 `p2p_encryption_peer_boot_info`，创建带加密密钥的加密器。

**设计说明**
- 该类将引导返回数据的用户数据修正与会话创建逻辑分离，支持可插拔的修正器与加密器类型。
- `fix_boot_request_back` 会在不影响原始 `p2p_boot_request_back` 的情况下提取并修正 peer boot infos。
- 通过 `session_creator` 组合类型，可在后续会话建立过程中统一生成会话 ID、校验器和加密器。

**成员变量**
- `user_data_fixer_type _user_data_fixer`：用户数据修正器。

**相关文件**
- 源文件： [yggr/p2p/p2p_peer_runtime_info_mgr.hpp](yggr/p2p/p2p_peer_runtime_info_mgr.hpp)
- 依赖：`yggr/p2p/p2p_default_peer_user_data_fixer.hpp`、`yggr/p2p/protocol/p2p_peer_boot_info.hpp`、`yggr/p2p/protocol/p2p_encryption_peer_boot_info.hpp`。

**作者 / 许可证**
- 源文件头部包含版权与 MIT 风格许可声明，详情请参见源文件顶部。
