# `args_anchor.hpp` 中文说明

## 用途

本头文件提供参数锚定包装器 `yggr::args_anchor_t<T>` 及构造函数，用于在接口中显式标记非拥有型参数：普通对象通过引用保存，指针按值保存，空参数则使用 `void` 锚点。

```cpp
#include <yggr/utility/args_anchor.hpp>
```

相关类型和函数定义于 `yggr::utility`，并通过 `yggr` 命名空间导出。

## 类型与构造接口

| 接口 | 行为 |
| --- | --- |
| `args_anchor(value)` | 为可修改对象创建 `args_anchor_t<T>`，内部以引用包装器保存对象引用。 |
| `const_args_anchor(value)` | 为 const 对象创建 `args_anchor_t<const T>`，以引用包装器保存只读引用。 |
| `args_anchor(pointer)` | 对指针参数使用 `args_anchor_t<T*>` 特化，按值保存指针。 |
| `args_anchor()` / `const_args_anchor()` | 创建 `args_anchor_t<void>` 空锚点。 |

普通引用锚点和指针锚点都提供 `get()` 及隐式转换运算符，以取得被引用对象或保存的指针。`args_anchor_t<void>` 使用 `yggr::mplex::null_t` 作为兼容占位类型，其 `get_pointer()` 返回空指针。

## 识别锚点类型

- `is_args_anchor_t<T>::value` 是编译期特征，用于检测经过 `native_t<T>` 规范化后的类型是否为 `args_anchor_t<...>`。
- `is_args_anchor<T>()` 和 `is_args_anchor(value)` 是返回相同判定结果的运行时形式；判定本身仍为编译期常量。

## 示例

```cpp
#include <yggr/utility/args_anchor.hpp>

int value = 42;
const int fixed_value = 7;
int* pointer = &value;

yggr::args_anchor_t<int> mutable_anchor = yggr::args_anchor(value);
yggr::args_anchor_t<const int> const_anchor =
    yggr::const_args_anchor(fixed_value);
yggr::args_anchor_t<int*> pointer_anchor = yggr::args_anchor(pointer);
yggr::args_anchor_t<void> empty_anchor = yggr::args_anchor();

int& referenced = mutable_anchor.get();
int* stored_pointer = pointer_anchor.get();
```

## 生命周期与实现注意事项

- 普通对象锚点不拥有对象，只保存引用；被锚定对象必须比锚点及其使用过程存活得更久。
- 指针锚点仅按值保存地址，不延长所指对象的生命周期，也不检查空指针或悬空指针。
- `native_t<T>` 会移除若干类型限定，`is_args_anchor_t` 的检测因此可能对某些带指针、引用或 cv 限定的包装类型进行规范化后再判断。
- 源码中普通引用特化的 `get_pointer()` 返回表达式为 `_ref.get_pointer`，而非 `_ref.get_pointer()`；如果实例化或调用该成员时遇到编译错误，应检查此实现处。
