# `traits.hpp` 中文说明

## 用途

本头文件定义通用类型包装模板 `yggr::traits<T>`，用于通过嵌套类型 `type` 暴露模板参数 `T`。

```cpp
#include <yggr/type_traits/traits.hpp>
```

## 接口

```cpp
template<typename T>
struct yggr::traits
{
    typedef T type;
};
```

该模板定义于 `yggr::type_traits` 命名空间，并通过 `yggr` 命名空间导出。因此可使用 `yggr::traits<T>::type` 或 `yggr::type_traits::traits<T>::type`。

## 示例

```cpp
#include <yggr/type_traits/traits.hpp>

typedef yggr::traits<const int&>::type original_type;
```

`original_type` 与传入的 `const int&` 完全相同；此模板不会去除引用或 cv 限定。

## 注意事项

- `traits<T>` 本身不执行类型变换，只提供一个便于泛型代码访问的 `type` 名称。
- 对于去除 cv、引用或指针等需求，应使用对应的类型变换特征，而不是依赖此模板。
- 该头文件仅包含 Yggdrasil 基础定义，不依赖 Boost.TypeTraits。
