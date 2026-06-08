# mongoc_index_opt_wt_native_ex.hpp

## 概述

此头文件定义了 `mongoc_index_opt_wt_native_ex` 类，该类提供静态方法来处理 MongoDB WiredTiger 存储引擎的索引选项。这些方法是对 MongoDB C 驱动程序 `mongoc_index_opt_wt_t` 结构体的扩展管理接口。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

### mongoc_index_opt_wt_native_ex

一个工具类，提供 WiredTiger 索引选项的管理方法。

#### 静态方法

##### `s_mongoc_index_opt_wt_get_default`

```cpp
inline static const mongoc_index_opt_wt_t* s_mongoc_index_opt_wt_get_default(void);
```

获取默认的 WiredTiger 索引选项。

**参数：**
- 无

**返回值：**
- 指向默认 WiredTiger 索引选项的常量指针

##### `s_mongoc_index_opt_wt_init`

```cpp
inline static mongoc_index_opt_wt_t* s_mongoc_index_opt_wt_init(mongoc_index_opt_wt_t* popt);
```

初始化 WiredTiger 索引选项结构体。

**参数：**
- `popt`：指向要初始化的 WiredTiger 索引选项结构体的指针

**返回值：**
- 初始化后的结构体指针，如果输入无效则返回 0

## 依赖项

- `<yggr/nsql_database_system/mongodb_config.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_index_opt_wt_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 获取默认选项
const mongoc_index_opt_wt_t* default_opts = mongoc_index_opt_wt_native_ex::s_mongoc_index_opt_wt_get_default();

// 初始化新选项
mongoc_index_opt_wt_t opts;
mongoc_index_opt_wt_native_ex::s_mongoc_index_opt_wt_init(&opts);
```

## 注意事项

- 所有方法都是静态的，不需要创建类的实例
- 这些方法提供对 WiredTiger 索引选项的初始化操作
- 使用这些方法时需要确保传入的指针有效
- 该类是对 MongoDB C 驱动程序 WiredTiger 索引选项管理的包装
- 文件中注释掉了原始的 `mongoc_index_opt_wt_t` 结构体定义，现在通过这些方法进行操作
- WiredTiger 是 MongoDB 的默认存储引擎，这些选项用于配置索引的存储参数
- `config_str` 字段通常包含 WiredTiger 特定的配置字符串</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_index_opt_wt_native_ex_cn.md