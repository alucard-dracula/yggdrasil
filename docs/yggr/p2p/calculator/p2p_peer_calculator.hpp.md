# p2p_peer_calculator.hpp

## 概述

此头文件定义了 `p2p_peer_calculator` 模板类，用于处理 P2P (Peer-to-Peer) 网络中对等节点的引导和连接计算。引导计算器负责处理引导请求响应、NAT穿透、节点连接建立等核心功能，帮助新节点加入 P2P 网络并建立稳定的对等连接。

## 命名空间

- `yggr::p2p`

## 类描述

### p2p_peer_calculator

一个模板类，用于 P2P 网络的对等节点引导计算和管理。它继承自 `boost::enable_shared_from_this`，支持共享指针管理，同时禁止复制和移动操作。

#### 模板参数

##### `Client_Single`

客户端单例类型。提供客户端实例的访问。

##### `Owner_Container`

所有者信息容器类型。存储节点所有者信息。

##### `P2P_Boot_Request_Back`

P2P 引导请求响应类型。定义引导响应的数据结构。

##### `P2P_Boot_Start`

P2P 引导启动类型。定义引导启动的数据结构。

##### `P2P_Runtime_Info_Mgr`

P2P 运行时信息管理器类型。管理运行时的网络信息。

#### 类型定义

##### 客户端和所有者类型

- `client_single_type`：客户端单例类型
- `owner_info_container_type`：所有者信息容器类型
- `owner_type`：所有者类型
- `owner_id_type`：所有者ID类型

##### 引导相关类型

- `p2p_boot_request_back_type`：P2P引导请求响应类型
- `p2p_boot_start_type`：P2P引导启动类型
- `p2p_runtime_info_mgr_type`：P2P运行时信息管理器类型

##### 网络协议类型

- `net_p2p_boot_request_back_type`：网络P2P引导请求响应协议类型
- `net_p2p_boot_start_type`：网络P2P引导启动协议类型
- `net_p2p_nat_reduce_type`：网络P2P NAT减少协议类型

##### 内部类型

- `peer_boot_info_type`：对等引导信息类型
- `socket_info_type`：套接字信息类型
- `p2p_session_creator_type`：P2P会话创建器类型
- `p2p_peer_start_mode_type`：P2P对等启动模式类型

#### 构造函数

##### 变参构造函数（C++11）

```cpp
template<typename ...Args>
p2p_peer_calculator(BOOST_FWD_REF(Args)... args)
```

使用完美转发构造运行时信息管理器。

**参数：**
- `args`：转发给运行时信息管理器的构造参数

##### 析构函数

```cpp
~p2p_peer_calculator(void)
```

清理资源。

#### 访问器方法

##### `p2p_runtim_info_mgt`

```cpp
p2p_runtime_info_mgr_type& p2p_runtim_info_mgt(void)
const p2p_runtime_info_mgr_type& p2p_runtim_info_mgt(void) const
```

获取运行时信息管理器的引用。

**返回值：**
- 运行时信息管理器的引用

#### 调度器注册方法

##### `register_dispatchers`

```cpp
template<typename Tag, typename CtrlCenter>
void register_dispatchers(CtrlCenter& cc)
```

向控制中心注册各种网络错误的调度器，包括连接重置、超时、消息大小错误等。

**模板参数：**
- `Tag`：标签类型
- `CtrlCenter`：控制中心类型

**参数：**
- `cc`：控制中心引用

#### 计算器注册方法

##### `register_cal_object`

```cpp
template<typename Runner, typename Action_Table, typename Recv_Handler>
void register_cal_object(Action_Table& at, const Recv_Handler& handler)
```

注册计算对象到动作表，处理引导请求响应和NAT减少消息。

**模板参数：**
- `Runner`：运行器类型
- `Action_Table`：动作表类型
- `Recv_Handler`：接收处理器类型

**参数：**
- `at`：动作表引用
- `handler`：接收处理器

##### `unregister_cal_object`

```cpp
template<typename Action_Table>
void unregister_cal_object(Action_Table& at)
```

从动作表中注销计算对象。

**参数：**
- `at`：动作表引用

#### 引导请求响应处理方法

##### `cal_p2p_boot_request_back`

```cpp
template<typename Runner, typename Handler>
void cal_p2p_boot_request_back(const owner_type& owner,
                              const net_p2p_boot_request_back_type& cdt,
                              Runner* prunner,
                              const Handler& handler)
```

处理P2P引导请求响应，解析对等节点信息，建立连接并启动引导过程。

**参数：**
- `owner`：所有者信息
- `cdt`：网络引导请求响应数据
- `prunner`：运行器指针
- `handler`：处理器

#### NAT减少处理方法

##### `cal_p2p_nat_reduce`

```cpp
template<typename Runner, typename Handler>
void cal_p2p_nat_reduce(const owner_type& owner,
                       const net_p2p_nat_reduce_type& cdt,
                       Runner* prunner,
                       const Handler& handler)
```

处理NAT减少消息，更新远程套接字信息以优化NAT穿透。

**参数：**
- `owner`：所有者信息
- `cdt`：网络NAT减少数据
- `prunner`：运行器指针
- `handler`：处理器

#### 网络调度方法

##### `cal_network_dispath`

```cpp
void cal_network_dispath(const owner_id_type& owner_id)
```

处理网络调度，移除断开的对等节点。

**参数：**
- `owner_id`：所有者ID

#### 静态辅助方法

##### `pro_s_get_localhost_socket_info`

```cpp
static socket_info_type&
pro_s_get_localhost_socket_info(socket_info_type& info,
                               typename client_single_type::obj_ptr_type pclt,
                               const owner_type& owner)
```

获取本地主机套接字信息。

**参数：**
- `info`：套接字信息输出
- `pclt`：客户端指针
- `owner`：所有者信息

**返回值：**
- 套接字信息引用

##### `pro_s_connect_and_update`

```cpp
static owner_info_container_type&
pro_s_connect_and_update(owner_info_container_type& owners,
                        typename client_single_type::obj_ptr_type pclt,
                        const typename p2p_boot_request_back_type::peer_boot_info_cont_type& peer_boot_info_cont,
                        const socket_info_type& localhost_socket_info,
                        const net_p2p_boot_start_type& net_boot_start)
```

连接并更新对等节点信息，支持NAT穿透和直接连接。

**参数：**
- `owners`：所有者容器输出
- `pclt`：客户端指针
- `peer_boot_info_cont`：对等引导信息容器
- `localhost_socket_info`：本地主机套接字信息
- `net_boot_start`：网络引导启动消息

**返回值：**
- 所有者容器引用

##### `pro_s_handler_get_localhost_socket_info`

```cpp
static void pro_s_handler_get_localhost_socket_info(typename session_mgr_type::smap_type::base_type& base,
                                                   typename session_mgr_type::smap_citer_type iter,
                                                   socket_info_type& localhost_socket_info)
```

处理获取本地主机套接字信息的回调。

**参数：**
- `base`：会话映射基类
- `iter`：会话映射迭代器
- `localhost_socket_info`：本地主机套接字信息输出

##### `pro_s_handler_send_nat_reduce`

```cpp
static void pro_s_handler_send_nat_reduce(typename session_mgr_type::smap_type::base_type& base,
                                         typename session_mgr_type::smap_iter_type iter,
                                         const owner_type& owner,
                                         const net_p2p_nat_reduce_type& msg,
                                         const socket_info_type& boot_remote_socket_info,
                                         const net_p2p_boot_start_type& boot_start)
```

发送NAT减少消息并处理响应。

**参数：**
- `base`：会话映射基类
- `iter`：会话映射迭代器
- `owner`：所有者信息
- `msg`：NAT减少消息
- `boot_remote_socket_info`：引导远程套接字信息
- `boot_start`：引导启动消息

##### `pro_s_handler_send_nat_reduce_end`

```cpp
static void pro_s_handler_send_nat_reduce_end(const boost::system::error_code& e,
                                             const owner_type& owner,
                                             typename session_mgr_type::session_ptr_type sptr,
                                             const socket_info_type& boot_remote_socket_info,
                                             const net_p2p_boot_start_type& boot_start)
```

处理NAT减少消息发送结束的回调，更新远程端点并发送引导启动消息。

**参数：**
- `e`：错误码
- `owner`：所有者信息
- `sptr`：会话指针
- `boot_remote_socket_info`：引导远程套接字信息
- `boot_start`：引导启动消息

##### `pro_s_handler_update_remote_socket_info`

```cpp
static void pro_s_handler_update_remote_socket_info(typename session_mgr_type::smap_type::base_type& base,
                                                   typename session_mgr_type::smap_iter_type& iter,
                                                   const socket_info_type& remote_socket_info)
```

更新远程套接字信息。

**参数：**
- `base`：会话映射基类
- `iter`：会话映射迭代器
- `remote_socket_info`：远程套接字信息

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/move/move.hpp>`
- `<yggr/system_controller/ctrl_center.hpp>`
- `<yggr/ppex/foo_params.hpp>`
- `<yggr/nonable/noncopyable.hpp>`
- `<yggr/nonable/nonmoveable.hpp>`
- `<yggr/network/network_protocol.hpp>`
- `<yggr/p2p/protocol/p2p_packet_def.hpp>`
- `<yggr/p2p/session_helper/p2p_peer_start_mode.hpp>`
- `<boost/range/functions.hpp>`
- `<boost/range/value_type.hpp>`
- `<boost/range/iterator.hpp>`
- `<cassert>`

## 使用示例

```cpp
#include <yggr/p2p/calculator/p2p_peer_calculator.hpp>

using namespace yggr::p2p;

// 定义模板参数类型
typedef p2p_peer_calculator<
    MyClientSingle,
    MyOwnerContainer,
    MyP2PBootRequestBack,
    MyP2PBootStart,
    MyP2PRuntimeInfoMgr
> peer_calculator_type;

// 创建对等节点计算器
peer_calculator_type calculator(/* 运行时信息管理器参数 */);

// 注册到控制中心
calculator.register_dispatchers<MyTag>(control_center);

// 注册计算对象
calculator.register_cal_object<MyRunner>(action_table, recv_handler);

// 获取运行时信息管理器
auto& runtime_mgr = calculator.p2p_runtim_info_mgt();
```

## 注意事项

- 这是一个复杂的模板类，需要正确配置所有模板参数
- 类继承自 `boost::enable_shared_from_this`，应该使用 `shared_ptr` 管理
- 禁止复制和移动操作，确保资源管理的唯一性
- 处理各种网络错误情况，确保网络的稳定性
- 引导计算涉及NAT穿透和节点发现等复杂逻辑
- 使用任务中心进行异步处理和结果返回
- 错误处理通过异常和错误码机制
- 网络协议使用特定的包ID进行消息路由
- 运行时信息管理器负责维护网络拓扑和节点状态
- 注册的调度器处理连接重置、超时等网络事件
- 计算器对象需要在动作表中正确注册和注销
- 引导请求处理涉及套接字信息获取和节点信息分发
- 支持多个节点的并发引导请求处理
- NAT减少机制用于优化NAT穿透效果
- 静态辅助方法提供底层的网络操作支持
- 异常处理确保网络操作的鲁棒性
- 调试代码被注释但保留，可用于故障排查</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\p2p\calculator\p2p_peer_calculator_cn.md