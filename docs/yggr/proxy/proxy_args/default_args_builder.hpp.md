# default_args_builder.hpp — 中文文档

概览
- `default_args_builder` 是为代理调用构建默认参数及路由调用封装的辅助模板类。它提供了用于修改/检查规则的占位模板（在本实现中默认断言失败），以及两个重载的 `route` 方法用于将调用路由到路由器对象。

模板参数
- `OwnerInfo`：调用方/拥有者信息的类型（`owner_info_type`）。
- `ProxyData`：代理相关的数据类型（`proxy_data_type`）。

类型别名
- `owner_info_type`：对应 `OwnerInfo`。
- `proxy_data_type`：对应 `ProxyData`。

构造与析构
- 默认构造与析构，均为空实现。

规则相关方法（占位）
- `modify_rule` 与 `check_rule`：用于修改或校验某个规则的接口模板。实现中：
  - 在支持 C++11 可变参数模板时，声明为 `template<typename Tag, typename ...Args> bool modify_rule(Args... args)` 和 `check_rule`，但函数体为 `assert(false)` 并返回 `false` — 表示默认情况下不提供具体规则实现。
  - 在不支持可变参数模板的情况下，使用预处理宏展开固定参数数量的模板版本，仍然是 `assert(false)` 并返回 `false`。

 说明：这些方法为占位符，实际项目中可能由特化或派生来实现具体逻辑。

路由方法
- `template<typename RouterPtr, typename Runner, typename Handler> void route(RouterPtr prouter, const owner_info_type& owner, const proxy_data_type& proxy_data, Runner* prunner, const Handler& handler, u32 cal_type) const`：
  - 若 `prouter` 非空，则调用 `prouter->route(owner, proxy_data, prunner, handler, cal_type)`。

- `template<typename RouterPtr, typename Runner, typename Handler, typename Args> void route(RouterPtr prouter, const owner_info_type& owner, const proxy_data_type& proxy_data, Runner* prunner, const Handler& handler, u32 cal_type, Args& args) const`：
  - 带额外 `Args` 参数的重载，但目前实现仍只在 `prouter` 非空时调用不带 `args` 的 `route` —— 可能是为了兼容接口或占位，路由器实现应决定如何处理 `args`。

依赖与包含
- 文件中包含：
  - `yggr/base/yggrdef.h`
  - `yggr/move/move.hpp`
  - `yggr/ppex/foo_params.hpp`（用于预处理变参支持）

注意事项
- `modify_rule` 和 `check_rule` 在本实现中不会实际运行（会触发 `assert(false)`），表明该构建器仅提供接口占位，实际业务逻辑应通过特化或继承覆盖这些方法。
- 第二个 `route` 重载接收 `Args& args`，但没有将其传递给 `prouter->route`，这可能为兼容旧接口或留给路由器实现内部获取参数的空间；使用时请确认路由器的 `route` 签名与期望行为一致。

示例用法（伪代码）

  default_args_builder<MyOwnerInfo, MyProxyData> db;
  // 调用路由（无额外 args）
  db.route(prouter, owner, proxy_data, prunner, handler, cal_type);

  // 带 args 的重载（当前实现不会传递 args 到 prouter）
  ArgsType args;
  db.route(prouter, owner, proxy_data, prunner, handler, cal_type, args);

版权与参考
- 源文件包含 yggdrasil 项目的许可声明（MIT 风格），本文档为源代码中文说明，保留原始版权信息。
- 原始实现： [default_args_builder.hpp](default_args_builder.hpp)
