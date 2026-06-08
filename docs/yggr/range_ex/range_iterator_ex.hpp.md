# range_iterator_ex.hpp - 中文文档

**文件位置**: [yggr/range_ex/range_iterator_ex.hpp](yggr/range_ex/range_iterator_ex.hpp#L1)

## 概述

该头文件定义了两个模板类型：

- `yggr::range_ex::range_iterator_ex<T>`：用于提取类型 `T` 的普通迭代器类型；
- `yggr::range_ex::range_const_iterator_ex<T>`：用于提取类型 `T` 的常量迭代器类型。

对于符合 Boost.Range 的类型，这两个模板分别继承自 `boost::range_iterator<T>` 和 `boost::range_const_iterator<T>`；
对于不满足 Range 特性或不公开迭代器的类型，则继承自 `mplex::null_type`。

因此，这两个模板可用于编译期判断类型是否具有有效的 Range 迭代器类型，并在元编程中选择不同实现分支。

## 主要实现要点

- 文件包含：
  - `yggr/base/yggrdef.h`
  - `yggr/mplex/null_t.hpp`
  - Boost.Range 的 `boost/range/iterator.hpp`、`boost/range/const_iterator.hpp` 和 `boost/range/has_range_iterator.hpp`

- 命名空间：`yggr::range_ex`。

- 实现细节：
  - `detail::range_iterator_ex_impl<T, has_iter>` 根据 `boost::has_range_iterator<T>::value` 特化：
    - `true` 时继承 `boost::range_iterator<T>`；
    - `false` 时继承 `mplex::null_type`；
  - `detail::range_const_iterator_ex_impl<T, has_iter>` 根据 `boost::has_range_const_iterator<T>::value` 特化：
    - `true` 时继承 `boost::range_const_iterator<T>`；
    - `false` 时继承 `mplex::null_type`；

- 最终暴露的模板：
  - `range_iterator_ex<T>` 继承自 `detail::range_iterator_ex_impl<T>`；
  - `range_const_iterator_ex<T>` 继承自 `detail::range_const_iterator_ex_impl<T>`。

## 使用示例

```cpp
#include <type_traits>
#include <vector>
#include <iostream>
#include "yggr/range_ex/range_iterator_ex.hpp"
#include "yggr/mplex/null_t.hpp"

int main() {
    using yggr::range_ex::range_iterator_ex;
    using yggr::range_ex::range_const_iterator_ex;
    using mplex::null_type;

    using VecIter = range_iterator_ex<std::vector<int>>;
    using VecConstIter = range_const_iterator_ex<std::vector<int>>;
    constexpr bool vec_has_iter = !std::is_base_of<null_type, VecIter>::value;
    constexpr bool vec_has_const_iter = !std::is_base_of<null_type, VecConstIter>::value;

    using IntIter = range_iterator_ex<int>;
    using IntConstIter = range_const_iterator_ex<int>;
    constexpr bool int_has_iter = !std::is_base_of<null_type, IntIter>::value;
    constexpr bool int_has_const_iter = !std::is_base_of<null_type, IntConstIter>::value;

    std::cout << "vector has iterator: " << vec_has_iter << "\n";
    std::cout << "vector has const iterator: " << vec_has_const_iter << "\n";
    std::cout << "int has iterator: " << int_has_iter << "\n";
    std::cout << "int has const iterator: " << int_has_const_iter << "\n";
}
```

在这个示例中，`vector` 应具有 iterator 和 const iterator，而 `int` 不具备。

## 何时使用

- 在模板代码中需要判断某个类型是否符合 Boost.Range 迭代器接口时使用。
- 可用于选择性启用基于 Range 的算法、类型萃取或 SFINAE 分支。
- 对于同时需要普通迭代器和常量迭代器的类型检测，该头文件提供了统一的编译期判断机制。

## 实现注意事项

- `range_iterator_ex<T>` 和 `range_const_iterator_ex<T>` 都不是简单别名，而是继承自 `boost::range_iterator<T>` / `boost::range_const_iterator<T>` 或 `mplex::null_type`。
- 可通过 `std::is_base_of<mplex::null_type, ...>` 或 `std::is_same<...>` 等 trait 判断是否成功提取到迭代器类型。
- 该实现依赖 Boost.Range 的 `has_range_iterator` / `has_range_const_iterator` 判定机制，因此判断规则与 Boost.Range 保持一致。

## 参考

- 源文件： [yggr/range_ex/range_iterator_ex.hpp](yggr/range_ex/range_iterator_ex.hpp#L1)

---

文档已生成，若需要我可以继续为该目录其他头文件生成 `.hpp.md` 文档。