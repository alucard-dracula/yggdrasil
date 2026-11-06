# `tuple_comparer.hpp` 

## 用途

本头文件为 Yggdrasil Tuple、Boost.Tuple 以及（配置支持时）`std::tuple` 提供跨类型比较运算符，并支持 Boost.Tuple、Yggdrasil Tuple 与 `std::pair` 之间的比较。它是 `yggr::tuple_ex` 的内部实现文件，主要提供运算符重载和比较辅助函数，不定义供一般业务代码直接构造的比较器对象。

```cpp
#include <yggr/tuple_ex/detail/tuple_comparer.hpp>
```

通常应优先包含上层接口头文件 `<yggr/tuple_ex/tuple.hpp>`，而不是直接依赖 `detail` 实现。

## 支持的比较

支持以下六种比较运算：

| 运算符 | 用途 |
| --- | --- |
| `==`、`!=` | 判断对应位置的元素是否全部相等 |
| `<`、`<=`、`>`、`>=` | 按元素顺序进行字典序比较 |

跨类型重载覆盖范围如下：

- `boost::tuple` 与 `yggr::tuples::tuple` 可双向比较。
- 当启用可变参数模板且标准库 tuple 可用时，`std::tuple` 可与 Boost.Tuple 或 Yggdrasil Tuple 比较。
- Boost.Tuple、Yggdrasil Tuple 可与 `std::pair` 比较；pair 按两个元素的序列参与比较。
- `std::tuple` 与 `std::pair` 之间没有由本头文件提供的额外重载。

对应位置的元素类型必须支持所使用的比较运算。比较结果遵循 tuple 类型和元素比较运算的语义。

## 长度不同时的行为

tuple 之间进行比较时，元素个数必须相同：

- `==` 返回 `false`，`!=` 返回 `true`。
- `<`、`<=`、`>`、`>=` 不接受不同长度的 tuple；实例化该比较会触发静态断言诊断。

与 `std::pair` 比较时，pair 表示两个元素，因而 tuple 也应具有两个元素，并且元素类型应能与 pair 对应元素比较。

## 实现结构

- `yggr::tuples::detail` 中的 `tuple_eq`、`tuple_neq`、`tuple_lt`、`tuple_lte`、`tuple_gt`、`tuple_gte` 根据 tuple 长度是否相同选择比较行为。
- 支持可变参数模板时，`yggr::tuple_ex::detail` 中的辅助函数使用索引序列逐项访问元素；相等性逐项判定，大小关系按字典序确定。
- 不支持可变参数模板时，使用 `BOOST_TUPLE_LIMIT_LENGTH()` 和预处理器宏展开 Boost.Tuple、Yggdrasil Tuple 及 `std::pair` 的重载。
- `std::tuple` 相关跨类型重载还要求未定义 `YGGR_NO_CXX11_HDR_TUPLE`。

## 配置与依赖

头文件依赖 Yggdrasil 的 tuple 定义、比较与流操作头文件，以及基础定义和静态断言工具。可变参数模板配置下还依赖索引序列及参数展开辅助；标准 tuple 可用时包含 `<tuple>`。

`YGGR_NO_CXX11_VARIADIC_TEMPLATES` 和 `YGGR_NO_CXX11_HDR_TUPLE` 会影响可用的重载集合。不同构建配置下，跨类型比较能力可能不同。

## 使用示例

```cpp
#include <yggr/tuple_ex/tuple.hpp>

yggr::tuple<int, double> left(1, 2.5);
boost::tuple<int, double> right(1, 3.0);

bool same = (left == right);
bool ordered = (left < right);
```

上述比较要求两个 tuple 长度相同，且对应元素支持 `==` 与 `<`。需要与 `std::tuple` 比较时，构建配置还必须启用可变参数模板并提供标准库 tuple 支持。
