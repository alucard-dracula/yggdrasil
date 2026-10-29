# `is_pair.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::is_pair<T>`，用于判断类型是否具有 pair 结构所需的 `first_type` 和 `second_type`，并显式识别 `std::pair`。

```cpp
#include <yggr/type_traits/is_pair.hpp>
```

## 判定规则

`yggr::is_pair<T>::value` 按以下规则确定：

- 对一般类型，只有当 `yggr::has_first_t<T>` 和 `yggr::has_second_t<T>` 均为 `true` 时，结果才为 `true`。
- 对 `std::pair<T1, T2>`，提供显式特化，结果为 `true`。

该特征在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/is_pair.hpp>
#include <utility>

struct pair_like
{
    typedef int first_type;
    typedef double second_type;
};

struct value_only
{
    typedef int first_type;
};

static_assert(yggr::is_pair<pair_like>::value,
              "both nested types are detected");
static_assert(!yggr::is_pair<value_only>::value,
              "second_type is missing");
static_assert(yggr::is_pair<std::pair<int, double> >::value,
              "std::pair is explicitly recognized");
```

## 注意事项

- 对一般类型，该特征仅检查两个嵌套类型是否存在且能被各自的检测特征处理；不验证类型的实际存储布局、成员或 pair 行为。
- `std::pair` 的显式特化按模板类型匹配；带 cv 限定或引用的 `std::pair` 类型不会自动先移除限定。
- 此头文件包含 `<utility>` 以支持对 `std::pair` 的识别。
