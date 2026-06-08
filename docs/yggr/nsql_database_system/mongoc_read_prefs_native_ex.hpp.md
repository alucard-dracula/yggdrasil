# mongoc_read_prefs_native_ex.hpp

## 概述

此头文件定义了 `yggr_mongoc_read_prefs` 结构体和 `mongoc_read_prefs_native_ex` 类，用于处理 MongoDB 的读偏好 (Read Preferences) 配置。读偏好用于控制从副本集的哪个成员读取数据。

## 命名空间

- `yggr::nsql_database_system`

## 结构体描述

### yggr_mongoc_read_prefs

一个结构体，封装了 MongoDB 读偏好的配置参数。

#### 成员变量

##### `mode_type mode`

读偏好模式。指定从哪个副本集成员读取数据。

##### `bson_t tags`

标签文档。用于指定具有特定标签的副本集成员。

##### `int64_t max_staleness_seconds`

最大滞后时间（秒）。副本集成员可以滞后的最大时间。

##### `bson_t hedge`

对冲选项。用于配置读取对冲以提高性能。

## 类描述

### mongoc_read_prefs_native_ex

一个工具类，提供读偏好的管理方法。

#### 类型定义

##### `mode_type`

读偏好模式的整数类型。

#### 常量定义

##### 最大滞后时间常量

- `E_MONGOC_NO_MAX_STALENESS = -1`：无最大滞后限制
- `E_MONGOC_SMALLEST_MAX_STALENESS_SECONDS = 90`：最小最大滞后时间（90秒）

##### 读偏好模式常量

- `E_MONGOC_READ_INVALIDATE = 0`：无效模式（用于销毁）
- `E_MONGOC_READ_PRIMARY = MONGOC_READ_PRIMARY`：仅从主节点读取
- `E_MONGOC_READ_SECONDARY = MONGOC_READ_SECONDARY`：仅从从节点读取
- `E_MONGOC_READ_PRIMARY_PREFERRED = MONGOC_READ_PRIMARY_PREFERRED`：优先从主节点读取，失败时从从节点读取
- `E_MONGOC_READ_SECONDARY_PREFERRED = MONGOC_READ_SECONDARY_PREFERRED`：优先从从节点读取，失败时从主节点读取
- `E_MONGOC_READ_NEAREST = MONGOC_READ_NEAREST`：从最近的节点读取（基于网络延迟）

#### 管理方法

##### `s_mongoc_read_prefs_init`

初始化读偏好对象。

**参数：**
- `pmrp`：指向读偏好对象的指针
- `mode`：读偏好模式（默认为 `E_MONGOC_READ_PRIMARY`）

**返回值：**
- 初始化后的读偏好对象指针

##### `s_mongoc_read_prefs_static_destroy`

销毁读偏好对象。

##### `s_mongoc_read_prefs_move`

移动读偏好对象。

##### `s_mongoc_read_prefs_copy`

复制读偏好对象。

##### `s_mongoc_read_prefs_swap`

交换两个读偏好对象。

#### 配置方法

##### `s_mongoc_read_prefs_add_tag`

添加标签到读偏好。

**参数：**
- `pmrp`：指向读偏好对象的指针
- `tag`：标签 BSON 文档

**返回值：**
- `true` 如果添加成功，`false` 否则

##### `s_mongoc_read_prefs_is_valid`

验证读偏好对象是否有效。

##### `s_mongoc_read_prefs_get_mode`

获取读偏好模式。

##### `s_mongoc_read_prefs_set_mode`

设置读偏好模式。

##### `s_mongoc_read_prefs_get_tags`

获取标签文档。

##### `s_mongoc_read_prefs_set_tags`

设置标签文档。

##### `s_mongoc_read_prefs_get_max_staleness_seconds`

获取最大滞后时间。

##### `s_mongoc_read_prefs_set_max_staleness_seconds`

设置最大滞后时间。

##### `s_mongoc_read_prefs_get_hedge`

获取对冲选项。

##### `s_mongoc_read_prefs_set_hedge`

设置对冲选项。

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/base/static_constant.hpp>`
- `<yggr/type_traits/number_t.hpp>`
- `<yggr/nsql_database_system/mongodb_config.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_read_prefs_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 创建读偏好对象
mongoc_read_prefs_t* read_prefs = nullptr;
read_prefs = (mongoc_read_prefs_t*)mongoc_read_prefs_native_ex::s_mongoc_read_prefs_init(
    read_prefs, mongoc_read_prefs_native_ex::E_MONGOC_READ_SECONDARY_PREFERRED);

// 设置最大滞后时间
mongoc_read_prefs_native_ex::s_mongoc_read_prefs_set_max_staleness_seconds(read_prefs, 120);

// 添加标签
bson_t tag = BSON_INITIALIZER;
BSON_APPEND_UTF8(&tag, "dc", "ny");
mongoc_read_prefs_native_ex::s_mongoc_read_prefs_add_tag(read_prefs, &tag);
```

## 注意事项

- 读偏好影响查询的性能和一致性
- `PRIMARY` 模式提供强一致性，但可能影响性能
- `SECONDARY` 模式可以减少主节点的负载，但可能读取稍微过时的数据
- `NEAREST` 模式基于网络延迟选择节点，适合地理分布的部署
- 标签用于将查询定向到具有特定属性的副本集成员
- 最大滞后时间防止从过于滞后的从节点读取数据
- 对冲读取可以同时从多个节点读取以提高性能
- 使用完读偏好对象后需要调用销毁方法释放资源</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_read_prefs_native_ex_cn.md