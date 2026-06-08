**概述**
- **描述：** 定义 P2P 协议中使用的包（packet）ID 范围与常量，以及用于检查包 ID 是否在该范围内的预处理宏。
- **头文件：** [yggr/p2p/protocol/p2p_packet_def.hpp](yggr/p2p/protocol/p2p_packet_def.hpp)

**结构体 `p2p_packet_def`**
- 提供一组静态常量（`YGGR_STATIC_CONSTANT`）来定义包 ID 的起始、结束与具体的包类型：
  - `E_id_start`：起始值，0x00008300。
  - `E_id_p2p_boot_request`：引导请求包 ID（E_id_start + 1）。
  - `E_id_p2p_boot_request_back`：引导请求回包 ID（E_id_start + 2）。
  - `E_id_p2p_boot_start`：引导开始包 ID（E_id_start + 3）。
  - `E_id_p2p_nat_reduce`：NAT 降级/打洞相关包 ID（E_id_start + 4）。
  - `E_id_end`：结束值，0x00008400。

**宏 `YGGR_PP_CHECK_P2P_PACKET_ID(__id__)`**
- 功能：检查给定的 `__id__` 是否在 `p2p_packet_def` 定义的 `[E_id_start, E_id_end)` 范围内。
- 实现依赖：`YGGR_PP_IS_IN_RANGE`（来自 `yggr/ppex/range_checker.hpp`），宏展开为布尔值（预处理级别的判定）。

**用途与注意事项**
- 包 ID 使用固定范围以避免与其他协议或模块的 ID 冲突，便于在消息路由与分发时进行快速判断。
- 范围上限 `E_id_end` 通常被视为开区间（不包含），请在扩展 ID 时确保不越界。
- 该头文件仅包含常量与宏，不依赖运行时对象，适合在编译期使用。

**示例用法（伪代码）**

```cpp
// 检查一个包 ID 是否属于 p2p 范围
u32 id = ...;
if(YGGR_PP_CHECK_P2P_PACKET_ID(id)) {
    // 处理 p2p 包
}

// 使用具体常量
if(id == ::yggr::p2p::p2p_packet_def::E_id_p2p_boot_request) {
    // 处理 boot request
}
```

**相关文件**
- 定义使用的辅助宏与检查器： [yggr/ppex/range_checker.hpp](yggr/ppex/range_checker.hpp)
- 源文件： [yggr/p2p/protocol/p2p_packet_def.hpp](yggr/p2p/protocol/p2p_packet_def.hpp)

**作者 / 许可证**
- 源文件头部包含版权与许可说明（MIT 风格），请参见源文件获取完整文本。
