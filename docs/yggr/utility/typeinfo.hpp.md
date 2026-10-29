# `typeinfo.hpp` 中文说明

## 用途

本头文件提供两类功能：

1. 在 `yggr::utility` / `yggr` 命名空间中提供 `addressof_typeinfo` 重载，获取 `std::type_info` 对象的地址。
2. 为 `std::type_info` 和 `boost::reference_wrapper<const std::type_info>` 提供 Boost 哈希器特化。

```cpp
#include <yggr/utility/typeinfo.hpp>
```

## `addressof_typeinfo`

函数在 `yggr::utility` 中定义，并导出到 `yggr`：

- `addressof_typeinfo<T>()`：返回 `typeid(T)` 对应的 `const std::type_info*`。
- `addressof_typeinfo(const T&)`：返回模板参数类型 `T` 的类型信息地址。该实现使用 `typeid(T)`，因此依据静态模板类型，而不是通过多态表达式查询动态类型。
- `addressof_typeinfo(const std::type_info&)`：返回传入类型信息对象的地址。
- `addressof_typeinfo(const std::type_info*)`：原样返回传入指针，包括空指针。
- `addressof_typeinfo(const boost::reference_wrapper<const std::type_info>&)`：返回包装引用所指向的类型信息对象地址。

示例：

```cpp
const std::type_info* int_info = yggr::addressof_typeinfo<int>();
const std::type_info* value_info = yggr::addressof_typeinfo(42);
```

以上第二个调用返回 `int` 的类型信息；若要取得多态对象的动态类型信息，应直接对多态表达式使用 `typeid(object)`，而不是依赖这个重载。

## Boost 哈希支持

头文件在 `boost` 命名空间为以下类型提供 `boost::hash` 特化：

- `std::type_info`
- `boost::reference_wrapper<const std::type_info>`

哈希实现基于 `std::type_info` 对象地址的指针哈希，而不是调用 `std::type_info::hash_code()`。因此，这里的哈希代表所引用类型信息对象的地址；不要把它当作跨进程、跨模块或序列化使用的稳定类型标识。

## 注意事项

- `typeid(T)` 返回的类型信息对象由运行时管理；本函数返回其地址，不转移所有权。
- 通过 `const T&` 重载得到的是静态类型 `T`。多态动态类型查询需要使用适当的 `typeid` 表达式。
- 不要解引用空的 `std::type_info*` 输入；该指针重载会直接返回原值，不进行空值检查。
- 哈希值不应视为稳定 ID；哈希容器仍需遵守所使用键类型的相等与哈希一致性要求。
