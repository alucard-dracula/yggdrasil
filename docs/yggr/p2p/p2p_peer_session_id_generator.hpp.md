**概述**
- **描述：** `p2p_peer_session_id_generator` 是一个用于生成 P2P 会话 ID 的模板类。它封装了一个 ID 生成器，确保生成的 ID 与 `Owner_ID` 类型一致，并支持在双方拥有者 ID 的情况下生成会话 ID。
- **头文件：** [yggr/p2p/p2p_peer_session_id_generator.hpp](yggr/p2p/p2p_peer_session_id_generator.hpp)

**模板参数**
- `Owner_ID`：会话 ID 的类型，也是对等节点所有者 ID 的类型。
- `ID_Generator`：实际的 ID 生成器类型，要求其 `id_type` 与 `Owner_ID` 相同。

**类型别名**
- `owner_id_type`：定义为 `Owner_ID`。
- `id_generator_type`：定义为 `ID_Generator`。
- `result_type`：定义为 `owner_id_type`，表示生成的结果类型。

**静态断言**
- 使用 `BOOST_MPL_ASSERT` 确保 `id_generator_type::id_type` 与 `owner_id_type` 相同，从而保证生成器返回的 ID 类型与所有者 ID 类型一致。

**构造与析构**
- 默认构造函数 `p2p_peer_session_id_generator()`。
- 默认析构函数 `~p2p_peer_session_id_generator()`。

**主要方法**
- `inline result_type operator()(void) const`
  - 创建 `id_generator_type` 的临时实例并调用 `gen()`，返回新生成的会话 ID。

- `inline result_type operator()(const owner_id_type& owner_id_self, const owner_id_type& owner_id_other) const`
  - 如果 `owner_id_self` 为默认构造值，则返回默认 `owner_id_type()`。
  - 否则同样创建 `id_generator_type` 的临时实例并返回 `gen()` 的结果。

**行为说明**
- 当自己节点的 `owner_id_self` 不存在或无效时，第二个重载返回默认值，表示不生成会话 ID。
- 该生成器不直接使用 `owner_id_other`，但存在这个参数以便接口与调用方兼容，并留出扩展空间。

**适用场景**
- 在 P2P 引导流程中，为每对节点分配唯一会话 ID。
- 结合具体的 `ID_Generator` 类型（如 UUID、整型 ID、用户自定义 ID 生成器）使用。

**示例用法**
```cpp
using Generator = yggr::p2p::p2p_peer_session_id_generator<MyOwnerId, MyIdGenerator>;
Generator gen;
MyOwnerId session_id = gen();
MyOwnerId conditional_id = gen(owner_self_id, owner_other_id);
```

**相关文件**
- 源文件： [yggr/p2p/p2p_peer_session_id_generator.hpp](yggr/p2p/p2p_peer_session_id_generator.hpp)

**作者 / 许可证**
- 源文件头部包含版权与 MIT 风格许可声明，详情请参见源文件顶部。
