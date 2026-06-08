# p2p_boot_calculator.hpp

## 概述

此头文件定义了 `p2p_boot_calcuator` 模板类（注意：文件名中的拼写为 calculator，但类名为 calcuator），用于处理 P2P (Peer-to-Peer) 网络中的引导计算。引导计算器负责处理节点的引导请求，计算和分发网络中的其他节点信息，以帮助新节点加入 P2P 网络。

## 命名空间

- `yggr::p2p`

## 类描述

### p2p_boot_calcuator

一个模板类，用于 P2P 网络的引导计算和管理。它继承自 `boost::enable_shared_from_this`，支持共享指针管理，同时禁止复制和移动操作。

#### 模板参数

##### `Server_Single`

服务器单例类型。提供服务器实例的访问。

##### `Session_Mgr`

会话管理器类型。管理网络会话。

##### `Owner_Container`

所有者信息容器类型。存储节点所有者信息。

##### `P2P_Boot_Request`

P2P 引导请求类型。定义引导请求的数据结构。

##### `P2P_Boot_Request_Back`

P2P 引导请求响应类型。定义引导响应的数据结构。

##### `P2P_Runtime_Info_Mgr`

P2P 运行时信息管理器类型。管理运行时的网络信息。

#### 类型定义

##### 服务器和会话类型

- `server_single_type`：服务器单例类型
- `session_mgr_type`：会话管理器类型

##### 所有者信息类型

- `owner_info_container_type`：所有者信息容器类型
- `owner_type`：所有者类型
- `owner_id_type`：所有者ID类型

##### 引导请求类型

- `p2p_boot_request_type`：P2P引导请求类型
- `p2p_boot_request_back_type`：P2P引导请求响应类型
- `socket_info_type`：套接字信息类型

##### 运行时信息管理

- `p2p_runtime_info_mgr_type`：P2P运行时信息管理器类型
- `p2p_boot_request_back_map_type`：引导请求响应映射类型

##### 网络协议类型

- `net_p2p_boot_request_type`：网络P2P引导请求协议类型
- `net_p2p_boot_request_back_type`：网络P2P引导请求响应协议类型

#### 构造函数

##### 变参构造函数（C++11）

```cpp
template<typename ...Args>
p2p_boot_calcuator(BOOST_FWD_REF(Args)... args)
```

使用完美转发构造运行时信息管理器。

**参数：**
- `args`：转发给运行时信息管理器的构造参数

#### 析构函数

```cpp
~p2p_boot_calcuator(void)
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

向控制中心注册各种网络错误的调度器。

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

注册计算对象到动作表。

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

#### 引导请求计算方法

##### `cal_p2p_boot_request`

```cpp
template<typename Runner, typename Handler>
void cal_p2p_boot_request(const owner_type& owner,
                         const net_p2p_boot_request_type& cdt,
                         Runner* prunner,
                         const Handler& handler)
```

计算P2P引导请求，处理节点的引导逻辑。

**参数：**
- `owner`：所有者信息
- `cdt`：网络引导请求数据
- `prunner`：运行器指针
- `handler`：处理器

#### 网络调度方法

##### `cal_network_dispath`

```cpp
void cal_network_dispath(const owner_id_type& owner_id)
```

处理网络调度，移除断开的节点。

**参数：**
- `owner_id`：所有者ID

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/system_controller/ctrl_center.hpp>`
- `<yggr/ppex/foo_params.hpp>`
- `<yggr/container/mapped_value.hpp>`
- `<yggr/network/network_protocol.hpp>`
- `<yggr/task_center/task_info_def.hpp>`
- `<yggr/iterator_ex/iterator.hpp>`
- `<yggr/p2p/protocol/p2p_packet_def.hpp>`
- `<yggr/p2p/session_helper/p2p_peer_start_mode.hpp>`
- `<yggr/p2p/p2p_peer_mapping.hpp>`
- `<boost/range/functions.hpp>`
- `<boost/range/value_type.hpp>`
- `<boost/range/iterator.hpp>`

## 使用示例

```cpp
#include <yggr/p2p/calculator/p2p_boot_calculator.hpp>

using namespace yggr::p2p;

// 定义模板参数类型
typedef p2p_boot_calcuator<
    MyServerSingle,
    MySessionMgr,
    MyOwnerContainer,
    MyP2PBootRequest,
    MyP2PBootRequestBack,
    MyP2PRuntimeInfoMgr
> boot_calculator_type;

// 创建引导计算器
boot_calculator_type calculator(/* 运行时信息管理器参数 */);

// 注册到控制中心
calculator.register_dispatchers<MyTag>(control_center);

// 注册计算对象
calculator.register_cal_object<MyRunner>(action_table, recv_handler);

// 获取运行时信息管理器
auto& runtime_mgr = calculator.p2p_runtim_info_mgt();
```

## 注意事项

- 这是一个复杂的模板类，需要正确配置所有模板参数
- 类名拼写为 `p2p_boot_calcuator`，但文件名使用 `calculator`
- 继承自 `boost::enable_shared_from_this`，应该使用 `shared_ptr` 管理
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
- 支持多个节点的并发引导请求处理</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\p2p\calculator\p2p_boot_calculator_cn.md