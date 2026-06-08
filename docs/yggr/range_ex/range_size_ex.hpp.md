# range_size_ex.hpp - 中文文档

**文件位置**: [yggr/range_ex/range_size_ex.hpp](yggr/range_ex/range_size_ex.hpp#L1)

## 概述

该头文件定义了模板类型 `yggr::range_ex::range_size_ex<T>`，用于在编译期为类型 `T` 提供“Range 大小类型（size_type）”标签：

- 如果 `T` 满足 Boost.Range 风格（具有 `range_iterator` 或 `range_const_iterator`），则 `range_size_ex<T>` 继承自 `boost::range_size<T>`，从而暴露对应的 `size_type`；
- 否则，`range_size_ex<T>` 继承自 `mplex::null_type`，表示该类型不是一个 Range 或没有可用的大小类型。

该类型可用于模板元编程中提取范围的 `size_type` 或判断某类型是否为 Boost.Range 支持的范围。

## 主要实现要点

- 文件包含：
  - `yggr/base/yggrdef.h`
  - `yggr/mplex/null_t.hpp`（提供 `mplex::null_type`）
  - Boost MPL 的 `boost/mpl/or.hpp`
  - Boost.Range 的 `boost/range/size_type.hpp` 和 `boost/range/has_range_iterator.hpp`

- 命名空间：`yggr::range_ex`。

- 关键实现：在 `detail` 命名空间中定义辅助模板 `range_size_ex_impl<has_iter, T>`：
  - 当 `has_iter == true` 时，`range_size_ex_impl<true, T>` 继承自 `boost::range_size<T>`；
  - 当 `has_iter == false` 时，`range_size_ex_impl<false, T>` 继承自 `mplex::null_type`；

- `range_size_ex<T>` 使用 `boost::mpl::or_<boost::has_range_iterator<T>, boost::has_range_const_iterator<T>>::type::value` 作为 `has_iter` 参数，选择合适的特化。

## 使用示例

```cpp
#include <type_traits>
#include <vector>
#include <iostream>
#include "yggr/range_ex/range_size_ex.hpp"
#include "yggr/mplex/null_t.hpp"

int main() {
    using yggr::range_ex::range_size_ex;
    using mplex::null_type;

    using VecSize = range_size_ex<std::vector<int>>;
    constexpr bool vec_has_size_type = !std::is_base_of<null_type, VecSize>::value;

    using IntSize = range_size_ex<int>;
    constexpr bool int_has_size_type = !std::is_base_of<null_type, IntSize>::value;

    std::cout << "vector has size_type: " << vec_has_size_type << "\n";
    std::cout << "int has size_type: " << int_has_size_type << "\n";
}
```

在上例中，`vector` 应具有 `size_type`，而 `int` 不具备。

## 何时使用

- 在模板代码中需要提取某个范围的 `size_type` 或判断类型是否为有效范围时使用。
- 可用于 SFINAE、类型萃取或基于范围大小类型的实现分支。

## 实现注意事项

- `range_size_ex<T>` 为一个继承类型（继承自 `boost::range_size<T>` 或 `mplex::null_type`），而非普通别名。
- 判断是否成功提取通常使用 `std::is_base_of<mplex::null_type, ...>` 或 `std::is_same<...>`。
- 实现依赖 Boost.Range 的 `has_range_iterator` / `has_range_const_iterator` 判定机制，因此判断规则与 Boost.Range 保持一致。

## 参考

- 源文件： [yggr/range_ex/range_size_ex.hpp](yggr/range_ex/range_size_ex.hpp#L1)

---

文档已生成并写入同目录文件。