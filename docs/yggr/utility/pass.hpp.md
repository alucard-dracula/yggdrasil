# `pass.hpp` 中文说明

## 用途

本头文件在 `yggr::utility` 命名空间提供轻量级辅助函数 `pass`：接收一个 const 引用并原样返回该引用。它不修改、不复制参数，常用于明确地使用一个本来未被读取的变量，例如抑制编译器的未使用变量警告。

```cpp
#include <yggr/utility/pass.hpp>

int value = 42;
yggr::utility::pass(value);
```

## 接口

```cpp
template<typename T>
const T& pass(const T& t);
```

返回类型始终是 `const T&`，因此即使传入的是可修改对象，也不能通过 `pass` 的返回值修改它。由于参数按引用传递，调用本身不会创建 `T` 的副本；返回引用仍受原对象生命周期约束。

该函数使用 `YGGR_CONSTEXPR_OR_INLINE` 宏声明，以适配项目配置下的 constexpr/inline 支持。

## 使用示例

```cpp
std::unordered_map<int, int> values;
auto result = values.emplace(1, 2);

yggr::utility::pass(result); // 明确表达 result 在此处被有意保留但不进一步使用
```

## 注意事项

- `pass` 只是一个返回 const 引用的辅助函数，不会改变变量的生命周期，也不会延长临时对象的生命周期。
- 它不会把对象转换为其他类型，也不等同于移动或完美转发。
- 若需要保留可修改引用或转发值类别，应选择与该用途相符的接口，而不是使用此函数。
