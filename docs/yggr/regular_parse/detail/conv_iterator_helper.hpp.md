# conv_iterator_helper.hpp - 中文文档

**文件位置**: [yggr/regular_parse/detail/conv_iterator_helper.hpp](yggr/regular_parse/detail/conv_iterator_helper.hpp#L1)

## 概述

该头文件定义了模板工具 `yggr::regular_parse::detail::conv_iterator_helper`，用于在不同字节宽度的编码迭代器之间进行转换。它通过对源类型和目标类型的字节大小进行组合匹配，选择合适的转换迭代器类型。

此工具常用于 Unicode 或多字节字符编码处理场景，例如将 UTF-8、UTF-16、UTF-32 迭代器互相转换。

## 主要类型

- `conv_iterator_helper_detail<SrcIter, Dst, Src, dst_size, src_size>`
  - 通用模板：默认定义 `type = mplex::null_type`，表示不存在有效的转换。
  - 特化模板根据 `dst_size` 与 `src_size` 选择合适的转换迭代器：
    - `byte_size == byte_size`：直接返回原始迭代器类型 `SrcIter`
    - `2 -> 1`：`u8_to_u16_iterator`
    - `1 -> 2`：`u16_to_u8_iterator`
    - `4 -> 1`：`u8_to_u32_iterator`
    - `1 -> 4`：`u32_to_u8_iterator`
    - `4 -> 2`：`u16_to_u32_iterator`
    - `2 -> 4`：`u32_to_u16_iterator`

- `conv_iterator_helper<SrcIter, Dst, Src>`
  - 继承自 `conv_iterator_helper_detail<SrcIter, Dst, Src>`。
  - 在 `yggr::regular_parse` 命名空间中导出为 `detail::conv_iterator_helper`。

## 设计要点

- 依赖 `iterator_helper` 的 `value_type` 来验证源迭代器元素类型是否与 `Src` 类型大小一致。
- 使用 `BOOST_MPL_ASSERT` 在编译期断言 `sizeof(chk_value_type) == sizeof(Src)`，确保转换安全。
- 每个特化定义：
  - `type`：转换后的迭代器类型；
  - `result_type`：由 `(begin, end)` 两个迭代器组成的 `std::pair<type, type>`。
- `operator()` 接受源迭代器 `i` 和 `j`，返回一对转换后的迭代器对象。

## 示例用途

假设有一个指向 UTF-8 数据的迭代器 `src_iter_type`，希望将其转换为 UTF-16 反向范围：

```cpp
using Helper = yggr::regular_parse::detail::conv_iterator_helper<src_iter_type, wchar_t, char>;
auto range = Helper()(begin, end);
// range.first / range.second 为转换后的 UTF-16 迭代器
```

对于 `SrcIter` 已经与 `Src` 类型字节宽一致的情况，`conv_iterator_helper` 会直接返回原始迭代器对。

## 典型转换关系

- `Dst` 与 `Src` 字节宽相同：无转换
- `2 -> 1`：UTF-8 -> UTF-16
- `1 -> 2`：UTF-16 -> UTF-8
- `4 -> 1`：UTF-8 -> UTF-32
- `1 -> 4`：UTF-32 -> UTF-8
- `4 -> 2`：UTF-16 -> UTF-32
- `2 -> 4`：UTF-32 -> UTF-16

## 注意事项

- 该工具仅支持 Boost.Range 风格迭代器，并且源迭代器元素类型大小必须与 `Src` 大小一致。
- 不支持的字节宽组合会退化为 `mplex::null_type`。
- `conv_iterator_helper` 的具体类型选择基于模板参数中的 `Dst` 和 `Src` 类型本身，而不是遍历的元素数量。

## 参考

- 源文件： [yggr/regular_parse/detail/conv_iterator_helper.hpp](yggr/regular_parse/detail/conv_iterator_helper.hpp#L1)

---

文档已生成并保存为 `conv_iterator_helper.hpp.md`。