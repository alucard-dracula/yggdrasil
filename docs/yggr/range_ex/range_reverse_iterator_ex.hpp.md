# range_reverse_iterator_ex.hpp - 中文文档

**文件位置**: [yggr/range_ex/range_reverse_iterator_ex.hpp](yggr/range_ex/range_reverse_iterator_ex.hpp#L1)

## 概述

该头文件定义了两个模板类型：

- `yggr::range_ex::range_reverse_iterator_ex<T>`：用于提取类型 `T` 的反向迭代器类型；
- `yggr::range_ex::range_const_reverse_iterator_ex<T>`：用于提取类型 `T` 的 const 反向迭代器类型。

对于符合 Boost.Range 的类型，这两个模板分别继承自 `boost::range_reverse_iterator<T>` 和 `boost::range_const_reverse_iterator<T>`；
对于不满足 Range 特性或不公开迭代器的类型，则继承自 `mplex::null_type`。

因此，这两个模板可用于编译期判断类型是否具有有效的反向迭代器类型，并在元编程中选择不同实现分支。

## 主要实现要点

- 文件包含：
  - `yggr/base/yggrdef.h`
  - `yggr/mplex/null_t.hpp`
  - Boost MPL 的 `boost/mpl/or.hpp`
  - Boost.Range 的 `boost/range/reverse_iterator.hpp`、`boost/range/const_reverse_iterator.hpp` 和 `boost/range/has_range_iterator.hpp`

- 命名空间：`yggr::range_ex`。

- 实现细节：
  - `detail::range_reverse_iterator_ex_impl<has_iter, T>` 根据 `boost::has_range_iterator<T>::value` 或 `boost::has_range_const_iterator<T>::value` 特化：
    - `true` 时继承 `boost::range_reverse_iterator<T>`；
    - `false` 时继承 `mplex::null_type`；
  - `detail::range_const_reverse_iterator_ex_impl<has_iter, T>` 根据相同条件特化：
    - `true` 时继承 `boost::range_const_reverse_iterator<T>`；
    - `false` 时继承 `mplex::null_type`；

- 最终暴露的模板：
  - `range_reverse_iterator_ex<T>` 继承自对应 detail 实现；
  - `range_const_reverse_iterator_ex<T>` 继承自对应 detail 实现。

## 使用示例

```cpp
#include <type_traits>
#include <vector>
#include <iostream>
#include "yggr/range_ex/range_reverse_iterator_ex.hpp"
#include "yggr/mplex/null_t.hpp"

int main() {
    using yggr::range_ex::range_reverse_iterator_ex;
    using yggr::range_ex::range_const_reverse_iterator_ex;
    using mplex::null_type;

    using VecRevIter = range_reverse_iterator_ex<std::vector<int>>;
    using VecConstRevIter = range_const_reverse_iterator_ex<std::vector<int>>;
    constexpr bool vec_has_rev_iter = !std::is_base_of<null_type, VecRevIter>::value;
    constexpr bool vec_has_const_rev_iter = !std::is_base_of<null_type, VecConstRevIter>::value;

    using IntRevIter = range_reverse_iterator_ex<int>;
    using IntConstRevIter = range_const_reverse_iterator_ex<int>;
    constexpr bool int_has_rev_iter = !std::is_base_of<null_type, IntRevIter>::value;
    constexpr bool int_has_const_rev_iter = !std::is_base_of<null_type, IntConstRevIter>::value;

    std::cout << "vector has reverse iterator: " << vec_has_rev_iter << "\n";
    std::cout << "vector has const reverse iterator: " << vec_has_const_rev_iter << "\n";
    std::cout << "int has reverse iterator: " << int_has_rev_iter << "\n";
    std::cout << "int has const reverse iterator: " << int_has_const_rev_iter << "\n";
}
```

在这个示例中，`vector` 应具有反向迭代器和 const 反向迭代器，而 `int` 不具备。

## 何时使用

- 在模板代码中需要判断某个类型是否符合 Boost.Range 并具有反向迭代器接口时使用。
- 可用于选择性启用基于反向迭代器的算法、类型萃取或 SFINAE 分支。
- 适用于既要支持普通遍历也要支持逆向遍历的泛型范围适配场景。

## 实现注意事项

- `range_reverse_iterator_ex<T>` 和 `range_const_reverse_iterator_ex<T>` 不是简单别名，而是继承自 `boost::range_reverse_iterator<T>` / `boost::range_const_reverse_iterator<T>` 或 `mplex::null_type`。
- 可通过 `std::is_base_of<mplex::null_type, ...>` 或 `std::is_same<...>` 判断是否成功提取到反向迭代器类型。
- 该实现依赖 Boost.Range 的 `has_range_iterator` / `has_range_const_iterator` 判定机制，因此判断规则与 Boost.Range 保持一致。

## 参考

- 源文件： [yggr/range_ex/range_reverse_iterator_ex.hpp](yggr/range_ex/range_reverse_iterator_ex.hpp#L1)

---

文档已生成，如需继续为该目录下其他头文件生成 `.hpp.md` 文档，请继续告知。