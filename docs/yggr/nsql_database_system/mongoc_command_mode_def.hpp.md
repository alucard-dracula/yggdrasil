# mongoc_command_mode_def.hpp

## 概述

此头文件定义了 `mongoc_command_mode_def` 结构体，该结构体包含用于 MongoDB 命令模式的静态常量定义。这些常量用于指定 MongoDB 操作的类型和权限级别。

## 命名空间

- `yggr::nsql_database_system`

## 结构体描述

### mongoc_command_mode_def

一个结构体，定义了 MongoDB 命令模式的枚举常量。

#### 静态常量

##### `E_MONGOC_CMD_RAW`

```cpp
YGGR_STATIC_CONSTANT(u32, E_MONGOC_CMD_RAW = 0);
```

原始命令模式。值为 0。

##### `E_MONGOC_CMD_READ`

```cpp
YGGR_STATIC_CONSTANT(u32, E_MONGOC_CMD_READ = 1);
```

读取命令模式。值为 1，表示只读操作。

##### `E_MONGOC_CMD_WRITE`

```cpp
YGGR_STATIC_CONSTANT(u32, E_MONGOC_CMD_WRITE = 2);
```

写入命令模式。值为 2，表示只写操作。

##### `E_MONGOC_CMD_RW`

```cpp
YGGR_STATIC_CONSTANT(u32, E_MONGOC_CMD_RW = 3);
```

读写命令模式。值为 3，表示同时支持读写操作。

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/base/static_constant.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_command_mode_def.hpp>

using namespace yggr::nsql_database_system;

// 使用常量
u32 mode = mongoc_command_mode_def::E_MONGOC_CMD_READ;

// 在条件判断中使用
if (mode == mongoc_command_mode_def::E_MONGOC_CMD_WRITE) {
    // 执行写入操作
}
```

## 注意事项

- 这些常量对应于 MongoDB C 驱动程序中的命令模式枚举
- 使用 `YGGR_STATIC_CONSTANT` 宏定义，确保常量在编译时解析
- 文件中注释掉了原始的 enum 定义，现在使用结构体包装的静态常量
- 这些模式用于区分不同类型的数据库操作，便于权限控制和操作分类</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_command_mode_def_cn.md