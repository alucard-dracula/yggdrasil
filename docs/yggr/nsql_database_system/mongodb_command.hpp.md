# mongodb_command.hpp

## 概述

此头文件定义了 `mongodb_command` 模板类，用于封装 MongoDB 数据库命令。它将命令键和用户数据组合在一起，支持任务中心的异步处理机制，包括复制、移动和交换操作。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

### mongodb_command

一个模板类，用于封装 MongoDB 命令及其相关数据。它实现了任务导入机制，可以在任务中心系统中使用。

#### 模板参数

##### `Key`

键类型。用于标识命令，通常是字符串或唯一标识符。

##### `InUserData`

输入用户数据类型。默认为 `::yggr::any`，用于存储命令相关的用户数据。

##### `TaskDataInfo`

任务数据信息类型。默认为 `task_center::default_task_data_info_type`，用于任务中心的类型信息。

#### 类型定义

##### `key_type`

键类型，从模板参数 `Key` 继承。

##### `in_user_data_type`

输入用户数据类型，从模板参数 `InUserData` 继承。

##### `task_data_info_type`

任务数据信息类型，从模板参数 `TaskDataInfo` 继承。

#### 包标识定义

##### `E_pak_id`

包标识：`mongodb_pak_id_def::E_id_start + 1`

##### `E_now_ver`

当前版本：`1`

#### 构造函数

##### 默认构造函数

```cpp
mongodb_command(void)
```

创建空的命令对象。

##### 键构造函数

```cpp
mongodb_command(const key_type& key)
```

使用指定的键创建命令对象。

**参数：**
- `key`：命令键

##### 完整构造函数

```cpp
mongodb_command(const key_type& key, const in_user_data_type& user_data)
```

使用键和用户数据创建命令对象。

**参数：**
- `key`：命令键
- `user_data`：用户数据

##### 移动构造函数

```cpp
mongodb_command(BOOST_RV_REF(this_type) right)
```

移动构造命令对象。

##### 复制构造函数

```cpp
mongodb_command(const this_type& right)
```

复制构造命令对象。

#### 析构函数

```cpp
~mongodb_command(void)
```

清理资源。

#### 赋值操作符

##### 移动赋值

```cpp
this_type& operator=(BOOST_RV_REF(this_type) right)
```

移动赋值操作符。

##### 复制赋值

```cpp
this_type& operator=(const this_type& right)
```

复制赋值操作符。

#### 交换方法

##### 移动交换

```cpp
void swap(BOOST_RV_REF(this_type) right)
```

移动交换对象。

##### 普通交换

```cpp
void swap(this_type& right)
```

交换两个对象的内容。

#### 访问器方法

##### `id`

```cpp
const key_type& id(void) const
```

获取命令的键标识。

**返回值：**
- 命令键的常量引用

##### `in_user_data`

```cpp
const in_user_data_type& in_user_data(void) const
```

获取输入用户数据。

**返回值：**
- 用户数据的常量引用

## 交换支持

### swap_support 命名空间

提供了全局交换函数支持：

```cpp
template<typename Key, typename InUserData, typename PakID, typename Ver, typename TaskDataInfo>
void swap(mongodb_command<Key, InUserData, PakID, Ver, TaskDataInfo>& l,
          mongodb_command<Key, InUserData, PakID, Ver, TaskDataInfo>& r)
```

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/utility/copy_or_move_or_swap.hpp>`
- `<yggr/charset/string.hpp>`
- `<yggr/any_val/any.hpp>`
- `<yggr/nsql_database_system/mongodb_pak_id_def.hpp>`
- `<yggr/task_center/task_info.hpp>`
- `<yggr/task_center/support/task_import.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongodb_command.hpp>

using namespace yggr::nsql_database_system;

// 定义命令类型
typedef mongodb_command<std::string, std::string> mongo_cmd_type;

// 创建命令对象
mongo_cmd_type cmd1;  // 默认构造

mongo_cmd_type cmd2("find_users");  // 带键构造

mongo_cmd_type cmd3("insert_user", "{\"name\":\"John\", \"age\":30}");  // 完整构造

// 复制和移动
mongo_cmd_type cmd4 = cmd3;  // 复制构造
mongo_cmd_type cmd5 = std::move(cmd3);  // 移动构造

// 赋值
cmd1 = cmd2;  // 复制赋值
cmd1 = std::move(cmd4);  // 移动赋值

// 交换
cmd1.swap(cmd2);

// 访问数据
const std::string& key = cmd1.id();
const std::string& data = cmd1.in_user_data();

// 在任务中心使用
// cmd1 将被序列化并通过任务中心发送
```

## 注意事项

- 这是一个模板类，需要指定键类型和用户数据类型
- 支持 C++11 的移动语义，提高性能
- 实现了完整的复制控制（Rule of Three/Five）
- 使用任务导入机制，可以在分布式系统中传递
- 包ID用于网络协议识别，版本控制用于兼容性
- 用户数据使用 `any` 类型，可以存储任意数据
- 交换操作是异常安全的
- 所有访问器返回常量引用，保证数据完整性
- 适用于异步任务处理场景
- 需要确保模板参数类型的可复制性和可移动性
- 在多线程环境中使用时需要适当同步
- 析构函数会自动清理所有资源</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongodb_command_cn.md