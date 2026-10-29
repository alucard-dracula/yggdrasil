# `tuple_comparison.hpp` 中文说明

## 用途

本头文件为 Boost.Tuple 与 Yggdrasil Tuple 的链式 `cons` 表示补充比较运算符，并支持它们与 `std::pair` 之间的比较。头文件包含 Boost.Tuple 自带的比较实现，并在相应命名空间中提供跨类型重载。

```cpp
#include <yggr/tuple/tuple_comparison.hpp>
```

## 支持的比较

提供的关系运算符包括：

| 运算符 | 比较方式 |
| --- | --- |
| `==` | 对应元素逐项使用 `==` 比较，所有元素均相等才返回 `true`。 |
| `!=` | 对应元素逐项使用 `!=` 比较，任一元素不等即返回 `true`。 |
| `<`、`>`、`<=`、`>=` | 按元素顺序进行字典序比较：先比较头元素；只有头元素在相应方向上无法区分时，才比较后续元素。 |

跨类型重载涵盖 Boost.Tuple `cons` 与 `yggr::tuples::cons` 之间的比较，以及二元 `cons` 与 `std::pair` 之间的比较。实现分别放在 `boost::tuples` 和 `yggr::tuples` 命名空间，以配合参数相关查找。

`std::pair` 只对应长度为 2 的 tuple；与 tuple 比较时会在编译期检查 tuple 长度。

## 长度约束

跨 tuple 类型比较时，双方必须具有相同的元素个数；与 `std::pair` 比较时，tuple 必须恰有两个元素。长度不匹配由 `BOOST_STATIC_ASSERT` 在编译期拒绝，而不是运行时比较。

## 使用示例

```cpp
#include <yggr/tuple/tuple.hpp>
#include <yggr/tuple/tuple_comparison.hpp>

yggr::tuples::tuple<int, int> left(1, 2);
yggr::tuples::tuple<int, int> right(1, 3);

bool equal = (left == right);     // false
bool ordered = (left < right);    // true：第一项相同，比较第二项
```

跨 `cons` 与 `std::pair` 比较的示意：

```cpp
yggr::tuples::cons<int, yggr::tuples::cons<int, yggr::tuples::null_type> > value;
std::pair<int, int> pair_value(1, 2);

bool same = (value == pair_value);
```

实际构造对象时，应使用项目中相应 tuple/cons 类型的构造方式。

## 比较语义注意事项

- 元素类型必须支持所使用的对应运算符。字典序判断根据头元素的 `<`、`>`、`<=` 或 `>=` 结果决定是否继续比较尾部。
- 各关系运算符独立按元素运算符实现；源码明确不假设 `!(a < b)` 等价于 `a >= b`，也不假设 `a != b` 等价于 `!(a == b)`。如果元素类型的比较运算符不满足通常的相互一致性，tuple 运算符也会保留这种行为。
- 本头文件中的跨类型重载针对 `cons` 链和 `std::pair`，不要据此推断任意不同类型或不同长度的 tuple 都可互相比较。
