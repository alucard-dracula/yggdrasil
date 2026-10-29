# `aligned_storage_sizeof.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::aligned_storage_sizeof<T>`，用于计算为存放一个 `T` 对象而配置指定大小和对齐要求的 `aligned_storage` 类型的实际大小。

```cpp
#include <yggr/type_traits/aligned_storage_sizeof.hpp>
```

## 接口

```cpp
template<typename T>
struct yggr::aligned_storage_sizeof;
```

该类型继承自 `boost::mpl::size_t`，其编译期数值等价于：

```cpp
sizeof(yggr::aligned_storage<sizeof(T), yggr::alignment_of<T>::value>)
```

因此可通过 `yggr::aligned_storage_sizeof<T>::value` 获取结果。该特征在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/aligned_storage_sizeof.hpp>

struct sample
{
    double value;
};

constexpr std::size_t storage_size =
    yggr::aligned_storage_sizeof<sample>::value;
```

## 注意事项

- 结果是满足 `sizeof(T)` 容量和 `T` 对齐要求的 `aligned_storage` 类型大小，可能因对齐填充而大于 `sizeof(T)`。
- 该值描述的是原始存储空间的大小，不会创建或初始化 `T` 对象，也不代表对象生命周期已开始。
- `T` 必须是可用于 `sizeof` 和 `alignment_of` 的完整类型。
- `aligned_storage` 和 `alignment_of` 由 Boost.TypeTraits 提供，并通过 Yggdrasil 接口引入。
