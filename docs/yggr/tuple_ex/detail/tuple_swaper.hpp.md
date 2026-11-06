# `tuple_swaper.hpp` 

## 用途

本头文件为 `std::tuple` 与 Boost.Tuple、Yggdrasil Tuple 之间提供跨类型 `swap` 支持。交换按元素位置逐项进行，属于 `yggr::tuple_ex` 的内部实现。

```cpp
#include <yggr/tuple_ex/detail/tuple_swaper.hpp>
```

通常建议通过上层接口 `<yggr/tuple_ex/tuple.hpp>` 使用 tuple 扩展功能，不直接依赖 `detail` 实现。

## 支持的操作

仅在标准库 `<tuple>` 可用（未定义 `YGGR_NO_CXX11_HDR_TUPLE`）时，本头文件提供以下跨类型交换：

- `std::tuple` 与 `boost::tuple`。
- `std::tuple` 与 `yggr::tuple`。

每组类型均提供两个参数顺序的重载。`std::tuple` 与自身、Boost.Tuple 与自身以及 Yggdrasil Tuple 与自身的交换不由此文件实现，应使用相应类型已有的 `swap` 支持。

## 交换规则与限制

- 两个 tuple 必须具有相同的元素个数；长度不一致时会触发静态断言诊断。
- 每对对应位置的元素通过 `yggr::swap` 交换，因此元素类型必须支持该交换操作。
- 交换直接修改两个 tuple 的元素，不创建新的 tuple。

## 实现结构

- `yggr::tuple_ex::detail::tuple_swap_helper<I, ISize>` 从索引 `0` 开始递归访问两个 tuple 的第 `I` 个元素并交换；到达 tuple 长度后由终止特化结束递归。
- `yggr::tuple_ex::swap_support` 提供跨类型重载，并通过 `boost::enable_if_c` / `boost::disable_if_c` 区分长度相同与不相同的情况。
- 重载通过 using-declaration 引入 `yggr::tuples`、`std` 和 `boost` 命名空间，以配合这些类型的调用与参数相关查找。

## 使用示例

```cpp
#include <yggr/tuple_ex/tuple.hpp>

std::tuple<int, double> standard(1, 2.5);
boost::tuple<int, double> legacy(3, 4.5);

using std::swap;
swap(standard, legacy);
```

调用后，两个 tuple 对应位置的值互换。示例要求构建启用了标准库 tuple 支持，且元素可通过 `yggr::swap` 交换。

## 配置与依赖

本头文件依赖 Yggdrasil tuple 定义、静态断言工具和标准库 `<tuple>`。所有跨类型交换重载均受 `YGGR_NO_CXX11_HDR_TUPLE` 控制；禁用标准库 tuple 时，这些重载不会定义。
