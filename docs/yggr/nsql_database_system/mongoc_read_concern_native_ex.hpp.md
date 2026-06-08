# mongoc_read_concern_native_ex.hpp

## 概述

此头文件定义了 `yggr_mongoc_read_concern` 结构体和 `mongoc_read_concern_native_ex` 类，用于处理 MongoDB 的读关注 (Read Concern) 配置。读关注用于控制从副本集读取数据的一致性级别。

## 命名空间

- `yggr::nsql_database_system`

## 结构体描述

### yggr_mongoc_read_concern

一个结构体，封装了 MongoDB 读关注的配置参数。

#### 成员变量

##### `char* level`

读关注级别字符串。指定数据一致性要求。

##### `bool frozen`

冻结状态标志。指示读关注是否已被冻结（不可修改）。

##### `bson_t compiled`

编译后的 BSON 文档。包含读关注的内部表示。

## 类描述

### mongoc_read_concern_native_ex

一个工具类，提供读关注的管理方法。

#### 读关注级别常量

##### `s_level_local()`

返回本地读关注级别字符串 ("local")。从最近的数据读取，可能包括未提交的写操作。

##### `s_level_majority()`

返回多数读关注级别字符串 ("majority")。确保读取已由多数节点确认的提交数据。

##### `s_level_available()`

返回可用读关注级别字符串 ("available")。从任何可用数据读取，优先考虑性能而非一致性。

##### `s_level_linearizable()`

返回线性化读关注级别字符串 ("linearizable")。提供最强的一致性保证。

##### `s_level_snapshot()`

返回快照读关注级别字符串 ("snapshot")。在事务中读取特定快照的数据。

#### 级别验证方法

##### `s_level_check(const char* str_lv)`

验证给定的字符串是否为有效的读关注级别。

**参数：**
- `str_lv`：要验证的级别字符串

**返回值：**
- `true` 如果是有效级别，`false` 否则

##### `s_level_check(const String& str_lv)`

模板方法，验证字符串对象是否为有效的读关注级别。

**参数：**
- `str_lv`：要验证的字符串对象

**返回值：**
- `true` 如果是有效级别，`false` 否则

#### 管理方法

##### `s_mongoc_read_concern_init`

初始化读关注对象。

##### `s_mongoc_read_concern_move`

移动读关注对象。

##### `s_mongoc_read_concern_copy`

复制读关注对象。

##### `s_mongoc_read_concern_unfreeze_copy`

创建未冻结的读关注副本。

##### `s_mongoc_read_concern_swap`

交换两个读关注对象。

##### `s_mongoc_read_concern_static_destroy`

销毁读关注对象。

#### 配置方法

##### `s_mongoc_read_concern_get_level`

获取读关注级别。

##### `s_mongoc_read_concern_set_level`

设置读关注级别。

##### `s_mongoc_read_concern_is_default`

检查是否为默认读关注。

##### `s_mongoc_read_concern_append`

将读关注追加到命令 BSON 文档。

##### `s_mongoc_read_concern_freeze`

冻结读关注，使其不可修改。

##### `s_mongoc_read_concern_unfreeze`

解冻读关注，使其可修改。

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/charset/utf8_string.hpp>`
- `<yggr/charset/utf8_string_view.hpp>`
- `<yggr/nsql_database_system/mongodb_config.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_read_concern_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 创建读关注对象
mongoc_read_concern_t* read_concern = nullptr;
read_concern = (mongoc_read_concern_t*)mongoc_read_concern_native_ex::s_mongoc_read_concern_init(read_concern);

// 设置多数读关注
mongoc_read_concern_native_ex::s_mongoc_read_concern_set_level(
    read_concern, mongoc_read_concern_native_ex::s_level_majority());

// 验证级别
bool is_valid = mongoc_read_concern_native_ex::s_level_check("majority");

// 冻结配置
mongoc_read_concern_native_ex::s_mongoc_read_concern_freeze(read_concern);
```

## 注意事项

- 读关注级别影响数据一致性和性能
- `local` 级别提供最佳性能，但可能读取未提交的数据
- `majority` 级别提供强一致性保证，但可能影响性能
- `linearizable` 级别提供最强一致性，但性能开销最大
- 冻结的读关注对象不可修改，必须先解冻
- 使用完读关注对象后需要调用销毁方法释放资源
- 级别字符串区分大小写</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_read_concern_native_ex_cn.md