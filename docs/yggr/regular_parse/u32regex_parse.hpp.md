# u32regex_parse.hpp - 中文文档

**文件位置**: [yggr/regular_parse/u32regex_parse.hpp](yggr/regular_parse/u32regex_parse.hpp)

## 概述

`yggr::regular_parse::u32regex_parse` 提供用于 UTF-32 正则表达式的静态包装工具函数，基于 Boost.Regex ICU 扩展实现。

本类支持：

- UTF-32 正则表达式构造
- 正则匹配 (`match`)
- 搜索 (`search`)
- 替换 (`replace`)
- 格式化 (`format`)
- 正则迭代器构造 (`make_regex_iterator` / `make_regex_token_iterator`)
- 扫描遍历 (`grep`)
- 按模式拆分 (`split`)

它广泛使用 Boost.MPL、类型萃取和辅助 `detail` 模板，以支持通用容器、字符串类型及迭代器适配。

## 核心类型

- `regex_type`：`boost::u32regex`
- `regex_base_type`：`boost::regbase`
- `regex_flag_type`：`regex_type::flag_type`
- `match_flag_type`：`boost::regex_constants::match_flag_type`
- `error_type`：`boost::regex_error`

## 主要功能

### 1. `make_regex`

提供两个重载：

- `make_regex(Iter s, Iter e, regex_flag_type re_flag)`
  - 接受双向迭代器范围
  - 利用 `detail::iterator_helper` 获取原始迭代器
  - 调用 `boost::make_u32regex`

- `make_regex(const StrRegex& str, regex_flag_type re_flag)`
  - 接受泛型字符串类型
  - 使用 `detail::string_helper` 提取 `begin` / `end`

### 2. `match`

支持以下形式：

- `match(BiIter text_s, BiIter text_e, const regex_type& re, match_flag_type)`
- `match(BiIter text_s, BiIter text_e, boost::match_results<...>& m, const regex_type& re, match_flag_type)`
- `match(const Target& text, const regex_type& re, match_flag_type)`
- `match(const Target& text, boost::match_results<...>& m, const regex_type& re, match_flag_type)`

其中字符串版本会转换为区间版本，统一调用 `boost::u32regex_match`。

### 3. `search`

结构与 `match` 相似，但调用 `boost::u32regex_search`。

### 4. `replace`

提供多个重载，支持：

- 直接输出到任意输出迭代器
- 返回 `charset::utf8_string`
- 使用自定义分配器生成结果字符串
- 返回任意容器类型字符串
- 输入文本既可以是迭代器范围，也可以是泛型字符串

内部通过 `u32regex_replace_ex` 实现，利用 `boost::YGGR_BOOST_REGEX_DETAIL_NS::do_regex_replace` 和 UTF-32 转换适配器。

### 5. `format`

将匹配结果格式化为输出迭代器或字符串，支持：

- `format(OutIter out_iter, const match_results&, const Formatter&, match_flag_type)`
- `format(const match_results&, const Formatter&, match_flag_type)` 返回 `utf8_string`
- 自定义结果分配器版本
- 任意容器字符串结果版本

实现细节包括：

- `u32regex_format_ex` 和 `u32regex_format_ex_detail`
- 格式字符串类型与输出迭代器类型的兼容性检查
- 使用 `detail::conv_iterator_helper` 进行 UTF-32 与目标字符类型转换

### 6. `make_regex_iterator`

构造 `boost::u32regex_iterator`，支持：

- 迭代器范围输入
- 泛型字符串输入

通过 `regex_iterator_t_maker` 计算返回类型。

### 7. `grep`

遍历正则迭代器并调用用户处理器，支持：

- `grep(handler, regex_iter)`
- `grep(handler, pred, regex_iter)`
- `grep(handler, text_s, text_e, re, mth_flag)`
- `grep(handler, text, re, mth_flag)`
- `grep(handler, pred, text, re, mth_flag)`

返回成功处理的匹配个数。

### 8. `make_regex_token_iterator`

构造 `boost::u32regex_token_iterator`，支持：

- 迭代器范围或泛型字符串输入
- `submatch` 参数为整数、数组、容器、自定义可转换类型
- `std::vector<int>` 子匹配集合

### 9. `split`

使用正则令牌迭代器拆分文本，基于默认空白正则或用户提供的正则表达式：

- `split(handler, token_iter)`
- `split(handler, pred, token_iter)`
- `split(handler, text_s, text_e)`
- `split(handler, text)`
- `split(handler, text_s, text_e, re, mth_flag)`
- `split(handler, text, re, mth_flag)`
- `split(handler, text_s, text_e, re, submatch, mth_flag)`
- `split(handler, text, re, submatch, mth_flag)`
- 带 predicate 的版本

默认分隔符为 `\s+`。

## 设计亮点

- 仅当 `BOOST_HAS_ICU` 可用时启用整个头文件内容。
- 通过 `detail::iterator_helper` / `detail::string_helper` 实现泛型迭代器/字符串统一适配。
- 使用 `boost::enable_if`、`boost::mpl` 条件组合保障模板重载安全。
- 对输出迭代器、格式字符串和目标字符串进行严格类型检查。
- `replace` 和 `format` 始终返回 UTF-8 结果或可定制容器结果。
- 通过 `detail::int_t_ptr_maker` 解决 Boost.Regex 内部 UTF-32 输出迭代器类型推导问题。

## 使用建议

- 调用前使用 `try/catch` 捕获 `u32regex_parse::error_type`。
- 对通用字符串输入优先使用 `make_regex` / `match` / `search` 等重载。
- `replace` 与 `format` 的输出可以直接接受 `utf8_string`、标准容器或自定义分配器。
- 若需要精确控制子串拆分，可使用 `make_regex_token_iterator` 提供的 `submatch` 参数。

## 典型示例

```cpp
using namespace yggr::regular_parse;

boost::u32regex re = u32regex_parse::make_regex(U"\d+", boost::regex::normal);

bool matched = u32regex_parse::match(U"12345", re);

bool found = u32regex_parse::search(U"abc123", re);

charset::utf8_string replaced = u32regex_parse::replace(U"abc123", re, U"<num>");

auto it = u32regex_parse::make_regex_iterator(U"abc123", re);

size_t count = u32regex_parse::grep([](const auto& m){ return 1; }, U"abc 123 def", re);

size_t split_count = u32regex_parse::split([](const auto& token){ return 1; }, U"a b c");
```

---

文档已生成：`u32regex_parse.hpp.md`。