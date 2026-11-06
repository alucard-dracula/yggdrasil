# `tuple_maker.hpp` 

## 用途

本头文件提供按目标 tuple 家族构造 tuple 的适配器。`tuple_maker<Tuple>::make_tuple(...)` 根据传入参数创建 `std::tuple`、`boost::tuple` 或 `yggr::tuple`，并转发参数到对应的 tuple 工厂函数。

```cpp
#include <yggr/tuple_ex/tuple_maker.hpp>
```

## 主要接口

```cpp
template<typename Tuple>
struct yggr::tuple_ex::tuple_maker;
```

`Tuple` 用于选择目标 tuple 类型对应的实现。创建适配器后调用其 `make_tuple` 成员函数：

```cpp
yggr::tuple_ex::tuple_maker<yggr::tuple<> > maker;
yggr::tuple<int, double> result = maker.make_tuple(1, 2.5);
```

`make_tuple` 根据实际传入的参数类型推导结果 tuple 的元素类型；类型会移除顶层 `const`、`volatile` 和引用限定。参数通过 `boost::forward` 转发，具体构造语义由对应库的 `make_tuple` 工厂决定。

## 支持的目标类型

- `yggr::tuple<...>`：调用 `yggr::make_tuple`。
- `boost::tuple<...>`：调用 `boost::make_tuple`。
- `std::tuple<...>`：在标准库 tuple 可用且启用可变参数模板时调用 `std::make_tuple`。

禁用可变参数模板时，通过预处理器展开 `boost::tuple` 和 `yggr::tuple` 的实现，并提供空参数的 `make_tuple()` 重载；该兼容分支不提供 `std::tuple` 的 `tuple_maker_impl`。

## 使用示例

```cpp
#include <yggr/tuple_ex/tuple_maker.hpp>

yggr::tuple_ex::tuple_maker<boost::tuple<> > boost_maker;
boost::tuple<int, double> value = boost_maker.make_tuple(7, 3.5);
```

传入参数个数应符合目标 tuple 实现支持的长度，且各参数应能被相应 tuple 的工厂函数构造。

## 配置与依赖

- 头文件依赖 Yggdrasil tuple、移动/转发工具及 cv/ref 移除类型特征。
- `std::tuple` 支持受 `YGGR_NO_CXX11_HDR_TUPLE` 控制，并且仅在可变参数模板路径中提供。
- 禁用可变参数模板时，参数个数上限受 `YGGR_TUPLE_LIMIT_LENGTH()` 配置约束。
