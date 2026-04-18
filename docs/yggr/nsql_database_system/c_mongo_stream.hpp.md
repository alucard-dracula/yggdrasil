# c_mongo_stream.hpp 文档

## 文件概述

`c_mongo_stream.hpp` 是 Yggdrasil 项目中的一个头文件，用于统一包含 MongoDB 流相关的头文件。它并不定义新的类，而是将多个具体的 MongoDB 流类集中导入，以便在其他模块中统一引用。

## 命名空间

- 该文件本身不定义新的命名空间，但包含的类均位于 `yggr::nsql_database_system` 命名空间。

## 包含的头文件

- `yggr/nsql_database_system/mongodb_config.hpp`：MongoDB 配置相关定义。
- `yggr/nsql_database_system/c_mongo_buffered_stream.hpp`：缓冲流封装类。
- `yggr/nsql_database_system/c_mongo_file_stream.hpp`：文件流封装类。
- `yggr/nsql_database_system/c_mongo_gridfs_stream.hpp`：GridFS 流封装类。
- `yggr/nsql_database_system/c_mongo_socket_stream.hpp`：套接字流封装类。
- `yggr/nsql_database_system/c_mongo_tls_stream.hpp`：TLS 加密流封装类。

## 使用说明

该头文件适用于需要同时使用 MongoDB 流类型的场景，以减少需要单独包含多个流类型头文件的工作量。它主要作为便利性入口头文件。

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。