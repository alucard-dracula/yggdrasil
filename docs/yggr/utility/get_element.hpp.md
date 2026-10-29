# `get_element.hpp` 中文说明

## 用途

本头文件为 `std::pair` 提供统一的首、末元素访问接口，并保留由 `get_element_default.hpp` 提供的通用类型默认实现。

```cpp
#include <yggr/utility/get_element.hpp>
```

## 接口

| 函数 | `std::pair<T1, T2>` 的访问目标 |
| --- | --- |
| `get_first_element(pair)` | `pair.first` |
| `get_last_element(pair)` | `pair.second` |

每个接口都提供可修改 pair 和 const pair 的重载，并返回相应元素的引用。函数定义在 `yggr::utility` 命名空间，同时通过 `yggr` 命名空间导出。

对于未由本文件专门支持的普通类型，默认重载会将对象自身同时作为首元素和末元素返回。tuple 和数组等类型的专门访问行为由其他头文件提供。

## 示例

```cpp
#include <yggr/utility/get_element.hpp>
#include <utility>

std::pair<int, double> value(3, 4.5);

int& first = yggr::get_first_element(value);
double& last = yggr::get_last_element(value);

first = 8; // 修改 value.first
```

const pair 调用对应的 const 重载，返回 const 元素引用。

## 注意事项

- 返回值是 pair 成员的引用；pair 必须在该引用使用期间保持存活。
- 该接口只覆盖 `std::pair` 的两个元素，不提供运行时索引或边界检查。
- 本文件依赖 `utility/pair.hpp` 的 `yggr::get` 与 pair 元素类型定义。
