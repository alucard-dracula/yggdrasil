# `has_operator_function.hpp` 中文说明

## 用途

本头文件提供两个编译期类型特征，用于检测函数对象的 `operator()` 或判断类型是否为函数类型/函数指针：

```cpp
#include <yggr/type_traits/has_operator_function.hpp>
```

## 类型特征

### `has_operator_function_class<F>`

检测经 cv 限定和引用移除后的类类型 `F` 是否具有可检测的成员 `operator()`。结果通过 `value` 提供。

```cpp
yggr::has_operator_function_class<Functor>::value
```

### `has_operator_function<F>`

根据类型类别执行检测：

- 对类类型，检测其是否具有 `operator()`。
- 对非类类型，移除指针后检查其是否为函数类型。因此函数类型及函数指针可得到 `true`，普通对象指针不会因本检测而得到 `true`。

```cpp
yggr::has_operator_function<Functor>::value
yggr::has_operator_function<void (*)(int)>::value
```

两个特征都在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/has_operator_function.hpp>

struct functor
{
    void operator()(int) const {}
};

struct plain
{
};

static_assert(yggr::has_operator_function_class<functor>::value,
              "functor has operator()");
static_assert(!yggr::has_operator_function_class<plain>::value,
              "plain has no operator()");
static_assert(yggr::has_operator_function<void (*)(int)>::value,
              "function pointer is recognized");
```

## 检测实现与边界

- 类类型检测通过构造派生类型并对 `operator()` 进行重载解析，判断它是否与用于探测的回退调用运算符相冲突。
- 这是对调用运算符存在性的启发式检测，不会验证特定参数列表、返回类型或表达式是否可调用。
- 重载、模板化或其他复杂形式的 `operator()` 可能影响重载解析结果；应在实际目标类型上验证。
- `has_operator_function` 不检测成员函数指针；成员函数指针不是普通函数指针，也不能仅凭该特征判断为可调用对象。
- cv 限定和引用在检测前会被移除。
