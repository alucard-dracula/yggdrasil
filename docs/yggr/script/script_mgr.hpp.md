# script_mgr.hpp - 中文文档

**文件位置**: [yggr/script/script_mgr.hpp](yggr/script/script_mgr.hpp)

## 概述

`yggr::script::script_mgr<Rule_ID, Script, Environment>` 是一个通用脚本管理器模板，负责：

- 维护按规则 ID 映射的脚本实例
- 提供脚本插入、执行、调用、查询和删除接口
- 继承环境类型 `Environment`，允许直接使用环境配置和模块加载方法

该类同时禁止拷贝与移动，并使用安全容器封装脚本对象。

## 关键类型

- `base_type`：继承自模板参数 `Environment`
- `rule_id_type`：规则 ID 类型
- `script_type`：脚本类型
- `enter_foo_name_type`：脚本入口函数名类型，由 `script_type` 定义
- `safe_script_type`：`safe_container::safe_wrap<script_type>`，用于线程安全访问脚本对象
- `safe_script_ptr_type`：`yggr::shared_ptr<safe_script_type>`
- `error_type`：`yggr::error`
- `string_code_type`、`file_code_type`：分别来自 `script_type` 的脚本代码类型和文件类型

## 内部存储

- `script_ptr_type`：`safe_script_type::element_ptr_type`
- `script_map_type`：`safe_container::safe_unordered_map<rule_id_type, safe_script_ptr_type>`
- `_script_map`：实际存储脚本对象的安全哈希表

## 构造函数

- 支持可变参数构造，直接转发到基类 `Environment` 的构造函数。
- 在不支持 C++11 可变模板时，使用 Boost.Preprocessor 生成多个重载版本。

## 脚本模块追加

- `using base_type::append_module;`
- `append_script_module(const rule_id_type& rule_id, const Code& code)`
  - 查找已存在脚本对象并调用其 `do_script(code)` 扩展脚本内容
  - 若发生 `error_type` 异常则转发并返回 false

## 插入脚本

### `insert(rule_id, enter_foo, code, args...)`

- 创建 `script_type` 实例：`new script_type(enter_foo, code, base_type::get_global_environment(), args...)`
- 仅当脚本对象非空且 `succeed()` 返回 true 时才继续插入
- 插入目标为 `safe_script_ptr_type`，并调用 `ptr->call_function(&script_type::succeed)` 进行二次验证
- 返回是否插入成功

### `make_script_object(args...)`

- 静态辅助函数，可在外部构造脚本对象并返回安全指针
- 若构造失败或 `succeed()` 不通过，则释放对象并返回 false

### `insert(rule_id, safe_script_ptr_type ptr)`

- 直接插入已经包装好的脚本指针
- 会检查指针有效性、容器是否非空、脚本是否成功加载

## 执行规则

提供两类执行接口：

### `execute_rule(rule_id_anchor, args...)`

- 查找脚本并调用脚本对象的 `execute_rule<void, Args...>(args...)`
- 成功返回 true，失败返回 false

### `execute_rule(R& out, rule_id_anchor, args...)`

- 查找脚本并调用脚本对象的 `execute_rule<R, Args...>(args...)`
- 将结果写回 `out`

这两个接口使用 `yggr::utility::args_anchor` 包装 `rule_id`，支持在可变参数表达式中安全传递规则键。

## 调用函数

提供两类动态函数调用接口：

### `call_function(rule_id_anchor, enter_foo_name, args...)`

- 调用指定脚本对象中的任意函数，并忽略返回值

### `call_function(R& out, rule_id_anchor, enter_foo_name, args...)`

- 调用指定函数并将结果写入 `out`

这两个接口内部使用脚本对象的 `call_function` 成员，并捕获 `error_type` 与 `compatibility::stl_exception`。

## 查询与管理

- `get_script(rule_id)`：返回脚本对象的安全指针
- `remove(rule_id)`：从映射中移除规则脚本
- `clear()`：清空脚本管理器
- `size()` / `empty()`：获取当前脚本数量和空状态

## 私有执行/调用实现

- `prv_execute_rule` / `prv_call_function`
  - 查找脚本实例
  - 通过 `func::member_foo_t_maker` 生成成员函数指针类型
  - 调用脚本对象内对应函数
  - 捕获异常并转换为库内部错误

- 代码兼容性处理
  - 若不支持 C++11 可变参数，使用 `BOOST_PP_LOCAL_ITERATE` 生成多个参数重载
  - 在 MSVC 下针对 rvalue 参数转换和函数指针重载做特别处理

## 设计亮点

- 结合 `Environment` 与 `Script` 模板参数实现可重用的脚本管理器
- 使用 `safe_container::safe_unordered_map` 和 `safe_wrap` 提供线程安全访问
- 自动管理脚本生命周期，并通过 `shared_ptr` 保证对象稳定性
- 提供统一的执行与调用 API，便于上层业务逻辑按规则 ID 触发脚本行为

## 典型用法

```cpp
using namespace yggr::script;

script_mgr<std::string, python_script, python_environment> mgr;

mgr.insert("rule1", "main", string_code("print('hello')"));

int result;
bool ok = mgr.execute_rule<int>(args_anchor("rule1"), 42, result);

mgr.call_function(args_anchor("rule1"), "other_func", 1, 2);
```

---

文档已生成：`script_mgr.hpp.md`。