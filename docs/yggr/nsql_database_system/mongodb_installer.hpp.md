# mongodb_installer.hpp

## 概述

此头文件定义了 `mongodb_installer` 类，用于 MongoDB C 驱动的初始化和清理。它管理网络上下文和 SSL 上下文的生命周期，确保 MongoDB 客户端能够正确初始化和释放资源。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

### mongodb_installer

一个不可实例化的工具类（继承自 `nonable::noncreateable`），提供 MongoDB 系统的全局初始化和清理功能。

#### 初始化标志常量

##### 基础标志

- `E_init_flag = 0`：无初始化标志
- `E_init_flag_network = 0x00000001`：网络初始化标志
- `E_init_flag_openssl = 0x00000002`：OpenSSL 初始化标志

##### 组合标志

- `E_init_flag_all = (E_init_flag_network | E_init_flag_openssl)`：所有初始化标志

#### 操作标志常量

- `E_chk_non_init = 0`：未初始化检查
- `E_chk_init = 1`：已初始化检查
- `E_chk_uninit = 2`：未初始化检查（用于卸载）

#### 安装方法

##### `install` (默认参数版本)

```cpp
static void install(u32 flag = E_init_flag_all)
```

使用默认参数安装 MongoDB 系统。

**参数：**
- `flag`：初始化标志，默认为 `E_init_flag_all`

##### `install` (完整参数版本)

```cpp
template<typename String1, typename String2, typename String3>
static void install(u32 flag,
                   const String1& str_driver_name,
                   const String2& str_driver_version,
                   const String3& str_platform)
```

使用完整参数安装 MongoDB 系统，包括驱动信息。

**参数：**
- `flag`：初始化标志
- `str_driver_name`：驱动名称字符串
- `str_driver_version`：驱动版本字符串
- `str_platform`：平台字符串

**模板参数：**
- `String1, String2, String3`：字符串类型，会自动转换为 UTF-8

#### 清理方法

##### `uninstall`

```cpp
static void uninstall(u32 flag = E_init_flag_all)
```

卸载 MongoDB 系统，清理资源。

**参数：**
- `flag`：清理标志，默认为 `E_init_flag_all`

##### `this_thread_clear`

```cpp
static void this_thread_clear(void)
```

清理当前线程的 MongoDB 相关资源。

#### 私有方法

##### 网络上下文管理

- `create_network_context`：创建网络上下文
- `destroy_network_context`：销毁网络上下文

##### OpenSSL 上下文管理 (条件编译)

当定义了 `MONGOC_ENABLE_SSL` 时可用：

- `create_openssl_context`：创建 OpenSSL 上下文
- `destroy_openssl_context`：销毁 OpenSSL 上下文
- `clear_openssl_error`：清除 OpenSSL 错误

## 依赖项

- `<yggr/base/yggrdef.h>`
- `<yggr/base/static_constant.hpp>`
- `<yggr/charset/utf8_string.hpp>`
- `<yggr/charset/utf8_string_view.hpp>`
- `<yggr/nsql_database_system/basic_mongodb_config.hpp>`
- `<boost/atomic.hpp>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongodb_installer.hpp>

using namespace yggr::nsql_database_system;

// 基本安装 - 初始化所有组件
mongodb_installer::install();

// 自定义安装 - 只初始化网络
mongodb_installer::install(mongodb_installer::E_init_flag_network);

// 完整安装 - 包含驱动信息
mongodb_installer::install(mongodb_installer::E_init_flag_all,
                          "MyApp Driver",
                          "1.0.0",
                          "Linux x64");

// 使用 MongoDB 客户端...

// 清理资源
mongodb_installer::uninstall();

// 在线程结束时清理
mongodb_installer::this_thread_clear();
```

## 注意事项

- 这是一个静态类，不能实例化
- `install` 和 `uninstall` 应该成对使用
- 通常在应用程序启动时调用 `install`，退出时调用 `uninstall`
- 多线程应用程序中，每个线程结束时应该调用 `this_thread_clear`
- 初始化标志可以组合使用，如 `E_init_flag_network | E_init_flag_openssl`
- OpenSSL 支持需要编译时定义 `MONGOC_ENABLE_SSL`
- 驱动信息参数是可选的，用于 MongoDB 服务器日志记录
- 字符串参数会自动转换为 UTF-8 编码
- 重复调用 `install` 是安全的（使用原子操作检查）
- 网络上下文管理确保套接字和连接池正确初始化
- SSL 上下文管理处理加密连接的证书和密钥
- 该类使用 Boost.Atomic 确保线程安全的初始化检查</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongodb_installer_cn.md