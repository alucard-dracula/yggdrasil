# proxy_netdata_def.hpp — 中文文档

概览
- `proxy_netdata_def` 定义了代理网络消息的 ID 范围。
- 它用于校验代理消息 ID 是否在允许的范围内。

结构说明
- `proxy_netdata_def` 是一个简单结构体，仅包含两个静态常量：
  - `E_id_start = 0x00008200`
  - `E_id_end = 0x00008300`

宏定义
- `YGGR_PP_CHECK_PROXY_MSG_ID(__id__)`：用于检查给定消息 ID 是否位于 `proxy_netdata_def` 定义的区间内。
- 宏展开为：
  - `YGGR_PP_IS_IN_RANGE(__id__, ::yggr::proxy::proxy_msg::proxy_netdata_def::E_id_start, ::yggr::proxy::proxy_msg::proxy_netdata_def::E_id_end)`

用途说明
- 该文件主要用于代理消息类中进行消息 ID 的范围检测。
- 例如，代理消息类会在类定义中使用 `YGGR_PP_CHECK_PROXY_MSG_ID(E_pak_id)` 验证其 ID 是否有效。

依赖与包含
- 依赖：`yggr/ppex/range_checker.hpp`

注意事项
- 该范围是左闭右闭区间，表示有效消息 ID 在 `0x00008200` 到 `0x00008300` 之间。
- 若新增代理消息类型，应确保其消息 ID 在该范围内，或根据需要调整范围定义。

参考文件
- 原始实现： [proxy_netdata_def.hpp](proxy_netdata_def.hpp)
