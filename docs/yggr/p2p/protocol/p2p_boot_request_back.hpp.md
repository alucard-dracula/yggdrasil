# p2p_boot_request_back.hpp

## 概述

该头文件定义了 `p2p_boot_request_back` 模板类，用于 P2P 引导请求响应的数据封装和序列化。该类保存对等节点引导信息列表以及附带的用户数据，并支持拷贝、移动、交换和序列化操作。

## 命名空间

- `yggr::p2p`
- `yggr::p2p::swap_support`

## 类描述

### p2p_boot_request_back

一个泛型数据类型，用于保存 P2P 引导响应中的对等节点信息集合和用户附加数据。

#### 模板参数

- `Peer_Boot_Info_Container`：对等引导信息容器类型，例如包含多个 `peer_boot_info` 对象的集合。
- `User_Data`：用户数据类型，默认使用 `u32`。

#### 成员类型

- `peer_boot_info_cont_type`：对等引导信息容器类型。
- `peer_boot_info_type`：容器中元素类型，即单个对等引导信息类型。
- `user_data_type`：用户数据类型。

## 构造与析构

### 构造函数

- `p2p_boot_request_back(void)`：默认构造，初始化用户数据为默认值。
- `p2p_boot_request_back(InputIter s, InputIter e, const user_data_type& user_data)`：使用迭代器范围构造对等引导信息集合，并初始化用户数据。
- `p2p_boot_request_back(const peer_boot_info_cont_type& peer_infos, const user_data_type& user_data)`：使用现成容器和用户数据构造。
- `p2p_boot_request_back(BOOST_RV_REF(peer_boot_info_cont_type) peer_infos, BOOST_RV_REF(user_data_type) user_data)`：通过移动构造对等引导信息和用户数据。
- `p2p_boot_request_back(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `p2p_boot_request_back(const this_type& right)`：拷贝构造函数。

### 析构函数

- `~p2p_boot_request_back(void)`：默认析构。

## 赋值与交换

### 移动赋值

- `this_type& operator=(BOOST_RV_REF(this_type) right)`：移动赋值，使用 `copy_or_move_or_swap` 保证资源安全迁移。

### 拷贝赋值

- `this_type& operator=(const this_type& right)`：拷贝赋值。

### 交换

- `void swap(BOOST_RV_REF(this_type) right)`：右值交换接口。
- `void swap(this_type& right)`：成员交换实现，将对等引导信息和用户数据交换。

## 访问与查询

- `bool empty(void) const`：判断对等引导信息集合是否为空。
- `peer_boot_info_cont_type& peer_boot_infos(void)`：获取可修改的对等引导信息集合引用。
- `const peer_boot_info_cont_type& peer_boot_infos(void) const`：获取只读对等引导信息集合引用。
- `user_data_type& user_data(void)`：获取可修改的用户数据引用。
- `const user_data_type& user_data(void) const`：获取只读用户数据引用。

## 序列化支持

该类支持序列化接口。

### `serialize`

```cpp
template<typename Archive>
void serialize(Archive& ar, const u32 version)
```

序列化成员 `_peer_boot_infos` 和 `_user_data`。

## 交换支持函数

### `yggr::p2p::swap_support::swap`

提供 `swap` 函数重载，调用类成员 `swap` 方法。

### `std::swap` 和 `boost::swap`

在 `std` 与 `boost` 命名空间中引入 `yggr::p2p::swap_support::swap`，用于通用交换操作。

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/utility/copy_or_move_or_swap.hpp>`
- `<yggr/container_ex/container_mapped_value_t.hpp>`
- `<yggr/serialization/access.hpp>`
- `<yggr/serialization/nvp.hpp>`
- `<boost/range/functions.hpp>`
- `<boost/range/value_type.hpp>`
- `<boost/range/iterator.hpp>`

## 使用说明

该类适用于封装 P2P 引导响应数据，通常用于网络消息传输和任务处理流程中。

### 典型用法

```cpp
#include <yggr/p2p/protocol/p2p_boot_request_back.hpp>

typedef std::vector<peer_boot_info> peer_boot_info_container_type;

typedef yggr::p2p::p2p_boot_request_back<peer_boot_info_container_type, u32> boot_request_back_type;

peer_boot_info_container_type infos;
// 填充 infos ...

boot_request_back_type req_back(infos, 123);

if(!req_back.empty()) {
    auto& data = req_back.user_data();
    auto& peers = req_back.peer_boot_infos();
}
```

## 注意事项

- `User_Data` 默认为 `u32`，可以根据需求替换为其他用户数据类型。
- 该类支持 C++ 拷贝与移动语义，并实现 `swap` 以提高性能和安全性。
- 序列化接口使用 `YGGR_SERIALIZATION_NVP` 宏包装成员。
- 如果容器类型支持移动操作，可通过移动构造或移动赋值避免额外拷贝。
- 交换函数在 `std` 和 `boost` 命名空间中均可使用。
