**概述**
- **描述：** `p2p_connect_handler` 是一个专用的 UDP 连接处理器，继承自 `network::session_helper::udp_connect_handler`，用于 P2P 场景中的连接发起、远端地址更新和会话管理。它封装了 P2P 连接建立过程，并与 session manager、socket pool 和错误处理机制协同工作。
- **头文件：** [yggr/p2p/session_helper/p2p_connect_handler.hpp](yggr/p2p/session_helper/p2p_connect_handler.hpp)

**模板参数**
- `ServicePool`：服务池类型，用于获取 I/O 服务对象。
- `SessionMgrWrap`：会话管理器包装类型，提供对 session manager 的访问。
- `OptionsSetter`：套接字选项设置器类型，用于初始化 UDP 连接的选项。
- `Error_Maker`：错误生成器类型，用于构造并抛出网络相关错误。

**继承关系**
- 继承自 `network::session_helper::udp_connect_handler<ServicePool, SessionMgrWrap, OptionsSetter, Error_Maker>`。
- 所有基类类型别名都在当前类中可用，例如 `socket_ptr_type`、`conn_ptr_type`、`session_mgr_ptr_type`、`endpoint_type` 等。

**主要成员类型**
- `error_maker_type` / `error_type`：错误相关类型。
- `socket_ptr_type`：UDP 套接字智能指针类型。
- `conn_ptr_type`：连接对象智能指针类型。
- `session_mgr_ptr_type`：会话管理器共享指针类型。
- `resolver_type` / `resolver_query_type` / `resolver_iter_type`：地址解析器类型，用于将地址字符串解析为 endpoint。
- `endpoint_type`：网络终结点类型。
- `address_type`：地址类型。

**构造函数**
- 支持可变参数构造函数：
  - 在启用 C++11 可变参数模板时，使用 `template<typename ...Args> explicit p2p_connect_handler(BOOST_FWD_REF(Args)... args)`。
  - 在不支持可变参数模板的环境下，通过预处理宏生成多个参数版本。
- 析构函数 `~p2p_connect_handler()`：默认实现。

**主要方法**
- `void p2p_connect(const Address& addr, const Port& port, Start_Mode& start_mode)`
  - 从 UDP 套接字池获取套接字。
  - 创建连接对象 `now_conn_type` 并初始化。
  - 将连接插入会话管理器，并启动接收。
  - 若分配失败，则通过 `error_maker_type` 抛出 `E_invalid_conn_alloc` 错误。
- `bool p2p_reset_remote_endpoint(const Address& addr, const Port& port)`
  - 将传入的地址字符串解析为 `address_type`，并构造 `endpoint_type`。
  - 通过 session manager 的 `accesse()` 访问器调用 `pro_s_handler_reset_remote_endpoint`，更新连接的远端 endpoint。

**静态辅助方法**
- `static void pro_s_init_connection(conn_ptr_type pconn, const Address& addr, const Port& port)`
  - 若 `pconn` 非空，则使用解析器将地址解析为 endpoint。
  - 调用 `pconn->connect(edpt)` 建立连接。
- `static bool pro_s_handler_reset_remote_endpoint(typename session_mgr_type::smap_iter_type& iter, const endpoint_type& ep)`
  - 如果迭代器指向的会话对象存在，则调用其 `update_remote_endpint(ep)` 更新远端终结点。

**设计与行为**
- 该类依赖基类提供的 `_pudp_socket_pool`、`_service_pool`、`_smgr_wrap` 等成员。
- 使用 `assert` 断言确保 socket pool 可用，并在失败时安全返回。
- `p2p_connect` 会先获取 socket，构造连接，然后插入会话管理器，最后启动异步接收。
- `p2p_reset_remote_endpoint` 适用于运行时调整对等节点的远端地址，例如 NAT 映射更新后重新连接。

**异常与错误处理**
- 当连接对象分配失败时，会通过基类的 `pro_s_throw_error()` 抛出由 `Error_Maker` 产生的错误，并使用系统错误码 `E_UDP_BASE_NETWORK_SYSTEM`。

**示例用法（伪代码）**
```cpp
using Handler = yggr::p2p::p2p_connect_handler<MyServicePool, MySessionMgrWrap, MyOptionsSetter, MyErrorMaker>;
Handler handler(...);
MyStartMode start_mode;
handler.p2p_connect("127.0.0.1", 9000, start_mode);

bool ok = handler.p2p_reset_remote_endpoint("192.168.0.2", 9000);
```

**相关文件**
- 源文件： [yggr/p2p/session_helper/p2p_connect_handler.hpp](yggr/p2p/session_helper/p2p_connect_handler.hpp)
- 继承自：`yggr/network/session_helper/udp_connect_handler.hpp`
- 地址解析帮助：`yggr/network/address_helper.hpp`

**作者 / 许可证**
- 源文件头部声明了版权与 MIT 风格许可，详情请参见源文件顶部的声明。
