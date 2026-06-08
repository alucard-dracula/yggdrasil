# proxy_modes.hpp — 中文文档

概览
- `proxy_modes.hpp` 是一个聚合头文件，用于包含代理模式相关的所有主要模式实现。
- 它不定义任何新类或函数，仅通过包含各个模式头文件，简化模式选择代码中的头文件引入。

包含的模式
- `proxy_mode_monopolize.hpp`：独占模式，选择列表中的第一个目标。
- `proxy_mode_source_hash.hpp`：源哈希模式，根据请求源 ID 哈希选择目标。
- `proxy_mode_blanace.hpp`：负载均衡模式，轮询选择目标。
- `proxy_mode_reduce.hpp`：广播模式，将请求转发到所有目标。
- `proxy_mode_forward.hpp`：转发模式，根据扩展参数定位目标。

用途说明
- 使用 `proxy_modes.hpp` 可以一次性包含所有默认代理模式实现，适合需要支持多种代理模式的场景。
- 该文件通常在代理模式工厂、模式创建器或代理管理模块中作为便捷入口头文件。

依赖与包含
- 仅包含以下头文件：
  - `yggr/proxy/proxy_mode/proxy_mode_monopolize.hpp`
  - `yggr/proxy/proxy_mode/proxy_mode_source_hash.hpp`
  - `yggr/proxy/proxy_mode/proxy_mode_blanace.hpp`
  - `yggr/proxy/proxy_mode/proxy_mode_reduce.hpp`
  - `yggr/proxy/proxy_mode/proxy_mode_forward.hpp`

注意事项
- 该文件与其对应模式实现文件共同提供一组默认代理模式，若项目只使用其中部分模式，可按需单独包含对应头文件。
- 不直接暴露任何类型或函数名，它只是汇总多个模式头文件的便捷入口。

参考文件
- 原始实现： [proxy_modes.hpp](proxy_modes.hpp)
