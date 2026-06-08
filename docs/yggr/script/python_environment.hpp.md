# python_environment.hpp - 中文文档

**文件位置**: [yggr/script/python_environment.hpp](yggr/script/python_environment.hpp)

## 概述

`yggr::script::python::python_environment` 提供对嵌入式 Python 解释器环境的全局访问及初始化/终止控制的封装，基于 Boost.Python。

该类负责：

- 管理全局 Python 名称空间（`__main__.__dict__`）的共享指针
- 提供 Python 初始化与终止接口
- 支持在解释器初始化前注册或导入扩展模块

注意：该头文件在 MSVC 使用静态 CRT(/MT 或 /MTd) 时会触发静态断言并禁用（通过编译时错误）。

## 关键类型

- `py_dict_ptr_type`：`yggr::shared_ptr<boost::python::dict>`，用于持有全局名称空间字典的智能指针。
- `this_type`：`python_environment` 的别名。

## 静态成员（私有）

- `static py_dict_ptr_type _s_pglobal;` — 指向全局 Python 名典（`__main__.__dict__`）。

## 内部辅助类型

- `detail::failed_dict`：提供一个未初始化时的 `boost::python::dict` 临时引用（通过内存缓冲区 reinterpret_cast 实现），用于在断言失败时避免解引用空指针导致未定义行为。

## 公共接口

- `static boost::python::dict& s_get_global_environment()`
  - 返回全局名称空间的引用。内部使用 `assert(_s_pglobal)` 以保证已初始化。
  - 当 `_s_pglobal` 非空时返回其解引用，否则返回 `detail::failed_dict()` 的引用（作为安全后备）。

- `boost::python::dict& get_global_environment()` / `const boost::python::dict& get_global_environment() const`
  - 非静态访问接口，转发至 `s_get_global_environment()`。

- `static bool append_module(const char* mod_name)`
  - 尝试导入模块（`PyImport_ImportModule`）。返回值反映导入是否成功。函数内部会先检查 Python 是否已初始化。

- `template<typename Module_Init_Foo> static bool append_module(const char* mod_name, Module_Init_Foo foo)`
  - 在解释器未初始化时，将模块初始化函数注册到内置模块表（`PyImport_AppendInittab`）。返回注册是否成功。

- `static bool py_initialize()`
  - 如果 Python 未初始化则调用 `Py_Initialize()`，并在首次初始化时从 `__main__` 导入全局字典用于 `_s_pglobal`。返回 `Py_IsInitialized()` 的最终状态。

- `static bool py_finalize()`
  - 如果已初始化则清理 `_s_pglobal`（清空并释放智能指针），然后调用 `Py_Finalize()`，返回终止是否成功。

## 使用注意

- 在调用 `s_get_global_environment()` 前，应保证 `py_initialize()` 已成功执行，否则断言会触发。
- `append_module` 的两个重载分别用于：运行时导入已有模块，或在解释器初始化前注册扩展模块（例如当你有 C/C++ 扩展模块需要在初始化时自动可用）。
- 在多次初始化/终止场景中，`py_finalize()` 会清空并释放持有的全局字典指针以避免引用悬空。

## 简单示例

```cpp
using namespace yggr::script::python;

// 初始化Python解释器并准备全局环境
if(!python_environment::py_initialize()) {
    // 处理初始化失败
}

// 导入模块或注册扩展
python_environment::append_module("math");

// 访问全局名称空间
boost::python::dict& g = python_environment::s_get_global_environment();

// 终止解释器并释放全局字典
python_environment::py_finalize();
```

## 设计要点

- 使用 `yggr::shared_ptr` 管理 `boost::python::dict` 的生命周期，避免裸指针泄漏。
- 在解释器未初始化时提供模块注册接口，以便在 `Py_Initialize()` 时可用。
- 通过 `detail::failed_dict` 提供在断言失效时的安全回退，降低调用时的未定义行为风险。

---

文档已生成：`python_environment.hpp.md`。