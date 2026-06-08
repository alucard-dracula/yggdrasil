# iterator_helper.hpp - 中文文档

**文件位置**: [yggr/regular_parse/detail/iterator_helper.hpp](yggr/regular_parse/detail/iterator_helper.hpp#L1)

## 概述

该头文件定义了 `yggr::regular_parse::detail::iterator_helper<T>` 模板，用于统一处理不同类型的迭代器。它通过 `iterator_id` 对迭代器进行分类，并为每种迭代器类型提供：

- 原始迭代器类型 `org_iter_type`
- 迭代器值类型 `value_type`
- 结果容器类型 `result_cont_type`
- 正则表达式类型 `regex_type`
- 迭代器构造/转换函数

此工具主要用于正则解析和 Unicode 字符串迭代器之间的兼容层。

## 迭代器分类规则

`iterator_helper<T, iterator_id>` 的默认 `iterator_id` 根据以下条件计算：

- `is_iterator<T>::value`：是否为普通迭代器
- `charset::is_any_utf8_string_iterator_t<T>::value`：是否为 UTF-8 字符串正向迭代器
- `charset::is_any_utf8_string_reverse_iterator_t<T>::value`：是否为 UTF-8 字符串反向迭代器

这些标志按位组合，形成一个 `std::size_t` 值，用于选择对应的特化。

## 主要特化

### 默认模板

当 `iterator_id` 不匹配任何特化时：

- `id_type = boost::mpl::size_t<0>`
- `value_type = mplex::null_type`
- `result_cont_type = mplex::null_type`
- `regex_type = mplex::null_type`

### `iterator_id == 1`

表示普通迭代器：

- `id_type = boost::mpl::size_t<1>`
- `value_type` 为去除 cv 限定的迭代器值类型
- `result_cont_type` 为 `charset::basic_string<value_type>`
- `regex_type` 为 `boost::basic_regex<value_type>`

同时提供 `org()` 和 `make_iterator()` 静态方法，直接返回原始迭代器。

### `iterator_id == 3`

表示 UTF-8 字符串迭代器：

- `id_type = boost::mpl::size_t<3>`
- `value_type` 为去除 cv 限定的迭代器值类型
- `result_cont_type` 为 `charset::utf8_string_impl<value_type>`
- `regex_type` 在 `BOOST_HAS_ICU` 启用时为 `boost::u32regex`，否则为 `mplex::null_type`

该特化提供多种 `make_iterator()` 重载，用于在原始迭代器和 UTF-8 迭代器之间转换。

### `charset::utf8_string_modify_iterator<Char, Basic_Iterator>` 专用特化

该特化专门处理 UTF-8 可修改迭代器：

- 同样使用 `id_type = boost::mpl::size_t<3>`
- 提供 `org()` 和多种 `make_iterator()` 重载
- 支持通过源迭代器、目标迭代器或 UTF-8 字符串构造修改迭代器

## 典型用途

`iterator_helper` 主要用于以下场景：

- 将不同类型的迭代器统一适配到正则解析模块
- 在 UTF-8 字符串迭代器与普通迭代器之间构造或恢复原始迭代器
- 推断迭代器对应的字符类型、结果字符串类型和正则表达式类型

## 注意事项

- 该工具依赖 `yggr::iterator_ex::is_iterator`、`charset::is_any_utf8_string_iterator_t` 和 `charset::is_any_utf8_string_reverse_iterator_t` 的类型检测。
- 对于普通迭代器和 UTF-8 字符串迭代器，`value_type` 会去除 cv 限定。
- 当未匹配任何支持的迭代器类型时，`iterator_helper` 的默认特化会将所有类型设为 `mplex::null_type`。

## 参考

- 源文件： [yggr/regular_parse/detail/iterator_helper.hpp](yggr/regular_parse/detail/iterator_helper.hpp#L1)

---

文档已生成并保存为 `iterator_helper.hpp.md`。