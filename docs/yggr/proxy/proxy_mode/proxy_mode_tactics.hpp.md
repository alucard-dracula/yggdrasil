# proxy_mode_tactics.hpp — 中文文档

概览
- `proxy_mode_tactics` 定义了代理模式转换的策略规则。
- 它用于在旧模式与新模式之间决定最终应用的代理模式，支持部分模式间的规则转换。

类型说明
- `mode_def_type`：别名为 `proxy_mode_def`，包含所有代理模式常量。

主要逻辑
- `u32 operator()(u32 old_mode, u32 new_mode) const`：
  - 根据当前旧模式 `old_mode` 和请求的新模式 `new_mode`，返回最终确定的模式 ID。
  - 返回值含义：
    - 返回 `0` 表示拒绝转换。
    - 返回与输入相同的模式表示保持原模式或接受相同模式。
    - 返回不同的非零模式表示已切换到该模式。

具体策略
- `E_proxy_mode_monopolize`：
  - 转换到 `E_proxy_mode_monopolize` 或 `E_proxy_mode_blanace` 时，结果为 `E_proxy_mode_blanace`。
  - 转换到 `E_proxy_mode_source_hash` 时，结果为 `E_proxy_mode_source_hash`。
  - 转换到 `E_proxy_mode_reduce` 时拒绝（返回 0）。

- `E_proxy_mode_source_hash`：
  - 转换到 `E_proxy_mode_monopolize` 或 `E_proxy_mode_source_hash` 时，结果为 `E_proxy_mode_source_hash`。
  - 转换到 `E_proxy_mode_blanace` 时，结果为 `E_proxy_mode_blanace`。
  - 转换到 `E_proxy_mode_reduce` 时拒绝。

- `E_proxy_mode_blanace`：
  - 转换到 `E_proxy_mode_monopolize` 或 `E_proxy_mode_blanace` 时，结果为 `E_proxy_mode_blanace`。
  - 转换到 `E_proxy_mode_source_hash` 时，结果为 `E_proxy_mode_source_hash`。
  - 转换到 `E_proxy_mode_reduce` 时拒绝。

默认规则
- 对于未显式列出的模式（例如 `E_proxy_mode_reduce`、`E_proxy_mode_forward` 或其他自定义模式），使用默认处理：
  - 如果 `old_mode == new_mode`，返回 `old_mode`。
  - 否则返回 `0`，表示拒绝转换。

依赖与包含
- `yggr/base/yggrdef.h`
- `yggr/proxy/proxy_mode/proxy_mode_def.hpp`

注意事项
- 本策略不允许从三种主要模式之一直接转换到 `E_proxy_mode_reduce`。
- `E_proxy_mode_blanace` 被视为最具兼容性的模式，多个转化路径都会将结果调整为该模式。
- 默认情况下，未列出的模式只允许自身转换。

示例用法（伪代码）

  proxy_mode_tactics tactics;
  u32 mode = tactics(old_mode, new_mode);
  if(mode == 0) {
      // 转换失败
  } else {
      // 使用 mode
  }

参考文件
- 原始实现： [proxy_mode_tactics.hpp](proxy_mode_tactics.hpp)
