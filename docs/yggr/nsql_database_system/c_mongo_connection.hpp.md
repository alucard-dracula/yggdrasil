**c_mongo_connection.hpp — 文档说明**

概述
- **名称**: `c_mongo_connection`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_connection.hpp](yggr/nsql_database_system/c_mongo_connection.hpp)
- **用途**: 对 libmongoc 的 `mongoc_client_t` 进行封装，提供连接生命周期管理、弹出/归还客户端句柄（兼容连接池）、URI / SSL 配置管理、APM 与 Server API 设置，以及线程安全的回调访问封装。

主要类型别名
- `org_type`: `mongoc_client_t`（原生客户端类型）
- `mongoc_client_ptr_type`: `shared_ptr<mongoc_client_t>`（带自定义删除器）
- `init_type`: `c_mongo_connection_init`（连接初始化参数）
- `connection_container_type`: 内部 `mongo_connection_container`（管理单一客户端的容器）

构造与析构
- `c_mongo_connection()` — 默认构造。
- 支持以 URI 字符串或 `init_type` 构造；在启用 SSL 时提供相应的 TLS 参数重载。
- `~c_mongo_connection()` — 清理内部资源（通过 `safe_wrap` 与自定义删除器保证线程安全与正确释放）。

核心方法
- `pop()` / `push()` — 弹出与归还 `mongoc_client_ptr_type`（通过内部 `connection_container` 实现）。
- `revocable()` / `revoke()` — 与原生指针兼容的撤销支持（用于可撤销句柄场景）。
- `uri()` / `uri_string()` — 获取连接的 URI 或其不同编码/字符集表示。
- `clear()` / `empty()` / `is_poped()` — 状态与清理操作。
- `id()` — 返回对象 id（基于指针到整数转换）。
- `reinit(const init_type&)` — 使用新的 `init` 重新初始化连接信息。
- `set_appname(...)` / `set_apm_callbacks(...)` / `set_error_api(...)` / `set_server_api(...)` — 设置全局或连接级别的属性与回调。
- `using_handler(Handler)` — 在安全上下文内以回调方式访问底层客户端句柄，若连接为空会抛出相关错误类型。

线程与安全
- 内部使用 `safe_wrap` 封装 `connection_container` 与 `init_type`，并通过 `locked_unsafe_using_handler` 等方法在修改时取得内部锁，保证并发安全。
- `using_handler` 系列方法将传入的可调用对象以安全方式执行，避免外部直接访问裸指针时的竞态。

实现/注意事项
- 自定义删除器 `mongoc_client_deleter` 用于正确调用 `mongoc_client_destroy`；`mongo_connection_container` 已实现 pop/push 与 revoke 语义以供连接池兼容。
- 多数设置方法通过 `safe_wrap` 的锁定回调在内部执行实际的 libmongoc 调用（例如 `handler_prv_set_connection_info`、`handler_prv_reset_connection`、`handler_prv_set_error_api` 等）。
- SSL 功能由 `MONGOC_ENABLE_SSL` 宏控制，相关方法在启用时才可用。

示例
```cpp
// 使用 URI 创建连接
yggr::nsql_database_system::c_mongo_connection conn("mongodb://localhost:27017");

// 弹出并使用客户端
auto client = conn.pop();
if(client) {
    // 通过 using_handler 在安全上下文内执行需要的操作
    conn.using_handler([&](auto& pclt, bool popped){
        // pclt 是 shared_ptr<mongoc_client_t>
    });

    conn.push(client);
}

// 设置 APM 回调
conn.set_apm_callbacks(apm_callbacks_instance);
```

依赖
- 头文件：
  - `c_mongo_connection_init.hpp`, `c_mongo_connection_pool.hpp`（使用场景相关）
  - `c_mongo_read_prefs.hpp`, `c_mongo_read_concern.hpp`, `c_mongo_write_concern.hpp`
  - `c_mongo_apm_callbacks.hpp`, `c_mongo_server_api.hpp`, `c_mongo_auto_encryption_opts.hpp`

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_connection.md](yggr/nsql_database_system/c_mongo_connection.md)

扩展建议
- 我可以进一步：
  - 读取并摘录 `handler_prv_*` 与 `handler_prv_reset_*` 的实现，补充重置/重建连接的具体流程与错误处理；
  - 添加同步示例，展示如何在高并发环境中安全使用 `using_handler` 与 `pop`/`push`。
