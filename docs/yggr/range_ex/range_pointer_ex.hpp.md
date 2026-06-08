# range_pointer_ex.hpp - 中文文档

**文件位置**: [yggr/range_ex/range_pointer_ex.hpp](yggr/range_ex/range_pointer_ex.hpp#L1)

## 概述

该头文件定义了模板类型 `yggr::range_ex::range_pointer_ex<T>`，用于在编译期为类型 `T` 提供“Range 指针类型”标签：

- 如果 `T` 满足 Boost.Range 风格（具有 `range_iterator` 或 `range_const_iterator`），则 `range_pointer_ex<T>` 继承自 `boost::range_pointer<T>`，从而暴露对应的指针类型；
- 否则，`range_pointer_ex<T>` 继承自 `mplex::null_type`，表示该类型不是一个 Range 或没有可用的指针类型。

该类型可用于模板元编程中判断或提取范围类型的底层指针类型。

## 主要实现要点

- 文件包含：
  - `yggr/base/yggrdef.h`
  - `yggr/mplex/null_t.hpp`
  - Boost MPL 的 `boost/mpl/or.hpp`
  - Boost.Range 的 `boost/range/pointer.hpp` 和 `boost/range/has_range_iterator.hpp`

- 命名空间：`yggr::range_ex`。

- 关键实现：在 `detail` 命名空间中定义辅助模板 `range_pointer_ex_impl<has_iter, T>`：
  - 当 `has_iter == true` 时，`range_pointer_ex_impl<true, T>` 继承自 `boost::range_pointer<T>`；
  - 当 `has_iter == false` 时，`range_pointer_ex_impl<false, T>` 继承自 `mplex::null_type`；

- `range_pointer_ex<T>` 使用 `boost::has_range_iterator<T>` 或 `boost::has_range_const_iterator<T>` 的逻辑或结果作为 `has_iter` 参数，选择适当的特化。

## 使用示例

```cpp
#include <type_traits>
#include <vector>
#include <iostream>
#include "yggr/range_ex/range_pointer_ex.hpp"
#include "yggr/mplex/null_t.hpp"

int main() {
    using yggr::range_ex::range_pointer_ex;
    using mplex::null_type;

    using VecPtr = range_pointer_ex<std::vector<int>>;
    constexpr bool vec_has_ptr = !std::is_base_of<null_type, VecPtr>::value;

    using IntPtr = range_pointer_ex<int>;
    constexpr bool int_has_ptr = !std::is_base_of<null_type, IntPtr>::value;

    std::cout << "vector has pointer type: " << vec_has_ptr << "\n";
    std::cout << "int has pointer type: " << int_has_ptr << "\n";
}
```

在上述示例中，`vector` 应具有指针类型，而 `int` 不具备。

## 何时使用

- 在模板代码中需要判断某个类型是否符合 Boost.Range 接口并具有指针类型时使用。
- 可用于 SFINAE、类型萃取或基于 Range 指针类型选择实现分支。

## 实现注意事项

- `range_pointer_ex<T>` 不是一个简单的类型别名，而是继承自 `boost::range_pointer<T>` 或 `mplex::null_type`。
- 典型的判断方式是使用 `std::is_base_of<mplex::null_type, ...>` 或 `std::is_same<...>`。
- 该实现依赖 Boost.Range 的 `has_range_iterator` / `has_range_const_iterator` 判定机制，因此其行为与 Boost.Range 的检测规则保持一致。

## 参考

- 源文件： [yggr/range_ex/range_pointer_ex.hpp](yggr/range_ex/range_pointer_ex.hpp#L1)

---

文档已生成。如需我继续为其他目录文件生成 `.hpp.md` 文档，请继续告知。