# `alignment_of.hpp` 中文说明

## 用途

本头文件将 Boost.TypeTraits 的 `alignment_of` 类型特征引入 Yggdrasil 命名空间，用于在编译期查询类型所需的对齐值。

```cpp
#include <yggr/type_traits/alignment_of.hpp>
```

## 接口

```cpp
yggr::alignment_of<T>::value
```

`alignment_of<T>` 是编译期类型特征，`value` 表示 `T` 的对齐要求。该特征在 `yggr::type_traits` 命名空间中通过 `using` 声明引入，并进一步导出为 `yggr::alignment_of`。

## 示例

```cpp
#include <yggr/type_traits/alignment_of.hpp>

struct sample
{
    double value;
};

constexpr std::size_t sample_alignment =
    yggr::alignment_of<sample>::value;
```

该结果可与 `yggr::aligned_storage` 配合，为类型准备具有相应对齐要求的原始存储空间。

## 注意事项

- 这是编译期查询，不会分配内存或创建对象。
- 对齐值由编译器、平台和类型布局决定；不应假设不同平台上的结果相同。
- 本头文件基于 Boost.TypeTraits 实现，并不额外定义或改变 Boost 的对齐语义。
