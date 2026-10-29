# `original_t.hpp` 中文说明

## 用途

本头文件提供一组编译期类型特征，用于移除类型的引用、指针及 cv 限定，并基于结果构造指针、引用和移动引用类型。

```cpp
#include <yggr/type_traits/original_t.hpp>
```

## 类型特征

所有特征均在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。结果类型由嵌套类型 `type` 获取。

| 特征 | 结果类型 |
| --- | --- |
| `original_t<T>` | 移除一层指针、引用及 cv 限定后的类型 |
| `original_pointer_t<T>` | 指向 `original_t<T>::type` 的指针 |
| `original_const_pointer_t<T>` | 指向 const `original_t<T>::type` 的指针 |
| `original_ref_t<T>` | `original_t<T>::type` 的左值引用 |
| `original_cref_t<T>` | const `original_t<T>::type` 的左值引用 |
| `original_rvref_t<T>` | Boost 移动兼容的右值引用形式 |

`original_t<T>` 的实现依次移除 volatile、引用、一层指针、const 和 cv 限定。

## 示例

```cpp
#include <yggr/type_traits/original_t.hpp>

typedef yggr::original_t<const int*>::type original_int;
typedef yggr::original_ref_t<const int*>::type original_int_ref;
typedef yggr::original_pointer_t<int&>::type original_int_pointer;
```

上述类型分别为 `int`、`int&` 和 `int*`。

## 注意事项

- 该特征移除的是一层指针；例如 `int**` 经处理后仍为 `int*`，不会递归去除所有指针层级。
- `original_t<T>` 不移除数组维度；数组类型会保留数组结构，除非类型本身经过了其他变换。
- 这些特征只进行编译期类型变换，不会解引用指针或创建对象。
- `original_const_pointer_t<T>` 表示指向 const 目标对象的指针，不表示指针变量自身为 const。
- 移动引用形式由 `BOOST_RV_REF` 提供，具体行为受 Boost/Yggdrasil 移动兼容配置和语言标准影响。
