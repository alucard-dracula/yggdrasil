# `pair_compare.hpp` 中文说明

## 用途

本头文件为 `std::pair` 提供跨元素类型的比较运算符重载，用于兼容较旧的语言标准或特定编译器配置。它包含 `<utility>`，不定义新的容器或包装类型。

```cpp
#include <yggr/utility/pair_compare.hpp>
```

## 启用条件

仅当以下条件之一成立时，才会定义这些重载：

- `YGGR_CPP_VERSION` 低于 C++11；
- 编译器是 GCC，且 `YGGR_CPP_VERSION` 低于 C++20。

此外，GCC/旧标准兼容分支下，Clang 会跳过这些重载。满足启用条件并且未跳过时，头文件在 `std` 命名空间中提供比较运算符模板。

## 比较行为

重载支持元素类型不同的两个 `std::pair`，并逐项比较其 `first` 和 `second`：

- `==`：两项分别相等。
- `!=`：两项不同时相等。
- `<`：按字典序比较；先比较 `first`，只有第一项相等时才比较 `second`。
- `<=`：等价于反向 `<` 不成立。
- `>`：等价于反向 `<`。
- `>=`：等价于正向 `<` 不成立。

示例：

```cpp
#include <yggr/utility/pair_compare.hpp>
#include <utility>

std::pair<int, short> a(1, 2);
std::pair<long, int> b(1, 3);

bool ordered = a < b; // true：first 相等，比较 second
```

## 注意事项

- 头文件只在指定的兼容条件下增加重载；在其他编译器或语言标准配置下，比较行为由标准库提供。
- 为满足这些旧环境的兼容性，此文件把重载声明在 `std` 命名空间中。不要在应用代码中自行向 `std` 添加其他声明；直接包含此兼容头文件即可。
- 比较要求相应元素类型支持所用的相等或小于比较操作。
