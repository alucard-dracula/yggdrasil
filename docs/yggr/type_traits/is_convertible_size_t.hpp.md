# `is_convertible_size_t.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::is_convertible_size_t<T>`，用于判断去除顶层 cv 限定和引用后的类型是否可转换为 `std::size_t`，或该类型是否为枚举。

```cpp
#include <yggr/type_traits/is_convertible_size_t.hpp>
```

## 接口与判定规则

```cpp
yggr::is_convertible_size_t<T>::value
```

结果为以下两个条件的逻辑或：

1. `T` 移除引用与 cv 限定后可转换为 `std::size_t`；
2. `T` 移除引用与 cv 限定后是枚举类型。

因此，枚举会被显式接受，即使其类型特征转换检查本身未将其判定为可转换。该特征位于 `yggr::type_traits` 命名空间，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/is_convertible_size_t.hpp>

enum class item_count : unsigned int
{
    one = 1
};

static_assert(yggr::is_convertible_size_t<unsigned int>::value,
              "unsigned integer is convertible to size_t");
static_assert(yggr::is_convertible_size_t<const std::size_t&>::value,
              "cv/ref qualifiers are removed");
static_assert(yggr::is_convertible_size_t<item_count>::value,
              "enum types are accepted");
```

## 注意事项

- 这是编译期类型判定，不会执行实际数值转换，也不验证数值是否为非负数或是否在 `std::size_t` 的表示范围内。
- 枚举类型一律由第二个条件接受；该特征不检查枚举底层值是否适合特定长度、索引或容量用途。
- 可转换性由 `yggr::is_convertible` 实现，其具体检测方式会根据编译器有所不同。
