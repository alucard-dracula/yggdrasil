# `basic_svr_handler.hpp`

## 概述

该头文件定义通用服务器处理器 `basic_svr_handler`。它负责组合服务处理器、网络 acceptor handler 和 session manager，为 TCP/UDP 服务器提供统一的生命周期、会话管理、协议注册、数据发送和访问接口。

主要职责包括：

- 创建并持有网络 acceptor；
- 创建并保护 session manager；
- 启动监听和停止服务；
- 注册或注销网络协议适配器；
- 查询和清理在线 session；
- 向 session manager 转发数据包和访问操作；
- 根据 TCP/UDP 协议执行状态检查；
- 向系统控制中心注册 controller。

## 头文件

```cpp
#include <yggr/server/basic_svr_handler.hpp>
```

## 依赖

主要依赖包括：

```cpp
#include <yggr/network/network_handler.hpp>
#include <yggr/network/session_helper/tcp_async_acceptor_handler.hpp>
#include <yggr/network/session_helper/udp_async_acceptor_handler.hpp>
#include <yggr/network/session_helper/options_setter.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/safe_container/safe_wrap.hpp>
#include <yggr/system_controller/ctrl_center.hpp>
#include <boost/asio.hpp>
```

此外还依赖 Yggdrasil 的共享指针、线程、绑定、类型断言、异常和不可复制/不可移动工具。

## 命名空间

实现位于：

```cpp
yggr::server
```

辅助初始化结构和协议状态检查操作位于：

```cpp
yggr::server::detail
```

## `basic_svr_handler_init_t`

该辅助结构用于保存服务器处理器初始化信息：

```cpp
template<typename Source, typename LinkHandlerInit>
struct basic_svr_handler_init_t
```

保存的数据包括：

| 成员 | 类型 | 用途 |
| --- | --- | --- |
| `_port` | `u16` | 监听端口 |
| `_src` | `Source&` | session manager 或适配器管理器的初始化源 |
| `_lkh_init` | `LinkHandlerInit` | acceptor handler 初始化参数 |

公开访问函数为：

```cpp
u16 port() const;
Source& session_mgr_init() const;
const LinkHandlerInit& lkh_init() const;
```

该结构支持复制构造，但不负责资源所有权管理。

## `basic_svr_handler_check_state_op`

该辅助操作根据协议标签选择状态检查范围：

```cpp
template<
    typename Protocol_Tag,
    typename SessionMgrPtr,
    typename AcceptorHandler>
struct basic_svr_handler_check_state_op;
```

### TCP

当协议标签为：

```cpp
yggr::network::type_traits::tag_tcp
```

只调用：

```cpp
session_manager->check_state();
```

### UDP

当协议标签为：

```cpp
yggr::network::type_traits::tag_udp
```

除了检查 session manager，还会检查 acceptor handler：

```cpp
session_manager->check_state();
acceptor_handler.check_state();
```

## 类模板

```cpp
template<
    typename ServerConfig,
    typename Service_Handler,
    typename SessionMgr,
    typename OptionsSetter = network::default_options_setter_type,
    template<
        typename _ServicePool,
        typename _Acceptor,
        typename _SessionMgrWrap,
        typename _OptionsSetter,
        typename _Error_Maker>
    class Acceptor_Handler =
        network::session_helper::tcp_async_acceptor_handler>
class basic_svr_handler;
```

模板参数含义如下：

| 参数 | 用途 |
| --- | --- |
| `ServerConfig` | 提供 `tag_type`、`acceptor_type`、错误类型等服务器配置 |
| `Service_Handler` | 提供 Asio service、service pool 和运行状态 |
| `SessionMgr` | 管理 session、协议适配器和数据发送 |
| `OptionsSetter` | 设置网络选项，默认使用 `default_options_setter_type` |
| `Acceptor_Handler` | 接受连接并驱动网络监听，默认使用 TCP 异步 acceptor handler |

类私有继承：

```cpp
private nonable::noncopyable,
private nonable::nonmoveable
```

因此 `basic_svr_handler` 不可复制，也不可移动。

## 关键类型别名

公开类型别名包括：

```cpp
typedef ServerConfig server_config_type;
typedef Service_Handler service_handler_type;
typedef SessionMgr session_mgr_type;
typedef OptionsSetter options_setter_type;
typedef typename session_mgr_type::adapter_mgr_type adapter_mgr_type;
typedef typename session_mgr_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;
typedef typename session_mgr_type::protocol_tag_type protocol_tag_type;
typedef typename acceptor_handler_type::init_type link_handler_init_type;
typedef typename server_config_type::tag_type tag_type;
typedef u32 id_type;
typedef detail::basic_svr_handler_init_t<
    source_type,
    link_handler_init_type> init_type;
```

其中 `tag_type` 必须与 `SessionMgr::tag_type` 相同：

```cpp
BOOST_MPL_ASSERT((
    boost::is_same<
        tag_type,
        typename session_mgr_type::tag_type>));
```

这保证服务器配置与 session manager 使用相同的协议或服务器标签。

## 构造与资源组成

构造函数形式为：

```cpp
template<typename Init>
basic_svr_handler(
    service_handler_type& service_handler,
    const Init& init);
```

构造过程会：

1. 保存 service handler 引用；
2. 使用 service handler 的 service 和 `init.port()` 创建 acceptor；
3. 根据 `SessionMgr::is_v4()` 选择 IPv4 或 IPv6 endpoint；
4. 使用 `init.session_mgr_init()` 创建 session manager；
5. 使用 service pool、acceptor 指针、session manager wrap 和 `init.lkh_init()` 创建 acceptor handler。

内部主要成员为：

```cpp
service_handler_type& _service_handler;
acceptor_ptr_type _pacceptor;
session_mgr_wrap_type _session_mgr_wrap;
acceptor_handler_type _acceptor_handler;
```

## 生命周期接口

### `run`

```cpp
inline void run();
```

调用私有 `listen()`，最终转发到：

```cpp
_acceptor_handler.listen();
```

### `stop`

```cpp
void stop();
```

停止流程要求 service handler 已经不在运行：

```cpp
assert(!_service_handler.is_running());
```

随后：

1. 清理 session manager 中的 session；
2. 清空 session manager wrap；
3. 取消并关闭 acceptor；
4. 捕获 `boost::system::system_error` 并转换为 Yggdrasil 异常；
5. 关闭 acceptor handler。

### `get_session_mgr`

```cpp
inline session_mgr_ptr_type get_session_mgr() const;
```

返回 session manager 的共享指针。若内部 wrap 没有有效对象，返回空指针。

## 会话管理

### `online_size`

```cpp
inline size_type online_size() const;
```

返回当前 session 数量。session manager 不存在时返回 `0`。

### `clear_sessions`

```cpp
inline void clear_sessions();
```

仅清理 session，不清理 session manager 本身。

### `clear`

```cpp
inline void clear();
```

调用 session manager 的 `clear()`，执行更完整的管理器清理操作。

## 网络协议注册

### 按数据类型注册

```cpp
template<typename Real_Data>
bool register_network_protocol(
    u32 stat = adapter_mgr_reg_def_type::E_both);
```

将 `Real_Data` 注册到 session manager 的 adapter manager 中。

### 按容器注册

```cpp
template<typename Real_Data, typename Container>
bool register_network_protocol_of_container(
    u32 stat = adapter_mgr_reg_def_type::E_both);
```

为指定容器注册数据适配器。

### 按发送/接收 ID 注册

```cpp
template<typename Container, typename Send_ID, typename Recv_ID>
bool register_network_protocol(
    const Send_ID& send_id,
    const Recv_ID& recv_id,
    u32 stat = adapter_mgr_reg_def_type::E_both);
```

允许显式指定发送 ID 和接收 ID。

### 注销协议

按数据类型注销：

```cpp
template<typename Real_Data>
bool unregister_network_protocol(
    u32 stat = adapter_mgr_reg_def_type::E_both);
```

按 ID、标签和状态注销：

```cpp
template<typename ID>
bool unregister_network_protocol(
    const ID& id,
    u32 tag = adapter_mgr_type::template default_tag<ID>::value,
    u32 stat = adapter_mgr_reg_def_type::E_both);
```

这些函数在 session manager 不存在时返回 `false`。

## 状态检查

```cpp
inline void check_state();
```

该函数取得 session manager 后调用协议相关的 `check_state_op_type`：

- TCP：检查 session manager；
- UDP：检查 session manager 和 acceptor handler。

如果 session manager 不存在，则直接返回。

## 数据发送

提供三种 `send_packet` 重载：

```cpp
template<typename Key, typename NetInfo,
         typename Data, typename Handler>
bool send_packet(
    const Key& key,
    const NetInfo& netinfo,
    const Data& data,
    const Handler& handler);
```

```cpp
template<typename Key, typename Pak, typename Handler>
bool send_packet(
    const Key& key,
    Pak& packet,
    const Handler& handler);
```

```cpp
template<typename Key, typename Pak>
bool send_packet(
    const Key& key,
    Pak& packet);
```

所有重载都将请求转发给 session manager；manager 不存在时返回 `false`。

## Session 访问

### 按 key 访问

```cpp
template<typename Key, typename Handler>
bool access(
    const Key& key,
    const Handler& handler);
```

提供 const 和非 const 两个版本，按 key 将访问请求转发给 session manager。

### 全量访问

```cpp
template<typename Handler>
typename func::foo_t_info<Handler>::result_type
access(const Handler& handler);
```

该接口直接让 handler 访问 session manager，并返回 handler 声明的 `result_type`。session manager 不存在时通过断言反映无效状态。

### `access_of_all`

```cpp
template<typename Handler>
void access_of_all(const Handler& handler);
```

提供 const 和非 const 版本，对全部 session 执行访问操作。session manager 不存在时直接返回。

## Adapter manager

```cpp
inline adapter_mgr_ptr_type get_adapter_mgr_ptr() const;
```

返回 session manager 中的 adapter manager 共享指针。session manager 不存在时返回默认构造的空指针。

## 系统控制器注册

类声明 `yggr::system_controller::ctrl_center` 为友元，并提供受保护函数：

```cpp
template<typename Tag, typename CtrlCenter>
inline void register_controller(CtrlCenter& center);
```

函数将当前 session manager 注册到系统控制中心：

```cpp
center.template register_controller<Tag>(*session_manager);
```

普通业务代码不能直接调用该受保护接口，应通过友元的系统控制中心流程完成注册。

## 生命周期关系

```text
构造
  -> 创建 acceptor
  -> 创建 session manager
  -> 创建 acceptor handler

run()
  -> acceptor_handler.listen()

网络事件
  -> acceptor handler
  -> session manager
  -> session 创建和管理

stop()
  -> 清理 session
  -> 关闭 acceptor
  -> 关闭 acceptor handler
```

## 使用限制与注意事项

- `ServerConfig::tag_type` 必须与 `SessionMgr::tag_type` 完全一致。
- `ServerConfig::acceptor_type` 必须与 `Acceptor_Handler` 支持的 acceptor 类型匹配。
- `stop()` 要求 service handler 已停止运行，否则调试构建中会触发断言。
- 类不可复制、不可移动，通常应通过外部生命周期管理器持有。
- `access`、`send_packet` 和协议注册接口依赖 session manager；manager 为空时，大多数接口返回失败或直接返回。
- TCP 和 UDP 的状态检查逻辑不同，UDP 还会检查 acceptor handler 状态。
