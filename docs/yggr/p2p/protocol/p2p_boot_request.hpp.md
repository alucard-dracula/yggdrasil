**概述**
- **描述：** `p2p_boot_request` 是一个用于 P2P 引导请求的模板类，封装本地套接字信息和可选的用户数据。它支持拷贝与移动语义、交换操作和序列化。
- **头文件：** [yggr/p2p/protocol/p2p_boot_request.hpp](yggr/p2p/protocol/p2p_boot_request.hpp)

**模板参数**
- `Socket_Info`：本地套接字信息类型。
- `User_Data`：用户数据类型，默认 `u32`。

**类型别名**
- `socket_info_type`：等同于 `Socket_Info`。
- `user_data_type`：等同于 `User_Data`。

**构造函数**
- `p2p_boot_request()`：默认构造，使用默认构造初始化 `_local_socket_info` 和 `_user_data`。
- `p2p_boot_request(const socket_info_type& socket_info)`：通过常量引用初始化本地套接字信息。
- `p2p_boot_request(const socket_info_type& socket_info, const user_data_type& user_data)`：同时初始化套接字信息和用户数据。
- `p2p_boot_request(BOOST_RV_REF(socket_info_type) socket_info)`：移动构造本地套接字信息。
- `p2p_boot_request(BOOST_RV_REF(socket_info_type) socket_info, BOOST_RV_REF(user_data_type) user_data)`：移动构造套接字信息和用户数据。
- `p2p_boot_request(BOOST_RV_REF(this_type) right)`：移动构造自身对象。
- `p2p_boot_request(const this_type& right)`：拷贝构造自身对象。

**赋值运算符**
- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值，使用 `copy_or_move_or_swap` 处理成员。
- `operator=(const this_type& right)`：拷贝赋值。

**交换（swap）**
- 成员函数 `swap(this_type& right)`：交换 `_local_socket_info` 和 `_user_data`。
- 右值 `swap(BOOST_RV_REF(this_type) right)`：调用左值版本。
- 在 `yggr::p2p::swap_support` 中提供自由函数 `swap`，并将其引入到 `std` 和 `boost` 命名空间。

**访问器**
- `socket_info_type& local_socket_info()` / `const socket_info_type& local_socket_info() const`：访问本地套接字信息。
- `user_data_type& user_data()` / `const user_data_type& user_data() const`：访问用户数据。

**序列化**
- 私有 `serialize(Archive& ar, const u32 version)`：序列化 `_local_socket_info` 和 `_user_data`。
- 该类为 `yggr::serialization::access` 友元。

**成员变量**
- `socket_info_type _local_socket_info`：本地套接字信息。
- `user_data_type _user_data`：用户数据。

**示例**
```cpp
using Request = yggr::p2p::p2p_boot_request<MySocketInfo, MyUserData>;
Request req(MySocketInfo(...), MyUserData(...));
auto& info = req.local_socket_info();
auto& data = req.user_data();
```

**作者 / 许可证**
- 源文件头部声明了版权与 MIT 风格许可，详情请参见源文件顶部。
