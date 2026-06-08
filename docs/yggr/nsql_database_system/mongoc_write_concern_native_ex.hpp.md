# mongoc_write_concern_native_ex.hpp

## 概述

此头文件定义了 `yggr_mongoc_write_concern` 结构体和 `mongoc_write_concern_native_ex` 类，用于处理 MongoDB 的写关注 (Write Concern) 配置。写关注用于控制写操作的确认行为，包括数据持久化和副本集同步要求。

## 命名空间

- `yggr::nsql_database_system`

## 结构体描述

### yggr_mongoc_write_concern

一个结构体，封装了 MongoDB 写关注的配置参数，对应 MongoDB C 驱动的 `_mongoc_write_concern_t` 结构。

#### 成员变量

##### `int8_t fsync_`

文件同步设置（已弃用）。

##### `int8_t journal`

日志同步设置。控制是否要求写操作同步到日志。

##### `int32_t w`

写确认级别。可以是数字（副本数量）或特殊值。

##### `int64_t wtimeout`

写超时时间（毫秒）。写操作等待确认的最长时间。

##### `char* wtag`

写标签。用于标签集副本的标签名称。

##### `bool frozen`

是否冻结。冻结后不能修改配置。

##### `bson_t compiled`

编译后的 BSON 表示。

##### `bool is_default_v`

是否为默认配置。

## 类描述

### mongoc_write_concern_native_ex

一个工具类，提供写关注的完整生命周期管理和配置方法。

#### 常量定义

##### 默认值常量

- `E_MONGOC_WRITE_CONCERN_FSYNC_DEFAULT = -1`：fsync 默认值
- `E_MONGOC_WRITE_CONCERN_JOURNAL_DEFAULT = -1`：journal 默认值

##### 写确认级别常量

- `E_MONGOC_WRITE_CONCERN_W_UNACKNOWLEDGED = 0`：不确认写操作
- `E_MONGOC_WRITE_CONCERN_W_ERRORS_IGNORED = -1`：忽略错误（已弃用）
- `E_MONGOC_WRITE_CONCERN_W_DEFAULT = -2`：使用默认确认级别
- `E_MONGOC_WRITE_CONCERN_W_MAJORITY = -3`：多数副本确认
- `E_MONGOC_WRITE_CONCERN_W_TAG = -4`：标签集确认

#### 管理方法

##### `s_mongoc_write_concern_init`

初始化写关注对象。

**参数：**
- `pmwc`：指向写关注对象的指针

**返回值：**
- 初始化后的写关注对象指针

##### `s_mongoc_write_concern_static_destroy`

销毁写关注对象。

##### `s_mongoc_write_concern_move`

移动写关注对象。

##### `s_mongoc_write_concern_copy`

复制写关注对象。

##### `s_mongoc_write_concern_unfreeze_copy`

复制写关注对象并解冻。

##### `s_mongoc_write_concern_swap`

交换两个写关注对象。

##### `s_mongoc_write_concern_is_valid`

验证写关注对象是否有效。

##### `s_mongoc_write_concern_append`

将写关注追加到 BSON 命令。

##### `s_mongoc_write_concern_freeze`

冻结写关注配置，使其不可修改。

##### `s_mongoc_write_concern_unfreeze`

解冻写关注配置，使其可以修改。

##### `s_mongoc_write_concern_get_bson`

获取写关注的 BSON 表示。

#### 配置方法

##### fsync 设置

- `s_mongoc_write_concern_get_fsync`：获取 fsync 设置
- `s_mongoc_write_concern_set_fsync`：设置 fsync（已弃用）

##### journal 设置

- `s_mongoc_write_concern_get_journal`：获取 journal 设置
- `s_mongoc_write_concern_set_journal`：设置 journal

##### 写确认级别设置

- `s_mongoc_write_concern_get_w`：获取写确认级别
- `s_mongoc_write_concern_set_w`：设置写确认级别
- `s_mongoc_write_concern_get_wmajority`：检查是否为多数确认
- `s_mongoc_write_concern_set_wmajority`：设置为多数确认

##### 写标签设置

- `s_mongoc_write_concern_get_wtag`：获取写标签
- `s_mongoc_write_concern_set_wtag`：设置写标签

##### 超时设置

- `s_mongoc_write_concern_get_wtimeout`：获取写超时时间（32位）
- `s_mongoc_write_concern_get_wtimeout_int64`：获取写超时时间（64位）
- `mongoc_write_concern_set_wtimeout`：设置写超时时间（32位）
- `mongoc_write_concern_set_wtimeout_int64`：设置写超时时间（64位）

#### 状态检查方法

##### `s_mongoc_write_concern_journal_is_set`

检查 journal 是否已设置。

##### `s_mongoc_write_concern_is_acknowledged`

检查写操作是否会被确认。

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/base/static_constant.hpp>`
- `<yggr/nsql_database_system/mongodb_config.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_write_concern_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 创建写关注对象
yggr_mongoc_write_concern wc;
mongoc_write_concern_native_ex::s_mongoc_write_concern_init(&wc);

// 设置为多数确认
mongoc_write_concern_native_ex::s_mongoc_write_concern_set_wmajority(&wc, 5000); // 5秒超时

// 设置写确认级别为3
mongoc_write_concern_native_ex::s_mongoc_write_concern_set_w(&wc, 3);

// 启用日志同步
mongoc_write_concern_native_ex::s_mongoc_write_concern_set_journal(&wc, true);

// 设置写标签
mongoc_write_concern_native_ex::s_mongoc_write_concern_set_wtag(&wc, "primaryDC");

// 冻结配置
mongoc_write_concern_native_ex::s_mongoc_write_concern_freeze(&wc);

// 检查配置
if (mongoc_write_concern_native_ex::s_mongoc_write_concern_is_acknowledged(&wc)) {
    std::cout << "Write operations will be acknowledged" << std::endl;
}

int32_t w_level = mongoc_write_concern_native_ex::s_mongoc_write_concern_get_w(&wc);
int64_t timeout = mongoc_write_concern_native_ex::s_mongoc_write_concern_get_wtimeout_int64(&wc);

// 获取 BSON 表示
const bson_t* bson_wc = mongoc_write_concern_native_ex::s_mongoc_write_concern_get_bson(&wc);

// 清理资源
mongoc_write_concern_native_ex::s_mongoc_write_concern_static_destroy(&wc);
```

## 注意事项

- 写关注影响写操作的性能和数据安全性
- `w=0`（不确认）提供最高性能，但不保证数据持久性
- `w=1`（仅主节点确认）平衡性能和安全性
- `w=majority`（多数确认）提供强一致性保证，但性能较低
- `wtimeout` 防止写操作无限等待确认
- `journal=true` 确保数据写入日志，提高崩溃恢复能力
- 写标签用于地理分布部署，将写操作定向到特定数据中心
- 冻结配置后不能修改，必须先解冻
- 默认写关注通常为 `w=1` 和 `journal=false`
- 超时时间为0表示无限等待
- 标签确认需要 MongoDB 副本集配置标签集
- fsync 参数已弃用，应使用 journal 参数
- 写关注可以通过连接字符串或单独对象设置
- 某些写关注设置可能不被所有 MongoDB 版本支持</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_write_concern_native_ex_cn.md