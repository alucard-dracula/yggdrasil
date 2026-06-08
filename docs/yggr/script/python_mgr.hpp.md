# python_mgr.hpp - 中文文档

**文件位置**: [yggr/script/python_mgr.hpp](yggr/script/python_mgr.hpp)

## 概述

`yggr::script::script_mgr<Rule_ID, Script, Environment>` 是一个模板化的脚本管理器，用于管理按规则ID索引的脚本实例。它继承自 `Environment`（用作运行时环境或配置），并禁止拷贝操作（`boost::noncopyable`）。

`Script` 通常是封装脚本加载/执行逻辑的类（例如 `lua_script` 或 Python 脚本封装），`Rule_ID` 为脚本的键（例如字符串或整型）。

## 主要类型别名

- `base_type`：继承自的环境类型 `Environment`。
- `rule_id_type`：规则 ID 的类型，模板参数 `Rule_ID`。
- `script_type`：脚本类型，模板参数 `Script`。
- `script_ptr_type`：`boost::shared_ptr<script_type>`。
- `script_map_type`：`boost::unordered_map<rule_id_type, script_ptr_type>`，用于存储脚本对象。

## 核心成员

- 私有成员：`script_map_type _script_map` — 存储映射关系。

## 构造 / 析构

- `script_mgr()`：默认构造。
- `~script_mgr()`：析构时调用 `clear()` 清空脚本映射。

## 主要接口

- `bool insert(const std::string& rule_name, const std::string& code, const std::string& enter_foo)`
  - 直接创建 `script_type(code, enter_foo, base_type::get_grobal_environment())` 并插入映射。
  - 返回插入是否成功（若 key 已存在则返回 false）。

- `bool insert(const std::string& rule_name, const script_type& script)`
  - 以已有 `script` 对象插入映射。

- `template<typename Output_Real_Data, typename Input_Real_Data>
  bool execute_rule(const rule_id_type& id, const Input_Real_Data& in, Output_Real_Data& out)`
  - 根据 `id` 查找脚本对象并调用其 `execute_rule<Output_Real_Data>(boost::cref(in))`。
  - 将返回值赋给 `out`，成功返回 true；找不到脚本返回 false。

- `template<typename Input_Real_Data>
  bool execute_rule(const rule_id_type& id, const Input_Real_Data& in)`
  - 对应无返回值的规则调用（`execute_rule_no_ret`），找不到脚本返回 false。

- `bool remove(const std::string& rule_name)`
  - 从映射中移除指定规则名，返回删除结果（影响元素数量）。

- `void clear()`
  - 清空内部脚本映射并释放脚本对象。

## 设计要点

- 将脚本实例以智能指针 (`boost::shared_ptr`) 存储，统一生命周期管理。
- 继承 `Environment` 允许直接访问/传递环境信息（例如全局运行环境句柄）。
- 使用 `boost::unordered_map` 提供高效的查找和插入操作。
- `insert` 提供两种重载：一是按源码构造脚本，二是直接插入已有脚本对象，方便灵活使用场景。

## 使用示例

```cpp
// 假设 Script 为 yggr::script::lua_script，Rule_ID 为 std::string
script_mgr<std::string, lua_script, lua_environment> mgr;

// 插入脚本（以源码初始化）
mgr.insert("rule1", "print('hello')", "main_entry");

// 执行有返回值的规则
int out;
bool ok = mgr.execute_rule<int, std::string>("rule1", std::string("input"), out);

// 执行无返回值的规则
mgr.execute_rule<std::string>("rule1", std::string("input"));

// 移除脚本
mgr.remove("rule1");
```

## 注意事项

- `rule_id_type` 的哈希和相等性需在 `boost::unordered_map` 中可用（例如 `std::string`、整型等）。
- 插入使用 `script_type(code, enter_foo, base_type::get_grobal_environment())` 的重载假定 `script_type` 提供相应构造函数。
- 多线程使用时需自行添加同步保护以避免竞态。

---

文档已生成：`python_mgr.hpp.md`。