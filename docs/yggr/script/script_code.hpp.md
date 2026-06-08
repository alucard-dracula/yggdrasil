# script_code.hpp - 中文文档

**文件位置**: [yggr/script/script_code.hpp](yggr/script/script_code.hpp)

## 概述

`yggr::script::script_code` 是一个轻量级封装，用于统一表示脚本源码或脚本文件路径字符串。

它基于 `charset::utf8_string`，并通过 `file_code` 与 `string_code` 两个别名区分文件代码和文本代码。

## 结构说明

- `struct tag_file_code {}`
- `struct tag_string_code {}`

两者仅作为模板标签，用于构造不同含义的 `script_code<Tag>` 类型。

## 主要模板类 `script_code<Tag>`

### 类型别名

- `string_type`：`charset::utf8_string`

### 构造函数

支持多种构造方式：

- `template<typename String> script_code(BOOST_RV_REF(String) code)`
- `template<typename String> script_code(const String& code)`
- `script_code(BOOST_RV_REF(string_type) code)`
- `script_code(const string_type& code)`
- `script_code(BOOST_RV_REF(this_type) right)`
- `script_code(const this_type& right)`

这些构造函数允许：

- 从任意可转换为 `utf8_string` 的字符串对象构造
- 从 `utf8_string` 的右值或左值构造
- 拷贝构造与移动构造

### 成员函数

- `operator const string_type&() const`
  - 将 `script_code` 隐式转换为 `utf8_string` 引用

- `const string_type& code() const`
  - 显式获取内部字符串引用

### 内部实现细节

- `_str_code`：保存内部 `utf8_string` 数据
- `_pstr_code`：指向实际字符串数据的指针

构造函数会将 `_pstr_code` 设置为：

- 指向 `_str_code`（当对象内部持有数据时）
- 指向外部传入字符串对象的地址（当传入 `const string_type&` 时）

`is_inner_link()` 用于判断当前对象是否持有自己的内部字符串。

## 类型别名

- `typedef script_code<tag_file_code> file_code`
- `typedef script_code<tag_string_code> string_code`

这两个别名表示不同语义的脚本输入类型：

- `file_code`：通常用于表示脚本文件路径或文件内容
- `string_code`：通常用于表示脚本文字内容

## 设计要点

- 通过标签类型区分脚本文件/脚本字符串，便于强类型调用接口。
- 采用 `utf8_string` 作为底层存储，保证脚本内容支持 UTF-8。
- 使用 `_pstr_code` 指针实现对外部字符串的引用共享，避免不必要拷贝。
- 通过移动构造支持高性能字符串转移。

## 使用示例

```cpp
using namespace yggr::script;

string_code code(std::string("print('hello')"));
file_code file_path(std::string("script.py"));

// 传递到脚本执行接口
python_script script("main", code, global_env);
```

## 注意事项

- 当使用 `const string_type&` 构造时，`script_code` 并不复制外部字符串，而是保存其地址。
- `operator const string_type&()` 返回内部指针所指的字符串，调用前应确保对象仍然有效。

---

文档已生成：`script_code.hpp.md`。