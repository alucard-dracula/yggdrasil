# `time_bson_impl.hpp`

## 概述

该头文件用于控制时间类型 BSON 序列化实现的条件编译。文件本身不定义时间序列化函数，也不提供可直接调用的 API，而是根据 BSON 输入或输出归档是否启用，设置相应的内部宏。

## 头文件

```cpp
#include <yggr/serialization/time_bson_impl.hpp>
```

通常由时间序列化相关头文件或 BSON 实现的重新包含流程间接使用。

## BSON 编译条件

当以下任一宏已定义时，文件启用自身的 include guard：

```cpp
YGGR_HAS_BSON_OARCHIVE
YGGR_HAS_BSON_IARCHIVE
```

对应判断为：

```cpp
#if defined(YGGR_HAS_BSON_OARCHIVE) \
 || defined(YGGR_HAS_BSON_IARCHIVE)
#   define __YGGR_SERIALIZATION_TIME_BSON_IMPL_HPP__
#endif
```

这表示 BSON 输出归档或 BSON 输入归档至少有一种可用。

如果两个 BSON 归档宏均未定义，则设置内部控制宏：

```cpp
YGGR_SERIALIZATION_TIME_BSON_IMPL_HPP_ARCHIVE
```

## 宏状态转换

| 条件 | 设置结果 | 含义 |
| --- | --- | --- |
| `YGGR_HAS_BSON_OARCHIVE` 或 `YGGR_HAS_BSON_IARCHIVE` 已定义 | `__YGGR_SERIALIZATION_TIME_BSON_IMPL_HPP__` | BSON 归档环境已启用，完成正常包含保护 |
| 两个 BSON 归档宏均未定义 | `YGGR_SERIALIZATION_TIME_BSON_IMPL_HPP_ARCHIVE` | 记录时间 BSON 实现的待处理状态 |

在检测当前状态前，如果发现 `YGGR_SERIALIZATION_TIME_BSON_IMPL_HPP_ARCHIVE` 已存在，文件会先取消定义它，避免旧状态影响本次判断。

## 重新包含用途

`YGGR_SERIALIZATION_TIME_BSON_IMPL_HPP_ARCHIVE` 是供 BSON 序列化重新包含机制使用的内部标记。它用于记录当前时间 BSON 实现需要在合适的 BSON 归档环境建立后再次处理。

该宏属于实现细节，业务代码不应主动定义、取消定义或依赖其存在状态。

## 注意事项

- 该文件不声明 `save`、`load` 或 `serialize` 函数。
- 时间类型的具体序列化实现位于其他时间序列化相关头文件中，本文件只负责条件编译状态协调。
- `YGGR_HAS_BSON_OARCHIVE` 控制 BSON 输出归档相关能力，`YGGR_HAS_BSON_IARCHIVE` 控制 BSON 输入归档相关能力。
- 修改 BSON 归档宏的定义顺序时，应同时检查时间序列化头文件以及相关 BSON 重新包含流程。
- 源文件顶部的历史注释写有 `utility_bson_impl.hpp`，但实际 include guard 和内部宏均属于 `time_bson_impl.hpp`。
