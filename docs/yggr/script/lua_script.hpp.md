# lua_script.hpp - 中文文档

**文件位置**: [yggr/script/lua_script.hpp](yggr/script/lua_script.hpp)

## 概述

`yggr::script::lua::lua_script` 是对 Lua/luabind 使用的封装类，负责：

- 创建并持有 Lua 虚拟机环境指针（`lua_State*`）
- 打开指定的标准库模块（通过 `lua_environment` 标志）
- 编译并执行脚本（字符串或文件）
- 查找并调用 Lua 中的函数（支持多种参数与返回值签名）
- 将 Lua 运行错误统一封装为库定义的 `error_type`

类为不可拷贝、不可移动类型（继承自 `noncopyable` 与 `nonmoveable`）。

## 主要类型别名

- `enter_foo_name_type`：用于表示要进入/调用的 Lua 函数名，类型为 `::yggr::string`。
- `string_type`：脚本/文件名字符串类型，`charset::utf8_string`。
- `string_code_type`：脚本内容类型，见 `string_code`。
- `file_code_type`：脚本文件路径类型，见 `file_code`。

## 错误定义

使用 `ERROR_MAKER_BEGIN("lua_script")` 宏定义了错误码和错误消息，常见错误包括：

- `E_lua_code_compile_error`：Lua 编译/加载错误
- `E_lua_code_runtime_error`：Lua 运行时错误
- `E_lua_code_runtime_error_detail`：运行时错误的详细信息

库内部在捕获 `luabind::error`、`luabind::cast_failed` 或 `compatibility::stl_exception` 时，会将其转换为上述 `error_type` 并抛出。

## 构造函数与析构函数

- `lua_script(u32 global_env, u32 local_env = 0)`
  - 创建新的 Lua 状态（`luaL_newstate()`），并根据 `global_env` / `local_env` 打开相应库。

- 模板构造函数带 `Handler_Module` 回调：
  - 在新建 Lua 状态并打开指定库后，调用 `handler_module(_lua_env)` 做用户自定义模块绑定。

- `lua_script(const enter_foo_name_type& enter_foo, u32 global_env, u32 local_env = 0)`
  - 指定进入的 Lua 函数名。

- 带脚本源码或脚本文件的构造函数版本：
  - 在完成库打开和模块绑定后，会尝试编译并加载脚本（`compile_from_string` 或 `compile_from_file`），若抛出 `error_type`，会把错误转为异常并关闭 Lua 状态。

- `~lua_script()`：析构时释放 Lua 状态（若持有）。

## 状态与检查

- `bool succeed() const`：当 `_lua_env` 有效且 `_enter_foo` 非空时返回 true。

## 脚本执行接口

- `void do_script(const string_code_type& code)`：从字符串编译并执行脚本（内部调用 `compile_from_string`）。
- `void do_script(const file_code_type& code)`：从文件编译并执行脚本（内部调用 `compile_from_file`）。

## 调用 Lua 函数（规则/函数）

提供两类高层调用接口，均支持可变参数（在不支持 C++11 时使用预处理宏展开实现）：

- `execute_rule<R>(Args...)`：调用构造时指定的 `_enter_foo`（入口规则），分为有返回值与无返回值版本。
- `call_function<R>(const enter_foo_name_type& foo_name, Args...)`：调用任意 Lua 函数名，分为有返回值与无返回值版本。

实现细节：
- 使用 `luabind::object` 获取函数对象并调用 `luabind::call_function`。
- 在调用过程中，对 `luabind::error`、`luabind::cast_failed`、以及 `compatibility::stl_exception` 进行捕获并封装为 `error_type`，异常信息由 `format_error_msg` 提供。

## 私有辅助函数

- `prv_get_function(const enter_foo_name_type& foo_name) const`：从 Lua 状态中获取函数对象（`luabind::object`）。
- `compile_from_string(const string_type& code)`：将字符串源码编译并加载到 Lua 环境。
- `compile_from_file(const string_type& fname)`：从指定文件加载并编译脚本。
- `format_error_msg(...)`：将不同异常类型格式化为可读错误信息。
- `open_libs(u32 env)`：根据 `lua_environment` 的位掩码打开需要的标准库。

## 错误处理策略

- 构造函数中若 `compile_from_string`／`compile_from_file` 抛出 `error_type`，构造器捕获后会将错误转发（`exception::exception::throw_error(e)`），并调用 `lua_close` 释放 `_lua_env`，将 `_lua_env` 置空。
- 调用函数时的任何运行时错误会包装为 `error_type` 并抛出，调用者应使用 `try/catch` 捕获处理。

## 线程与资源注意事项

- 类持有 `lua_State*`，默认不提供线程安全保证；若在多线程环境使用须自行同步或为每线程创建独立 `lua_script` 实例。
- 构造失败后（如脚本编译错误）类会释放 `_lua_env`，因此在构造后应检查 `succeed()`。

## 使用示例

```cpp
using namespace yggr::script::lua;

// 简单构造并加载脚本文件
lua_script s("main_entry", file_code_type("script.lua"), lua_environment::E_lib_all);
if(!s.succeed()) {
    // 处理错误
}

// 执行入口规则（无返回值）
s.execute_rule<void>(arg1, arg2);

// 调用任意函数并获取返回值
int r = s.call_function<int>("add", 1, 2);

// 直接执行脚本文本
s.do_script(string_code_type("print('hello')"));
```

## 设计要点总结

- 提供对 Lua 与 luabind 的高层封装，简化脚本加载、函数调用及错误转换。
- 使用模板与 SFINAE（或预处理宏）同时支持 C++11 可变参数与传统扩展实现。
- 错误统一由 `error_maker` 宏与 `error_type` 表示，便于上层捕获与定位问题。

---

文档已生成：`lua_script.hpp.md`。