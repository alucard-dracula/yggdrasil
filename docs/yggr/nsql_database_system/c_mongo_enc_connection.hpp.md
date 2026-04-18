**c_mongo_enc_connection.hpp — 文档说明**

概述
- **名称**: `c_mongo_enc_connection`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_enc_connection.hpp](yggr/nsql_database_system/c_mongo_enc_connection.hpp)
- **用途**: 轻量封装单个 `c_mongo_connection`，并支持一个可选的 keyvault 专用连接。用于从封装连接中弹出/归还 `mongoc_client_t` 智能指针，同时在需要时维护 keyvault 客户端的额外包装，以便配合客户端端加密操作。

主要类型别名
- `mongo_connection_type`: `c_mongo_connection`
- `mongo_connection_ptr_type`: `shared_ptr<c_mongo_connection>`
- `mongoc_client_ptr_type`: `shared_ptr<mongoc_client_t>`
- `mongoc_client_wrap_type`: `safe_wrap<mongoc_client_t>`（用于在弹出 keyvault 客户端时安全保存底层指针视图）

核心行为与方法
- `pop()` — 从内部连接 `_pconn` 弹出 `mongoc_client_ptr_type`，若 keyvault 专用连接存在且匹配，则弹出相应 keyvault 客户端并设置 `_poped_keyvault_clt_wrap`。
- `push(const mongoc_client_ptr_type)` — 将客户端归还到内部连接（返回是否成功）。
- `init(handler)` — 使用外部 `handler` 初始化 `_pconn` 与可选的 `_pconn_keyvalut`（handler 接受两个引用并负责赋值）。初始化成功后调用 `pro_format()` 以修正内部状态。
- `empty()` / `validate()` / `is_poped()` / `id()` — 状态查询方法，分别检查是否无连接、是否已初始化、是否已被弹出以及返回连接 id。

实现细节与语义
- `pro_format()` 用于调整初始化后内部指针关系（例如当主连接和 keyvault 连接指向同一实例时，处理重复引用）。
- `_poped_keyvault_clt_wrap` 用于临时保存弹出的 keyvault 客户端的安全包装视图，避免在使用期间底层指针被提前释放。
- 类不可拷贝且不可移动，生命周期由外部管理。

示例
```cpp
// handler 负责创建或提供主连接与 keyvault 连接（keyvault 可选）
auto handler = [&](std::shared_ptr<c_mongo_connection>& main_conn,
                   std::shared_ptr<c_mongo_connection>& keyvault_conn) -> bool {
    main_conn = std::make_shared<c_mongo_connection>("mongodb://main:27017");
    keyvault_conn = std::make_shared<c_mongo_connection>("mongodb://keyvault:27017");
    return true;
};

c_mongo_enc_connection enc_conn(handler);
auto client = enc_conn.pop();
if(client) {
    // 使用 client
    enc_conn.push(client);
}
```

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_enc_connection.md](yggr/nsql_database_system/c_mongo_enc_connection.md)

扩展建议
- 我可以补充 `pro_format()` 的具体实现行为或示例说明何时需要 keyvault 专用连接（例如在启用客户端端加密时如何配合使用）。
