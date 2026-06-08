**概述**
- **描述：** `p2p_peer_mapping` 是一个 P2P 节点映射管理类，用于在 `owner_id` 与 `Owner` 对象之间建立唯一映射，并附加每个所有者的 NAT 和本地 socket 信息对。
- **头文件：** [yggr/p2p/p2p_peer_mapping.hpp](yggr/p2p/p2p_peer_mapping.hpp)

**模板参数**
- `Owner`：拥有者类型。该类型必须包含 `owner_id_type`，并提供 `owner_id()` 方法。
- `Socket_Info`：套接字信息类型，用于表示 NAT 与本地端点信息。

**类型别名**
- `owner_type`：`Owner`。
- `owner_id_type`：来自 `Owner::owner_id_type`。
- `socket_info_type`：`Socket_Info`。
- `socket_info_pair_type`：`std::pair<socket_info_type, socket_info_type>`，表示 NAT 与本地 socket 信息对。
- `base_type`：使用 `yggr::mapping::unique_mapping<owner_id_type, owner_type, socket_info_pair_type>` 的基类。
- `mapping_type`：等同于 `base_type`。
- `mapping_owner_result_type`：返回 `Owner` 与布尔标志的类型。
- `mapping_owner_id_result_type`：返回 `owner_id_type` 与布尔标志的类型。
- `mapping_socket_info_result_type`：返回 `socket_info_pair_type` 与布尔标志的类型。

**构造与析构**
- 默认构造函数 `p2p_peer_mapping()`。
- 默认析构函数 `~p2p_peer_mapping()`。

**主要方法**
- `bool append(const owner_type& owner, const socket_info_type& nat_socket_info, const socket_info_type& local_socket_info)`
  - 如果映射不存在，则追加新的 `owner_id -> (owner, socket_info_pair)` 映射。

- `bool compulsory_append(const owner_type& owner, const socket_info_type& nat_socket_info, const socket_info_type& local_socket_info)`
  - 强制追加映射，通常会覆盖已有项或确保插入成功。

- `mapping_owner_result_type get_owner(const owner_id_type& oid) const`
  - 根据 `owner_id` 查询 `Owner` 对象及其存在状态。

- `mapping_socket_info_result_type get_socket_info(const owner_id_type& oid) const`
  - 根据 `owner_id` 查询对应的 socket 信息对。

- `mapping_socket_info_result_type get_socket_info(const owner_type& owner) const`
  - 根据 `Owner` 对象查询对应的 socket 信息对。

- `template<typename Container> Container& mapping_other_owners(const owner_type& owner, Container& out) const`
  - 获取除当前 `owner` 之外的所有者集合。

- `template<typename Container> Container& mapping_other_owners(const owner_id_type& oid, Container& out) const`
  - 根据 `owner_id` 获取除指定节点之外的所有者集合。

- `template<typename Container> Container& mapping_other_socket_infos(const owner_type& owner, Container& out) const`
  - 获取除当前 `owner` 之外的所有者对应 socket 信息集合。

- `template<typename Container> Container& mapping_other_socket_infos(const owner_id_type& oid, Container& out) const`
  - 根据 `owner_id` 获取除指定节点之外的 socket 信息集合。

- `bool remove(const owner_type& owner)`
  - 删除指定 `Owner` 对象对应的映射。

- `bool remove(const owner_id_type& oid)`
  - 删除指定 `owner_id` 对应的映射。

**设计说明**
- 该类继承自 `yggr::mapping::unique_mapping`，利用其通用映射机制进行键值存储与查询。
- 映射值类型为 `std::pair<Socket_Info, Socket_Info>`，分别表示 NAT socket 信息和本地 socket 信息。
- 通过 `mapping_other_values<0, N>` 系列方法，可以方便地获取除当前节点之外的所有者或 socket 信息，用于 P2P 引导数据分发。

**相关文件**
- 源文件： [yggr/p2p/p2p_peer_mapping.hpp](yggr/p2p/p2p_peer_mapping.hpp)
- 依赖：`yggr/mapping/unique_mapping.hpp`

**作者 / 许可证**
- 源文件头部包含版权与 MIT 风格许可声明，详情请参见源文件顶部。
