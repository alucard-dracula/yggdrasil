# `get_element.hpp` 中文说明

## 用途

本头文件为 Boost.Tuple 和 Yggdrasil Tuple 提供便捷访问函数，用于获取 tuple 的首元素或末元素。函数位于 `yggr::tuples` 命名空间，并通过 `yggr` 命名空间重新导出。

```cpp
#include <yggr/tuple_ex/get_element.hpp>
```

## 接口

| 函数 | 功能 |
| --- | --- |
| `get_first_element(tuple)` | 返回 tuple 中索引为 `0` 的元素。 |
| `get_last_element(tuple)` | 返回 tuple 中索引为 `length - 1` 的元素。 |

两组函数均支持 `boost::tuple` 和 `yggr::tuple`，并分别提供可修改 tuple 的非 const 重载和接受 const tuple 的重载。返回类型遵循对应 tuple 的 `access_traits`，因此保留其定义的引用及 const 属性。

## 示例

```cpp
#include <yggr/tuple_ex/get_element.hpp>
#include <yggr/tuple/tuple.hpp>

yggr::tuple<int, double> values(7, 2.5);

int& first = yggr::get_first_element(values);
double& last = yggr::get_last_element(values);

first = 9;
```

同样的函数也可用于 `boost::tuple`：

```cpp
#include <boost/tuple/tuple.hpp>
#include <yggr/tuple_ex/get_element.hpp>

boost::tuple<int, double> values(7, 2.5);
double& last = yggr::get_last_element(values);
```

## 实现与兼容性

- 访问操作委托给 `yggr::get<Index>`；末元素的索引由 tuple 长度在编译期计算。
- 启用可变模板时使用参数包实现；禁用可变模板时，接口通过预处理器按 Boost.Tuple 和 Yggdrasil Tuple 各自配置的最大长度展开。
- 这些函数要求 tuple 至少包含一个元素。空 tuple 没有首尾元素，不能用于这两个接口。
- 元素索引在编译期确定，不提供运行时索引或边界检查。
