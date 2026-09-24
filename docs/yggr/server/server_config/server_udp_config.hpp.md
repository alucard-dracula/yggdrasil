# `server_udp_config.hpp`

## 概述

该头文件定义 UDP 服务器使用的配置类型：

```cpp
server_udp_config_type
```

该类型将通用服务器标签绑定为 `tag_server`，并将网络处理类型绑定为 Boost.Asio 的：

```cpp
boost::asio::ip::udp::socket
```

服务器错误码、错误消息和基础类型别名由 `server_config_template.hpp` 提供，本文件只负责 UDP 协议的具体类型适配。

## 头文件

```cpp
#include <yggr/server/server_config/server_udp_config.hpp>
```

## 依赖

主要依赖包括：

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/server/server_config/server_config_template.hpp>
#include <boost/asio.hpp>
```

其中：

- `socket_conflict_fixer.hpp` 提供网络套接字相关的冲突处理适配；
- `tags.hpp` 提供 `yggr::network::type_traits::tag_server`；
- `server_config_template.hpp` 提供配置模板和声明宏；
- `boost/asio.hpp` 提供 UDP socket 类型。

## 命名空间

配置类型位于：

```cpp
yggr::server::server_config
```

## `server_udp_config_type`

该类型的两个核心类型成员为：

```cpp
server_udp_config_type::tag_type
// yggr::network::type_traits::tag_server

server_udp_config_type::acceptor_type
// boost::asio::ip::udp::socket
```

虽然成员名统一为 `acceptor_type`，UDP 配置实际使用的是 UDP socket，而不是 TCP acceptor。这使上层服务器模板可以通过统一的类型成员获取协议相关网络类型。

配置类型还包含通用服务器配置提供的 `server_handler` 错误码和错误消息，包括无效访问、空连接、无效连接、连接分配失败、连接管理失败、连接请求无效和无效服务器等错误。

## 配置声明方式

文件通过 `YGGR_TPL_CONFIG_USING_TPL` 选择两种等价的声明方式。

### 宏声明方式

当未定义 `YGGR_TPL_CONFIG_USING_TPL` 时，使用：

```cpp
YGGR_PP_SERVER_CONFIG_TEMPLATE_DECL(
    server_udp_config_type,
    yggr::network::type_traits::tag_server,
    boost::asio::ip::udp::socket)
```

该宏直接生成 `server_udp_config_type` 结构体，并生成 `tag_type`、`acceptor_type` 以及通用服务器错误定义。

### 模板 typedef 方式

当定义 `YGGR_TPL_CONFIG_USING_TPL` 时，使用：

```cpp
typedef
    ::yggr::server::server_config::server_config_template<
        yggr::network::type_traits::tag_server,
        boost::asio::ip::udp::socket>
    server_udp_config_type;
```

该分支直接复用 `server_config_template<Tag, Acceptor>`，避免重复展开配置结构体。

## 与通用配置模板的关系

```text
server_config_template.hpp
  -> server_config_template<Tag, Acceptor>
  -> YGGR_PP_SERVER_CONFIG_TEMPLATE_DECL
      -> server_udp_config_type
          tag_type      = tag_server
          acceptor_type = udp::socket
```

无论采用宏声明还是 typedef，外部代码都应通过 `server_udp_config_type::tag_type` 和 `server_udp_config_type::acceptor_type` 访问配置类型信息。

## 与 TCP 配置的区别

同目录的 `server_tcp_config.hpp` 使用相同的服务器标签，但将 `acceptor_type` 设置为：

```cpp
boost::asio::ip::tcp::acceptor
```

两种配置的主要差异是网络处理类型：

| 配置类型 | `tag_type` | `acceptor_type` |
| --- | --- | --- |
| `server_tcp_config_type` | `tag_server` | `boost::asio::ip::tcp::acceptor` |
| `server_udp_config_type` | `tag_server` | `boost::asio::ip::udp::socket` |

## 使用限制与注意事项

- 使用该配置时，服务器实现应按 UDP socket 的接口处理数据报通信。
- `server_udp_config_type` 只是编译期类型配置，不包含运行时端口、地址或 socket 参数。
- `YGGR_TPL_CONFIG_USING_TPL` 会改变声明实现方式，但不应改变对外类型成员。
- 修改 `tag_type` 或 `acceptor_type` 时，应同步检查依赖该配置的服务器处理器和网络管理代码。
- 具体错误码定义由 `server_config_template.hpp` 统一维护，本文件不应重复定义。
