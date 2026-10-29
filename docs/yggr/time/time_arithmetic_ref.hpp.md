# `time_arithmetic_ref.hpp` 中文说明

## 用途

本头文件为时间相关计算提供浮点除法的算术引用包装器。它定义专用操作标签，并提供可写引用和常量引用的包装函数，底层实现复用 `yggr::math::arithmetic_ref`。

```cpp
#include <yggr/time/time_arithmetic_ref.hpp>
```

## 操作标签

```cpp
struct tag_float_divides {};
typedef tag_float_divides tag_float_divides_type;
```

`tag_float_divides_type` 是浮点除法操作标签的别名，用于在 `arith_reference_wrapper` 中标识算术操作类别。

## 可写算术引用

```cpp
template<typename T>
math::arith_reference_wrapper<tag_float_divides_type, f64, T>
time_float_divides_ref(T& value);

template<typename R, typename T>
math::arith_reference_wrapper<tag_float_divides_type, R, T>
time_float_divides_ref(T& value);
```

这两个重载为 `value` 创建浮点除法引用包装器：

- 不显式指定结果类型时，默认结果类型为 `f64`。
- 指定模板参数 `R` 时，包装器使用 `R` 作为结果类型。
- 包装器引用原对象，不会复制 `value`；对象生命周期必须覆盖包装器的使用期。

## 常量算术引用

```cpp
template<typename T>
math::arith_reference_wrapper<tag_float_divides_type, f64, const T>
time_float_divides_cref(const T& value);

template<typename R, typename T>
math::arith_reference_wrapper<tag_float_divides_type, R, const T>
time_float_divides_cref(const T& value);
```

这两个重载为常量对象创建只读引用包装器，结果类型默认为 `f64`，也可通过模板参数指定为 `R`。

## 使用示意

```cpp
yggr::f64 value = 2.0;

auto ref = yggr::time::time_float_divides_ref(value);
auto cref = yggr::time::time_float_divides_cref(value);
```

包装器本身描述算术引用及其操作标签；具体除法表达式的求值和支持类型由 `yggr::math::arithmetic_ref` 的相关实现决定。

## 平台兼容

头文件针对特定 Borland 编译器版本定义 `YGGR_REF_CONST` 为空，以规避该编译器兼容问题；其他平台将其定义为 `const`。宏仅在本头文件内部使用，随后会取消定义。
