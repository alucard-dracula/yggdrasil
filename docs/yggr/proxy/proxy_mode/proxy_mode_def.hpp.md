# proxy_mode_def.hpp — 中文文档

概览
- `proxy_mode_def` 定义了代理模式的枚举常量，用于标识不同的代理路由策略。
- 这些常量在代理模式创建、选择和处理逻辑中作为模式 ID 使用。

常量定义
- `E_proxy_mode_failed = 0`：表示代理模式失败或无效。
- `E_proxy_mode_monopolize = 1`：表示独占模式，通常将请求固定路由到一个目标。
- `E_proxy_mode_source_hash = 2`：表示按源哈希分配模式。
- `E_proxy_mode_blanace = 3`：表示负载均衡模式。
- `E_proxy_mode_reduce = 4`：表示广播/分发到所有服务器模式（旧名称为 `proxy_mode_all`）。
- `E_proxy_mode_forward = 5`：表示转发模式。
- `E_proxy_mode_extern = 0x000000ff`：表示外部扩展模式 ID，供自定义或扩展使用。

用途说明
- 这些模式值通常用于代理消息处理、代理模式创建器和路由选择逻辑中。
- 例如，`proxy_mode_creator` 会根据 `E_mode` 值创建对应的 `basic_proxy_mode` 派生对象。

依赖与包含
- 包含头文件：
  - `yggr/base/yggrdef.h`
  - `yggr/base/static_constant.hpp`
  - `boost/mpl/vector_c.hpp`

注意事项
- `E_proxy_mode_extern` 预留为外部扩展模式，用于定义自定义代理模式 ID。
- 如果添加新模式，请确保在代理模式工厂和相关处理逻辑中同步支持该模式。

示例用法（伪代码）

  if(mode == proxy_mode_def::E_proxy_mode_blanace) {
      // 负载均衡处理
  }

参考文件
- 原始实现： [proxy_mode_def.hpp](proxy_mode_def.hpp)
