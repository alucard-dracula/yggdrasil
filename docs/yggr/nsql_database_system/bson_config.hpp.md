# bson_config.hpp 文档

## 概述

`bson_config.hpp` 是 Yggdrasil 项目中的一个 C++ 头文件，提供 BSON (Binary JSON) 库的配置和初始化设置。该文件包含了 BSON 库的版本控制、宏定义和必要的头文件包含，用于正确配置 BSON 库的环境。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

该文件主要用于全局配置，不定义特定的命名空间。

## 文件描述

`bson_config.hpp` 是一个配置头文件，用于设置 BSON 库的编译时和运行时环境。它包含了：

- 版本检查和兼容性验证
- 静态链接配置
- MongoCrypt 相关配置
- 必要的头文件包含
- BSON 空标记常量定义

## 主要配置项

### 版本控制
- `BUILD_MONGODB_DRIVER_YGGR_VER`: 定义构建版本
- 版本检查：确保 BSON 库版本 >= 1.23.0

### 静态链接配置
当 BSON 版本 >= 1.7.0 时：
- `BSON_STATIC`: 启用静态链接

### MongoCrypt 配置
- `MONGOCRYPT_INTELDFP`: 在 x86 平台上启用 Intel DFP 支持（如果未禁用 Decimal128）

### BSON 常量
- `YGGR_BSON_EMPTY_MARK()`: BSON 空标记字节序列 `"\005\0\0\0\0"`
- `YGGR_BSON_EMPTY_MARK_LENGTH()`: 空标记长度 5 字节

## 依赖项

该文件依赖于以下头文件：
- `<yggr/base/yggrdef.h>`
- `<bson/bson-version.h>`
- `<bson.h>`

## 使用注意事项

1. 该文件通常在需要使用 BSON 功能之前包含
2. 它设置了全局的编译时配置
3. 包含了必要的 BSON 库头文件
4. 提供了版本兼容性检查

## 宏定义说明

### 强制定义
- `BSON_INSIDE`: 允许访问内部 API（临时定义后撤销）

### 条件定义
- `BSON_STATIC`: 静态链接配置（版本 >= 1.7.0）
- `MONGOCRYPT_INTELDFP`: Intel DFP 支持（x86 平台且未禁用 Decimal128）

## 版本要求

- BSON 库版本必须 >= 1.23.0

## 错误处理

该文件在编译时进行版本检查，如果不满足要求会产生编译错误：
- `#error "yggdrasil not support now bson version."`

## BSON 空标记

BSON 空标记是一个特殊的 5 字节序列 `"\005\0\0\0\0"`，用于表示空的 BSON 文档。这个常量在 Yggdrasil 框架中用于各种 BSON 操作的初始化和验证。

## 平台支持

- 所有支持 BSON 库的平台
- 特别支持 x86 平台的 Intel DFP 扩展（用于高精度十进制运算）

## 重要说明

该配置文件是 BSON 功能集成的基础，确保了正确的编译环境和运行时兼容性。任何使用 BSON 的代码都应该在包含此文件后使用 BSON 功能。