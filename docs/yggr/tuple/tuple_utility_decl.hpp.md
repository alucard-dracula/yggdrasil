# `tuple_utility_decl.hpp` 中文说明

## 用途

本头文件在 `yggr` 命名空间中前置声明 tuple 的两个元信息模板：`tuple_size` 和 `tuple_element`。它仅声明模板接口，不定义如何获取元素数量或元素类型。

```cpp
#include <yggr/tuple/tuple_utility_decl.hpp>
```

## 模板声明

```cpp
template<typename T>
struct tuple_size;

template<std::size_t I, typename T>
struct tuple_element;
```

| 模板 | 预期用途 |
| --- | --- |
| `yggr::tuple_size<T>` | 查询 tuple 类型 `T` 的元素数量；具体定义由后续 tuple 实用工具头文件提供。 |
| `yggr::tuple_element<I, T>` | 查询 tuple 类型 `T` 中索引为 `I` 的元素类型；具体定义由后续 tuple 实用工具头文件提供。 |

## 注意事项

- 这是轻量的声明头文件，适合需要提前声明这些模板的代码依赖。
- 仅包含本文件时，模板仍是不完整类型，不能读取 `value` 或 `type` 等成员；应包含提供相应特化和定义的 tuple 头文件。
- 元素索引 `I` 从 `0` 开始，具体边界检查由对应定义实现。
