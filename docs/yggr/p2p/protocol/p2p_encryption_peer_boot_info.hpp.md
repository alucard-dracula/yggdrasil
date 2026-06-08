**概述**
- **描述：** `p2p_encryption_peer_boot_info` 模板类是 `p2p_peer_boot_info` 的扩展，用于在 P2P 引导（bootstrap）流程中携带加密密钥（encryption keys）。该类保存节点标识、源/目标的 NAT 与本地套接字信息，以及对称/会话加密密钥。
- **头文件：** [yggr/p2p/protocol/p2p_encryption_peer_boot_info.hpp](yggr/p2p/protocol/p2p_encryption_peer_boot_info.hpp)

**模板参数**
- `Owner_ID`：节点或所有者标识类型。
- `Socket_Info`：套接字/端点信息类型（如地址/端口结构）。
- `EKeys`：加密密钥类型，通常由 `p2p_ekeys_generator` 生成。

**继承**
- 继承自 `p2p_peer_boot_info<Owner_ID, Socket_Info>`，因此包含 `owner_id`、`src_nat_socket_info`、`src_local_socket_info`、`dst_nat_socket_info` 与 `dst_local_socket_info` 等成员与访问器。

**别名**
- `base_type`：父类别名 `p2p_peer_boot_info<Owner_ID, Socket_Info>`。
- `owner_id_type`, `socket_info_type`, `ekeys_type`：分别对应模板参数。

**构造函数**
- `p2p_encryption_peer_boot_info()`：默认构造，`_ekeys` 默认构造。
- 支持带右值参数的构造以避免拷贝（使用 `BOOST_RV_REF` 与 `boost::move`）：可同时传入 `owner_id`、五个 `socket_info`（src/dst NAT 与 local）和 `ekeys`。
- 支持常量引用构造以传入左值参数。
- 提供拷贝构造与移动构造。

**赋值与移动**
- 支持拷贝赋值与移动赋值。移动赋值使用 `copy_or_move_or_swap` 来在不同语义间安全转换，且对基类的成员通过 `static_cast<base_type&>` 做移动或复制。

**Swap 支持**
- 成员函数 `swap(this_type& right)`：交换基类内容与 `_ekeys`。
- 右值 `swap(BOOST_RV_REF(this_type) right)`：调用左值版本。
- 在 `yggr::p2p::swap_support` 命名空间提供自由函数 `swap`，并通过 `using` 将其引入 `std` 与 `boost`，以便 ADL 支持。

**访问器**
- 继承自基类的访问器：`owner_id()`、`src_nat_socket_info()`、`src_local_socket_info()`、`dst_nat_socket_info()`、`dst_local_socket_info()`。
- 本类新增：`ekeys_type& encryption_keys()` / `const ekeys_type& encryption_keys() const`：访问或读取加密密钥。

**哈希支持**
- 提供 `hash()` 成员方法，它在基类哈希值上结合 `_ekeys` 的哈希（使用 `boost::hash_combine`）。
- 提供 `hash_value` 自由函数，以及在支持 `std::hash` 的配置下为该类型提供 `std::hash` 特化。

**序列化**
- 私有 `serialize(Archive& ar, const u32 version)`：先序列化基类对象（键名为 "peer_info"），再序列化 `_ekeys`。使用 `yggr::serialization::access` 作为友元。

**辅助生成器 `peer_boot_info_generator` 特化**
- 为 `p2p_encryption_peer_boot_info` 提供了 `peer_boot_info_generator` 的特化实现，返回一对加密的 `peer_boot_info` 对象（source/target），
 该实现调用 `p2p_ekeys_generator` 生成一对密钥（`src_key` / `dst_key`），将 `src_key` 赋给第一个对象，交换 src/dst 并将 `dst_key` 赋给第二个对象。

**成员变量**
- `ekeys_type _ekeys`：存储加密密钥（对称/会话密钥等）。

**设计要点与注意事项**
- 使用 `BOOST_COPYABLE_AND_MOVABLE` 宏支持移动语义。
- `copy_or_move_or_swap` 用于在移动不可用时安全退回到拷贝或交换操作，提升异常安全性与性能。
- `peer_boot_info_generator` 提供了两种重载（接受左值或右值），以避免不必要的拷贝。

**示例用法（伪代码）**

```cpp
using Info = yggr::p2p::p2p_encryption_peer_boot_info<MyID, MySocketInfo, MyEKeys>;
using Gen = yggr::p2p::peer_boot_info_generator<Info>;

Info a, b;
Gen gen;
Gen::result_type ret;
gen(ret, owner_id, src_nat, src_local, dst_nat, dst_local);
// ret.first 和 ret.second 分别包含源与目标的加密密钥
```

**相关文件**
- 源文件： [yggr/p2p/protocol/p2p_encryption_peer_boot_info.hpp](yggr/p2p/protocol/p2p_encryption_peer_boot_info.hpp)
- 相关生成器： [yggr/p2p/p2p_ekeys_generator.hpp](yggr/p2p/p2p_ekeys_generator.hpp)

**作者 / 许可证**
- 源文件头部声明了版权与 MIT 风格许可，请参见源文件以获取完整文本。
