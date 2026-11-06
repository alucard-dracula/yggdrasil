# `tuple_substructure.hpp` 

## 用途

本头文件提供 tuple 子序列的类型计算和运行时提取接口。给定源 tuple、起始位置和长度，可以在编译期确定结果 tuple 类型，并在运行时按顺序复制相应元素。

```cpp
#include <yggr/tuple_ex/detail/tuple_substructure.hpp>
```

文件位于 `detail` 目录，通常应通过上层 tuple 扩展接口使用，不建议将内部辅助模板作为稳定公共 API 依赖。

## 主要接口

### `sub_tuple_t`

```cpp
template<typename Tuple, std::size_t Pos = 0,
         std::size_t Len = static_cast<std::size_t>(-1)>
struct yggr::tuple_ex::sub_tuple_t;
```

`sub_tuple_t<Tuple, Pos, Len>::type` 是从 `Tuple` 中选取子序列后的 tuple 类型。`Pos` 为从零开始的起始索引，`Len` 为请求的元素个数；默认位置为 `0`，默认长度表示取到 tuple 末尾。对 Boost.Tuple 和 Yggdrasil 的 `cons` 类型也提供了类型转换处理。

### `sub_tuple`

```cpp
template<std::size_t SubPos, std::size_t SubLen, typename SrcTuple>
typename sub_tuple_t<SrcTuple, SubPos, SubLen>::type
sub_tuple(const SrcTuple& src);
```

该函数返回源 tuple 指定范围内元素组成的新 tuple。结果中的元素顺序与源 tuple 一致，元素值通过 tuple 构造辅助器传入。

## 范围边界

- 有效范围从 `SubPos` 开始，最多选取 `SubLen` 个元素。
- 若请求长度超过从起点到末尾的剩余元素数，实际提取长度会限制为剩余长度。
- 起点位于 tuple 末尾时，结果为空 tuple。
- 起点超出 tuple 范围时，模板的有效性检查会阻止访问越界元素，结果为默认构造的结果 tuple；本实现将该无效范围映射为空结果，而不是抛出异常。

调用方应优先传入合法起点，并确保结果 tuple 可默认构造（无效范围分支会默认构造结果）。

## 实现概览

- `cons_to_vector_impl` 将 `cons` 链转换为 MPL 类型序列。
- `sub_tuple_t_impl` 使用 Boost.MPL 擦除和迭代器范围选取类型序列，再转换回源 tuple 模板对应的 tuple 类型。
- `sub_tuple_impl_helper` 使用索引序列（或旧编译器下的预处理器展开）逐项读取源 tuple，并通过 `tuple_maker` 构造目标 tuple。
- 不支持可变参数模板时，相关类型与函数实现按 `YGGR_TUPLE_LIMIT_LENGTH()` 展开；支持标准库 tuple 头文件时包含 `<tuple>`，但子序列构造仍通过本项目的 tuple 辅助设施完成。

## 使用示例

```cpp
#include <yggr/tuple_ex/tuple.hpp>
#include <yggr/tuple_ex/detail/tuple_substructure.hpp>

yggr::tuple<int, double, char> source(10, 2.5, 'x');
yggr::tuple<double, char> tail =
    yggr::tuple_ex::sub_tuple<1, 2>(source);
```

该示例从索引 `1` 开始取两个元素，结果类型为包含 `double` 和 `char` 的 tuple。结果元素类型需能从源 tuple 对应位置的元素构造。

## 依赖与配置

实现依赖 Yggdrasil tuple、tuple 转换与构造工具，Boost.MPL 类型序列算法，以及基础配置定义。可变参数模板配置使用索引序列；旧编译器路径使用项目预处理器宏展开。相关可用长度受 `YGGR_TUPLE_LIMIT_LENGTH()` 等项目配置约束。
