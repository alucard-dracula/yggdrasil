# `native_t.hpp` 中文说明

## 用途

本头文件提供一组编译期类型特征，用于从类型 `T` 中提取规范化的底层类型，并基于该类型构造指针、引用及移动引用类型。

```cpp
#include <yggr/type_traits/native_t.hpp>
```

## 类型特征

所有特征均在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。结果类型由嵌套类型 `type` 获取。

| 特征 | 结果类型 |
| --- | --- |
| `native_t<T>` | 规范化后的底层类型 |
| `native_pointer_t<T>` | `native_t<T>::type*` |
| `native_const_pointer_t<T>` | 指向规范化底层类型的 const 指针 |
| `native_ref_t<T>` | 规范化底层类型的左值引用 |
| `native_cref_t<T>` | 规范化底层类型的 const 左值引用 |
| `native_rvref_t<T>` | 规范化底层类型的 Boost 移动兼容右值引用形式 |

`native_t<T>` 的实现会先移除数组的所有维度，再移除 volatile、引用、指针、const 和 cv 限定。因此它适合提取经这些限定包装后的底层类型。

## 示例

```cpp
#include <yggr/type_traits/native_t.hpp>

typedef yggr::native_t<const int*>::type native_int;
typedef yggr::native_ref_t<const int*>::type native_int_ref;
typedef yggr::native_pointer_t<int[4]>::type native_int_pointer;
```

上述类型分别为 `int`、`int&` 和 `int*`。

## 注意事项

- 这些特征只进行编译期类型变换，不会创建对象、分配存储或执行指针解引用。
- `native_t` 会移除指针和数组维度；例如多级指针经过处理后不会保留指针层级。若需要保留这些结构，请使用其他适当的类型变换。
- `native_const_pointer_t<T>` 表示指向 const 底层对象的指针（`const native_t<T>::type*`），而不是“指针本身为 const”。
- 右值引用形式由 `BOOST_RV_REF` 提供，具体行为受 Boost/Yggdrasil 移动兼容配置和语言标准影响。
- 底层规范化逻辑由 [`detail/native_t_detail.hpp`](./detail/native_t_detail.hpp) 实现；被移除的类型限定与数组规则由 [`detail/original_t_detail.hpp`](./detail/original_t_detail.hpp) 补充定义。
