**概述**
- **描述：** `packet.hpp` 定义了 `yggr::packet::packet<Archive_Partner>` 和 `yggr::packet::ref_packet<Archive_Partner>` 两个模板类，用于在 `yggr` 框架中封装可序列化的数据缓冲区。
- **头文件：** [yggr/packet/packet.hpp](yggr/packet/packet.hpp)

**主要类型**
- `yggr::packet::packet<Archive_Partner>`
  - 封装一个可拥有的数据缓冲区 `buf_type`。
  - 适用于序列化/反序列化操作的独立数据包对象。
- `yggr::packet::ref_packet<Archive_Partner>`
  - 封装一个对外部缓冲区的引用 `buf_type&`。
  - 适用于不拥有缓冲区、仅引用已有数据的场景。

**核心成员**
- `archive_partner_type` / `archive_type` / `archive_helper_type`
  - 由 `Archive_Partner` 提供，决定具体的归档实现、缓冲区类型和辅助操作。
- `buf_type` / `buf_val_type` / `buf_real_val_type`
  - 用于访问缓冲区数据、值类型和底层真实数据指针。

**构造函数与赋值**
- `packet` 支持：
  - 默认构造
  - 迭代器范围构造
  - 从 `buf_type` 复制或移动构造
  - 从同类型或兼容 `packet` / `ref_packet` 构造，要求缓冲区类型一致
- `ref_packet` 支持：
  - 从 `buf_type` 引用构造
  - 从同类型或兼容 `packet` / `ref_packet` 构造
  - 复制构造
- 两者都支持移动/复制赋值，以及与缓冲区和其他包对象的赋值。

**交换与缓冲区访问**
- `swap` 系列函数支持：
  - `packet` 与 `buf_type`
  - `packet` 与 `packet`
  - `packet` 与 `ref_packet`
  - `ref_packet` 与 `buf_type`
  - `ref_packet` 与 `packet`
  - `ref_packet` 与 `ref_packet`
- 提供 `clear()`、`org_buf()`、`empty()`、`size()`、`resize()`、`buf()`、`val_buf()` 等缓冲区访问与修改接口。

**序列化接口**
- `packet::save(const T& t)`
  - 将对象 `t` 序列化到内部缓冲区。
  - 如果当前归档为加载模式或保存失败，返回 `false`。
  - 保存后若缓冲区仍为空，则抛出 `E_invalid_argument` 错误。
- `packet::load(T& t) const`
  - 从内部缓冲区反序列化到对象 `t`。
  - 当归档为保存模式或缓冲区为空时返回 `false`。

**比较运算**
- `compare_eq` 支持与：
  - 直接缓冲区 `buf_type`
  - 其他 `packet<Other_Archive_Partner>`
  - `ref_packet<Other_Archive_Partner>`
  - 同类型 `packet`
- 通过辅助类型 `archive_helper_type::buf_compare` 实现实际比较逻辑。
- 全局 `operator==` / `operator!=` 重载为 `packet` / `ref_packet` 和缓冲区类型之间提供对等比较。

**辅助函数与扩展**
- `cypher(const Handler& handler)`
  - 将内部缓冲区通过用户提供的 `Handler` 处理，并返回处理结果。
- `operator<<` 支持将 `packet` / `ref_packet` 输出到 `std::ostream`。
- 通过 `yggr::packet::swap_support::swap` 以及 `std::swap` / `boost::swap` 引入，使对象可与标准交换算法兼容。

**用途场景**
- `packet` 适用于拥有可序列化数据的传输单元，如网络包、消息缓冲区。
- `ref_packet` 用于引用外部已有缓冲区，避免数据复制，便于零拷贝处理。

**相关文件**
- `yggr/packet/packet.hpp`
- `yggr/packet/packet_info.hpp`
- `yggr/packet/packet_info_caster.hpp`

**作者 / 许可证**
- 源文件头部包含 MIT 风格许可声明，许可条款请参考源文件顶部。