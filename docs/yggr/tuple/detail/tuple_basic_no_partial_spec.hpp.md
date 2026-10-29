# `tuple_basic_no_partial_spec.hpp` 中文说明

## 用途

本头文件是 Yggdrasil Tuple 的兼容实现，供编译器不支持类模板偏特化时使用。它在 `yggr::tuples` 中定义链式 `cons` 和定长参数的 `tuple`，并提供元素访问、构造、赋值、转换、交换以及 `make_tuple()` / `tie()` 等辅助接口。

```cpp
#include <yggr/tuple/tuple.hpp>
```

通常应包含公开入口 `yggr/tuple/tuple.hpp`，由该头文件根据 `BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION` 配置选择本实现，而不是直接包含 `detail` 头文件。

## 表示方式与配置限制

- `cons<Head, Tail>` 以 `head` 和 `tail` 递归保存异构元素；最后一个尾部使用 `null_type`。
- `tuple<T...>` 通过 `detail::map_tuple_to_cons` 将类型参数映射为嵌套的 `cons`，并继承该链式表示。
- tuple 参数数量受 `YGGR_TUPLE_LIMIT_LENGTH()` 配置限制。由于缺少模板偏特化，部分类型和函数重载由 Boost.Preprocessor 宏按配置上限生成。
- 实现复用 Boost.Tuple 的 `null_type`、类型访问、长度及部分 tie 辅助类型，以兼容 Boost.Tuple。

## 主要类型与访问接口

### `cons<HT, TT>`

公开保存 `head` 和 `tail`，并提供 `get_head()`、`get_tail()` 的可变及常量访问重载。它支持默认构造、由头尾构造、复制/移动构造，以及从其他 `cons` 或 Boost.Tuple `cons` 构造。

### `tuple<T...>`

继承由元素类型构建的 `cons` 链。支持默认构造、按元素构造、由 `cons` 或 Boost.Tuple 类型构造，以及复制和移动。`inherited` 别名表示底层链式类型。

### 元素访问和元信息

- `get_head()` / `get_tail()` 访问链表的首元素和尾部。
- `get_inherited()` 获取 tuple 的底层继承链。
- `detail::get_class<N>` 递归定位第 `N` 个元素；外围 tuple 设施通过 `get` 暴露索引访问。
- `tuple_head_t<Tuple>` 和 `tuple_tail_t<Tuple>` 分别提取链式类型的头部和尾部类型。
- `element`、`length` 等接口复用 Boost.Tuple 的定义。

## 构造辅助函数

```cpp
yggr::tuples::make_tuple(values...);
yggr::tuples::tie(references...);
```

- `make_tuple()` 根据传入参数构造对应的 `yggr::tuples::tuple`，可接受的参数数量受 tuple 配置上限约束。
- `tie()` 生成由 `assign_to_pointee` 包装的 tuple，用于将 tuple 元素与外部变量引用关联。
- `boost::tuples::ignore` 被引入 `yggr::tuples`，可用于忽略 tie 解包中的指定位置。

## 转换、赋值与交换

本实现通过 `cons_conv_helper`、`cons_conv()` 和 `cons_swap()` 在 Yggdrasil `cons` 与 Boost `cons` 之间递归转换或交换元素。对 tuple 链的转换和交换会在编译期检查元素数量是否匹配；`std::pair` 相关转换要求目标或来源为二元结构。

`cons` 与 `tuple` 提供相应的赋值和 `swap()` 重载，支持本库类型与 Boost.Tuple 之间的互操作。头文件也提供 `yggr::tuples::swap` 及对应的 Boost.Tuple 扩展和命名空间导入。

## 使用示例

```cpp
#include <yggr/tuple/tuple.hpp>

int count = 4;
std::string name = "item";

yggr::tuples::tuple<int, std::string> value =
    yggr::tuples::make_tuple(count, name);

yggr::tuples::tuple<int&, std::string&> refs =
    yggr::tuples::tie(count, name);
```

具体头文件路径和命名空间以项目公开入口 `yggr/tuple/tuple.hpp` 为准；本文件为该入口在特定编译器能力配置下使用的底层实现。

## 实现注意事项

- 这是为不支持模板偏特化的旧编译器准备的兼容分支，类型和重载集合依赖预处理器生成及编译配置。
- 跨类型转换、赋值和交换通常要求双方 tuple 长度相等；不满足时通过静态断言报错。
- 源码中若干 `std::pair` 转换重载存在可疑实现（例如返回 `*this`、拼写异常的转发调用及未见定义的类型别名）。这些路径应在目标编译器上实例化验证；不要仅凭其他 `cons` 转换重载的行为推断其可用性。
- 本头文件属于 `detail` 实现层，不建议业务代码直接依赖其内部辅助模板或宏生成细节。
