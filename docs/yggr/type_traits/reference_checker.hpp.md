# `reference_checker.hpp` 中文说明

## 用途

本头文件定义编译期引用检测特征，用于区分 C++ 原生引用与 Boost `reference_wrapper` 包装对象。

```cpp
#include <yggr/type_traits/reference_checker.hpp>
```

## 类型特征

以下特征定义于 `yggr::type_traits` 命名空间，并通过 `yggr` 命名空间导出：

| 特征 | 判定规则 |
| --- | --- |
| `is_reference<T>` | 若 `T` 是原生引用类型或 Boost `reference_wrapper` 类型，则为 `true`。 |
| `is_nude_reference<T>` | 仅当 `T` 是 C++ 原生引用类型时为 `true`。 |

两者均通过继承的 `value` 提供编译期布尔结果。

## 示例

```cpp
#include <yggr/type_traits/reference_checker.hpp>
#include <boost/ref.hpp>

static_assert(yggr::is_reference<int&>::value,
              "native reference is recognized");
static_assert(yggr::is_nude_reference<const int&>::value,
              "native reference is nude reference");
static_assert(yggr::is_reference<boost::reference_wrapper<int> >::value,
              "reference_wrapper is recognized as reference");
static_assert(!yggr::is_nude_reference<boost::reference_wrapper<int> >::value,
              "reference_wrapper is not a native reference");
```

## 注意事项

- `is_reference` 的命名范围比标准库的原生引用检测更广：它也把 `boost::reference_wrapper<T>` 视为引用包装形式。
- 这些特征只检查类型类别，不会获取被引用对象，也不延长对象生命周期。
- 对 `const`、`volatile` 等限定形式的检测遵循 Boost.TypeTraits 对传入类型的直接判定；如需其他语义，应先按需要规范化类型。
