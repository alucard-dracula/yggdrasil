# mongoc_host_list_native_ex.hpp

## 概述

此头文件定义了 `mongoc_host_list_native_ex` 结构体，该结构体提供静态方法来管理 MongoDB 主机列表对象的复制和销毁操作。这些方法是对 MongoDB C 驱动程序 `mongoc_host_list_t` 结构体的扩展管理接口。

## 命名空间

- `yggr::nsql_database_system`

## 结构体描述

### mongoc_host_list_native_ex

一个工具结构体，提供主机列表的管理方法。

#### 静态方法

##### `s_mongoc_host_list_copy_all`

```cpp
static mongoc_host_list_t* s_mongoc_host_list_copy_all(const mongoc_host_list_t* src);
```

复制整个主机列表。

**参数：**
- `src`：指向要复制的主机列表的常量指针

**返回值：**
- 指向新复制的主机列表的指针

##### `s_mongoc_host_list_destroy_all`

```cpp
static void s_mongoc_host_list_destroy_all(mongoc_host_list_t* src);
```

销毁整个主机列表，释放所有相关资源。

**参数：**
- `src`：指向要销毁的主机列表的指针

**返回值：**
- 无

## 依赖项

- `<yggr/nsql_database_system/mongodb_config.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_host_list_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 假设有一个有效的 mongoc_host_list_t* host_list
mongoc_host_list_t* copied_list = mongoc_host_list_native_ex::s_mongoc_host_list_copy_all(host_list);

// 使用复制的列表...

// 清理资源
mongoc_host_list_native_ex::s_mongoc_host_list_destroy_all(copied_list);
```

## 注意事项

- 所有方法都是静态的，不需要创建结构体的实例
- 这些方法提供对主机列表的完整复制和销毁操作
- 使用这些方法时需要确保传入的指针有效
- 该结构体是对 MongoDB C 驱动程序主机列表管理的包装，提供便捷的批量操作接口
- `s_mongoc_host_list_copy_all` 会创建主机列表的深拷贝
- `s_mongoc_host_list_destroy_all` 会释放主机列表及其所有节点占用的内存
- 使用完复制的主机列表后，务必调用销毁方法以避免内存泄漏</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_host_list_native_ex_cn.md