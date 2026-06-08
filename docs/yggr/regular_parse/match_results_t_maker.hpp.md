# match_results_t_maker.hpp - 中文文档

**文件位置**: [yggr/regular_parse/match_results_t_maker.hpp](yggr/regular_parse/match_results_t_maker.hpp#L1)

## 概述

该头文件定义了两个模板辅助类型：

- `yggr::regular_parse::match_results_t_maker<InputIter>`
- `yggr::regular_parse::sub_match_t_maker<InputIter>`

它们根据输入迭代器类型推导出对应的 Boost.Regex 结果类型。主要用于将正则匹配结果类型与不同输入迭代器类型统一起来。

## 主要实现

- `match_results_t_maker<InputIter>`
  - `input_iterator_type`：原始输入迭代器类型
  - `base_iterator_type`：通过 `detail::iterator_helper<InputIter>::org_iter_type` 提取的底层迭代器类型
  - `type`：对应的 `boost::match_results<base_iterator_type>` 类型

- `sub_match_t_maker<InputIter>`
  - `input_iterator_type`：原始输入迭代器类型
  - `base_iterator_type`：通过 `detail::iterator_helper<InputIter>::org_iter_type` 提取的底层迭代器类型
  - `type`：对应的 `boost::sub_match<base_iterator_type>` 类型

## 设计要点

- 通过 `detail::iterator_helper` 处理输入迭代器类型，支持普通迭代器、UTF-8/UTF-16 迭代器及相关适配器。
- `match_results_t_maker` 和 `sub_match_t_maker` 只负责类型萃取，而不执行实际匹配或迭代操作。
- 该头文件为模板代码提供了统一的正则匹配结果类型定义，避免手动重复编写底层基础迭代器类型。

## 典型用途

在泛型正则解析代码中，如果输入迭代器可能是不同编码适配器类型，则可以使用这些辅助模板：

```cpp
using match_results_type = yggr::regular_parse::match_results_t_maker<InputIter>::type;
using sub_match_type = yggr::regular_parse::sub_match_t_maker<InputIter>::type;
```

这使得正则匹配结果对 `InputIter` 的适配更加透明。

## 注意事项

- `InputIter` 不必是底层基础迭代器；它可以是封装了编码适配逻辑的迭代器类型。
- 成功推导 `base_iterator_type` 依赖 `detail::iterator_helper<InputIter>::org_iter_type` 的正确特化。
- 如果 `InputIter` 不是支持的迭代器类型，则 `detail::iterator_helper` 可能生成 `mplex::null_type`，从而导致类型推导失败或编译错误。

## 参考

- 源文件： [yggr/regular_parse/match_results_t_maker.hpp](yggr/regular_parse/match_results_t_maker.hpp#L1)

---

文档已生成并保存为 `match_results_t_maker.hpp.md`。