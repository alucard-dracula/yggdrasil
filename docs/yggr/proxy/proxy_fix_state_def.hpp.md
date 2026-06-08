# proxy_fix_state_def.hpp — 中文文档

概览
- `proxy_fix_state_def` 定义了一组用于代理（proxy）模式创建/切换/注册流程的状态码常量，表示操作结果或状态变化。

常量
- `E_failed = 0`：操作失败。
- `E_accepted = 1`：操作被接受（成功并生效）。
- `E_changed = 2`：操作导致了状态/模式的变更。
- `E_refused = 3`：操作被拒绝（策略决定不进行变更）。
- `E_existsed = 4`：待添加对象已存在（重复）。

用途与语义
- 本结构体常用于 `proxy_container`、`proxy_dispatcher` 等组件返回创建或注册操作的结果码，以便上层逻辑根据不同返回值决定后续处理（例如触发回调、回滚、清理等）。
- `E_changed` 通常表示从旧模式切换到新模式并成功替换；`E_accepted` 表示接受但旧模式与新模式相同或直接接受新加入而无需切换。

依赖与包含
- 包含头文件：`yggr/base/yggrdef.h`、`yggr/base/static_constant.hpp`。

示例（伪代码）
- 创建模式并检查返回：
  - `auto res = container.create_mode(mode);`
  - `if(res.second == proxy_fix_state_def::E_changed) { /* 已切换，执行回调 */ }
  - `else if(res.second == proxy_fix_state_def::E_existsed) { /* 已存在 */ }`

注意事项
- 常量类型为 `u32`（无符号 32 位整型），可直接用于函数返回的第二项（例如 `std::pair<u32,u32>` 中的状态位）。
- 名称 `E_existsed` 拼写保留自源码（非英语语法），其语义为“已存在”。

参考
- 源文件： `proxy_fix_state_def.hpp`
