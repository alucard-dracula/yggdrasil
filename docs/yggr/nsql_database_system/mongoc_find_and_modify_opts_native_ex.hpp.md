# mongoc_find_and_modify_opts_native_ex.hpp

## 概述

此头文件定义了 `yggr_mongoc_find_and_modify_opts` 结构体和 `mongoc_find_and_modify_opts_native_ex` 类，用于处理 MongoDB 的 findAndModify 操作选项。这些定义提供了对 MongoDB C 驱动程序 findAndModify 功能的扩展包装。

## 命名空间

- `yggr::nsql_database_system`

## 结构体描述

### yggr_mongoc_find_and_modify_opts

一个结构体，封装了 MongoDB findAndModify 操作的选项参数。

#### 成员变量

##### `bson_t* sort`

排序条件 BSON 文档。指定结果文档的排序顺序。

##### `bson_t* update`

更新操作 BSON 文档。定义要应用的更新操作。

##### `bson_t* fields`

字段投影 BSON 文档。指定要返回的字段。

##### `u32 flags`

操作标志。使用静态常量定义的标志组合。

##### `u32 bypass_document_validation`

是否绕过文档验证。值为 0 表示不绕过，1 表示绕过。

##### `uint32_t max_time_ms`

最大执行时间（毫秒）。操作的最大允许执行时间。

##### `bson_t extra`

额外的 BSON 文档。用于传递额外的选项。

## 类描述

### mongoc_find_and_modify_opts_native_ex

一个类，提供 findAndModify 选项的静态常量和初始化方法。

#### 静态常量

##### 文档验证绕过常量

- `E_MONGOC_BYPASS_DOCUMENT_VALIDATION_FALSE = 0`：不绕过文档验证
- `E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE = (1 << 0)`：绕过文档验证
- `E_MONGOC_BYPASS_DOCUMENT_VALIDATION_DEFAULT = E_MONGOC_BYPASS_DOCUMENT_VALIDATION_TRUE`：默认绕过文档验证

##### 操作标志常量

- `E_MONGOC_FIND_AND_MODIFY_NONE = MONGOC_FIND_AND_MODIFY_NONE`：无特殊标志
- `E_MONGOC_FIND_AND_MODIFY_REMOVE = MONGOC_FIND_AND_MODIFY_REMOVE`：删除匹配的文档
- `E_MONGOC_FIND_AND_MODIFY_UPSERT = MONGOC_FIND_AND_MODIFY_UPSERT`：如果未找到匹配文档则插入
- `E_MONGOC_FIND_AND_MODIFY_RETURN_NEW = MONGOC_FIND_AND_MODIFY_RETURN_NEW`：返回修改后的文档

#### 静态方法

##### `s_mongoc_find_and_modify_opts_init`

```cpp
static void* s_mongoc_find_and_modify_opts_init(void* popts);
```

初始化 findAndModify 选项对象。

**参数：**
- `popts`：指向选项对象的指针

**返回值：**
- 初始化后的选项对象指针

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/base/static_constant.hpp>`
- `<yggr/nsql_database_system/mongodb_config.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_find_and_modify_opts_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 创建选项结构体
yggr_mongoc_find_and_modify_opts opts = {0};
opts.flags = mongoc_find_and_modify_opts_native_ex::E_MONGOC_FIND_AND_MODIFY_UPSERT |
             mongoc_find_and_modify_opts_native_ex::E_MONGOC_FIND_AND_MODIFY_RETURN_NEW;
opts.bypass_document_validation = mongoc_find_and_modify_opts_native_ex::E_MONGOC_BYPASS_DOCUMENT_VALIDATION_FALSE;
opts.max_time_ms = 5000;

// 初始化选项
void* mongoc_opts = mongoc_find_and_modify_opts_native_ex::s_mongoc_find_and_modify_opts_init(&opts);
```

## 注意事项

- 该结构体是对 MongoDB C 驱动程序 `_mongoc_find_and_modify_opts_t` 的自定义包装
- 标志可以使用按位或运算符组合使用
- `bypass_document_validation` 控制是否跳过 MongoDB 的文档验证规则
- `max_time_ms` 可以防止操作运行时间过长
- 静态常量使用 `YGGR_STATIC_CONSTANT` 宏定义，确保编译时常量折叠</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_find_and_modify_opts_native_ex_cn.md