**概述**
- **描述：** `p2p_peer_start` 模板类表示 P2P 引导流程中的“peer start”消息载荷。它仅包含可选的用户数据，并支持移动/拷贝语义、交换以及序列化。
- **头文件：** [yggr/p2p/protocol/p2p_peer_start.hpp](yggr/p2p/protocol/p2p_peer_start.hpp)

**模板参数**
- `User_Data`：用户数据类型，默认类型为 `u32`。

**类型别名**
- `user_data_type`：等同于模板参数 `User_Data`。

**构造函数**
- `p2p_peer_start()`：默认构造，`_user_data` 使用默认构造函数初始化。
- `p2p_peer_start(BOOST_RV_REF(user_data_type) user_data)`：接收右值并移动构造用户数据。
- `p2p_peer_start(const user_data_type& user_data)`：拷贝构造用户数据。
- `p2p_peer_start(BOOST_RV_REF(this_type) right)`：移动构造。
- `p2p_peer_start(const this_type& right)`：拷贝构造。

**赋值运算符**
- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值，使用 `copy_or_move_or_swap` 在移动或拷贝间切换。
- `operator=(const this_type& right)`：拷贝赋值。

**交换支持**
- 成员函数 `swap(this_type& right)`：交换内部 `_user_data`。
- 右值 `swap(BOOST_RV_REF(this_type) right)`：调用左值版本。
- 在命名空间 `yggr::p2p::swap_support` 中提供了自由函数 `swap`，并通过 `using` 引入 `std` 与 `boost`，支持 ADL。

**访问器**
- `user_data_type& user_data()`：返回可变引用，允许修改用户数据。
- `const user_data_type& user_data() const`：返回常量引用，允许读取用户数据。

**序列化**
- 私有 `serialize(Archive& ar, const u32 version)`：序列化成员 `_user_data`。
- `yggr::serialization::access` 是友元以允许访问私有序列化函数。

**成员变量**
- `user_data_type _user_data`：保存用户数据。

**用途**
- 该类用于在 P2P 协议中传递启动阶段的额外信息，通常作为 `peer start` 消息的 payload。
- 由于模板参数可定制用户数据类型，它可以支持简单整数类型，也可以支持更复杂的自定义结构。

**示例（伪代码）**
```cpp
using StartInfo = yggr::p2p::p2p_peer_start<MyUserData>;
StartInfo start(MyUserData(...));
MyUserData data = start.user_data();
```

**相关文件**
- 源文件： [yggr/p2p/protocol/p2p_peer_start.hpp](yggr/p2p/protocol/p2p_peer_start.hpp)

**作者 / 许可证**
- 源文件头部声明了版权与 MIT 风格许可，详情请参见源文件顶部的声明。
