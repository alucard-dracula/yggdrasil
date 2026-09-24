# `server_config_template.hpp`

## 概述

该头文件为 Yggdrasil 服务器配置提供通用模板和声明宏。配置类型由两个模板参数决定：

- `Tag`：服务器或网络类型标签；
- `Acceptor`：对应网络协议使用的接收器或套接字类型。

配置类型除了保存这两个类型别名外，还通过 `ERROR_MAKER` 体系提供统一的服务器处理错误码和错误消息。

## 头文件

```cpp
#include <yggr/server/server_config/server_config_template.hpp>
```

## 依赖

主要依赖包括：

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/base/error_make.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/exception/exception_sort.hpp>
```

其中：

- `error_make.hpp` 提供错误码和错误消息生成宏；
- `exception.hpp` 和 `exception_sort.hpp` 提供异常类型及错误码范围定义；
- `yggrdef.h` 提供基础平台定义。

## 命名空间

实现位于：

```cpp
yggr::server::server_config
```

## `server_config_template`

核心模板为：

```cpp
template<typename Tag, typename Acceptor>
struct server_config_template
{
    typedef Tag tag_type;
    typedef Acceptor acceptor_type;
};
```

公开类型别名为：

| 别名 | 含义 |
| --- | --- |
| `tag_type` | 模板参数 `Tag`，用于标识服务器或网络类型 |
| `acceptor_type` | 模板参数 `Acceptor`，用于表示连接接收器或网络套接字 |

该模板不保存运行时配置字段，主要作为类型配置和错误定义的统一载体。

## 错误码定义

模板和声明宏都会创建名为 `server_handler` 的错误集合：

```cpp
ERROR_MAKER_BEGIN("server_handler")
    ...
ERROR_MAKER_END()
```

错误码范围从：

```cpp
exception::exception_sort::E_code_start_2
```

开始，到：

```cpp
exception::exception_sort::E_code_end_2
```

结束。

定义的错误码如下：

| 错误码 | 消息 | 含义 |
| --- | --- | --- |
| `E_invalid_access` | `invalid access` | 访问无效 |
| `E_empty_conn` | `empty connect` | 连接为空 |
| `E_invalid_conn` | `invalid connect` | 连接无效 |
| `E_invalid_conn_alloc` | `connect alloc out of memory` | 连接分配内存不足 |
| `E_invalid_conn_manage` | `connect manage invalided` | 连接管理状态无效 |
| `E_invalid_conn_request` | `connect request invalided` | 连接请求无效 |
| `E_invalid_server` | `invalid server` | 服务器无效 |

错误消息文本由 `ERROR_CODE_MSG` 宏注册，实际错误对象和构造方式由 `error_make.hpp` 提供的错误框架决定。

## `YGGR_PP_SERVER_CONFIG_TEMPLATE_DECL`

文件定义以下声明宏：

```cpp
YGGR_PP_SERVER_CONFIG_TEMPLATE_DECL(__name__, Tag, Acceptor)
```

宏展开为一个结构体：

```cpp
struct __name__
{
    typedef Tag tag_type;
    typedef Acceptor acceptor_type;

    // server_handler 错误码和错误消息
};
```

宏中的错误定义与 `server_config_template<Tag, Acceptor>` 完全一致，适合在具体协议配置头中直接生成具名配置类型。

## TCP 配置示例

`server_tcp_config.hpp` 使用：

```cpp
YGGR_PP_SERVER_CONFIG_TEMPLATE_DECL(
    server_tcp_config_type,
    yggr::network::type_traits::tag_server,
    boost::asio::ip::tcp::acceptor)
```

得到的类型等价于：

```cpp
server_tcp_config_type::tag_type
// yggr::network::type_traits::tag_server

server_tcp_config_type::acceptor_type
// boost::asio::ip::tcp::acceptor
```

## UDP 配置示例

`server_udp_config.hpp` 使用：

```cpp
YGGR_PP_SERVER_CONFIG_TEMPLATE_DECL(
    server_udp_config_type,
    yggr::network::type_traits::tag_server,
    boost::asio::ip::udp::socket)
```

得到的 `acceptor_type` 为：

```cpp
boost::asio::ip::udp::socket
```

虽然名称统一为 `acceptor_type`，实际类型可以是 TCP acceptor，也可以是 UDP socket，具体取决于协议配置头的传入参数。

## `YGGR_TPL_CONFIG_USING_TPL` 分支

具体配置头通常根据以下宏选择声明方式：

```cpp
#ifndef YGGR_TPL_CONFIG_USING_TPL
// 使用 YGGR_PP_SERVER_CONFIG_TEMPLATE_DECL 宏
#else
// 使用 server_config_template<Tag, Acceptor> typedef
#endif
```

宏未定义时，使用声明宏生成独立结构体；宏已定义时，直接使用模板别名形式：

```cpp
typedef ::yggr::server::server_config::server_config_template<
    Tag,
    Acceptor> server_config_type;
```

两种方式都提供 `tag_type`、`acceptor_type` 以及相同的 `server_handler` 错误定义。

## 设计关系

```text
server_config_template.hpp
  -> server_config_template<Tag, Acceptor>
  -> YGGR_PP_SERVER_CONFIG_TEMPLATE_DECL
      -> server_tcp_config_type
      -> server_udp_config_type
```

该文件只定义通用配置形状和错误集合，TCP/UDP 具体头文件负责选择网络标签和 Asio 类型。

## 使用限制与注意事项

- `Tag` 应提供服务器处理流程能够识别的网络或协议标签。
- `Acceptor` 必须与对应服务器实现的连接建立方式匹配；TCP 通常使用 `tcp::acceptor`，UDP 通常使用 `udp::socket`。
- 宏生成的结构体和 `server_config_template` 应保持相同的 `tag_type`、`acceptor_type` 和错误定义语义。
- `YGGR_TPL_CONFIG_USING_TPL` 会改变具体配置头的声明形式，但不应改变配置类型对外提供的接口。
- `ERROR_MAKER` 相关宏属于项目异常基础设施，新增或调整错误码时必须保持错误码范围和消息定义成对出现。
