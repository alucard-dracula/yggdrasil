# `marked_ref.hpp` 中文说明

## 用途

本头文件定义带有编译期标记类型的引用包装器 `marked_reference_wrapper<M, T>`，以及用于创建可修改或只读包装器的 `mref`、`mcref` 函数。其基础行为与 `boost::reference_wrapper` 一致，同时保留额外的标记类型 `M`。

```cpp
#include <yggr/utility/marked_ref.hpp>
```

## 类型与创建函数

### `marked_reference_wrapper<M, T>`

- `M` 通过嵌套类型 `mark_type` 暴露，供泛型代码识别引用的类别或用途。
- `type` 和 `base_type` 分别表示被引用类型及 `boost::reference_wrapper<T>` 基类。
- 构造时接收 `T&`，不复制或拥有被引用对象。

### `mref<M>(value)`

为可修改左值创建 `marked_reference_wrapper<M, T>`。

### `mcref<M>(value)`

为 const 左值创建 `marked_reference_wrapper<M, const T>`，通过返回类型保留只读限定。

这些接口定义于 `yggr::utility`，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/utility/marked_ref.hpp>

struct user_data_mark {};

int value = 5;
const int fixed_value = 10;

yggr::marked_reference_wrapper<user_data_mark, int> ref =
    yggr::mref<user_data_mark>(value);
yggr::marked_reference_wrapper<user_data_mark, const int> cref =
    yggr::mcref<user_data_mark>(fixed_value);
```

## Boost 互操作

在支持相应模板特化的 Boost 配置下，该包装器会被识别为 `boost::reference_wrapper`，并由 `boost::unwrap_reference` 解包为底层 `T`。头文件也提供 `boost::get_pointer` 适配；对于不支持模板偏特化的配置，使用 Boost 提供的替代检测入口。

## 注意事项

- 包装器不拥有被引用对象；被引用对象必须在包装器使用期间保持存活。
- `mref` 需要可修改左值；`mcref` 用于只读引用。两者都不复制对象。
- `M` 只是类型标记，不会改变被引用对象的运行时行为或生命周期。
- 对 `marked_reference_wrapper<M, T>` 的识别依赖 Boost 配置；其 cv 限定形式是否被识别也受 Boost 的 cv 特化支持影响。
