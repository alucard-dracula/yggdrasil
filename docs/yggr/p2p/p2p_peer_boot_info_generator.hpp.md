**概述**
- **描述：** `peer_boot_info_generator` 是一个模板前置声明，用于为不同类型的 P2P 引导信息提供生成器特化。该文件本身不包含具体实现，主要定义了可扩展的接口模式和示例。
- **头文件：** [yggr/p2p/p2p_peer_boot_info_generator.hpp](yggr/p2p/p2p_peer_boot_info_generator.hpp)

**主要内容**
- 声明模板：`template<typename Peer_Boot_Info> class peer_boot_info_generator;`
- 该类模板非定义，需在其他地方提供针对具体 `Peer_Boot_Info` 类型的特化实现。

**示例说明**
- 文件中包含一个注释示例，演示如何为具体的 `you_boot_info<Owner_ID, Socket_Info[, ...]>` 类型特化 `peer_boot_info_generator`：
  - 定义 `owner_id_type`、`socket_info_type` 等别名。
  - 定义 `now_p2p_peer_boot_info_type` 为特化类型。
  - 定义 `result_type` 为 `std::pair<now_p2p_peer_boot_info_type, now_p2p_peer_boot_info_type>`。
  - 提供两个 `operator()` 重载：一个接受左值参数，一个接受右值参数，用于生成源/目标对等引导信息。

**用途**
- 该文件用于定义可扩展的生成器类型接口，允许不同的 P2P 引导信息类型通过特化 `peer_boot_info_generator` 实现自定义创建逻辑。
- 通常在 P2P 引导流程中，需要生成一对对等 `peer_boot_info`，此模板为该流程提供统一扩展点。

**注意**
- `peer_boot_info_generator` 仅是声明，不可直接实例化，必须提供对具体 `Peer_Boot_Info` 类型的特化定义。
- 示例中提到的 `you_boot_info_gen` 需要由用户自行实现。

**相关文件**
- 源文件： [yggr/p2p/p2p_peer_boot_info_generator.hpp](yggr/p2p/p2p_peer_boot_info_generator.hpp)

**作者 / 许可证**
- 源文件头部包含版权与 MIT 风格许可声明，详情请参见源文件顶部。
