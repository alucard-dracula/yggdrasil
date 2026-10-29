# `bit_counter.hpp` 中文说明

## 用途

本头文件定义编译期选择器 `yggr::bit_counter<b>`，用于统计对象表示中的置位数或零位数。

```cpp
#include <yggr/utility/bit_counter.hpp>
```

## 接口

```cpp
yggr::bit_counter<true>::bit_count(value)
yggr::bit_counter<false>::bit_count(value)
```

两个特化都支持静态函数 `bit_count(const T&)` 和函数调用运算符 `operator()(const T&)`，返回类型为 `yggr::size_type`：

- `bit_counter<true>`：统计 `T` 对象表示中所有为 1 的位。
- `bit_counter<false>`：统计对象表示中的 0 位，结果为 `sizeof(T) * 8 - 置位数`。

该模板在 `yggr::utility` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/utility/bit_counter.hpp>

unsigned char value = 0x0F;

yggr::size_type ones =
    yggr::bit_counter<true>::bit_count(value);
yggr::size_type zeros =
    yggr::bit_counter<false>()(value);
```

在 8 位字节的平台上，`ones` 为 4，`zeros` 为 4。

## 实现与注意事项

- 置位统计通过读取对象的字节表示，并对每个字节中的 8 个单个位域求和。
- 统计对象表示中的位，不是对象的逻辑值、有效数据位或数值位数。
- `sizeof(T) * 8` 假设一个字节包含 8 位；该实现面向常见 8 位字节平台。
- 对浮点数、结构体或含填充位的类型，结果也会包含其对象表示的全部位，包括符号位、填充位或未指定表示位。
- 返回的是表示中 0/1 位的数量；结果不代表类型的数值范围、有效载荷长度或编码字符数。
