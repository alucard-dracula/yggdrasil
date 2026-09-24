# `server.hpp`

## 概述

该头文件定义轻量服务器包装类：

```cpp
template<typename Handler>
class server;
```

`server<Handler>` 通过公有继承复用已有 handler 的实现，并负责：

- 将构造参数转发给 handler；
- 暴露 handler 的 `join`、`run` 和 `stop` 接口；
- 兼容支持和不支持 C++11 可变参数模板的编译环境。

该类不增加服务器运行时状态，也不实现监听、会话管理或线程调度逻辑，具体行为全部由 `Handler` 提供。

## 头文件

```cpp
#include <yggr/server/server.hpp>
```

## 依赖

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/ppex/foo_params.hpp>
```

`yggrdef.h` 提供基础定义，`foo_params.hpp` 为不支持可变参数模板的编译分支提供参数列表生成宏。

## 命名空间

类位于：

```cpp
yggr::server
```

## `server<Handler>`

定义形式为：

```cpp
template<typename Handler>
class server : public Handler
{
public:
    typedef Handler base_type;
};
```

公开类型别名：

```cpp
typedef Handler base_type;
```

`base_type` 用于表示实际服务器 handler 类型，也用于构造函数初始化和成员函数转发。

## 构造函数转发

### 支持可变参数模板时

当未定义以下宏时：

```cpp
YGGR_NO_CXX11_VARIADIC_TEMPLATES
```

类提供可变参数构造函数：

```cpp
template<typename ...Args>
explicit server(BOOST_FWD_REF(Args) ...args)
    : base_type(boost::forward<Args>(args)...)
{
}
```

该构造函数将任意数量的参数完美转发给 `Handler` 的对应构造函数。

### 不支持可变参数模板时

当定义 `YGGR_NO_CXX11_VARIADIC_TEMPLATES` 时，文件通过预处理器循环生成多组构造函数：

```cpp
#define BOOST_PP_LOCAL_MACRO(__n__)
#define BOOST_PP_LOCAL_LIMITS \
    (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#include BOOST_PP_LOCAL_ITERATE()
```

生成的构造函数使用 `YGGR_PP_FOO_PARAMS_DEF` 创建参数列表，并使用 `boost::forward` 将参数传递给基类构造函数。

该分支用于兼容不支持 C++11 variadic templates 的编译器，实际支持的最大参数数量由：

```cpp
YGGR_PP_FOO_DEFAULT_PARAMS_LEN
```

决定。

## 继承接口

类通过 using 声明将基类接口公开到 `server`：

```cpp
using base_type::join;
using base_type::run;
using base_type::stop;
```

因此，使用者可以直接调用：

```cpp
server_instance.join();
server_instance.run();
server_instance.stop();
```

实际执行的逻辑分别由 `Handler::join`、`Handler::run` 和 `Handler::stop` 实现。

如果基类没有提供这些成员函数，类模板实例化或 using 声明会产生编译错误。

## 生命周期

```text
构造 server<Handler>
  -> 参数转发到 Handler 构造函数

run()
  -> Handler::run()

join()
  -> Handler::join()

stop()
  -> Handler::stop()

析构
  -> 执行 server 析构
  -> 随后析构 Handler 基类
```

`server` 的析构函数为空，不主动调用 `stop` 或 `join`。调用顺序和线程停止责任由外部代码及 `Handler` 决定。

## 与 `basic_svr_handler` 的关系

`basic_svr_handler` 可以作为 `Handler` 的一种实现：

```cpp
server<basic_svr_handler<
    ServerConfig,
    ServiceHandler,
    SessionMgr>>
```

此时：

- `server` 负责构造参数转发和统一入口；
- `basic_svr_handler` 负责 acceptor、session manager、监听和会话处理；
- `run`、`stop` 等调用最终落到 `basic_svr_handler`。

## 使用限制与注意事项

- `Handler` 必须提供与传入参数匹配的构造函数。
- `Handler` 必须提供 `join`、`run` 和 `stop` 成员函数。
- `server` 使用公有继承，因此 `Handler` 的公共接口和生命周期语义会影响包装类。
- 该类不负责自动停止服务；析构前是否调用 `stop` 或 `join` 由使用者和基类约定决定。
- 老编译器分支的构造函数数量受 `YGGR_PP_FOO_DEFAULT_PARAMS_LEN` 限制。
