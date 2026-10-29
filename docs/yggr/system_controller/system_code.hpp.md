# `system_code.hpp` 中文说明

## 用途

`yggr::system_controller::system_code` 定义系统控制器使用的系统代码常量，用于标识框架内的基础设施、网络、会话及其他系统模块。该类型仅提供静态常量，不用于创建对象。

```cpp
#include <yggr/system_controller/system_code.hpp>
```

## 常量

| 常量 | 数值 | 含义 |
| --- | ---: | --- |
| `E_SYSTEM_CODE` | `0x00006000` | 系统代码区间的起始基值。 |
| `E_IO_SERVICE_POOL` | `E_SYSTEM_CODE + 1` (`0x00006001`) | IO 服务池。 |
| `E_TCP_SERVER_SYSTEM` | `E_SYSTEM_CODE + 2` (`0x00006002`) | TCP 服务端系统。 |
| `E_UDP_SERVER_SYSTEM` | `E_SYSTEM_CODE + 3` (`0x00006003`) | UDP 服务端系统。 |
| `E_TCP_CLIENT_SYSTEM` | `E_SYSTEM_CODE + 4` (`0x00006004`) | TCP 客户端系统。 |
| `E_UDP_CLIENT_SYSTEM` | `E_SYSTEM_CODE + 5` (`0x00006005`) | UDP 客户端系统。 |
| `E_TCP_BASE_NETWORK_SYSTEM` | `E_SYSTEM_CODE + 6` (`0x00006006`) | TCP 基础网络系统。 |
| `E_UDP_BASE_NETWORK_SYSTEM` | `E_SYSTEM_CODE + 7` (`0x00006007`) | UDP 基础网络系统。 |
| `E_SESSION_SYSTEM` | `E_SYSTEM_CODE + 8` (`0x00006008`) | 会话系统。 |
| `E_PROXY_SYSTEM` | `E_SYSTEM_CODE + 9` (`0x00006009`) | 代理系统。 |
| `E_CONNECT_GROUP_SYSTEM` | `E_SYSTEM_CODE + 10` (`0x0000600A`) | 连接组系统。 |
| `E_FILE_SYSTEM` | `E_SYSTEM_CODE + 11` (`0x0000600B`) | 文件系统。 |
| `E_DATABASE_SYSTEM` | `E_SYSTEM_CODE + 12` (`0x0000600C`) | 数据库系统。 |
| `E_NSQL_DATABASE_SYSTEM` | `E_SYSTEM_CODE + 13` (`0x0000600D`) | NSQL 数据库系统。 |
| `E_SCRIPT_SYSTEM` | `E_SYSTEM_CODE + 14` (`0x0000600E`) | 脚本系统。 |
| `E_ARCHIVE_SYSTEM` | `E_SYSTEM_CODE + 15` (`0x0000600F`) | 归档系统。 |
| `E_SYS_CODE_END` | `0x00006FFF` | 系统代码区间的末尾。 |
| `E_SYS_CODE_USER` | `0x00007000` | 用户自定义代码区间的起始值。 |

所有常量的类型均为 `u32`。系统代码区间为 `0x00006000` 至 `0x00006FFF`；用户代码从 `0x00007000` 开始。

## 使用示例

这些常量可作为系统控制器的代码使用，例如传给 `ctrl_center` 的注册和分发接口：

```cpp
#include <yggr/system_controller/ctrl_center.hpp>
#include <yggr/system_controller/system_code.hpp>

struct io_handler
{
    void operator()() const
    {
    }
};

void example(yggr::system_controller::ctrl_center& center)
{
    const yggr::system_controller::ctrl_center::code_type class_code = 1;

    center.reg_ctrl(
        yggr::system_controller::system_code::E_IO_SERVICE_POOL,
        class_code,
        io_handler());

    center.ctrl(
        yggr::system_controller::system_code::E_IO_SERVICE_POOL,
        class_code);
}
```

## 类型特性

`system_code` 私有继承 `nonable::noncreateable`，用于限制对象创建。通过类名直接访问常量即可，无需构造实例。

在 MSVC 编译器下，头文件临时禁用警告 4624，并在声明结束后恢复此前的警告状态。
