# `fwd_arg_t.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::fwd_arg_t<T>`，根据 `T` 是否为类类型，选择适用于转发参数的类型形式。

```cpp
#include <yggr/type_traits/fwd_arg_t.hpp>
```

## 类型映射

`fwd_arg_t<T>` 继承自项目的 `yggr::traits`，最终类型由其嵌套类型 `type` 表示：

- 若 `T` 是类类型，映射到 `BOOST_FWD_REF(T)`，即由 Yggdrasil/Boost 移动兼容层定义的转发引用形式。
- 若 `T` 不是类类型，映射为 `T`，按值传递。

示例：

```cpp
#include <yggr/type_traits/fwd_arg_t.hpp>

typedef yggr::fwd_arg_t<int>::type int_arg_type;
typedef yggr::fwd_arg_t<my_class>::type class_arg_type;
```

类型特征在 `yggr::type_traits` 命名空间中定义，并通过 `yggr` 命名空间导出。该头文件包含 `yggr/move/move.hpp`，以获得项目的转发引用兼容定义。

## 与 `const_arg_t` 的区别

`fwd_arg_t<T>` 面向转发场景：类类型使用 `BOOST_FWD_REF(T)`，可按移动兼容层支持的规则接收参数；`const_arg_t<T>` 则将类类型映射为 `const T&`。选择哪种特征取决于函数是否需要转发或修改/消费参数。

## 注意事项

- 是否为类类型由 `boost::is_class<T>` 判断；枚举、标量、指针等非类类型按值映射。
- 此类型特征只提供参数类型，不会自动实现完美转发，也不管理对象生命周期或所有权。
- 实际引用行为受项目的 `BOOST_FWD_REF` 配置及编译器语言特性支持影响；使用时应遵循相应的移动/转发接口约定。
