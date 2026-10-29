# `has_operator_pointer_member_access.hpp` 中文说明

## 用途

本头文件提供编译期类型特征，用于检测类类型是否具有成员访问运算符 `operator->`，以及判断类型是否为类指针或具有该运算符的类。

```cpp
#include <yggr/type_traits/has_operator_pointer_member_access.hpp>
```

## 类型特征

### `has_operator_pointer_member_access_class<T>`

检测移除 cv 限定和引用后的类类型是否具有可检测的 `operator->`。结果通过 `value` 提供。

### `has_operator_pointer_member_access<T>`

在类检测基础上，还将指向类类型的原生指针识别为支持指针式成员访问：

- 类类型：通过类的 `operator->` 检测逻辑判定。
- 指向类类型的原生指针：返回 `true`。
- 其他类型（包括指向内置类型的指针）：返回 `false`。

两个特征都在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/has_operator_pointer_member_access.hpp>

struct object
{
    void operator->();
};

struct plain
{
};

static_assert(
    yggr::has_operator_pointer_member_access_class<object>::value,
    "object defines operator->");
static_assert(
    !yggr::has_operator_pointer_member_access_class<plain>::value,
    "plain has no operator->");
static_assert(
    yggr::has_operator_pointer_member_access<object*>::value,
    "pointer to class is recognized");
static_assert(
    !yggr::has_operator_pointer_member_access<int*>::value,
    "pointer to non-class is not recognized");
```

## 检测实现与边界

- 类检测通过派生探测类型和回退 `operator->` 参与重载解析实现，是对运算符存在性的启发式检测。
- 类检测不验证 `operator->` 的返回类型是否为指针或代理对象，也不保证表达式 `value->member` 可用于某个具体成员。
- 对类指针的判断使用 `is_class_pointer`，仅接受指向类类型的原生指针；指向标量、`void` 或函数的指针不会被该分支识别。
- 该特征移除顶层 cv 限定和引用，但不负责检查对象生命周期、指针有效性或成员访问权限。
- 该特征可用于识别智能指针类等代理类型，但具体行为仍取决于其 `operator->` 实现。
