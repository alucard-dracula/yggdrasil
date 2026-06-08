# regex_parse.hpp - 中文文档

**文件位置**: [yggr/regular_parse/regex_parse.hpp](yggr/regular_parse/regex_parse.hpp#L1)

## 概述

`yggr::regular_parse::regex_parse` 是一个静态工具类，封装了 Boost.Regex 的常见正则表达式操作，包括：

- 正则表达式构造（`make_regex`）
- 匹配（`match`）
- 搜索（`search`）
- 替换（`replace`）
- 格式化（`format`）
- 统计/遍历（`grep`）
- 拆分（`split`）

它支持普通容器迭代器、C 风格字符串、泛型字符串类型，以及各种编码适配器迭代器（主要通过 `string_helper`、`iterator_helper`、`conv_iterator_helper` 等辅助模板完成）。

## 关键类型定义

- `regex_base_type`：`boost::regbase`
- `regex_flag_type`：`regex_base_type::flag_type`
- `match_flag_type`：`boost::regex_constants::match_flag_type`
- `error_type`：`boost::regex_error`

## 核心功能

### 1. `make_regex`

提供两个重载：

- 通过 `[begin, end)` 迭代器构造正则对象
- 通过字符串类型构造正则对象

它们分别对普通迭代器与泛型字符串类型做 SFINAE 约束，确保匹配不同字符类型和编码。

### 2. `match`

支持以下形式：

- `match(text_s, text_e, re)`
- `match(text_s, text_e, result, re)`
- `match(text, re)`
- `match(text, result, re)`

对于字符串类型，会自动通过 `string_helper` 提取 `begin`/`end`。

### 3. `search`

提供与 `match` 类似的重载集，用于查找文本中是否存在正则匹配。

### 4. `replace`

提供多种重载形式，包括：

- 输出迭代器形式
- 返回结果字符串
- 自定义分配器
- 目标字符串类型与替换格式类型皆为泛型字符串

`replace` 使用 `boost::regex_replace` 实现，并在内部通过 `YGGR_BOOST_REGEX_DETAIL_NS::string_out_iterator` 输出结果。

### 5. `format`

封装了 `boost::match_results::format`，提供：

- 输出迭代器版本
- 返回结果字符串版本
- 自定义结果字符串分配器版本
- 自定义返回字符串类型版本

### 6. `make_regex_iterator` / `make_regex_token_iterator`

用于构造正则迭代器和正则令牌迭代器，支持：

- 直接 `[begin, end)` 迭代器输入
- 泛型字符串输入
- 提供可选的 `submatch` 参数（整数、数组、容器）

### 7. `grep`

遍历 `boost::regex_iterator` 并调用用户提供的处理器（`Handler`），可选额外的谓词 `Pred` 进行条件终止。

### 8. `split`

使用正则令牌迭代器对文本拆分，并通过 `Handler` / `Pred` 回调处理每个子串。支持：

- 默认以空白分隔
- 自定义正则表达式拆分
- 对普通迭代器和泛型字符串均适用
- 支持 `submatch` 作为拆分子表达式索引

## 设计要点

- 使用 `boost::enable_if` 和 Boost.MPL 进行重载约束，避免不合适类型进入模板定义。
- 使用 `detail::string_helper` 支持泛型字符串类型和 `begin` / `end` 提取。
- 使用 `detail::iterator_helper` 支持输入迭代器类型的 `value_type`、`citer_type`、`regex_type`、`result_cont_type`。
- 对输出迭代器和正则替换格式类型进行严格分类，确保不混淆 UTF-8 与非 UTF-8 字符串类型。
- `replace` 和 `format` 的结果容器可通过 `Alloc` 自定义，确保与用户容器兼容。

## 典型使用示例

```cpp
using namespace yggr::regular_parse;

// 构造正则
auto re = regex_parse::make_regex(text, boost::regex::normal);

// 匹配
bool ok = regex_parse::match(text, re);

// 搜索
bool found = regex_parse::search(text, re);

// 替换
auto result = regex_parse::replace(text, re, format);

// 格式化
auto formatted = regex_parse::format(match_results, format);

// 拆分
auto count = regex_parse::split(handler, text);
```

## 注意事项

- 本类全部函数为静态函数，不需要实例化对象。
- 所有字符串相关模板均限制为非 UTF-8 字符串类型，UTF-8 支持通过专门的 UTF-8 适配器和 `string_helper` 实现。
- 需要使用 `try/catch` 捕获 `regex_parse::error_type` 及 `yggr::stl_exceptrion`。
- 通过 `BOOST_NO_WREGEX` 控制对宽字符正则支持的可用性。

## 参考

- 源文件： [yggr/regular_parse/regex_parse.hpp](yggr/regular_parse/regex_parse.hpp#L1)

---

文档已生成并保存为 `regex_parse.hpp.md`。