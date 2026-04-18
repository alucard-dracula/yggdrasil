# c_mongo_apm_callbacks.hpp 文档

**概述**:
- **说明**: `c_mongo_apm_callbacks` 是对 libmongoc APM (Application Performance Monitoring) 回调结构的轻量封装与适配器，封装了 `mongoc_apm_callbacks_t` 的各类回调指针，并提供构造、复制、交换、访问器与空状态检查。该类型便于在项目中以 C++ 风格管理 MongoDB 驱动的 APM 回调集。
- **所在文件**: [yggr/nsql_database_system/c_mongo_apm_callbacks.hpp](yggr/nsql_database_system/c_mongo_apm_callbacks.hpp)

**命名空间**:
- `yggr::nsql_database_system`

**主要结构与类型**:
- `yggr_mongoc_apm_callbacks_t`:
  - 按 libmongoc 定义包含的函数指针字段：
    - 命令事件: `started`, `succeeded`, `failed`。
    - 服务器事件: `server_changed`, `server_opening`, `server_closed`。
    - 拓扑事件: `topology_changed`, `topology_opening`, `topology_closed`。
    - 心跳事件: `server_heartbeat_started`, `server_heartbeat_succeeded`, `server_heartbeat_failed`。

- `c_mongo_apm_callbacks`:
  - 继承（protected）自 `yggr_mongoc_apm_callbacks_t`，并通过 `org_type = mongoc_apm_callbacks_t` 提供对原生类型的访问。
  - 类型别名：为每个回调指针与事件类型声明了友好的别名，例如 `mongoc_apm_command_started_cb_type`、`mongoc_apm_command_started_type` 等。
  - 构造/赋值/复制：
    - 默认构造，可用一组回调指针进行构造，或从 `mongoc_apm_callbacks_t` 指针/引用构造。
    - 拷贝/移动构造与赋值（实现调用 `pro_copy` / `pro_swap`）。
  - 访问器：对每个回调指针提供 getter/setter 风格访问函数（例如 `cb_command_strted()`、`cb_server_opening()` 等，既有非常量也有常量版本）。
  - 状态检查：`empty()` 判断所有回调是否均为 null。
  - 原生指针/引用转换：提供 `org_pointer()`、`operator org_type&/org_type*` 等方便在需要原生 `mongoc_apm_callbacks_t` 场景下使用。
  - 交换支持：提供与原生 `mongoc_apm_callbacks_t` 以及同类型之间的 `swap`。

**行为细节**:
- 构造函数允许只传入 `started` 回调指针（其它回调默认 0），便于只关心命令启动事件的场景。
- `empty()` 通过与零初始化的 `yggr_mongoc_apm_callbacks_t` 内存比较实现，快速判断是否未设置任何回调。
- `org_pointer()` 将内部 `yggr_mongoc_apm_callbacks_t` 的地址 reinterpret_cast 为 `mongoc_apm_callbacks_t*`，便于直接传给 libmongoc 的 API（例如 `mongoc_client_set_apm_callbacks`）。

**示例**:

```cpp
using yggr::nsql_database_system::c_mongo_apm_callbacks;

// 假设 user_started_cb 符合 mongoc_apm_command_started_cb_t 签名
c_mongo_apm_callbacks cb(user_started_cb);
if(!cb.empty()) {
    mongoc_client_set_apm_callbacks(client, cb.org_pointer(), user_data);
}

// 修改某个回调
cb.cb_command_succeeded() = my_succeeded_cb;
```

**依赖**:
- libmongoc 类型和回调签名：`mongoc_apm_callbacks_t` 与各类 `mongoc_apm_*` 回调/事件类型（通过 `mongodb_config.hpp` 引入）。
- Boost（`boost::move` / `boost::addressof` / `boost::ref` 等）用于移动/交换与地址操作。

**位置**:
- 源文件: [yggr/nsql_database_system/c_mongo_apm_callbacks.hpp](yggr/nsql_database_system/c_mongo_apm_callbacks.hpp)

如果你希望，我可以：
- 将文档扩展为列出每个回调指针的完整 C 函数签名和参数说明；
- 添加如何在项目中将 `c_mongo_apm_callbacks` 安全传递给 libmongoc（例如生命周期/用户数据管理示例）。
