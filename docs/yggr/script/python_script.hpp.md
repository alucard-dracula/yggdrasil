# python_script.hpp - 中文文档

**文件位置**: [yggr/script/python_script.hpp](yggr/script/python_script.hpp)

## 概述

`yggr::script::python::python_script` 是一个对 Boost.Python 的封装类，用于加载 Python 脚本并在指定的 Python 全局环境中调用函数。

该类同样是不可拷贝、不可移动类型，通过构造函数传入入口函数名、脚本代码或文件，以及 Python 全局字典环境。它将 Python 编译/运行错误封装为内部定义的 `error_type`。

## 关键类型

- `enter_foo_name_type`：Lua 函数名类型，`::yggr::string`。
- `string_type`：脚本字符串类型，`charset::utf8_string`。
- `string_code_type`：脚本文本编码类型。
- `file_code_type`：脚本文件路径类型。
- `py_dict_type`：`boost::python::dict`。
- `py_object_type`：`boost::python::object`。

## 错误定义

使用 `ERROR_MAKER_BEGIN("python_script")` 定义错误码：

- `E_python_code_compile_error`：Python 代码编译错误
- `E_python_code_runtime_error`：Python 运行时错误

## 构造与析构

- `python_script(const enter_foo_name_type& enter_foo, const string_code_type& code, py_dict_type& global)`
- `python_script(const enter_foo_name_type& enter_foo, const file_code_type& code, py_dict_type& global)`
- `~python_script()`

构造函数将入口函数名、脚本内容或文件路径，以及 Python 全局环境字典保存到对象中，并尝试编译脚本以准备执行。

## 核心接口

- `bool succeed() const`
  - 返回 `_bsuccess`，表示脚本是否成功加载和初始化。

- `template<typename R, typename ...Args> R execute_rule(Args... args) const`
  - 调用构造时指定的入口函数 `_enter_foo`。
  - 支持有返回值和无返回值两种版本。

- `template<typename R, typename ...Args> R call_function(const enter_foo_name_type& foo_name, Args... args) const`
  - 调用任意指定名称的 Python 函数。
  - 同样支持有返回值和无返回值两种版本。

## 内部调用逻辑

- `prv_execute_rule_of_non_ret` / `prv_execute_rule_of_ret`
  - 通过 `_local[_enter_foo.data()]` 查找入口函数对象，使用 `boost::python::call<void>` 或 `boost::python::object(...)` 执行。
  - 捕获 `boost::python::error_already_set` 和 `compatibility::stl_exception`，将其转为 `error_type` 并抛出。

- `prv_call_function_of_non_ret` / `prv_call_function_of_ret`
  - 类似执行规则，但通过传入的函数名 `foo_name` 查找和值调用。

## 脚本加载方法

- `get_exception_msg()`
  - 从 Python 异常状态中提取详细错误信息。

- `compile_from_string(const string_type& code)`
  - 从字符串编译 Python 脚本。

- `compile_from_file(const string_type& code)`
  - 从文件编译 Python 脚本。

## 成员变量

- `_enter_foo`：入口函数名。
- `_local`：局部 Python 字典环境，用于查找函数和变量。
- `_object`：Python 对象句柄，保存脚本模块或执行结果。
- `_bsuccess`：脚本加载是否成功的标志。

## 设计要点

- 通过 `boost::python::dict` 传入运行环境，支持在外部共享 Python 全局对象。
- 对入口函数与其他函数调用进行统一封装，支持有返回值和无返回值版本。
- 错误处理严格区分编译错误与运行时错误，便于上层逻辑区分并处理。
- 兼容无 C++11 可变模板环境，使用 Boost.Preprocessor 生成变参重载。

## 使用示例

```cpp
using namespace yggr::script::python;

boost::python::dict global = python_environment::s_get_global_environment();
python_script script("main", string_code_type("def main(x): return x + 1"), global);

if(script.succeed()) {
    int result = script.execute_rule<int>(42);
}

script.call_function<void>("print", std::string("Hello from Python"));
```

## 注意事项

- `execute_rule` 仅在 `_bsuccess` 为 `true` 时可用，否则会抛出编译错误类型。
- `call_function` 与 `execute_rule` 在异常发生时均会捕获 Python 异常并转换为 `error_type`。
- `boost::python::error_already_set` 发生后会调用 `boost::python::handle_exception()` 来清理异常状态。

---

文档已生成：`python_script.hpp.md`。