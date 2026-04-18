**c_mongo_enc_connection_pool.hpp — 文档说明**

概述
- **名称**: `c_mongo_enc_connection_pool`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_enc_connection_pool.hpp](yggr/nsql_database_system/c_mongo_enc_connection_pool.hpp)
- **用途**: 简单包装并管理两个 `c_mongo_connection_pool`（一个用于常规连接，一个用于 keyvault/加密相关连接）的轻量适配器。用于在需要时初始化并在内部协调两个连接池的引用关系。

主要类型别名
- `mongo_connection_pool_type`: `c_mongo_connection_pool`（被封装的池类型）
- `mongo_connection_pool_ptr_type`: `shared_ptr<mongo_connection_pool_type>`
- `mongoc_client_ptr_type`: 底层 `mongoc_client_t` 的智能指针（来自被封装池）
- `uri_type` / `inner_string_type`: 继承自被封装池的 URI / 字符串类型别名

主要行为
- `pop()` / `push()` — 将调用委托给内部 `_pconn_pool`（若存在），返回或归还 `mongoc_client_ptr_type`。
- `init(handler)` — 接受一个 handler，用于同时初始化内部两个池 `_pconn_pool` 和 `_pconn_pool_keyvalut`（handler 应设置这两个指针）；初始化成功后会调用 `pro_format()` 做必要的格式化（如果两个指针相同则清空 keyvault 指针以避免自引用）。
- `empty()` / `validate()` — 快速检查内部池是否初始化。

实现细节与注意事项
- 该类为不可拷贝且不可移动；设计为轻量的适配器，生命周期由外部管理。
- `pro_format()` 用于确保当主池和 keyvault 池指向同一实例时，将 `_pconn_pool_keyvalut` 清空，以避免重复使用同一池作为两个角色。
- `init` 要求 handler 同时设置 `_pconn_pool` 与 `_pconn_pool_keyvalut`，并在返回 true 时调用 `pro_format()`。

示例
```cpp
// handler 负责创建两个池或将已有池赋值给引用
auto handler = [&](std::shared_ptr<c_mongo_connection_pool>& pmain,
                   std::shared_ptr<c_mongo_connection_pool>& pkeyvault) -> bool {
    pmain = std::make_shared<c_mongo_connection_pool>("mongodb://main:27017");
    pkeyvault = std::make_shared<c_mongo_connection_pool>("mongodb://keyvault:27017");
    return true;
};

c_mongo_enc_connection_pool enc_pool(handler);
auto client = enc_pool.pop();
if(client) {
    // 使用 client
    enc_pool.push(client);
}
```

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_enc_connection_pool.md](yggr/nsql_database_system/c_mongo_enc_connection_pool.md)

扩展建议
- 我可以补充典型 handler 实现示例（例如如何从配置构建两个不同的 `c_mongo_connection_pool`），或检查并记录当两个指针相同时的具体行为（为何需要清空 keyvault 指针）。
