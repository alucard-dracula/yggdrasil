**c_mongo_connection_pool.hpp — 文档说明**

概述
- **名称**: `c_mongo_connection_pool`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_connection_pool.hpp](yggr/nsql_database_system/c_mongo_connection_pool.hpp)
- **用途**: 对 libmongoc 的 `mongoc_client_pool_t` 进行 C++ 封装，提供安全线程访问、弹出/归还（pop/push）客户端句柄、以及配置与管理连接池的能力。

核心类型别名
- `org_type`: `mongoc_client_pool_t`（原生池类型）
- `org_client_type`: `mongoc_client_t`（原生客户端类型）
- `mongoc_client_pool_ptr_type`: `shared_ptr<org_type>`（池智能指针，带自定义删除器）
- `mongoc_client_ptr_type`: `shared_ptr<org_client_type>`（客户端智能指针，带自定义删除器）
- `init_type`: `c_mongo_connection_init`（用于初始化连接信息的类型）

主要功能
- 创建并管理 `mongoc_client_pool_t` 实例（支持最小/最大池大小设置）。
- `pop()` / `push()`：弹出与归还 `mongoc_client_t` 智能指针。支持两种弹出模式：正常模式与 `pop_mode`（可配置）。
- `necessarily_pop()` / `try_pop()`：分别为阻塞式或尝试式弹出（在类中声明，具体行为在实现中）。
- `set_appname()`、`set_apm_callbacks()`、`set_error_api()`、`set_server_api()`：对池全局设置（AppName、APM 回调、错误 API 版本、Server API 等）。
- `min_size()` / `max_size()`：运行时调整池的最小/最大大小。
- `client_pool_pointer()`：获取底层池智能指针。
- `uri()` / `uri_string()`：获取初始化时的 URI 字符串的多种表示形式（带字符集/allocator 支持）。

线程与安全
- 使用 `safe_wrap` 封装底层 `client_pool_ptr_type` 与 `init_type`，并使用 `boost::shared_mutex` / `boost::atomic` 等以保证并发安全访问。
- `clear()` 用于销毁池并等待内部线程结束，注意销毁顺序（先释放池再释放 SSL 选项）。

行为与边界
- 构造支持：直接用 URI 字符串构造（可选 TLS/SSL 参数），或以 `init_type` 构造并指定 `pool_min_size`/`pool_max_size` 与 `pop_mode`。
- `operator bool()` 显示池是否已就绪（通过安全加载 `_wrap_client_pool` 判断）。
- `revoke()` / `revoke(org_client_type*)`：将原生客户端句柄包装为智能指针并归还池。
- `gen_connection()`：根据当前 `init` 生成 `c_mongo_connection` 对象（用于快速创建基于池配置的连接）。

实现/注意事项
- 自定义删除器 `mongoc_client_pool_deleter` 与 `pointer_mongoc_client_deleter` 用于正确释放 libmongoc 资源。后者支持 `nonfree` 模式以避免重复释放。
- 许多方法通过 `safe_wrap::locked_unsafe_using_handler` 与绑定函数在内部同步修改操作（例如 `prv_set_connection_info`）。
- SSL 支持由 `MONGOC_ENABLE_SSL` 宏控制，相关构造/方法仅在启用时可用。

示例
```cpp
// 基本构造
yggr::nsql_database_system::c_mongo_connection_pool pool("mongodb://localhost:27017", 0, 50);

// 弹出客户端
auto client = pool.pop();
if(client) {
    // 使用 client
    pool.push(client);
}

// 设置 appname
pool.set_appname(std::string("my_app"));

// 清理
pool.clear();
```

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_connection_pool.md](yggr/nsql_database_system/c_mongo_connection_pool.md)

扩展建议
- 我可以：
  - 展开 `necessarily_pop()` / `try_pop()` 的实现细节并示例化它们的不同语义；
  - 补充关于销毁顺序、线程竞争与等待策略的说明（需读取实现文件）。
