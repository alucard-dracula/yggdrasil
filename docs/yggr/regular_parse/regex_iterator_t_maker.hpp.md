# regex_iterator_t_maker.hpp - 中文文档

**文件位置**: [yggr/regular_parse/regex_iterator_t_maker.hpp](yggr/regular_parse/regex_iterator_t_maker.hpp#L1)

## 概述

该头文件定义了一组模板辅助类型，用于根据输入迭代器和正则表达式类型推导适当的 Boost.Regex 迭代器类型。

主要类型包括：

- `yggr::regular_parse::regex_iterator_t_maker<InputIter, Regex>`
- `yggr::regular_parse::regex_token_iterator_t_maker<InputIter, Regex>`

这些模板帮助将编码适配器迭代器与 Boost.Regex 迭代器结合起来，自动选择正确的底层迭代器类型。

## 主要实现

### 公共逻辑

`regex_iterator_t_maker` 和 `regex_token_iterator_t_maker` 都包含以下类型定义：

- `input_iterator_type`：输入迭代器类型 `InputIter`
- `base_iterator_type`：通过 `detail::iterator_helper<InputIter>::org_iter_type` 提取的底层基础迭代器
- `value_type`：通过 `detail::iterator_helper<InputIter>::value_type` 提取的元素类型
- `regex_type`：模板参数 `Regex`
- `re_value_type`：通过 `regex_traits<Regex>::value_type` 提取的正则表达式字符类型
- `re_traits_type`：通过 `regex_traits<Regex>::traits_type` 提取的正则表达式 traits 类型

### 类型转换适配

对于非 ICU 或普通 Regex 类型：

- 通过 `detail::conv_iterator_helper<base_iterator_type, re_value_type, value_type>::type` 推导出用于正则迭代的 `iterator_type`
- `regex_iterator_type` 为 `boost::regex_iterator<iterator_type, re_value_type, re_traits_type>`
- `regex_token_iterator_type` 为 `boost::regex_token_iterator<iterator_type, re_value_type, re_traits_type>`

### BOOST_HAS_ICU 特化

当启用 `BOOST_HAS_ICU` 时，还会提供 `boost::u32regex` 专用特化：

- `regex_iterator_t_maker<InputIter, boost::u32regex>`
- `regex_token_iterator_t_maker<InputIter, boost::u32regex>`

在此特化中：

- `iterator_type` 直接使用 `base_iterator_type`
- `regex_iterator_type` 为 `boost::u32regex_iterator<iterator_type>`
- `regex_token_iterator_type` 为 `boost::u32regex_token_iterator<iterator_type>`

## 设计要点

- 该头文件主要用于在泛型正则匹配代码中统一不同输入迭代器与不同正则表达式类型。
- `detail::iterator_helper` 负责提取原始迭代器和元素类型，支持各种编码适配器。
- `detail::conv_iterator_helper` 负责将基础迭代器转换为与正则表达式字符类型兼容的迭代器。

## 典型用途

在正则解析模块中，可以这样使用：

```cpp
using regex_iter_t = yggr::regular_parse::regex_iterator_t_maker<InputIter, MyRegex>::type;
using token_iter_t = yggr::regular_parse::regex_token_iterator_t_maker<InputIter, MyRegex>::type;
```

这样可让 `InputIter` 既能是普通迭代器，也能是 UTF-8/UTF-16 等编码适配器迭代器。

## 注意事项

- `regex_iterator_t_maker` 和 `regex_token_iterator_t_maker` 仅负责类型推导，不创建实际迭代器对象。
- 对于 `boost::u32regex`，需要启用 `BOOST_HAS_ICU` 才能使用专用特化。
- 成功推导 `iterator_type` 依赖 `detail::conv_iterator_helper` 支持 `base_iterator_type`、`re_value_type` 和 `value_type` 之间的转换。

## 参考

- 源文件： [yggr/regular_parse/regex_iterator_t_maker.hpp](yggr/regular_parse/regex_iterator_t_maker.hpp#L1)

---

文档已生成并保存为 `regex_iterator_t_maker.hpp.md`。