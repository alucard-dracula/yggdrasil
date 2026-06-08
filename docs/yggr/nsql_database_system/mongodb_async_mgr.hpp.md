# mongodb_async_mgr.hpp

## 概述

此头文件定义了 `mongodb_async_mgr` 模板类，用于管理异步 MongoDB 数据库操作。它继承自异步数据库管理器基类，提供 MongoDB 特定的异步操作接口，包括连接管理、命令执行和通知处理。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

### mongodb_async_mgr

一个模板类，用于异步管理 MongoDB 数据库操作。它封装了连接池、访问器和异步执行机制。

#### 模板参数

##### `Key`

键类型。用于标识数据库连接或操作。

##### `Conn`

连接类型。MongoDB 连接对象类型。

##### `Accesser`

访问器类型。默认为 `mongo_accesser`，用于执行 MongoDB 操作。

##### `InUserData`

输入用户数据类型。默认为 `yggr::any`，用于传递给异步操作的用户数据。

##### `OutUserData`

输出用户数据类型。默认为 `yggr::any`，用于从异步操作返回的用户数据。

##### `Thread_Config`

线程配置类型。默认为 `thread::boost_thread_config_type`，用于配置异步执行线程。

#### 类型定义

##### `base_type`

基类类型：`dbs_manager::async_dbs_mgr<Key, Conn, Accesser, InUserData, OutUserData, Thread_Config>`

##### `key_type`

键类型，从基类继承。

##### `conn_type`

连接类型，从基类继承。

##### `accesser_type`

访问器类型，从基类继承。

##### `in_user_data_type`

输入用户数据类型，从基类继承。

##### `out_user_data_type`

输出用户数据类型，从基类继承。

##### `init_type`

初始化类型，从基类继承。

#### 构造函数

##### 变参构造函数（C++11）

```cpp
template<typename ...Args>
mongodb_async_mgr(BOOST_FWD_REF(Args)... args)
```

使用完美转发构造基类。

**参数：**
- `args`：转发给基类的构造参数

##### 初始化构造函数

```cpp
mongodb_async_mgr(const init_type& init)
```

使用初始化对象构造。

**参数：**
- `init`：初始化配置对象

#### 析构函数

```cpp
~mongodb_async_mgr(void)
```

清理资源。

#### 继承的方法

##### 初始化和控制

- `init`：初始化管理器
- `start`：启动异步处理
- `is_running`：检查是否正在运行
- `stop`：停止异步处理
- `join`：等待异步处理完成

##### 异步操作

- `async_execute`：异步执行数据库操作
- `async_run_command`：异步运行数据库命令

##### 清理方法

- `clear_notify`：清除通知
- `clear`：清除所有资源

## 依赖项

- `<yggr/dbs_manager/async_dbs_mgr.hpp>`
- `<yggr/nsql_database_system/mongo_accesser.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongodb_async_mgr.hpp>

using namespace yggr::nsql_database_system;

// 定义类型
typedef mongodb_async_mgr<
    std::string,  // Key type
    mongo_connection,  // Connection type
    mongo_accesser,  // Accesser type
    std::string,  // Input user data
    bson_t*       // Output user data
> mongo_async_mgr_type;

// 创建初始化配置
mongo_async_mgr_type::init_type init;
// 配置初始化参数...

// 创建异步管理器
mongo_async_mgr_type mgr(init);

// 启动异步处理
mgr.start();

// 异步执行查询
std::string key = "query1";
mongo_async_mgr_type::in_user_data_type in_data = "find users where age > 18";
mgr.async_execute(key, in_data);

// 检查运行状态
if (mgr.is_running()) {
    std::cout << "Manager is running" << std::endl;
}

// 停止并等待完成
mgr.stop();
mgr.join();

// 清理资源
mgr.clear();
```

## 注意事项

- 这是一个模板类，需要根据具体需求指定模板参数
- 继承自异步数据库管理器基类，提供异步操作能力
- 支持多种用户数据类型，便于传递上下文信息
- 线程配置可以自定义，以适应不同的并发需求
- 使用前需要正确初始化和启动管理器
- 异步操作是非阻塞的，需要适当的同步机制
- 停止管理器时会等待所有异步操作完成
- 清理方法会释放所有资源，包括连接和通知
- 适用于高并发 MongoDB 应用场景
- 需要确保模板参数类型的一致性和兼容性
- 错误处理应该通过通知机制或异常处理
- 性能取决于线程配置和连接池设置</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongodb_async_mgr_cn.md