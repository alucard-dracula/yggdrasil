# `aligned_storage.hpp` 中文说明

## 用途

本头文件将 Boost.TypeTraits 中的 `aligned_storage` 引入 Yggdrasil 命名空间，供需要按指定字节数和对齐要求分配原始存储空间的代码使用。

```cpp
#include <yggr/type_traits/aligned_storage.hpp>
```

## 接口

```cpp
yggr::aligned_storage<Length, Alignment>::type
```

`Length` 指定存储容量（字节数），`Alignment` 指定所需的对齐值。该模板别名接口来自 `boost::aligned_storage`，可通过 `yggr::type_traits::aligned_storage` 或 `yggr::aligned_storage` 使用。

## 示例

```cpp
#include <yggr/type_traits/aligned_storage.hpp>
#include <yggr/type_traits/alignment_of.hpp>

struct sample
{
    double value;
};

typedef yggr::aligned_storage<
    sizeof(sample),
    yggr::alignment_of<sample>::value
>::type sample_storage;

sample_storage storage;
```

该示例声明了大小和对齐均适合 `sample` 的原始存储；若要在其中创建对象，还需按项目所用 C++ 标准采用适当的对象构造方式，并在生命周期结束时正确销毁对象。

## 兼容性处理

在 GNU 编译器环境下，头文件会在包含 Boost 的 `aligned_storage` 实现前后调整 `BOOST_STATIC_CONSTANT` 宏，以适配相关编译器及 Boost 配置。此宏处理是实现层面的兼容措施；应用代码通常只需使用 Yggdrasil 暴露的类型接口。

## 注意事项

- `aligned_storage` 提供的是原始字节存储，不会自动构造其中的对象，也不负责对象生命周期管理。
- `Length` 和 `Alignment` 应满足目标类型的大小与对齐要求；可结合 `sizeof(T)` 和 `yggr::alignment_of<T>::value` 使用。
- 此头文件不包含 `alignment_of.hpp`，需要查询类型对齐时应显式包含对应头文件。
