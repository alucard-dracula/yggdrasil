# `is_class_pointer.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::is_class_pointer<T>`，用于判断 `T` 是否为指向类类型的原生指针。

```cpp
#include <yggr/type_traits/is_class_pointer.hpp>
```

## 接口

```cpp
yggr::is_class_pointer<T>::value
```

该特征继承自 Boost.MPL 布尔类型。仅当以下条件同时成立时，`value` 为 `true`：

1. `T` 是原生指针类型；
2. 移除指针和被指向类型的 cv 限定后，目标类型是类类型。

该特征定义于 `yggr::type_traits` 命名空间，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/is_class_pointer.hpp>

struct sample {};

static_assert(yggr::is_class_pointer<sample*>::value,
              "pointer to class is recognized");
static_assert(yggr::is_class_pointer<const sample*>::value,
              "pointer to const class is recognized");
static_assert(!yggr::is_class_pointer<int*>::value,
              "pointer to scalar is not a class pointer");
static_assert(!yggr::is_class_pointer<sample>::value,
              "class type itself is not a pointer");
```

## 注意事项

- 该特征只检查类型类别，不检查指针是否为空、是否指向有效对象或对象生命周期。
- 指向标量、`void` 或函数类型的原生指针不会被识别为类指针。
- 该特征不等同于智能指针检测；类包装指针不是原生指针类型。
