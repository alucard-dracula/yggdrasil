# `tuple_hasher.hpp` 

## 用途

本头文件为 Boost.Tuple、Yggdrasil Tuple 及配置支持时的 `std::tuple` 提供哈希实现，使这些类型可以用于依赖哈希函数的场景。实现通过 `boost::hash_combine` 按元素顺序组合哈希值。

```cpp
#include <yggr/tuple_ex/detail/tuple_hasher.hpp>
```

这是 `detail` 内部实现头文件。通常建议通过上层接口 `<yggr/tuple_ex/tuple.hpp>` 使用 tuple 相关功能。

## 支持的类型与接口

### `boost::hash`

在 `boost` 命名空间中提供以下类型的 `boost::hash<T>` 特化：

- `boost::tuples::cons`、`boost::tuples::tuple` 和 `boost::tuples::null_type`。
- `yggr::tuples::cons` 和 `yggr::tuples::tuple`。
- 当标准库 tuple 头文件可用且启用可变参数模板时，`std::tuple`。

调用方式示例：

```cpp
#include <yggr/tuple_ex/tuple.hpp>

yggr::tuple<int, std::string> value(7, "item");
std::size_t result = boost::hash<yggr::tuple<int, std::string> >()(value);
```

### `std::hash`

仅在定义 `YGGR_HAS_CXX11_STD_HASH` 时，本头文件才提供 `std::hash` 特化：

- Boost.Tuple 的 `cons`、`tuple`、`null_type`。
- Yggdrasil Tuple 的 `cons`、`tuple`。
- 在标准 tuple 可用且启用可变参数模板时，`std::tuple`。

使用 `std::hash` 前，还需确保相应类型及其元素类型满足当前标准库和项目配置的哈希要求。

## 哈希计算方式

- 对 `cons` 链，辅助器从 head 开始递归处理 tail，依次将每个元素的哈希合并到 seed 中。
- 对 `std::tuple`，辅助器通过索引从第一个元素遍历到最后一个元素，并逐项调用 `boost::hash_combine`。
- tuple 哈希的初始 seed 为 `0`。`null_type` 的辅助处理会将 seed 设为 `boost::hash<std::string>` 对空字符串的哈希值。
- 结果是 `std::size_t`。哈希值用于哈希容器等场景，不应视为稳定的序列化值或跨进程标识。

tuple 中的每种元素类型都必须能由 Boost 哈希机制处理；若同时使用 `std::hash` 接口，也需符合相应标准库哈希特化的要求。哈希值可能发生碰撞，容器仍需结合相等比较判断键是否相同。

## 配置与兼容性

- 未定义 `YGGR_NO_CXX11_HDR_TUPLE` 时包含并支持标准库 `<tuple>`；`std::tuple` 哈希还要求可变参数模板可用。
- `YGGR_NO_CXX11_VARIADIC_TEMPLATES` 启用时，Boost.Tuple 和 Yggdrasil Tuple 的模板特化通过预处理器按项目或 Boost 配置的 tuple 长度上限展开。
- 在 C++17 之前的配置下，部分 `boost::hash` 特化继承 `std::unary_function`；C++17 及更新配置不使用该基类。
- 头文件依赖 Yggdrasil tuple 定义及 Boost 哈希工具。
