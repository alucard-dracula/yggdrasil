# range_value_ex.hpp - 中文文档

**文件位置**: [yggr/range_ex/range_value_ex.hpp](yggr/range_ex/range_value_ex.hpp#L1)

## 概述

该头文件定义了模板类型 `yggr::range_ex::range_value_ex<T>`，用于在编译期为类型 `T` 提供“Range 元素值类型”标签：

- 如果 `T` 满足 Boost.Range 风格（具有 `range_iterator` 或 `range_const_iterator`），则 `range_value_ex<T>` 继承自 `boost::range_value<T>`，返回该范围元素的值类型；
- 否则，`range_value_ex<T>` 继承自 `mplex::null_type`，表示该类型不是一个 Range 或没有可用的值类型。

该类型常用于模板元编程中提取 Range 的 `value_type` 或判断类型是否为 Boost.Range 支持的范围。

## 主要实现要点

- 文件包含：
  - `yggr/base/yggrdef.h`
  - `yggr/mplex/null_t.hpp`
  - Boost MPL 的 `boost/mpl/or.hpp`
  - Boost.Range 的 `boost/range/value_type.hpp` 和 `boost/range/has_range_iterator.hpp`

- 命名空间：`yggr::range_ex`。

- 关键实现：在 `detail` 命名空间中定义辅助模板 `range_value_ex_impl<has_iter, T>`：
  - 当 `has_iter == true` 时，`range_value_ex_impl<true, T>` 继承自 `boost::range_value<T>`；
  - 当 `has_iter == false` 时，`range_value_ex_impl<false, T>` 继承自 `mplex::null_type`；

- `range_value_ex<T>` 使用 `boost::has_range_iterator<T>` 或 `boost::has_range_const_iterator<T>` 的逻辑或结果作为 `has_iter` 参数，选择适当的特化。

## 使用示例

```cpp
#include <type_traits>
#include <vector>
#include <iostream>
#include "yggr/range_ex/range_value_ex.hpp"
#include "yggr/mplex/null_t.hpp"

int main() {
    using yggr::range_ex::range_value_ex;
    using mplex::null_type;

    using VecValue = range_value_ex<std::vector<int>>;
    constexpr bool vec_has_value_type = !std::is_base_of<null_type, VecValue>::value;

    using IntValue = range_value_ex<int>;
    constexpr bool int_has_value_type = !std::is_base_of<null_type, IntValue>::value;

    std::cout << "vector has value_type: " << vec_has_value_type << "\n";
    std::cout << "int has value_type: " << int_has_value_type << "\n";
}
```

在上例中，`vector` 应具有值类型，而 `int` 不具备。

## 何时使用

- 在模板代码中需要提取某个范围的元素值类型时使用。
- 可用于 SFINAE、类型萃取或基于 Range `value_type` 的实现选择。
- 适用于需要区分“符合 Boost.Range 接口的类型”与“普通类型”的场景。

## 实现注意事项

- `range_value_ex<T>` 不是简单的类型别名，而是继承自 `boost::range_value<T>` 或 `mplex::null_type`。
- 通常通过 `std::is_base_of<mplex::null_type, ...>` 或 `std::is_same<...>` 来判断是否成功提取值类型。
- 该实现依赖 Boost.Range 的 `has_range_iterator` / `has_range_const_iterator` 判定机制，因此其判断规则与 Boost.Range 保持一致。

## 参考

- 源文件： [yggr/range_ex/range_value_ex.hpp](yggr/range_ex/range_value_ex.hpp#L1)

---

文档已生成。如需我继续为当前目录下的其他文件生成 `.hpp.md` 文档，请继续告知。