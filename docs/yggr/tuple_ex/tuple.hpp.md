# `tuple.hpp` 

## 用途

本头文件是 tuple 扩展功能的聚合入口，集中包含比较、哈希、交换和子 tuple 提取实现。它本身不定义 tuple 类型；tuple 类型及基础操作由 `<yggr/tuple/tuple.hpp>` 等基础头文件提供。

```cpp
#include <yggr/tuple_ex/tuple.hpp>
```

## 汇总的功能

- **比较**：加载 `detail/tuple_comparer.hpp`，为支持的 Yggdrasil Tuple、Boost.Tuple 及配置支持时的 `std::tuple` 类型提供比较运算，并支持 tuple 与 `std::pair` 的相应比较。
- **哈希**：加载 `detail/tuple_hasher.hpp`，提供 Boost.Tuple、Yggdrasil Tuple 及配置支持时的 `std::tuple` 哈希实现。
- **交换**：加载 `detail/tuple_swaper.hpp`，在标准库 tuple 可用时提供 `std::tuple` 与 Boost.Tuple、Yggdrasil Tuple 之间的交换支持。
- **子 tuple 提取**：加载 `detail/tuple_substructure.hpp`，提供 `sub_tuple_t` 类型计算和 `sub_tuple` 子序列提取接口。

具体可用的重载受各实现所用的编译配置宏控制；请参见对应的 detail 头文件文档了解长度约束和边界行为。

## 使用示例

```cpp
#include <yggr/tuple_ex/tuple.hpp>
#include <yggr/tuple_ex/tuple_maker.hpp>

yggr::tuple<int, double> left(1, 2.5);
boost::tuple<int, double> right(1, 3.0);

bool ordered = (left < right);
std::size_t hash_value = boost::hash<yggr::tuple<int, double> >()(left);
```

`tuple_maker.hpp` 不由本聚合头文件直接包含；示例单独引入它以说明 tuple 构造工具的使用。

## 配置与依赖

本文件只依赖基础配置头，并转发包含四个 `detail` 头文件。可变参数模板、标准库 `<tuple>` 支持及项目 tuple 长度上限等配置会影响各项扩展的可用范围。若只需要某一项功能，也可以直接包含对应的公开接口头文件或实现入口。
