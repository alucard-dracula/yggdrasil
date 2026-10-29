# `byte_size.hpp` 中文说明

## 用途

本头文件为 tuple 类型扩展 `yggr::bytes::byte_size()` 的计算支持。它通过递归遍历 tuple 中的元素，并将每个元素交给通用字节数计算器，最后累加得到整个 tuple 的字节数。

```cpp
#include <yggr/tuple/byte_size.hpp>
```

支持的 tuple 形式包括：

- `boost::tuple` 及 `boost::tuples::cons`；
- `yggr::tuple` 及 `yggr::tuples::cons`；
- 配置允许使用标准 tuple 时的 `std::tuple`（未定义 `YGGR_NO_CXX11_HDR_TUPLE`）。

## 计算方式

对于运行时 tuple 对象，辅助模板按元素索引递归调用 `byte_size_helper`，并将各元素的结果相加。tuple 链式表示中的 `null_type` 作为结束节点，其字节数为 `0`。

元素的具体计算规则由 `yggr/bytes/byte_size.hpp` 提供的通用机制决定。例如，元素类型可通过成员或静态 `byte_size` 接口自定义计算；没有相应接口时，通用计算器使用兼容的类型大小规则。因此，结果不一定等于 tuple 对象的 `sizeof`，也不包含 tuple 的填充字节或对象布局开销。

辅助特化同时提供运行时对象和无参数调用形式，分别用于按元素值计算，或根据元素类型的默认规则计算。

## 使用示例

```cpp
#include <yggr/tuple/tuple.hpp>
#include <yggr/tuple/byte_size.hpp>
#include <yggr/bytes/byte_size.hpp>

yggr::tuple<int, short> value(10, 20);
yggr::u64 size = yggr::bytes::byte_size(value);
```

每个 tuple 元素将根据自身的 `byte_size_helper` 规则计算，结果以 `yggr::u64` 返回。

## 实现说明

- `byte_size_tuple_helper<T, I, ISize>` 使用索引 `I` 递归访问 tuple 元素，直到达到元素总数。
- Boost.Tuple 与 Yggdrasil Tuple 的 `cons` 链分别有模板特化，并对 `null_type` 结束节点处理。
- C++11 可变模板配置下，tuple 特化直接展开类型参数；不支持可变模板时，使用预处理器按 Boost.Tuple 的最大长度生成参数列表。
- `boost::remove_cv` 用于去除元素类型的 `const` / `volatile` 限定后选择对应的字节数计算器。

## 注意事项

- 该头文件提供的是 `byte_size` 的 tuple 类型适配，不应将其结果当作 `sizeof(tuple)`。
- 结果可能依赖元素值及元素类型自定义的 `byte_size` 行为；对于具有动态内容的元素，传入对象的计算结果可能不同于仅按类型计算的结果。
- `std::tuple` 支持受 `YGGR_NO_CXX11_HDR_TUPLE` 配置影响。
