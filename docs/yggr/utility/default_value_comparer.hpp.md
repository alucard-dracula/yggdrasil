# `default_value_comparer.hpp` 中文说明

## 用途

本头文件提供一组比较函数，将传入值与 `T` 的默认构造值进行比较。

```cpp
#include <yggr/utility/default_value_comparer.hpp>
```

函数定义于 `yggr::utility` 命名空间，并通过 `yggr` 命名空间导出。

## 接口

| 函数 | 判定逻辑 |
| --- | --- |
| `equal_to_default_value(value)` | `value == T{}` |
| `not_equal_to_default_value(value)` | `!(value == T{})` |
| `less_default_value(value)` | `value < T{}` |
| `less_equal_default_value(value)` | `!(T{} < value)` |
| `greater_default_value(value)` | `!(T{} < value)` 的逻辑否定，即 `T{} < value` |
| `greater_equal_default_value(value)` | `!(value < T{})` |

实际默认值由 `create_object<T>()` 创建，要求 `T` 可默认构造，并支持相应比较运算符。

## 示例

```cpp
#include <yggr/utility/default_value_comparer.hpp>

int value = 0;
bool is_default = yggr::equal_to_default_value(value);
bool is_positive = yggr::greater_default_value(value);
```

## 注意事项

- 这些函数比较的是 `T` 的默认构造对象，而不是泛指数值 `0`；自定义类型的默认值由其构造函数定义。
- `equal_to_default_value`、`less_default_value` 等要求 `T` 提供相应比较运算符。
- `greater_default_value` 的实现等价于 `T{} < value`，而 `greater_equal_default_value` 等价于 `!(value < T{})`。
- 如果类型的比较运算不满足通常的全序关系，互补函数之间未必呈现数学上预期的对称关系。
