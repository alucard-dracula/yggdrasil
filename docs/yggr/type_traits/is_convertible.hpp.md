# `is_convertible.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::is_convertible<T, U>`，用于判断表达式类型 `T` 是否可隐式转换为类型 `U`。

```cpp
#include <yggr/type_traits/is_convertible.hpp>
```

## 接口

```cpp
yggr::is_convertible<From, To>::value
```

该特征继承自 Boost.MPL 布尔类型，并提供 `value` 和 `type`。若从 `From` 到 `To` 的隐式转换可行，`value` 为 `true`；否则为 `false`。

## 示例

```cpp
#include <yggr/type_traits/is_convertible.hpp>

static_assert(yggr::is_convertible<int, double>::value,
              "int converts to double");
static_assert(!yggr::is_convertible<double*, int*>::value,
              "unrelated pointers do not convert");
```

## 实现与兼容性

- 在 MSVC 版本宏 `_MSC_VER >= 1400` 时，使用编译器内建检测 `__is_convertible_to`。
- 其他环境使用基于重载解析和 `sizeof` 的 SFINAE 检测实现。
- 两种实现都以编译期方式判断隐式可转换性，不会执行实际的转换或构造对象。

## 注意事项

- 此特征检查的是隐式可转换性，不等同于 `static_cast` 是否可用。
- 结果受目标编译器对转换规则及相关类型特征的实现影响。
- 对引用、数组、函数等特殊类型的判断应结合具体类型形式验证；本特征不会自动移除 `const`、`volatile` 或引用限定。
