# `get_element.hpp` 

## 用途

本头文件为 Boost.Tuple 和 Yggdrasil Tuple 提供获取首元素与末元素的便捷函数。接口位于 `yggr::tuples` 命名空间，并通过 `using` 声明导入 `yggr` 命名空间。

```cpp
#include <yggr/tuple_ex/get_element.hpp>
```

## 接口

- `get_first_element(tuple)`：获取索引 `0` 处的元素。
- `get_last_element(tuple)`：获取 tuple 最后一个索引处的元素。

两个函数均提供可变对象和 `const` 对象的重载，返回类型由对应 tuple 的 `access_traits` 决定；因此对可变 tuple 调用时可修改返回的元素，对 const tuple 调用时返回 const 访问类型。函数内部通过 `yggr::get<I>` 访问元素。

支持的 tuple 类型：

- `boost::tuple`。
- `yggr::tuple`（即 Yggdrasil Tuple 类型）。

本文件没有为 `std::tuple` 提供重载。

## 使用示例

```cpp
#include <yggr/tuple_ex/get_element.hpp>

yggr::tuple<int, double, char> value(10, 2.5, 'x');

int& first = yggr::get_first_element(value);
char& last = yggr::get_last_element(value);
first = 20;
```

对 `const` tuple 调用时，应使用相应的 const 引用类型接收结果。

## 注意事项与配置

- tuple 必须至少包含一个元素；空 tuple 没有可访问的首元素或末元素。
- 函数按索引访问，不进行边界检查；可访问的索引由 tuple 的元素个数决定。
- 启用可变参数模板时，接口通过参数包实现；禁用时使用预处理器按 Boost.Tuple 或 Yggdrasil Tuple 的配置长度展开，支持上限受相应 tuple 长度配置约束。
- 头文件依赖 Yggdrasil 的 tuple 扩展接口和默认元素访问工具。
