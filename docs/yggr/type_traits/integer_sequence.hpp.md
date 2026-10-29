# `integer_sequence.hpp` 中文说明

## 用途

本头文件是 Yggdrasil 整数序列工具的公开入口，供 C++11 及以上、且启用了可变模板的构建使用。它包含 `yggr/type_traits/detail/integer_sequence.hpp`，由该实现头文件提供整数序列类型及其生成工具。

```cpp
#include <yggr/type_traits/integer_sequence.hpp>
```

若构建定义了 `YGGR_NO_CXX11_VARIADIC_TEMPLATES`，该头文件会以预处理错误提示不支持当前配置。

## 提供的接口

具体实现根据标准库与编译器能力，在 `yggr` 命名空间提供以下接口：

- `integer_sequence<T, I...>`：保存类型为 `T` 的编译期整数序列；`T` 必须是整数类型，并可通过 `value_type` 和 `size()` 查询元素类型与序列长度。
- `make_integer_sequence<T, N>`：生成包含 `N` 个整数的序列，值依次为 `0` 到 `N - 1`。
- `index_sequence<I...>`：以 `std::size_t` 为元素类型的整数序列别名。
- `make_index_sequence<N>`：生成索引 `0` 到 `N - 1` 的索引序列。
- `index_sequence_for<T...>`：按类型参数包的长度生成对应索引序列。

当标准库支持且 Yggdrasil 配置允许时，接口会转发到 `std` 中的实现；其他配置下则使用 Yggdrasil 实现。若标准库设施不可用，整数序列生成器采用递归拆分与拼接方式实现。

## 示例

```cpp
#include <yggr/type_traits/integer_sequence.hpp>

typedef yggr::make_index_sequence<3> indices;
static_assert(indices::size() == 3, "three indices are generated");

typedef yggr::make_integer_sequence<int, 4> values;
static_assert(values::size() == 4, "four values are generated");
```

## 注意事项

- `N` 必须非负；负值会触发编译期断言。
- `integer_sequence` 的元素是模板参数，不是运行时容器，不占用运行时存储。
- 本头文件只负责包含实现；标准库转发、配置分支和生成逻辑详见 [`detail/integer_sequence.hpp`](./detail/integer_sequence.hpp)。
