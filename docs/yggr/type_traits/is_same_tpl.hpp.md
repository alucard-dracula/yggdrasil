# `is_same_tpl.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::is_same_tpl<TL, TR>`，用于判断两个具体类型是否由同一个类模板实例化而来，而不要求它们的模板实参相同。

```cpp
#include <yggr/type_traits/is_same_tpl.hpp>
```

## 接口与判定规则

```cpp
yggr::is_same_tpl<TL, TR>::value
```

判定方式如下：

- 如果 `TL` 与 `TR` 是完全相同的类型，结果为 `true`。
- 如果它们分别是同一个类型模板的实例，例如 `std::vector<int>` 与 `std::vector<double>`，结果也为 `true`。
- 若模板来源不同，例如 `std::vector<int>` 与 `std::list<int>`，结果为 `false`。

该特征在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/is_same_tpl.hpp>
#include <list>
#include <vector>

static_assert(
    yggr::is_same_tpl<std::vector<int>, std::vector<double> >::value,
    "same class template");
static_assert(
    !yggr::is_same_tpl<std::vector<int>, std::list<int> >::value,
    "different class templates");
static_assert(
    yggr::is_same_tpl<std::vector<int>, std::vector<int> >::value,
    "identical types");
```

## 实现与兼容性

- 支持可变模板时，模板模板参数可匹配不同数量的类型实参。
- 禁用可变模板时，头文件使用预处理器按配置的模板参数上限展开检测特化。
- 该实现适用于以类型参数为参数的类模板实例。

## 注意事项

- 该特征关注模板来源，不表示两个实例具有相同的元素类型、布局、接口或行为。
- 本实现不会比较模板的非类型实参或模板模板实参；其匹配范围由代码中的类型参数模板模板形参决定。
- `TL` 和 `TR` 应直接传入要比较的类型；带 cv 限定或引用的类型不会先被自动规范化，除非两侧类型本身完全相同。
