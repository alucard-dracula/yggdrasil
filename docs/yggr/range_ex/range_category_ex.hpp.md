# range_category_ex.hpp - 中文文档

**文件位置**: [yggr/range_ex/range_category_ex.hpp](yggr/range_ex/range_category_ex.hpp#L1)

## 概述

该头文件定义了模板类型 `yggr::range_ex::range_category_ex<T>`，用于在编译期为类型 `T` 提供“Range 类别”标签：

- 如果 `T` 满足 Boost.Range 风格（具有 `range_iterator` 或 `range_const_iterator`），则 `range_category_ex<T>` 从 `boost::range_category<T>` 继承，从而暴露对应的 Range 类别标签；
- 否则，`range_category_ex<T>` 从 `mplex::null_type` 继承，表示该类型不是一个 Range（或不公开 Range 迭代器）。

该类型可用于模板元编程中判断或分发与 Range 类别相关的行为（例如选择适当的迭代器/算法重载）。

## 主要实现要点

- 文件包含：
  - `yggr/base/yggrdef.h`
  - `yggr/mplex/null_t.hpp`（提供 `mplex::null_type`）
  - Boost MPL 的 `boost/mpl/or.hpp`
  - Boost.Range 的 `boost/range/category.hpp` 和 `boost/range/has_range_iterator.hpp`

- 命名空间：`yggr::range_ex`。

- 关键实现：在 `detail` 命名空间中定义了辅助模板 `range_category_ex_impl<has_iter, T>`：
  - 当 `has_iter == true` 时，`range_category_ex_impl<true, T>` 继承自 `boost::range_category<T>`；
  - 当 `has_iter == false` 时，`range_category_ex_impl<false, T>` 继承自 `mplex::null_type`；

- 最终 `range_category_ex<T>` 以 `boost::has_range_iterator<T>` 或 `boost::has_range_const_iterator<T>` 的逻辑或结果作为 `has_iter`，选择相应的实现特化。

## 使用示例

以下示例展示如何在编译期检测类型是否被视为 Range：

```cpp
#include <type_traits>
#include <vector>
#include <iostream>
#include "yggr/range_ex/range_category_ex.hpp"
#include "yggr/mplex/null_t.hpp"

int main() {
    using yggr::range_ex::range_category_ex;
    using mplex::null_type;

    // 对于 std::vector<int>，range_category_ex 应继承自 boost::range_category<std::vector<int>>
    using CatVec = range_category_ex<std::vector<int>>;
    constexpr bool vec_is_range = !std::is_base_of<null_type, CatVec>::value;

    // 对于非 Range 类型，例如 int
    using CatInt = range_category_ex<int>;
    constexpr bool int_is_range = !std::is_base_of<null_type, CatInt>::value;

    std::cout << "vector is range: " << vec_is_range << "\n";
    std::cout << "int is range: " << int_is_range << "\n";
}
```

在上例中，`vec_is_range` 应为 `true`，而 `int_is_range` 应为 `false`。

## 何时使用

- 在实现需要区分“符合 Boost.Range 接口的类型”与“普通类型”的模板函数或类时使用。
- 可用于选择性继承、SFINAE 或 Boost.MPL 分支，以在编译期启用或禁用某些重载/实现。

## 实现注意事项

- `range_category_ex<T>` 并非直接返回一个 `type` 别名；它是一个类型，该类型会继承自 `boost::range_category<T>`（对 Range 类型）或 `mplex::null_type`（对非 Range）。因此在元编程中通常以 `std::is_base_of`、`std::is_same` 等 trait 来判断或分发。
- 该实现依赖 Boost.Range 提供的 trait（`has_range_iterator` / `has_range_const_iterator`），因此其判断逻辑与 Boost.Range 的检测规则一致。

## 参考

- 源文件： [yggr/range_ex/range_category_ex.hpp](yggr/range_ex/range_category_ex.hpp#L1)

---

文档由代码自动生成，若需更详细的示例或把 `range_category_ex` 集成到项目样板中，请告知要生成的用例类型或上下文。
