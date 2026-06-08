# unicode_iterator_ex.hpp - 中文文档

**文件位置**: [yggr/regular_parse/detail/unicode_iterator_ex.hpp](yggr/regular_parse/detail/unicode_iterator_ex.hpp#L1)

## 概述

该头文件定义了两个 Unicode 迭代器转换适配器：

- `yggr::regular_parse::detail::u16_to_u8_iterator<BaseIterator, U8Type>`
- `yggr::regular_parse::detail::u8_to_u16_iterator<BaseIterator, U16Type>`

它们分别用于将 UTF-16 迭代器转换为 UTF-8 迭代器，以及将 UTF-8 迭代器转换为 UTF-16 迭代器。

此外，还提供 `yggr::regular_parse::extract_to_base_iterator` 系列辅助模板和函数，用于从各种 Unicode 输出迭代器中提取底层基础迭代器。

## 主要类型说明

### `u16_to_u8_iterator<BaseIterator, U8Type>`

- 继承自 `boost::iterator_facade`，迭代器类型为 `U8Type`。
- 适用于源迭代器元素为 UTF-16 单元类型的情况。
- 支持双向迭代。
- 内部维护 `m_values[5]` 缓冲区，用于存储当前编码单元的 UTF-8 字节序列。
- 自动处理 UTF-16 surrogate pair（代理对）并在遇到非法 surrogate 时抛出异常。

主要行为：

- `dereference()`：返回当前 UTF-8 字节；
- `increment()`：推进到下一个 UTF-8 字节，必要时读取下一个 UTF-16 code unit；
- `decrement()`：向后移动一个 UTF-8 字节，并正确处理 surrogate pair 后退；
- `base()`：返回当前底层 UTF-16 迭代器位置。

### `u8_to_u16_iterator<BaseIterator, U16Type>`

- 继承自 `boost::iterator_facade`，迭代器类型为 `U16Type`。
- 适用于源迭代器元素为 UTF-8 单元类型的情况。
- 支持双向迭代。
- 内部维护 `m_values[3]` 缓冲区，用于存储当前 UTF-16 code unit 或 surrogate pair。
- 自动解析 UTF-8 字节序列，将其转换为 UTF-16 单元，遇到非法 UTF-8 序列时抛出异常。

主要行为：

- `dereference()`：返回当前 UTF-16 code unit；
- `increment()`：推进到下一个 UTF-16 单元，必要时读取多个 UTF-8 字节；
- `decrement()`：向后回退到前一个 UTF-8 code point 起始位置，并验证 UTF-8 序列合法性；
- `base()`：返回当前底层 UTF-8 迭代器位置。

## 构造函数和合法性检查

两个迭代器类都提供：

- 默认构造函数。
- 直接以单个底层迭代器构造。
- 以底层迭代器和 `[start, end)` 范围构造，并在构造时进行边界和编码合法性检查。

`u16_to_u8_iterator` 会检查范围起点和终点是否包含非法 surrogate；
`u8_to_u16_iterator` 会检查 UTF-8 起始字节、续字节数量以及结尾是否形成完整序列。

## 辅助类型 `extract_to_base_iterator_t` 和函数

该文件还定义了一组辅助模板 `extract_to_base_iterator_t<Iter>`，用于提取不同 Unicode 输出迭代器的底层基础迭代器类型：

- `boost::utf8_output_iterator<Iter>`
- `boost::utf16_output_iterator<Iter>`
- `boost::u32_to_u16_iterator<Iter, T>`
- `boost::u16_to_u32_iterator<Iter, T>`
- `boost::u32_to_u8_iterator<Iter, T>`
- `boost::u8_to_u32_iterator<Iter, T>`
- `u16_to_u8_iterator<Iter, T>`
- `u8_to_u16_iterator<Iter, T>`

对应的函数 `extract_to_base_iterator(...)` 会返回这些适配器的 `.base()`，以获取原始底层迭代器。

## 典型应用场景

- 将 UTF-16 数据视为 UTF-8 字节序列进行遍历和编码转换。
- 将 UTF-8 数据视为 UTF-16 单元序列进行处理。
- 在正则表达式或文本解析模块中统一不同 Unicode 迭代器类型。
- 从复杂编码适配器中提取原始迭代器继续处理底层容器。

## 注意事项

- 这两个转换迭代器均为双向迭代器。
- 在遇到非法 UTF-8 或 UTF-16 surrogate 时，会通过 `boost::throw_exception` 抛出 `std::out_of_range` 异常。
- `u16_to_u8_iterator` 依赖 `boost::detail::is_surrogate`、`is_low_surrogate`、`is_high_surrogate` 等 Unicode 解析辅助函数。
- `u8_to_u16_iterator` 依赖 `boost::detail::utf8_trailing_byte_count`、`utf8_byte_count` 等 UTF-8 解析辅助函数。

## 参考

- 源文件： [yggr/regular_parse/detail/unicode_iterator_ex.hpp](yggr/regular_parse/detail/unicode_iterator_ex.hpp#L1)

---

文档已生成并保存为 `unicode_iterator_ex.hpp.md`。