# string_helper.hpp - 中文文档

**文件位置**: [yggr/regular_parse/detail/string_helper.hpp](yggr/regular_parse/detail/string_helper.hpp#L1)

## 概述

该头文件定义了 `yggr::regular_parse::detail::string_helper<T>` 系列模板，用于统一处理不同字符串容器和字符指针的迭代器与字符串访问接口。

它提供对普通容器、C 风格字符串、UTF-8 字符串类型以及可修改 UTF-8 迭代器的统一访问方法，如 `begin`、`end`、`org`、`recount`、`c_str` 和 `create_empty_result_cont`。

## 主要组件

### 1. `string_helper_failed<T>`

默认失败实现，用于不支持的类型。

- `value_type`, `iter_type`, `citer_type`, `result_cont_type` 均为 `mplex::null_type`
- `begin`, `end`, `org`, `recount`, `c_str`, `create_empty_result_cont` 返回失败类型或空结果

### 2. `string_helper_rangeable<T, rangeable>`

对于类类型 `T`，此辅助模板用于判断是否可以按范围处理。

- 若 `T` 不是类类型，则继承自 `string_helper_failed<T>`。
- 若 `T` 是类类型，则定义：
  - `value_type`：通过 `range_ex::range_value_ex<cont_type>::type` 提取
  - `iter_type`：通过 `range_ex::range_iterator_ex<cont_type>::type` 提取
  - `citer_type`：通过 `range_ex::range_iterator_ex<const cont_type>::type` 提取
  - `result_cont_type`：`cont_type`
- 提供对容器的 `begin`, `end`, `org`, `recount`, `c_str`, `create_empty_result_cont` 的默认实现

### 3. `string_helper<T, is_pointer, is_outiter>`

主模板基于 `T` 是否为指针和是否为输出迭代器选择不同特化：

- 默认特化：继承 `string_helper_failed<T>`。
- `T` 为非指针且非输出迭代器：继承 `string_helper_rangeable<T>`。
- `T[N]` 数组特化：支持 C 风格定长数组，并在 `end()` 中处理以 N-1 位置为终止符的情况。
- 指针输出迭代器特化：`iter_type = value_type*`，`citer_type = const value_type*`，并提供指针版本的 `begin`, `end`, `org`, `recount`, `c_str`。
- `T, true, false` 特化：与 `T, true, true` 特化等价。

### 4. UTF-8 字符串特化

- `string_helper<charset::utf8_string_impl<Char, Traits, Alloc>, false, false>`
- `string_helper<charset::basic_utf8_string_view<Char, Traits>, false, false>`

这些特化直接访问底层 `org_str()` 对象，并提供：

- `begin` / `end`：转换到基础容器迭代器
- `org`：返回底层原始容器引用
- `recount`：调用 `recount_length()`
- `c_str`：返回 `data()` 指针
- `create_empty_result_cont`：根据原字符串的分配器创建空结果容器

## 使用场景

该工具可用于正则解析、字符串搜索、编码转换以及泛型文本处理场景，常见用途包括：

- 将不同字符串类型统一为同一接口
- 提取容器值类型和迭代器类型
- 为正则表达式匹配提供可访问的字符序列
- 在编译期区分 C 风格字符串、STL 容器和 UTF-8 专用类型

## 主要方法说明

- `begin(Cont&)`: 返回容器或字符串的起始迭代器
- `end(Cont&)`: 返回容器或字符串的结束迭代器
- `org(Cont&)`: 返回源容器的原始引用
- `recount(Cont&)`: 更新字符串长度或不执行任何操作
- `c_str(const Cont&)`: 返回底层字符数组指针
- `create_empty_result_cont(Cont&)`: 创建空的结果容器，用于输出或结果保存

## 注意事项

- `string_helper_rangeable<T>` 仅适用于类类型 `T`，它依赖于 `boost::is_class<T>` 判断。
- `string_helper<T, true, true>` 适用于指针类型输出迭代器，而 `string_helper<T, true, false>` 也复用相同实现。
- `string_helper<T[N], false, false>` 专用于长度为 `N` 的数组，并在 `end()` 中考虑终止符。
- UTF-8 字符串特化在 `value_type` 上使用 `YGGR_ASSERT_CHAR_TYPE(value_type)` 来验证字符类型。

## 参考

- 源文件： [yggr/regular_parse/detail/string_helper.hpp](yggr/regular_parse/detail/string_helper.hpp#L1)

---

文档已生成并保存为 `string_helper.hpp.md`。