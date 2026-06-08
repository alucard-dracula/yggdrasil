# mongodb_mgr.hpp

## 概述

此头文件定义了 `mongodb_mgr` 模板类，用于同步管理 MongoDB 数据库连接和操作。它继承自数据库管理器基类，提供 MongoDB 特定的连接管理和命令执行功能。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

### mongodb_mgr

一个模板类，用于同步管理 MongoDB 数据库操作。它封装了连接池、访问器和同步执行机制。

#### 模板参数

##### `Key`

键类型。用于标识数据库连接或操作。

##### `Conn`

连接类型。MongoDB 连接对象类型。

##### `Accesser`

访问器类型。默认为 `mongo_accesser`，用于执行 MongoDB 操作。

##### `InUserData`

输入用户数据类型。默认为 `yggr::any`，用于传递给操作的用户数据。

##### `OutUserData`

输出用户数据类型。默认为 `yggr::any`，用于从操作返回的用户数据。

#### 类型定义

##### `base_type`

基类类型：`dbs_manager::dbs_mgr<Key, Conn, Accesser, InUserData, OutUserData>`

##### `key_type`

键类型，从基类继承。

##### `conn_type`

连接类型，从基类继承。

##### `init_type`

初始化类型，从基类继承。

##### `accesser_type`

访问器类型，从基类继承。

##### `in_user_data_type`

输入用户数据类型，从基类继承。

##### `out_user_data_type`

输出用户数据类型，从基类继承。

#### 构造函数

##### 变参构造函数（C++11）

```cpp
template<typename ...Args>
mongodb_mgr(BOOST_FWD_REF(Args)... args)
```

使用完美转发构造基类。

**参数：**
- `args`：转发给基类的构造参数

##### 初始化构造函数

```cpp
mongodb_mgr(const init_type& init)
```

使用初始化对象构造。

**参数：**
- `init`：初始化配置对象

#### 析构函数

```cpp
~mongodb_mgr(void)
```

清理资源。

#### 继承的方法

##### 初始化和连接管理

- `init`：初始化管理器
- `reconnect`：重新连接数据库
- `clear`：清除所有资源

##### 命令管理

- `register_command`：注册命令
- `unregister_command`：注销命令

##### 操作执行

- `execute`：执行数据库操作
- `run_command`：运行数据库命令

## 依赖项

- `<yggr/dbs_manager/dbs_mgr.hpp>`
- `<yggr/nsql_database_system/mongo_accesser.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongodb_mgr.hpp>

using namespace yggr::nsql_database_system;

// 定义管理器类型
typedef mongodb_mgr<
    std::string,  // Key type
    mongo_connection,  // Connection type
    mongo_accesser,  // Accesser type
    std::string,  // Input user data
    bson_t*       // Output user data
> mongo_mgr_type;

// 创建初始化配置
mongo_mgr_type::init_type init;
// 配置初始化参数...

// 创建管理器
mongo_mgr_type mgr(init);

// 注册命令
mgr.register_command("find_users", some_handler);

// 执行操作
std::string key = "query1";
mongo_mgr_type::in_user_data_type in_data = "find users where age > 18";
mongo_mgr_type::out_user_data_type out_data;
mgr.execute(key, in_data, out_data);

// 运行命令
mgr.run_command("find_users", in_data);

// 重新连接（如果需要）
mgr.reconnect();

// 清理资源
mgr.clear();
```

## 注意事项

- 这是一个模板类，需要根据具体需求指定模板参数
- 提供同步操作接口，与异步版本 `mongodb_async_mgr` 相对应
- 继承自数据库管理器基类，提供标准的数据库管理功能
- 支持多种用户数据类型，便于传递上下文信息
- 使用前需要正确初始化和配置
- 命令注册和注销需要成对使用
- 重新连接功能用于处理连接断开的情况
- 清理方法会释放所有资源，包括连接和命令处理器
- 适用于需要同步响应的 MongoDB 应用场景
- 需要确保模板参数类型的兼容性和正确性
- 在多线程环境中使用时需要适当同步
- 错误处理应该通过异常或错误码机制</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongodb_mgr_cn.md