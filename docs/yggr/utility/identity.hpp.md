# `identity.hpp` 中文说明

## 用途

本头文件定义函数对象 `yggr::utility::identity`，调用时原样返回输入值，并尽可能保留其值类别和引用属性。

```cpp
#include <yggr/utility/identity.hpp>
```

## 接口与行为

- 支持右值引用的配置下，调用运算符使用转发引用参数，并通过 `std::forward` 返回，保留左值或右值属性。
- 不支持右值引用的配置下，提供 `const T&` 和 `T&` 两个重载，分别返回对应引用。
- `identity::result<Signature>::type` 提供部分调用签名的结果类型定义。
- `typedef void is_transparent;` 声明透明比较器标记，可供支持透明查找的泛型关联容器识别。

## 示例

```cpp
#include <yggr/utility/identity.hpp>

int value = 10;
yggr::utility::identity id;

int& ref = id(value);
```

在启用右值引用的配置中，右值调用也会保持右值类别，适合与转发代码组合使用。

## 注意事项

- `identity` 不复制或转换输入值；返回结果与输入对象共享同一对象。
- 返回引用不得超过原对象的生命周期。
- `is_transparent` 仅提供类型标记；容器是否支持异构查找还取决于容器及其比较器的实现。
- 本头文件使用 `std::forward`，需确保构建配置中包含适当的标准库声明与 C++ 特性支持。
