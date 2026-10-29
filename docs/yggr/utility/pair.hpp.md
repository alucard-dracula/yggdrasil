# `pair.hpp` 中文说明

## 用途

本头文件为 `std::pair` 提供 Yggdrasil 元组接口适配：支持按编译期索引获取元素，并将 `std::pair` 纳入 `yggr::tuple_size`、`yggr::tuple_element` 元组元数据体系。它不重新定义 pair 类型。

```cpp
#include <yggr/utility/pair.hpp>
```

## 元素访问

头文件在 `yggr` 命名空间定义两个 `get` 函数模板：

- `get<idx>(pair)`：通过非类型模板参数 `idx` 指定元素索引。
- `get<Index>(pair)`：通过类型参数 `Index` 指定索引；该类型需能由 `YGGR_INDEX_T_VALUE_GET_CAST` 提取为 `std::size_t` 常量值。

两种形式都提供可修改和 const 左值重载，返回对应成员的引用，不复制元素。索引 `0` 对应 `first`，索引 `1` 对应 `second`。

```cpp
#include <yggr/utility/pair.hpp>
#include <utility>

std::pair<int, long> values(3, 8);
yggr::get<0>(values) = 4;
const long& second = yggr::get<1>(values);
```

## 元组元数据

本头文件为 `yggr::tuple_size` 提供 `std::pair<T1, T2>` 特化，大小为 `2`；`yggr::tuple_element<I, Pair>` 的元素类型由 `first` 或 `second` 的类型决定。`tuple_element` 对 `I < 2` 使用 MPL 编译期断言进行检查。

这使 pair 能与依赖 Yggdrasil 元组元数据的泛型代码协作。此头文件定义的是 `yggr::get` 及 `yggr` 的元组元数据特化，不应据此假定它增加了 `std::get` 重载。

## 注意事项

- 索引必须是编译期常量，且只能为 `0` 或 `1`；其他索引没有对应的元素访问器。
- `get` 返回对 pair 成员的引用。引用的有效期受原 pair 对象生命周期约束。
- 使用此文件进行 pair 元组适配时，应包含本头文件；仅包含 `<utility>` 不会声明 `yggr::get` 或上述 `yggr` 元数据特化。
