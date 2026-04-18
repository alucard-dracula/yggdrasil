# c_bson_regex.hpp 文档

## 概述

`c_bson_regex.hpp` 是 Yggdrasil 项目中的 C++ 头文件，封装 BSON 的正则表达式类型为 `c_bson_regex`，基于 `c_bson_string_item` 扩展，提供正则表达式模式、选项字符串的管理、序列化、比较和 JSON 风格输出。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`
- `yggr::nsql_database_system::detail`
- `yggr::nsql_database_system::swap_support`

## 文件说明

`c_bson_regex` 表示 BSON 中的正则表达式类型。它包含两个主要成员：正则表达式模式字符串（继承自 `c_bson_string_item` 的 `_str`）和选项字符串 `_opts`。类提供丰富的构造/赋值接口，支持不同字符串类型、移动语义、以及从 `bson_value_t` 初始化。

## 类型 ID

```cpp
YGGR_PP_BSON_TYPEID_IMPL(c_bson_regex, bson_typeid_def::E_BSON_TYPE_REGEX)
```

## 主要特性

- 管理正则表达式模式与选项字符串
- 自动规范化与排序选项（使用底层 `bson_regex_native_ex::s_bson_regex_options_sorted`）
- 支持从 BSON 值读取并比较
- 支持保存为 BSON 字段或 BSON 值
- JSON 输出：支持不同 JSON 模式（canonical/relaxed/legacy）

## 类结构

- 继承自 `c_bson_string_item`，重用其字符串成员 `_str`
- 新增成员 `string_type _opts` 存储正则选项

## 构造函数

- `c_bson_regex()` 默认构造
- `c_bson_regex(const bson_value_t& val)` 从 BSON 值构造
- 多个模板构造函数，支持：
  - 两个任意可转换为 UTF-8 的字符串参数（pattern, options）
  - 支持 `string_type` 的移动或拷贝构造
  - 支持 `BOOST_RV_REF(string_type)` 移动语义

## 赋值与交换

- 支持 `operator=(const BsonValue&)` 从 `bson_value_t` 赋值
- 支持移动与复制赋值
- `swap()` 方法与 `swap_support` 宏实现交换行为

## 选项处理

- `pro_set_options_tpl` / `pro_set_options` 会调用 `bson_regex_native_ex::s_bson_regex_options_sorted` 将选项规范化并按字典序排序
- `options()` 返回一个 `member_var_modify_helper`，可用于以原子方式修改选项字符串

## 访问器

- `regex()` 返回对模式字符串（`_str`）的引用
- `options()` 返回上面提到的修改帮助器或常量访问方式

## 比较

- `compare_eq(const BsonValue& val)` 使用 `bson_value_native_ex::s_value_cmp_eq_regex` 比较 BSON 值是否匹配相同的模式与选项
- `compare_eq(const c_bson_regex& right)` 检查两个对象是否相同（包括 `_str` 和 `_opts`）

## 序列化

- `save(Bson& bs, const String& name)` 将正则表达式写入 BSON 字段，调用 `bson_append_regex`
- `save_value(Bson& bs)` 将其作为 BSON 值写入，调用 `s_bson_save_value_of_regex`
- `load(const BsonIter& iter, const String& name)` / `load_value(const BsonIter& iter)` 从迭代器读取正则表达式的模式与选项并规范化选项

## 流输出 (JSON)

`detail::bson_operator_outter<c_bson_regex>` 实现了 JSON 风格的输出：

- 对于 `E_BSON_JSON_MODE_CANONICAL` / `E_BSON_JSON_MODE_RELAXED`：
  ```json
  { "$regularExpression" : { "pattern" : "...", "options" : "..." } }
  ```
- 对于其他（legacy）模式：
  ```json
  { "$regex" : "...", "$options" : "..." }
  ```

输出会对 pattern 做 JSON 转义（使用 `json_escape_string`）。

## 依赖项

- `<yggr/utility/copy_or_move_or_swap.hpp>`
- `<yggr/utility/member_var_modify_helper.hpp>`
- `<yggr/charset/utf8_string.hpp>`
- `<yggr/nsql_database_system/c_bson_string_item.hpp>`
- `<yggr/nsql_database_system/bson_native_ex.hpp>`
- `<yggr/nsql_database_system/bson_value_native_ex.hpp>`
- `<yggr/nsql_database_system/json_escape_string.hpp>`
- `<yggr/nsql_database_system/bson_visit_t.hpp>`
- Boost `enable_if` / MPL / type traits

## 使用示例

```cpp
#include <yggr/nsql_database_system/c_bson_regex.hpp>

using yggr::nsql_database_system::c_bson_regex;

c_bson_regex r1("^abc.*", "i"); // pattern ^abc.* with case-insensitive option

// 修改 options
r1.options() = "im"; // sets options to sorted/normalized form

// 保存到 BSON
bson_t b; bson_init(&b);
r1.save(b, "regex_field");

// 输出 JSON
std::cout << r1 << std::endl;
```

## 设计考虑

- 通过继承 `c_bson_string_item` 重用字符串处理逻辑
- 规范化选项字符串以确保比较的一致性
- 支持广泛的字符串输入类型并保证 UTF-8 表示
- 将实际 BSON 操作委托给底层扩展函数，保持接口清晰

## 保存路径

`e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\c_bson_regex.md`
