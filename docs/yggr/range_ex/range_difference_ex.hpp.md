# range_difference_ex.hpp - 中文文档

**文件位置**: [yggr/range_ex/range_difference_ex.hpp](yggr/range_ex/range_difference_ex.hpp#L1)

## 概述

该头文件定义了模板类型 `yggr::range_ex::range_difference_ex<T>`，用于在编译期为类型 `T` 提供“Range 差值类型”标签：

- 如果 `T` 满足 Boost.Range 风格（具有 `range_iterator` 或 `range_const_iterator`），则 `range_difference_ex<T>` 继承自 `boost::range_difference<T>`，暴露对应的差值类型；
- 否则，`range_difference_ex<T>` 继承自 `mplex::null_type`，表示该类型不是一个 Range 或没有可用的差值类型。

该类型可用于模板元编程中判断或选择范围差值类型的逻辑。

## 主要实现要点

- 文件包含：
  - `yggr/base/yggrdef.h`
  - `yggr/mplex/null_t.hpp`（提供 `mplex::null_type`）
  - Boost MPL 的 `boost/mpl/or.hpp`
  - Boost.Range 的 `boost/range/difference_type.hpp` 和 `boost/range/has_range_iterator.hpp`

- 命名空间：`yggr::range_ex`。

- 关键实现：在 `detail` 命名空间中定义辅助模板 `range_difference_ex_impl<has_iter, T>`：
  - 当 `has_iter == true` 时，`range_difference_ex_impl<true, T>` 继承自 `boost::range_difference<T>`；
  - 当 `has_iter == false` 时，`range_difference_ex_impl<false, T>` 继承自 `mplex::null_type`；

- `range_difference_ex<T>` 使用 `boost::has_range_iterator<T>` 或 `boost::has_range_const_iterator<T>` 的逻辑或结果作为 `has_iter` 参数，选择适当的特化。

## 使用示例

以下示例说明如何在编译期检测类型是否具有有效的 Range 差值类型：

```cpp
#include <type_traits>
#include <vector>
#include <iostream>
#include "yggr/range_ex/range_difference_ex.hpp"
#include "yggr/mplex/null_t.hpp"

int main() {
    using yggr::range_ex::range_difference_ex;
    using mplex::null_type;

    using VecDiff = range_difference_ex<std::vector<int>>;
    constexpr bool vec_has_diff = !std::is_base_of<null_type, VecDiff>::value;

    using IntDiff = range_difference_ex<int>;
    constexpr bool int_has_diff = !std::is_base_of<null_type, IntDiff>::value;

    std::cout << "vector has difference type: " << vec_has_diff << "\n";
    std::cout << "int has difference type: " << int_has_diff << "\n";
}
```

在此示例中，`vec_has_diff` 应为 `true`，而 `int_has_diff` 应为 `false`。

## 何时使用

- 在模板代码中需要区分“符合 Boost.Range 接口且可获取差值类型”的类型与“非 Range 类型”时使用。
- 可用于 SFINAE、类型萃取或基于 Range 差值类型选择实现分支。

## 实现注意事项

- `range_difference_ex<T>` 并不是一个简单的别名，而是一个继承自 `boost::range_difference<T>` 或 `mplex::null_type` 的类型。通常通过 `std::is_base_of`、`std::is_same` 等类型 trait 来判断或分发行为。
- 该实现依赖 Boost.Range 的 `has_range_iterator` / `has_range_const_iterator` trait，因此其可用性与 Boost.Range 的检测规则一致。

## 参考

- 源文件： [yggr/range_ex/range_difference_ex.hpp](yggr/range_ex/range_difference_ex.hpp#L1)

---

文档已生成。若需更详细的示例或将此文档扩展到相关 Range 工具，请告诉我。