# `remove_cv_ref.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::remove_cv_ref<T>`，用于移除类型的引用以及顶层 `const`、`volatile` 限定。

```cpp
#include <yggr/type_traits/remove_cv_ref.hpp>
```

## 接口

```cpp
yggr::remove_cv_ref<T>::type
```

结果类型可通过嵌套类型 `type` 获取。该特征在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/remove_cv_ref.hpp>

typedef yggr::remove_cv_ref<const int&>::type plain_int;
typedef yggr::remove_cv_ref<volatile int&&>::type another_plain_int;
```

上述结果类型均为 `int`。

## 实现与兼容性

- 当 Boost 版本低于 `1.65.0` 时，Yggdrasil 组合使用 `boost::remove_reference` 与 `boost::remove_cv` 实现。
- Boost 版本不低于 `1.65.0` 时，直接使用 `boost::remove_cv_ref`。
- 两种分支提供相同的用途：先去除引用，再移除所得类型的顶层 cv 限定。

## 注意事项

- 此特征只移除引用和顶层 cv 限定，不会移除指针、数组维度或其他类型包装。
- 对 `const int*`，结果仍是 `const int*`；指向对象的 `const` 不属于指针本身的顶层限定。
- 该操作只改变编译期类型，不会创建或转换运行时对象。
