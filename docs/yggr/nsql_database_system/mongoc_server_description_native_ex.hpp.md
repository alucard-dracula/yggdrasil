# mongoc_server_description_native_ex.hpp

## 概述

此头文件定义了 `mongoc_server_description_native_ex` 类，用于处理 MongoDB 服务器描述 (Server Description) 的类型标识和管理。服务器描述包含 MongoDB 集群中各个服务器的状态和类型信息。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

### mongoc_server_description_native_ex

一个工具类，提供服务器描述的类型管理和销毁方法。

#### 类型定义

##### `typeid_type`

服务器描述类型的整数类型，使用 `YGGR_PP_UINTEGER_T_TYPE` 定义。

#### 常量定义

##### 服务器描述类型常量

这些常量对应 MongoDB 驱动中的服务器类型枚举：

- `E_MONGOC_SERVER_UNKNOWN = 0`：未知服务器类型
- `E_MONGOC_SERVER_STANDALONE = 1`：独立服务器（单机部署）
- `E_MONGOC_SERVER_MONGOS = 2`：Mongos 路由服务器（分片集群）
- `E_MONGOC_SERVER_POSSIBLE_PRIMARY = 3`：可能的 Primary 服务器
- `E_MONGOC_SERVER_RS_PRIMARY = 4`：副本集 Primary 服务器
- `E_MONGOC_SERVER_RS_SECONDARY = 5`：副本集 Secondary 服务器
- `E_MONGOC_SERVER_RS_ARBITER = 6`：副本集 Arbiter 服务器
- `E_MONGOC_SERVER_RS_OTHER = 7`：副本集其他类型服务器
- `E_MONGOC_SERVER_RS_GHOST = 8`：副本集 Ghost 服务器（已移除但仍可见）
- `E_MONGOC_SERVER_LOAD_BALANCER = 9`：负载均衡器
- `E_MONGOC_SERVER_DESCRIPTION_TYPES = 10`：服务器描述类型总数

#### 管理方法

##### `s_typeid`

获取服务器描述的类型标识。

**参数：**
- `pdesc`：指向服务器描述对象的指针

**返回值：**
- 服务器的类型标识（`typeid_type`）

##### `s_destroy_all_exclude_outter_array`

销毁服务器描述数组，但不销毁外部数组本身。

**参数：**
- `sds`：指向服务器描述对象指针数组的指针
- `n`：数组中的元素数量

**注意：** 此方法用于清理服务器描述对象，但保留数组结构。

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/base/static_constant.hpp>`
- `<yggr/type_traits/number_t.hpp>`
- `<yggr/nsql_database_system/mongodb_config.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_server_description_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 获取服务器描述的类型
mongoc_server_description_t* server_desc = /* 获取服务器描述 */;
mongoc_server_description_native_ex::typeid_type type =
    mongoc_server_description_native_ex::s_typeid(server_desc);

switch(type) {
    case mongoc_server_description_native_ex::E_MONGOC_SERVER_RS_PRIMARY:
        std::cout << "Primary server" << std::endl;
        break;
    case mongoc_server_description_native_ex::E_MONGOC_SERVER_RS_SECONDARY:
        std::cout << "Secondary server" << std::endl;
        break;
    // ... 其他类型处理
}

// 清理服务器描述数组
mongoc_server_description_t** server_descs = /* 服务器描述数组 */;
std::size_t count = /* 数组大小 */;
mongoc_server_description_native_ex::s_destroy_all_exclude_outter_array(server_descs, count);
```

## 注意事项

- 服务器类型用于确定 MongoDB 集群的拓扑结构
- `UNKNOWN` 类型表示服务器状态尚未确定或连接失败
- `STANDALONE` 用于单机 MongoDB 实例
- `MONGOS` 用于分片集群的路由服务器
- 副本集相关类型（RS_*）用于副本集成员
- `POSSIBLE_PRIMARY` 表示服务器可能成为 Primary，但尚未确认
- `LOAD_BALANCER` 用于支持负载均衡的 MongoDB 部署
- 使用 `s_destroy_all_exclude_outter_array` 时，确保外部数组的内存管理由调用者负责
- 这些常量对应 MongoDB C 驱动的枚举值，用于类型安全的封装</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_server_description_native_ex_cn.md