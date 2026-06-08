# regex_traits.hpp - 中文文档

**文件位置**: [yggr/regular_parse/regex_traits.hpp](yggr/regular_parse/regex_traits.hpp)

## 概述

`yggr::regular_parse::regex_traits` 是一个模板元类型，用于检测和提取 Boost.Regex 正则对象的类型信息。

它定义了一个默认模板和一个针对 `boost::basic_regex<Char, Traits>` 的特化，用于：

- 判定类型是否为 Boost.Regex 正则类型
- 提取正则对象的内嵌类型别名

## 主要模板

### 默认模板 `regex_traits<T>`

默认版本继承自 `boost::mpl::false_`，表示 `T` 不是合法的 Boost.Regex 正则类型。

定义的成员类型全部为 `mplex::null_type`：

- `regex_type`
- `value_type`
- `traits_type`
- `const_iterator`
- `iterator`
- `reference`
- `const_reference`
- `difference_type`
- `size_type`
- `flag_type`
- `locale_type`

这使得在泛型编程中可以安全地对非正则类型进行类型选择和 SFINAE 约束。

### 特化模板 `regex_traits< boost::basic_regex<Char, Traits> >`

对于 Boost.Regex 的 `boost::basic_regex<Char, Traits>` 类型，该特化继承自 `boost::mpl::true_`，表示类型有效。

它将内部类型映射为正则对象本身的成员类型：

- `regex_type`：`boost::basic_regex<Char, Traits>`
- `value_type`：正则字符类型
- `traits_type`：正则特性类型
- `const_iterator`：正则对象常量迭代器
- `iterator`：正则对象迭代器
- `reference`：正则对象引用类型
- `const_reference`：正则对象常量引用类型
- `difference_type`：差值类型
- `size_type`：大小类型
- `flag_type`：标志类型
- `locale_type`：本地化类型

## 用途

`regex_traits` 常用于模板元编程场景中：

- `boost::mpl::true_` / `false_` 条件判断
- 提取正则对象的类型信息以便统一接口调用
- 在正则解析库内部对 `boost::basic_regex` 类型进行类型选择

## 典型示例

```cpp
using namespace yggr::regular_parse;

static_assert(regex_traits<int>::value == false, "int 不是正则类型");
static_assert(regex_traits<boost::regex>::value == true, "boost::regex 是正则类型");

using traits = regex_traits<boost::regex>;
using regex_type = traits::regex_type;
using char_type = traits::value_type;
```

## 设计要点

- 通过默认模板和特化实现类型检测与类型提取的分离。
- 使用 `mplex::null_type` 作为无效类型占位符，兼容泛型类型萃取。
- 依赖 Boost.Regex 提供的 `boost::basic_regex` 内部类型定义。

---

文档已生成为 `regex_traits.hpp.md`。