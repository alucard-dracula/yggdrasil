# lua_environment.hpp - 中文文档

**文件位置**: [yggr/script/lua_environment.hpp](yggr/script/lua_environment.hpp)

## 概述

`yggr::script::lua::lua_environment` 是一个轻量级的 Lua 环境标志类，用于控制 Lua 状态中要加载的标准库模块。

该类本身不管理 `lua_State` 生命周期，也不直接加载 Lua 库；它只保存一个位掩码 `_env_code`，表示当前环境中启用的模块集合。

## 主要类型

- `lua_env_type`：`lua_State*`，用于表示 Lua 虚拟机环境指针。

## 常量定义

类中定义了一组静态常量，用于表示不同 Lua 标准库模块：

- `E_lua_lib = 0x00000000`
- `E_lib_base = 0x00000001`
- `E_lib_package = 0x00000002`
- `E_lib_string = 0x00000004`
- `E_lib_table = 0x00000008`
- `E_lib_math = 0x00000010`
- `E_lib_io = 0x00000020`
- `E_lib_os = 0x00000040`
- `E_lib_debug = 0x00000080`
- `E_lib_all = E_lib_base | E_lib_package | E_lib_string | E_lib_table | E_lib_math | E_lib_io | E_lib_os`（0x0000007f）

这些常量可用于组合 Lua 环境模块加载策略。

## 主要接口

### 构造与析构

- `lua_environment()`
  - 默认构造函数
  - 初始化 `_env_code` 为 `0`
- `~lua_environment()`
  - 默认析构函数，无特殊资源释放逻辑

### 功能成员

- `bool append_module(u32 mod_idx = E_lib_base)`
  - 将指定模块标志加入当前环境位掩码
  - 通过按位或运算 ` _env_code |= mod_idx`
  - 默认添加 `E_lib_base`
  - 返回 `true`

- `u32 get_global_environment() const`
  - 返回当前环境位掩码 `_env_code`

## 设计说明

- 类采用位掩码方式保存模块组合状态，便于按需启用多个 Lua 标准库模块。
- `append_module` 不做重复检查，允许重复添加同一标志；位运算本身保证结果一致。
- `E_lib_all` 代表除 `E_lib_debug` 之外的所有默认标准库模块。
- 该类仅负责标志存储，不负责实际的 Lua 模块加载逻辑，适合作为 Lua 环境配置对象。

## 典型用法

```cpp
using namespace yggr::script::lua;

lua_environment env;
env.append_module(lua_environment::E_lib_base);
env.append_module(lua_environment::E_lib_string);

u32 code = env.get_global_environment();
// code 表示已启用的模块集合
```

## 注意事项

- `append_module` 默认参数为 `E_lib_base`，因此如果不传参数则添加基础库标志。
- 如需启用多个模块，可传入组合掩码，例如 `E_lib_string | E_lib_table`。
- 该类不包含 `lua_State` 初始化、关闭或模块加载函数；它仅作为环境标志持有者。

---

文档已生成：`lua_environment.hpp.md`。