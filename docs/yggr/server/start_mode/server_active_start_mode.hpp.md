# `server_active_start_mode.hpp`

## 概述

该头文件定义主动式服务器启动策略 `server_active_start_mode`。它用于在连接建立后主动完成会话创建流程：

1. 从连接生成会话或连接 ID；
2. 根据 ID 生成启动数据；
3. 生成 owner 信息；
4. 通过连接发送启动包；
5. 在异步发送回调中创建 session；
6. 成功创建后启动 session。

该策略本身不保存运行时状态，行为由模板参数提供的生成器、连接对象和 session manager 接口决定。

## 头文件

```cpp
#include <yggr/server/start_mode/server_active_start_mode.hpp>
```

## 依赖

主要依赖包括：

```cpp
#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/network/socket_conflict_fixer.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <boost/asio.hpp>
#include <boost/type_traits/is_same.hpp>
#include <cassert>
```

这些依赖分别提供基础定义、绑定回调、类型工具、网络标签、Asio 错误类型、类型判断和调试断言。

## 命名空间

类位于：

```cpp
yggr::server::start_mode
```

## 类模板

```cpp
template<
    typename ID_Generator,
    typename Start_Data_Generator,
    typename Owner_Generator,
    typename Session_Creator>
class server_active_start_mode;
```

模板参数含义如下：

| 参数 | 要求和用途 |
| --- | --- |
| `ID_Generator` | 根据连接生成 ID，必须提供 `id_type` 和可调用的 `operator()(ConnPtr)` |
| `Start_Data_Generator` | 根据 ID 生成启动数据，必须提供 `start_data_type` 和可调用的 `operator()(id_type)` |
| `Owner_Generator` | 生成发送启动包所需的 owner 信息，必须可调用并接受 `id_type` |
| `Session_Creator` | 保存连接和启动数据，并在回调中用于创建 session |

类内部将这些参数别名为：

```cpp
typedef ID_Generator id_generator_type;
typedef typename id_generator_type::id_type id_type;
typedef Start_Data_Generator start_data_generator_type;
typedef typename start_data_generator_type::start_data_type start_data_type;
typedef Owner_Generator owner_generator_type;
typedef Session_Creator session_creator_type;
```

## 调用接口

公共调用运算符为：

```cpp
template<typename ConnPtr, typename Session_Mgr>
inline void operator()(
    ConnPtr connection,
    Session_Mgr& session_manager) const;
```

实现通过 `session_manager.shared_from_this()` 获取 session manager 的共享指针，再进入内部创建流程：

```cpp
this_type::prv_create_helper(
    connection,
    session_manager.shared_from_this());
```

因此 `Session_Mgr` 必须提供 `shared_from_this()`，并且返回值应能被 `mplex::pointer_to_value_t` 解出 session manager 类型。

## Session manager 类型约束

内部流程会取得 session manager 的 `tag_type`，并使用编译期断言要求它等于：

```cpp
yggr::network::type_traits::tag_server
```

等价约束为：

```cpp
BOOST_MPL_ASSERT((
    boost::is_same<
        session_mgr_tag_type,
        yggr::network::type_traits::tag_server>));
```

这保证该启动策略只能用于服务器标签对应的 session manager。

## 会话创建流程

内部函数接收连接指针和 session manager 共享指针：

```cpp
template<typename ConnPtr, typename Session_Mgr_Ptr>
inline void prv_create_helper(
    ConnPtr connection,
    Session_Mgr_Ptr session_manager) const;
```

处理步骤如下：

### 1. 检查指针

首先断言 session manager 非空，并在连接或 manager 为空时直接返回：

```cpp
assert(session_manager);
if(!(connection && session_manager))
{
    return;
}
```

该策略不会对空指针抛出异常，也不会继续执行后续发送流程。

### 2. 生成 ID

创建 `ID_Generator` 并从连接生成 ID：

```cpp
id_generator_type id_generator;
id_type current_id = id_generator(connection);
```

如果生成的 ID 等于默认构造的空 ID，则直接返回：

```cpp
if(current_id == id_type())
{
    return;
}
```

因此 `id_type` 必须支持默认构造和相等比较。

### 3. 生成启动数据

创建 `Start_Data_Generator`，并根据 ID 生成启动数据：

```cpp
start_data_generator_type start_generator;
start_data_type start_data(
    start_generator(current_id));
```

生成结果会被复制或移动到 `Session_Creator` 使用的启动数据对象中。

### 4. 发送启动包

创建 `Owner_Generator` 后，通过连接发送启动包：

```cpp
owner_generator_type owner_generator;
connection->send_packet(
    owner_generator(id_type()),
    start_data,
    boost::bind(
        &this_type::s_hanlder_create<session_mgr_ptr_type>,
        boost::asio::placeholders::error,
        session_creator_type(connection, start_data),
        session_manager));
```

发送内容包括：

- owner：使用默认构造的 `id_type()` 生成；
- start data：由当前连接 ID 生成；
- completion handler：发送完成后创建 session。

源码中的回调函数名称为 `s_hanlder_create`，其中 `hanlder` 是现有代码中的拼写。

## 异步回调与 session 启动

回调函数为：

```cpp
template<typename Session_Mgr_Ptr>
static void s_hanlder_create(
    const boost::system::error_code& error,
    const session_creator_type& creator,
    Session_Mgr_Ptr session_manager);
```

处理流程如下：

1. 断言并检查 session manager；
2. 调用 `create_session_only(error, creator)` 创建 session；
3. 如果创建失败，直接返回；
4. 对创建出的 session 调用 `start()`。

核心逻辑为：

```cpp
session_ptr_type session =
    session_manager->create_session_only(error, creator);

if(!session)
{
    return;
}

session->start();
```

发送错误不会在本类中被转换或抛出，而是原样传递给 `create_session_only`，由 session manager 决定如何处理。

## 所需连接接口

`ConnPtr` 至少需要提供：

```cpp
explicit operator bool() const;
void send_packet(
    Owner owner,
    StartData start_data,
    CompletionHandler handler);
```

其中 `send_packet` 必须接受由 `boost::bind` 创建的异步完成回调，并在发送结束时传入 `boost::system::error_code`。

## 所需 session manager 接口

`Session_Mgr` 或其共享对象类型至少需要提供：

```cpp
auto shared_from_this();
typedef tag_server tag_type;
typedef session_type::pointer session_ptr_type;

session_ptr_type create_session_only(
    const boost::system::error_code& error,
    const Session_Creator& creator);
```

返回的 session 对象必须支持：

```cpp
void start();
```

## 流程示意

```text
operator()(connection, session_manager)
  -> session_manager.shared_from_this()
  -> 检查 connection / manager
  -> ID_Generator(connection)
  -> 生成 start_data
  -> connection->send_packet(...)
      -> s_hanlder_create(error, creator, manager)
          -> create_session_only(error, creator)
          -> session->start()
```

## 使用限制与注意事项

- `Session_Mgr::tag_type` 必须是 `yggr::network::type_traits::tag_server`，否则编译期断言失败。
- 空连接、空 session manager、空 ID 或 session 创建失败都会使流程提前结束。
- 发送错误由 `create_session_only` 接收处理，本类不会自行重试或抛出异常。
- `Session_Creator` 必须能够由连接指针和启动数据构造，并且能被 session manager 使用。
- `id_type` 必须支持默认构造和相等比较，因为默认值用于判断无效 ID，并用于生成初始 owner。
- 该类是无状态策略对象，可以按值传递或临时构造使用。
