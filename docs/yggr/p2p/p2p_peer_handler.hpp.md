**概述**
- **描述：** `p2p_peer_handler` 是 P2P 客户端处理器类，继承自 `client::basic_clt_handler`，并暴露 P2P 专用连接功能，包括 `p2p_connect` 和 `p2p_nat_reduce`。
- **头文件：** [yggr/p2p/p2p_peer_handler.hpp](yggr/p2p/p2p_peer_handler.hpp)

**模板参数**
- `ClientConfig`：客户端配置类型。
- `Service_Handler`：服务处理器类型。
- `SessionMgr`：会话管理器类型。
- `OptionsSetter`：套接字选项设置器类型，默认值为 `network::default_options_setter_type`。
- `Connect_Handler`：连接处理器模板类型，默认值为 `p2p_connect_handler`。

**继承关系**
- `p2p_peer_handler` 继承自：
  `client::basic_clt_handler<ClientConfig, Service_Handler, SessionMgr, OptionsSetter, Connect_Handler>`。
- 其基类提供网络协议注册、连接管理、会话访问等通用客户端功能。

**类型别名**
- `client_config_type`：定义客户端配置类型。
- `service_handler_type`：定义服务处理器类型。
- `session_mgr_type`：定义会话管理器类型。
- `options_setter_type`：定义选项设置器类型。
- `base_type`：当前类的基类别名。
- `session_mgr_ptr_type`、`protocol_tag_type`、`link_handler_init_type`、`tag_type`、`init_type`、`adapter_mgr_reg_def_type`：均继承自基类。

**友元声明**
- 使用 `YGGR_PP_FRIEND_CLASS(network::network_handler<this_type>)` 声明 `network::network_handler<this_type>` 为友元类。

**构造函数**
- `template<typename Init> p2p_peer_handler(service_handler_type& sh, const Init& init)`
  - 调用基类构造函数 `base_type(sh, init)` 初始化。
- 默认析构函数 `~p2p_peer_handler()`。

**主要方法**
- `template<typename Address, typename Port, typename Start_Mode> void p2p_connect(const Address& addr, const Port& port, Start_Mode& smode)`
  - 调用基类 `_connect_handler.p2p_connect(addr, port, smode)`，用于发起 P2P 连接。

- `template<typename Address, typename Port, typename Owner, typename Data> void p2p_nat_reduce(const Address& addr, const Port& port, const Owner& owner, const Data& data)`
  - 调用基类 `_connect_handler.p2p_nat_reduce(addr, port, owner, data)`，用于处理 P2P NAT 穿透或远端地址更新。

**继承并公开的基类方法**
- `connect`：发起普通连接。
- `register_network_protocol`：注册网络协议处理器。
- `register_network_protocol_of_container`：注册容器协议处理器。
- `unregister_network_protocol`：注销协议处理器。
- `run`：启动网络处理。
- `get_session_mgr`：访问会话管理器。
- `stop`：停止网络处理。
- `online_size`：获取在线会话数量。
- `clear_sessions`：清理会话。
- `clear`：清理内部状态。
- `check_state`：检查状态。
- `send_packet`：发送数据包。
- `access` / `access_of_all`：访问单个或全部会话。

**设计要点**
- 该类通过模板参数灵活配置连接处理器、会话类型与错误处理。
- P2P 专用方法 `p2p_connect` 与 `p2p_nat_reduce` 直接委托给 `_connect_handler`，保持功能封装和职责分离。
- 利用基类 `basic_clt_handler` 的已有机制，增强了 P2P 客户端处理功能。

**相关文件**
- 源文件： [yggr/p2p/p2p_peer_handler.hpp](yggr/p2p/p2p_peer_handler.hpp)
- 依赖：`yggr/p2p/session_helper/p2p_connect_handler.hpp`

**作者 / 许可证**
- 源文件头部包含版权与 MIT 风格许可声明，详情请参见源文件顶部。
