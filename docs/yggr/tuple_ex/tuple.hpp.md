# `tuple.hpp` 中文说明

## 用途

本头文件扩展 Boost.Tuple 和 Yggdrasil Tuple 的互操作能力，并在支持 C++ 标准 tuple 时为 `std::tuple` 提供比较、哈希和交换支持。它集中提供跨 tuple 类型的比较运算符、tuple 哈希实现，以及不同 tuple 类型之间的交换函数。

```cpp
#include <yggr/tuple_ex/tuple.hpp>
```

头文件同时包含 Yggdrasil tuple、tuple 比较和流操作接口；支持可变模板时还会引入索引序列等辅助定义。

## 支持的操作

### 跨类型比较

提供 `==`、`!=`、`<`、`<=`、`>`、`>=` 运算符，以支持 Boost.Tuple、`yggr::tuple` 之间的比较；若标准库 tuple 可用，也支持它们与 `std::tuple` 之间的比较。Boost.Tuple 和 Yggdrasil Tuple 还提供与 `std::pair` 的比较重载。

同长度 tuple 按元素逐项比较：相等性运算逐项比较所有元素；大小关系采用字典序，即从首元素开始，遇到首个不相等的元素时确定结果。对应元素类型必须支持所使用的比较运算。

tuple 长度不同时，等于比较结果为 `false`，不等于比较结果为 `true`；大小关系比较不支持不同长度，实例化该操作会触发断言诊断。

### 哈希

为 Boost.Tuple 和 Yggdrasil Tuple 的 `cons`、tuple 类型提供 `boost::hash` 支持。标准 tuple 可用时，也为 `std::tuple` 提供 Boost 哈希；在启用 `YGGR_HAS_CXX11_STD_HASH` 的配置下，头文件还提供相应的 `std::hash` 实现。

哈希按元素顺序组合各元素的哈希值，因此 tuple 中的每种元素类型都需要具备对应的哈希支持。

### 跨类型交换

当标准库 `std::tuple` 可用时，提供 `std::tuple` 与 Boost.Tuple 或 Yggdrasil Tuple 之间的 `swap` 重载，并将相关重载导出到 tuple、Boost 和标准库相关命名空间。交换逐项进行，两个 tuple 必须具有相同的元素个数，且对应元素需可交换。

## 使用示例

```cpp
#include <yggr/tuple_ex/tuple.hpp>

yggr::tuple<int, double> left(1, 2.5);
boost::tuple<int, double> right(1, 3.0);

bool same = (left == right);
bool ordered = (left < right);

std::size_t hash_value = boost::hash<yggr::tuple<int, double> >()(left);
```

在启用标准 tuple 的构建配置中，也可以比较兼容长度的 `std::tuple` 与 Yggdrasil/Boost tuple，并调用支持的跨类型 `swap`。

## 配置与注意事项

- `std::tuple` 相关重载仅在标准 tuple 头文件可用时启用；禁用可变模板的构建路径会影响部分跨类型比较接口。
- 不同 tuple 类型进行比较或交换时，应确保元素个数符合相应操作的要求。
- 比较、哈希和交换最终依赖元素类型本身提供所需操作；模板接口在实例化时才会暴露元素级不兼容问题。
- 本文件包含大量针对不同 tuple 类型及编译器配置的重载展开；通常直接包含本头文件即可，无需依赖其 `detail` 实现。
