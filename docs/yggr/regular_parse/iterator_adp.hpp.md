# iterator_adp.hpp - 中文文档

**文件位置**: [yggr/regular_parse/iterator_adp.hpp](yggr/regular_parse/iterator_adp.hpp#L1)

## 概述

该头文件提供了 `yggr::regular_parse` 命名空间下的 `unwrap_iterator` 辅助函数模板，用于从各种 Unicode 适配器迭代器中提取底层基础迭代器。

其主要用途是将封装了编码转换功能的迭代器还原为原始迭代器，以便在需要访问底层序列或继续使用原始迭代器类型的场景中使用。

## 支持的迭代器类型

`iterator_adp.hpp` 支持如下适配器类型的解包：

- `boost::u32_to_u16_iterator<BaseIter, T>`
- `boost::u16_to_u32_iterator<BaseIter, T>`
- `boost::u32_to_u8_iterator<BaseIter, T>`
- `boost::u8_to_u32_iterator<BaseIter, T>`
- `detail::u16_to_u8_iterator<BaseIter, T>`
- `detail::u8_to_u16_iterator<BaseIter, T>`
- `boost::utf16_output_iterator<BaseIter>`
- `boost::utf8_output_iterator<BaseIter>`

另外，对于普通迭代器类型，`unwrap_iterator` 直接返回输入迭代器本身。

## 实现要点

- 函数模板 `unwrap_iterator(Iter i)`：默认实现直接返回传入迭代器。
- 对于每种适配器类型，定义特化函数模板，调用适配器的 `base()` 成员并返回底层迭代器。
- 对于 `detail::u16_to_u8_iterator` 和 `detail::u8_to_u16_iterator`，这两个类型来自 `yggr::regular_parse::detail`，用于 UTF-16 与 UTF-8 迭代器转换。
- 对于 Boost 提供的 UTF 输出迭代器（`utf16_output_iterator`、`utf8_output_iterator`），同样通过 `base()` 提取底层迭代器。

## 典型用途

- 在 Unicode 编码转换后的迭代器上执行操作后，恢复到原始基础迭代器。
- 在泛型算法中统一处理编码适配器和基础迭代器。
- 与正则解析、文本处理模块配合，避免直接依赖特定编码适配器类型。

## 注意事项

- `unwrap_iterator` 仅针对已知的适配器类型提供特化实现；对其他自定义适配器类型需要额外扩展。
- 所有适配器特化都依赖其 `base()` 方法返回底层迭代器。
- 默认模板适用于非适配器迭代器，不会做任何转换。

## 参考

- 源文件： [yggr/regular_parse/iterator_adp.hpp](yggr/regular_parse/iterator_adp.hpp#L1)

---

文档已生成并保存为 `iterator_adp.hpp.md`。