# basic_mongodb_config.hpp 文档

## 概述

`basic_mongodb_config.hpp` 是 Yggdrasil 项目中的一个 C++ 头文件，提供 MongoDB C 驱动程序的配置和初始化设置。该文件包含了各种宏定义、条件编译指令和必要的头文件包含，用于正确配置和初始化 MongoDB 驱动程序的环境。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

该文件主要用于全局配置，不定义特定的命名空间。

## 文件描述

`basic_mongodb_config.hpp` 是一个配置头文件，用于设置 MongoDB C 驱动程序的编译时和运行时环境。它包含了：

- 版本检查和兼容性验证
- 客户端加密（CSFLE）支持配置
- SSL/TLS 支持配置
- 平台特定的调整
- 必要的头文件包含

## 主要配置项

### 版本控制
- `BUILD_MONGODB_DRIVER_YGGR_VER`: 定义构建版本
- 版本检查：确保 MongoDB C 驱动版本 >= 1.24.4
- BSON 和 MongoC 版本一致性检查

### 客户端加密支持
当 `MONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION` 定义时：
- 配置 MLIB 库支持
- 配置 KMS 消息库
- 配置 MongoCrypt 库
- 设置字节序（小端）
- 可选的 Intel DFP 支持

### SSL/TLS 支持
当 `MONGOC_ENABLE_SSL` 定义时：
- 包含 OpenSSL 头文件
- 支持 SSL 连接

### 平台特定配置
- Windows：包含套接字冲突修复器
- Android/iOS：禁用客户端加密支持（CSFLE）

## 依赖项

该文件依赖于以下头文件：
- `<yggr/base/yggrdef.h>`
- `<yggr/nsql_database_system/bson_config.hpp>`
- `<mongoc-version.h>`
- `<mongoc-config.h>`
- `<mongoc.h>`
- `<openssl/ssl.h>` (条件编译)
- `<yggr/charset/string.hpp>`
- `<yggr/nonable/noncreateable.hpp>`
- `<yggr/nsql_database_system/bson_typeid_def.hpp>`
- `<yggr/network/socket_conflict_fixer.hpp>` (Windows)

## 使用注意事项

1. 该文件通常作为其他 MongoDB 相关文件的第一个包含文件
2. 它设置了全局的编译时配置
3. 包含了必要的 MongoDB C 驱动头文件
4. 提供了版本兼容性检查

## 宏定义说明

### 强制定义
- `MONGOC_INSIDE`: 允许访问内部 API（临时定义后撤销）
- `MONGOC_STATIC`: 静态链接配置（版本 >= 1.7.0）

### 条件定义
- `BID_USING_IN_MONGODB`: MLIB 在 MongoDB 中的使用标志
- `MLIB_USER`: MLIB 用户标志
- `KMS_MSG_STATIC`: KMS 消息静态链接
- `MONGOCRYPT_STATIC_DEFINE`: MongoCrypt 静态定义
- `MONGOCRYPT_LITTLE_ENDIAN`: 小端字节序
- `MONGOCRYPT_INTELDFP`: Intel DFP 支持

### 平台特定
- `YGGR_MONGODB_NOT_SUPPORT_CSFLE`: 在移动平台上禁用客户端加密

## 版本要求

- MongoDB C 驱动版本必须 >= 1.24.4
- BSON 版本必须与 MongoC 版本一致
- 驱动必须支持 Yggdrasil 版本

## 错误处理

该文件在编译时进行版本检查，如果不满足要求会产生编译错误：
- `#error "yggdrasil not support now mongoc version."`
- `#error "bson version not equal mongoc version!!!"`
- `#error "this driver is not support yggdrasil!!!!"`

## 平台支持

- Windows (Win32/Win64)
- Linux/Unix
- macOS
- Android (CSFLE 不支持)
- iOS (CSFLE 不支持)

## 安全特性

- 支持客户端字段级加密（CSFLE）
- 支持 SSL/TLS 加密连接
- 版本验证确保安全更新

## 重要说明

该配置文件是整个 MongoDB 集成的基础，确保了正确的编译环境和运行时兼容性。任何 MongoDB 相关的代码都应该在包含此文件后使用。