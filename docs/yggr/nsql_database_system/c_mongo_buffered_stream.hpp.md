**c_mongo_buffered_stream.hpp — 文档说明**

概述
- **名称**: `c_mongo_buffered_stream`
- **所在命名空间**: `yggr::nsql_database_system`
- **所属头文件**: [yggr/nsql_database_system/c_mongo_buffered_stream.hpp](yggr/nsql_database_system/c_mongo_buffered_stream.hpp)
- **用途**: 基于 `c_mongo_basic_stream` 的缓冲流包装类。用于在已有的 Mongo C 驱动原生 `mongoc_stream_t` 之上封装成带缓冲能力的流对象，保留所有基础读写接口与管理语义。

主要类型别名
- **`base_type`**: `c_mongo_basic_stream`（基类）
- **`org_type` / `base_org_type`**: 底层原生流类型（从基类继承）
- **`stream_ptr_type`**: 智能指针类型（从基类继承）

重要特性
- 类为**移动不拷贝**语义（`BOOST_MOVABLE_BUT_NOT_COPYABLE`）。
- 继承并复用 `c_mongo_basic_stream` 的读写接口与资源管理行为。
- 提供多种构造方式以支持：空构造、由原生 `mongoc_stream_t*` 构造（可指定是否由本对象负责销毁）、以及带缓冲区大小的构造。

构造 / 析构 / 赋值
- `c_mongo_buffered_stream()` — 默认构造。
- `c_mongo_buffered_stream(base_org_type* pstream)` — 包装已有原生流，默认不负责删除（行为由基类处理）。
- `c_mongo_buffered_stream(base_org_type* pstream, bool need_del)` — 包装并可指定是否在析构时释放原生流。
- `c_mongo_buffered_stream(base_org_type* porg_base_stream, size_t buffer_size)` — 指定缓冲区大小的构造（用于创建缓冲包装）。
- 移动构造与移动赋值支持，拷贝被禁用。
- `~c_mongo_buffered_stream()` — 析构，若创建时标记需删除原生流则释放。

主要方法
- `operator=(BOOST_RV_REF(this_type) right)` — 移动赋值，通过基类的 `pro_move` 完成资源转移。
- `swap(this_type& right)` 与 `swap(BOOST_RV_REF(this_type) right)` — 交换实现，已导出到 `std` 与 `boost` 名称空间。

实现/语义备注
- 该类并未在头文件中重新实现读写接口（例如 `read`/`write`），而是复用 `c_mongo_basic_stream` 的实现。因此所有 I/O 行为与 `c_mongo_basic_stream` 保持一致，包括对 `iovec` 和原生缓冲的支持。
- 构造时传入的 `buffer_size` 用于创建一个缓冲层（具体缓冲实现和细节在实现或基类中定义）。
- 所有者语义：通过构造函数中 `need_del` 参数或基类语义决定是否在析构时销毁原生 `mongoc_stream_t`。

依赖
- 头文件: `c_mongo_basic_stream.hpp`
- 项目工具: `yggr/utility/copy_or_move_or_swap.hpp`
- 第三方: libmongoc（`mongoc_stream_t` 等原生类型）

示例用法
```cpp
// 包装已有原生流，不由本对象销毁
mongoc_stream_t* native = /* 从某处获得 */;
yggr::nsql_database_system::c_mongo_buffered_stream s(native);

// 包装并由本对象负责销毁
yggr::nsql_database_system::c_mongo_buffered_stream s2(native, true);

// 指定缓冲区大小（示例值）
yggr::nsql_database_system::c_mongo_buffered_stream s3(native, 8192);

// 移动语义
yggr::nsql_database_system::c_mongo_buffered_stream s4(boost::move(s3));

// 使用基类提供的读写接口：
// s2.write(...); s2.read(...);
```

何时扩展文档
- 若需要更详细的 API（按每个重载完整签名、行为边界、错误处理、线程安全说明、缓冲策略细节与性能提示），我可以继续读取并展开基类 `c_mongo_basic_stream` 的实现细节并补全示例。

文件位置
- 文档已生成为: [yggr/nsql_database_system/c_mongo_buffered_stream.md](yggr/nsql_database_system/c_mongo_buffered_stream.md)
