# `reference_trans.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::reference_trans<T>`，用于将原生引用类型转换为 Boost `reference_wrapper` 类型；非引用类型保持不变。

```cpp
#include <yggr/type_traits/reference_trans.hpp>
```

## 接口与转换规则

转换后的类型通过 `reference_trans<T>::type` 获取：

- 若 `T` 是引用类型，结果为 `boost::reference_wrapper<remove_reference<T>::type>`。
- 若 `T` 不是引用类型，结果为 `T`。

该特征定义于 `yggr::type_traits` 命名空间，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/reference_trans.hpp>

typedef yggr::reference_trans<int&>::type wrapped_reference;
typedef yggr::reference_trans<const int&>::type wrapped_const_reference;
typedef yggr::reference_trans<int>::type unchanged_type;
```

上述类型分别为 `boost::reference_wrapper<int>`、`boost::reference_wrapper<const int>` 和 `int`。

## 注意事项

- 这是类型层面的转换，不会构造 `boost::reference_wrapper` 对象，也不会自动绑定到实际变量。
- 引用被移除后，引用所指对象的 `const` 等类型限定会保留在包装器的目标类型中。
- 非引用类型（包括指针类型）会原样保留。
- 包装器不拥有被引用对象；使用时必须确保目标对象的生命周期足够长。
