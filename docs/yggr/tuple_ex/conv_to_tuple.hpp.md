# `conv_to_tuple.hpp` 

## 用途

本头文件提供将 Boost.MPL 类型序列转换为 Yggdrasil Tuple、Boost.Tuple 或 `std::tuple` 类型的编译期工具。转换结果通过模板的 `type` 成员类型获取，不会在运行时创建或转换 tuple 对象。

```cpp
#include <yggr/tuple_ex/conv_to_tuple.hpp>
```

## 主要接口

### `conv_to_tuple`

基础模板的形式为：

```cpp
template<template<typename...> class Tuple,
         typename Vector,
         typename Null_T = yggr::tuples::null_type>
struct conv_to_tuple;
```

`Vector` 是包含类型的 Boost.MPL 序列，`Tuple` 是目标 tuple 模板，`Null_T` 是用于检查序列是否含有终止标记的类型。转换结果为 `conv_to_tuple<Tuple, Vector, Null_T>::type`。

序列中不得包含 `Null_T`，序列长度不得超过 `YGGR_TUPLE_LIMIT_LENGTH()`；违反条件会触发编译期断言。序列中的类型按原有顺序成为目标 tuple 的模板参数。

### 便捷封装

- `conv_to_yggr_tuple<Vector, Null_T>`：转换为 `yggr::tuple`，默认终止标记为 `yggr::tuples::null_type`。
- `conv_to_boost_tuple<Vector, Null_T>`：转换为 `boost::tuple`，默认终止标记为 `boost::tuples::null_type`。
- `conv_to_stl_tuple<Vector, Null_T>`：转换为 `std::tuple`，仅在标准库 tuple 可用时声明；默认终止标记为 `boost::tuples::null_type`。

这些封装均通过 `type` 成员暴露最终类型。

## 使用示例

```cpp
#include <yggr/tuple_ex/conv_to_tuple.hpp>
#include <boost/mpl/vector.hpp>

typedef boost::mpl::vector<int, double> element_types;
typedef yggr::tuple_ex::conv_to_yggr_tuple<element_types>::type result_tuple;
```

示例中的 `result_tuple` 等价于 `yggr::tuple<int, double>`。序列应使用与转换目标相匹配的 `Null_T` 约定；若需要自定义终止标记，可显式传入第二个模板参数。

## 配置与实现

- 启用可变参数模板时，头文件包含 `detail/conv_to_tuple_vtpl.hpp`，使用索引序列展开 MPL 序列。
- 禁用可变参数模板时，包含 `detail/conv_to_tuple_vmacro.hpp`，通过预处理器按 tuple 长度上限展开转换实现。
- `conv_to_stl_tuple` 仅在未定义 `YGGR_NO_CXX11_HDR_TUPLE` 时可用。
- 具体可转换的最大长度受 `YGGR_TUPLE_LIMIT_LENGTH()` 配置限制。
