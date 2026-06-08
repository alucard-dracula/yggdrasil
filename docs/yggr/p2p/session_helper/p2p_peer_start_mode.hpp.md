**概述**
- **描述：** `p2p_peer_start_mode` 是一个用于 P2P 会话启动的函数对象模板，用于在建立连接后基于 `start_data` 和 `session_creator_type` 创建并启动会话。
- **头文件：** [yggr/p2p/session_helper/p2p_peer_start_mode.hpp](yggr/p2p/session_helper/p2p_peer_start_mode.hpp)

**模板参数**
- `Start_Data`：启动用的数据类型，必须支持 `empty()` 方法用于判断是否包含有效信息。
- `Session_Creator`：会话创建器类型，用于在 `create_session_only()` 中构造会话。

**类型别名**
- `start_data_type`：等同于 `Start_Data`。
- `session_creator_type`：等同于 `Session_Creator`。
- `this_type`：当前类型别名。

**构造函数**
- 默认构造函数 `p2p_peer_start_mode()`：仅用于编译需要，但总是触发 `assert(false)`，说明该类型仅在 P2P 模式下有效。
- 右值构造 `p2p_peer_start_mode(BOOST_RV_REF(start_data_type) start_data)`：移动构造启动数据。
- 拷贝构造 `p2p_peer_start_mode(const start_data_type& start_data)`：拷贝启动数据。
- 提供移动构造与拷贝构造。

**赋值与交换**
- 移动赋值 `operator=(BOOST_RV_REF(this_type) right)`：使用 `copy_or_move_or_swap` 处理 `_start_data`。
- 拷贝赋值 `operator=(const this_type& right)`。
- 成员交换 `swap(this_type& right)`：交换 `_start_data`。
- 右值 `swap(BOOST_RV_REF(this_type) right)`：调用左值版本。
- 自由函数 `swap` 在 `yggr::p2p::swap_support` 中定义，并引入 `std` / `boost`。

**调用操作符**
- `template<typename ConnPtr, typename Session_Mgr> void operator()(ConnPtr pconn, Session_Mgr& smgr)`
  - 将连接指针 `pconn` 与会话管理器 `smgr` 传递给静态辅助函数 `prv_s_create_helper()`。
  - 该操作符使对象可作为回调或函数对象使用，通常在连接建立后执行会话创建。

**静态辅助函数**
- `prv_s_create_helper(ConnPtr pconn, Session_Mgr_Ptr psmgr, start_data_type& start_data)`
  - 检查 `pconn`、`psmgr` 是否有效，并判断 `start_data.empty()`。
  - 调用 `psmgr->create_session_only(boost::system::error_code(), session_creator_type(pconn, start_data))` 创建会话。
  - 若会话创建成功，则调用 `sptr->start()` 启动会话。

**设计要点**
- 默认构造函数中的 `assert(false)` 表明该类不应在不带参数的情况下使用。
- `start_data_type` 需要是可移动或可拷贝类型，并且应支持 `empty()` 方法。
- 该类通过 `session_creator_type` 生成会话对象，保持会话创建逻辑与启动数据的解耦。

**使用场景**
- 在 P2P 连接建立后，将该对象作为回调传递给连接处理代码，使其自动创建并启动新会话。
- `start_data` 包含会话初始化所需的信息，例如协议版本、认证数据或连接参数。

**示例（伪代码）**
```cpp
using StartMode = yggr::p2p::p2p_peer_start_mode<MyStartData, MySessionCreator>;
MyStartData data(...);
StartMode start_mode(data);
handler(pconn, session_mgr); // 通过 operator() 调用会创建并启动会话
```

**相关文件**
- 源文件： [yggr/p2p/session_helper/p2p_peer_start_mode.hpp](yggr/p2p/session_helper/p2p_peer_start_mode.hpp)

**作者 / 许可证**
- 源文件头部包含版权与 MIT 风格许可声明，详情请参见源文件顶部。
