**概述**
- **描述：** `p2p_peer_boot_info` 模板类封装了 P2P 引导（bootstrap）流程中一对对等节点所需的基本信息：所有者 ID（节点标识）、源/目标的 NAT 与本地套接字信息。该类提供构造、移动/拷贝语义、交换、序列化以及若干实用方法（如判断是否启用 NAT 打洞尝试）。
- **头文件：** [yggr/p2p/protocol/p2p_peer_boot_info.hpp](yggr/p2p/protocol/p2p_peer_boot_info.hpp)

**模板参数**
- `Owner_ID`：节点或所有者标识类型。
- `Socket_Info`：套接字/端点信息类型，需提供 `host()` 与 `port()` 等访问器，用于地址解析与比较。

**别名与私有类型**
- `owner_id_type`、`socket_info_type`：对应模板参数。
- `this_type`：当前类型别名。类使用 `BOOST_COPYABLE_AND_MOVABLE` 支持移动语义。

**构造函数**
- 默认构造：所有成员默认构造。
- 接受右值参数的构造（使用 `BOOST_RV_REF` 与 `boost::move`）以避免拷贝开销，参数顺序：`owner_id, src_nat, src_local, dst_nat, dst_local`。
- 接受常量引用的构造以支持左值初始化。
- 提供拷贝构造与移动构造。

**赋值与移动赋值**
- 拷贝赋值与移动赋值均已实现。移动赋值中使用 `copy_or_move_or_swap` 对每个成员进行移动或按需回退到拷贝/交换，以提高异常安全性与性能。

**交换（swap）支持**
- 成员函数 `swap(this_type& right)`：逐字段使用 `yggr::swap` 交换 `_owner_id` 与四个 `socket_info` 成员。
- 右值版本 `swap(BOOST_RV_REF(this_type) right)` 会调用左值版本。
- 在 `yggr::p2p::swap_support` 中提供自由函数 `swap`，并将其引入 `std` 与 `boost`，支持 ADL 调用。

**访问器（Accessors）**
- `owner_id()` / `owner_id() const`：访问所有者 ID。
- `dst_nat_socket_info()` / `dst_local_socket_info()` / `src_nat_socket_info()` / `src_local_socket_info()`：分别访问对应的套接字信息。

**重要方法**
- `swap_src_dst()`：交换源与目标的 NAT 与本地套接字信息，便于生成对称的 peer 信息。
- `is_try_reduce_link_enable()`：判定是否应尝试 NAT 打洞（reduce link）的逻辑：
  - 使用 `yggr::network::address_helper::address_from_string` 从 `host()` 字符串解析地址。
  - 判断条件：目标 NAT 地址与目标本地地址不相同（说明目标存在 NAT），目标 NAT 地址与源 NAT 地址相同（两端处于同一 NAT），且不是本地回环通信（local 地址和端口完全相同）。
- `empty()`：判断对象是否未初始化（任一关键字段为其默认值则视为空）。
- `hash()`：基于 Boost.Hash 将 `owner_id` 与四个 socket 信息组合生成哈希值。

**序列化**
- 私有 `serialize(Archive& ar, const u32 version)`：按成员顺序序列化 `_owner_id`、`_src_nat_socket_info`、`_src_local_socket_info`、`_dst_nat_socket_info`、`_dst_local_socket_info`。类声明了 `yggr::serialization::access` 为友元。

**辅助生成器 `peer_boot_info_generator` 特化**
- 为 `p2p_peer_boot_info` 提供了 `peer_boot_info_generator` 的实现，用于生成一对 `p2p_peer_boot_info`（first=src, second=dst）：
  - 接受左值/右值两种重载，分别用以避免不必要的拷贝。
  - 生成后会将 `info` 赋值到 `ret.first`，然后调用 `swap_src_dst()` 得到对称信息并赋予 `ret.second`。

**哈希兼容性**
- 提供了 `hash_value` 自由函数以便与 Boost.Hash 协作。
- 在启用了 `YGGR_HAS_CXX11_STD_HASH` 环境下，为该类型提供了 `std::hash` 的特化实现，`operator()` 调用成员 `hash()`。

**成员变量**
- `owner_id_type _owner_id`。
- `socket_info_type _src_nat_socket_info`。
- `socket_info_type _src_local_socket_info`。
- `socket_info_type _dst_nat_socket_info`。
- `socket_info_type _dst_local_socket_info`。

**示例用法（伪代码）**

```cpp
using Info = yggr::p2p::p2p_peer_boot_info<MyID, MySocketInfo>;
Info info(owner, src_nat, src_local, dst_nat, dst_local);
if(info.is_try_reduce_link_enable()) {
    // 尝试 NAT 打洞流程
}

// 使用生成器创建一对 src/dst 信息
peer_boot_info_generator<Info>::result_type ret;
peer_boot_info_generator<Info> gen;
gen(ret, owner, src_nat, src_local, dst_nat, dst_local);
```

**相关文件**
- 源文件： [yggr/p2p/protocol/p2p_peer_boot_info.hpp](yggr/p2p/protocol/p2p_peer_boot_info.hpp)
- 地址解析辅助： [yggr/network/address_helper.hpp](yggr/network/address_helper.hpp)

**作者 / 许可证**
- 源文件头部包含版权与 MIT 风格许可，详情请参见源文件顶部的声明。
