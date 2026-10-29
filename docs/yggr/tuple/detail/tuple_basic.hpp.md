# `tuple_basic.hpp` 中文说明

## 用途

本头文件是 Yggdrasil Tuple 的基础实现，定义 `yggr::tuples::cons`、`yggr::tuples::tuple` 及其构造、访问、转换和交换机制，并与 Boost.Tuple 互操作。它由公开入口 `yggr/tuple/tuple.hpp` 在编译器支持模板偏特化时引入；业务代码通常应包含公开入口，而不是直接依赖本 `detail` 头文件。

```cpp
#include <yggr/tuple/tuple.hpp>
```

## 数据表示

- `null_type` 复用 `boost::tuples::null_type`，表示链式 tuple 的结束位置。
- `cons<HT, TT>` 以 `head` 和 `tail` 递归保存异构元素。
- `tuple<T...>` 通过 `detail::map_tuple_to_cons` 将类型参数映射为嵌套 `cons` 链，并继承该底层表示。
- tuple 参数数量受 `YGGR_TUPLE_LIMIT_LENGTH()` 配置限制。

`cons` 的 `stored_head_type` 使用 `wrap_non_storeable_type` 处理 `void`、函数等不能直接作为数据成员存储的类型。这允许某些仅用于类型表达的 tuple 类型被声明，但不意味着它们都能构造为运行时对象。

## 类型别名与访问接口

### 类型信息

- `tuple_head_t<Tuple>::type`：取得链式类型的头部类型。
- `tuple_tail_t<Tuple>::type`：取得链式类型的尾部类型。
- `element<N, Tuple>`、`length<Tuple>` 和 `access_traits` 复用 Boost.Tuple 的类型元信息接口。
- `tuple<T...>::inherited`：tuple 对应的底层 `cons` 链类型。

### 元素访问

- `get_head(value)` / `get_tail(value)`：访问 `cons` 或 `std::pair` 的首元素、尾部/第二元素。
- `get_inherited(value)`：取得 tuple 对象的底层链式基类引用。
- `cons::get<N>()`：通过 Boost.Tuple 的索引 `get` 访问指定元素。
- `boost::tuples::get<N>(yggr_tuple)`：本头文件扩展 Boost.Tuple 的 `get`，使其可访问 Yggdrasil `cons`。

访问返回类型通过 `access_traits` 处理引用元素和不可存储类型；常量对象与可变对象具有相应的常量/可变访问形式。

## 构造与辅助函数

```cpp
yggr::tuples::make_tuple(values...);
yggr::tuples::tie(references...);
```

- `make_tuple()` 根据 `make_tuple_traits` 对参数类型进行映射并返回对应 tuple；零参数重载返回空 tuple。
- `tie()` 将参数作为引用关联到 tuple 元素，可用于将 tuple 中的值赋回变量。
- `ignore` 从 Boost.Tuple 引入，允许 `tie()` 中忽略指定位置。
- `tuple<>` 是空 tuple 特化，继承 `null_type`，并提供空 tuple 的构造、赋值、转换和交换行为。

## 转换、赋值与交换

`cons_conv_helper` 定义 Boost `cons` 与 Yggdrasil `cons` 之间的递归类型映射；`cons_conv()` 执行实际值转换，也支持 `std::pair` 与二元 `cons` 之间转换。移动来源会尝试移动元素，常量来源则复制元素。

`cons_swap()` 递归交换两个 `cons` 链中的对应元素。跨类型转换和交换会在编译期检查 tuple 长度是否一致；`std::pair` 转换要求链长度为 2。

`cons` 和 `tuple` 提供相应的复制/移动构造、赋值和 `swap()` 重载，并为 Boost.Tuple 类型提供互操作重载。`yggr::tuples::swap` 还被导入 `std` 和 `boost` 命名空间，以配合项目的交换调用方式。

## 使用示例

```cpp
#include <yggr/tuple/tuple.hpp>

int count = 4;
std::string name = "item";

yggr::tuples::tuple<int, std::string> value =
    yggr::tuples::make_tuple(count, name);

int current_count = yggr::tuples::get<0>(value);
yggr::tuples::tuple<int&, std::string&> refs =
    yggr::tuples::tie(count, name);
```

索引访问和 `tie()` 的可用形式还受相应 tuple 工具头文件提供的接口影响。

## 实现与配置说明

- 对可变模板的支持只用于 `boost_tie_converter` 等特定路径；主要 `tuple` 参数列表和部分重载仍由 Boost.Preprocessor 根据 `YGGR_TUPLE_LIMIT_LENGTH()` 生成。
- 本文件声明并实现 Boost.Tuple 的 `get` 扩展、空 Yggdrasil tuple 的 `length` 特化和跨类型 `swap` 扩展。
- `boost_tie_converter` 会将 Yggdrasil tuple 的元素引用重新绑定到 Boost.Tuple tie 结果中。
- 不支持模板偏特化的编译器使用另一个兼容实现 `tuple_basic_no_partial_spec.hpp`；本文件适用于模板偏特化可用的配置。

## 注意事项

- 本文件位于 `detail` 层；优先包含 `yggr/tuple/tuple.hpp`，让项目配置选择合适实现。
- 跨类型转换要求元素数量兼容，并且相应元素赋值/构造操作可用；不满足条件时可能在模板实例化或静态断言处报错。
- `tie()` 产生的 tuple 保存对原变量的引用，要求被引用对象在 tuple 使用期间保持有效。
- `void`、函数等非存储类型可参与部分类型表达，但对应 tuple 对象不能因此自动成为可实例化、可赋值的运行时对象。
