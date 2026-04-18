**c_mongo_connection_encryption.hpp — 文档说明**

概述
- **名称**: `c_mongo_connection_encryption`
- **命名空间**: `yggr::nsql_database_system`
- **头文件**: [yggr/nsql_database_system/c_mongo_connection_encryption.hpp](yggr/nsql_database_system/c_mongo_connection_encryption.hpp)
- **用途**: 将 `mongoc_client_encryption_t` 的客户端端加密实例与 `c_mongo_connection` 连接对象绑定、管理并在需要时弹出/推入（pool-like 管理）。类本身为非拷贝、非移动类型，使用智能指针与自定义 deleter 管理原生资源。

主要类型别名
- `connection_type`: `c_mongo_connection`（关联的连接类型）
- `connection_ptr_type`: `shared_ptr<c_mongo_connection>`（连接共享指针）
- `mongoc_client_encryption_ptr_type`: `shared_ptr<mongoc_client_encryption_t>`（加密客户端智能指针，配合 `mongoc_client_encryption_deleter`）
- `client_encryption_opts_type`: `c_mongo_client_encryption_opts`（用于创建加密客户端时的选项类型）

核心职责与语义
- 绑定一个 `c_mongo_connection`（`_pconn`）与一个 `mongoc_client_encryption_t` 实例（`_penc`）。
- 提供 `pop()` / `push()` 接口来在外部代码中获取/归还加密客户端实例。
- 提供 `validate()` 检查是否处于有效绑定状态（即同时有连接和加密实例）。
- `get_client_shared_ptr()` 在条件满足（连接已被 pop 且请求的加密实例与本对象一致）时返回对应的 `mongoc_client_t` 智能指针。

主要方法
- `mongoc_client_encryption_ptr_type pop()` — 将内部 `mongoc_client_encryption_t` 实例返回给调用方（语义上“弹出”以供使用），并在必要时设置 `_poped_pclt_wrap` 以保存弹出时对应的 `mongoc_client` 包装。
- `bool push(const mongoc_client_encryption_ptr_type& ptr)` — 将加密客户端归还到对象（返回是否成功）。
- `bool validate() const` — 是否已正确绑定（`_pconn` 与 `_penc` 均存在）。
- `bool is_poped() const` — 判断关联连接是否处于已弹出状态（委托给 `_pconn->is_poped()`）。
- `connection_ptr_type get_connection_shared_ptr() const` — 返回关联的 `c_mongo_connection` 共享指针。
- `mongoc_client_ptr_type get_client_shared_ptr(const mongoc_client_encryption_ptr_type& penc) const` — 若条件满足（连接被弹出且传入 `penc` 与本对象相同），返回对应的 `mongoc_client_t` 共享指针，否则返回空指针。
- `connection_id_type connection_id() const` — 返回连接 id（若无则返回默认值）。
- `template<typename Handler> bool init(const Handler& handler)` — 使用外部 handler 初始化内部绑定（handler 被调用以设置 `_pconn` 与 `_penc`），若已绑定则返回 false。

实现/注意事项
- 类为不可拷贝/不可移动（继承 `noncopyable` 与 `nonmoveable`），生命周期由调用方显式管理。
- 内部使用 `detail::mongoc_client_encryption_deleter` 作为 `mongoc_client_encryption_t` 智能指针的自定义删除器，确保与 libmongoc 的释放接口正确配合。
- `init` 接受任意可调用对象 `handler`，该 handler 负责把合适的 `connection_ptr_type` 与 `mongoc_client_encryption_ptr_type` 通过引用返回给对象。
- `get_client_shared_ptr` 使用 `_poped_pclt_wrap`（`safe_wrap`）在弹出期间安全地返回底层 `mongoc_client_t` 的共享指针视图。

示例
```cpp
// 假设有一个初始化函数：
auto handler = [&](c_mongo_connection::connection_ptr_type& out_conn, 
                   c_mongo_connection_encryption::mongoc_client_encryption_ptr_type& out_penc) -> bool {
    // 创建或获取现有连接/加密实例
    out_conn = existing_conn_ptr;
    out_penc = mongoc_client_encryption_ptr_type(mongoc_client_encryption_create(...), deleter);
    return true;
};

c_mongo_connection_encryption enc(handler);

// 获取加密客户端实例
auto penc = enc.pop();
if(penc) {
    // 使用 penc 进行加密相关操作，必要时通过 get_client_shared_ptr 取得 mongoc_client
    auto pclt = enc.get_client_shared_ptr(penc);
    // ... 使用 pclt / penc ...
    enc.push(penc); // 使用完成后归还
}
```

依赖
- 头文件：
  - `c_mongo_connection.hpp`
  - `c_mongo_client_encryption_opts.hpp`
  - `detail/mongoc_client_encryption_deleter.hpp`
  - `yggr/smart_ptr_ex/shared_ptr.hpp`, `yggr/safe_container/safe_wrap.hpp`

文件位置
- 文档已生成到: [yggr/nsql_database_system/c_mongo_connection_encryption.md](yggr/nsql_database_system/c_mongo_connection_encryption.md)

扩展建议
- 我可以进一步：
  - 读取并摘录 `detail/mongoc_client_encryption_deleter.hpp`，说明删除器行为；
  - 从相关实现（若存在）补充 `pop`/`push` 具体行为与竞态处理细节；
  - 添加更完整的例子，展示与 `mongoc_client_encryption_t` 的创建/销毁配合方式。
