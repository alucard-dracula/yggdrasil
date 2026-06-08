# mongodb_pak_id_def.hpp

## 概述

此头文件定义了 MongoDB 系统的包ID定义，包括包ID的范围常量和检查宏。包ID用于网络协议中的消息标识和版本控制。

## 命名空间

- `yggr::nsql_database_system`

## 结构体描述

### mongodb_pak_id_def

一个结构体，定义了 MongoDB 系统包ID的范围常量。

#### 常量定义

##### `E_id_start = 0x00008400`

包ID起始值。十六进制表示为 0x00008400 (33792)。

##### `E_id_end = 0x00008500`

包ID结束值。十六进制表示为 0x00008500 (34048)。

#### 范围说明

- 包ID范围：0x00008400 到 0x00008500
- 总共可用包ID数量：0x00008500 - 0x00008400 = 0x0100 = 256 个
- 这个范围专门为 MongoDB 系统预留

## 宏定义

### YGGR_PP_CHECK_MONGODB_PACKET_ID

```cpp
#define YGGR_PP_CHECK_MONGODB_PACKET_ID( __id__ ) \
    YGGR_PP_IS_IN_RANGE(__id__, \
                         ::yggr::nsql_database_system::mongodb_pak_id_def::E_id_start, \
                         ::yggr::nsql_database_system::mongodb_pak_id_def::E_id_end )
```

#### 功能

检查给定的包ID是否在有效的 MongoDB 包ID范围内。

#### 参数

- `__id__`：要检查的包ID值

#### 返回值

- 如果包ID在范围内，返回真值
- 如果包ID超出范围，返回假值

#### 使用示例

```cpp
// 在编译时检查包ID是否有效
YGGR_PP_CHECK_MONGODB_PACKET_ID(E_my_packet_id);

// 如果包ID无效，编译时会报错
```

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/base/static_constant.hpp>`
- `<yggr/ppex/range_checker.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongodb_pak_id_def.hpp>

using namespace yggr::nsql_database_system;

// 获取包ID范围
u32 start_id = mongodb_pak_id_def::E_id_start;  // 0x00008400
u32 end_id = mongodb_pak_id_def::E_id_end;      // 0x00008500

// 定义新的包ID（必须在范围内）
const u32 MY_COMMAND_ID = 0x00008401;  // 有效
// const u32 INVALID_ID = 0x00008600;  // 无效，会导致编译错误

// 在类中使用包ID检查
class MyMongoCommand {
public:
    YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, MY_COMMAND_ID, 1)
    
private:
    YGGR_PP_CHECK_MONGODB_PACKET_ID(MY_COMMAND_ID);  // 编译时验证
};
```

## 注意事项

- 包ID范围是预留的，确保与其他系统不冲突
- 使用 `YGGR_PP_CHECK_MONGODB_PACKET_ID` 宏可以在编译时捕获无效的包ID
- 包ID用于网络协议的消息路由和版本控制
- 范围从 0x00008400 到 0x00008500 提供 256 个可用ID
- 包ID应该是唯一的，避免消息处理冲突
- 这个头文件通常与其他 MongoDB 组件一起使用
- 宏检查使用预处理器范围检查，确保类型安全
- 包ID定义遵循 YGGR 框架的命名约定
- 在定义新包ID时，应该仔细选择不冲突的值</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongodb_pak_id_def_cn.md