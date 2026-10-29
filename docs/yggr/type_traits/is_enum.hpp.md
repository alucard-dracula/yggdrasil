# `is_enum.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::is_enum<T>`，用于判断 `T` 是否为枚举类型。

```cpp
#include <yggr/type_traits/is_enum.hpp>
```

## 接口

```cpp
yggr::is_enum<T>::value
```

该特征将 `boost::is_enum<T>::value` 包装为 Boost.MPL 布尔类型，并通过 `value` 提供判断结果。类型在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/is_enum.hpp>

enum class status
{
    ready
};

struct record {};

static_assert(yggr::is_enum<status>::value, "status is an enum");
static_assert(!yggr::is_enum<record>::value, "record is not an enum");
```

## 注意事项

- 该特征只检查传入的类型形式；如需处理带 cv 限定或引用的类型，应按需要先使用相应的类型变换特征。
- 它只判断类型类别，不会提取枚举的底层类型，也不会判断枚举值的范围或语义。
- 实际枚举检测由 Boost.TypeTraits 提供。
