# `args_holder.hpp` 中文说明

## 用途

本头文件提供可通过 `basic_args_holder` 基类统一持有和传递的参数容器，支持两种存储策略：

- `args_holder<T>`：按值保存 `T` 对象。
- `args_holder_ref_wrap<T>`：通过 `boost::reference_wrapper<T>` 保存对现有对象的引用。

两种包装器均支持 `void` 特化，可作为不携带值的占位参数。

```cpp
#include <yggr/utility/args_holder.hpp>
```

## 主要类型

### `basic_args_holder`

多态基类，具有虚析构函数，可用于通过基类引用或指针传递不同类型的 holder。

### `args_holder<T>`

按值保存 `T`，支持复制和 Boost 移动兼容构造、赋值。`get_pointer()` 返回内部对象的地址；const holder 返回 `const T*`。

### `args_holder_ref_wrap<T>`

通过引用包装保存一个已有的 `T` 对象，不复制该对象。构造时必须提供 `T&`，`get_pointer()` 返回被引用对象的地址。

### `args_holder<void>` 与 `args_holder_ref_wrap<void>`

空值占位类型。两者的 `get_pointer()` 均返回空指针；引用包装器的 `void` 特化也允许接受一个忽略的构造参数。

## 基类转换与类型提取

`args_holder_trans(holder)` 将具体 holder 转为 `basic_args_holder&` 或 `const basic_args_holder&`，常用于统一传递。

对 `basic_args_holder`，头文件提供相应的类型提取函数：

| 函数 | 行为 |
| --- | --- |
| `args_holder_dynamic_cast<T>(holder)` | 用 `dynamic_cast` 检查是否为 `args_holder<T>`；失败时返回空指针。 |
| `args_holder_dynamic_cast_throw_if_nil<T>(holder)` | 检查类型及内部指针；失败时抛出 `bad_args_holder_cast`。 |
| `args_holder_check<T>(holder)` | 检查 holder 是否为 `args_holder<T>`。 |
| `args_holder_static_cast<T>(holder)` | 断言检查类型后，通过静态转换取得内部对象指针。 |

对 `args_holder_ref_wrap<T>` 也提供对应的 `args_holder_ref_wrap_dynamic_cast`、`args_holder_ref_wrap_dynamic_cast_throw_if_nil`、`args_holder_ref_wrap_check` 和 `args_holder_ref_wrap_static_cast` 函数。

## 示例

```cpp
#include <yggr/utility/args_holder.hpp>

int value = 42;
yggr::utility::args_holder<int> owned(value);
yggr::utility::args_holder_ref_wrap<int> borrowed(value);

yggr::utility::basic_args_holder& base =
    yggr::utility::args_holder_trans(owned);

int* stored = yggr::utility::args_holder_dynamic_cast<int>(base);
bool matches = yggr::utility::args_holder_check<int>(base);
```

`owned` 保存自己的 `int` 副本；`borrowed` 则引用外部的 `value`。

## 异常与注意事项

- `bad_args_holder_cast` 继承自 `std::bad_cast`，由带 `throw_if_nil` 的提取函数在 holder 类型不匹配或其内部指针为空时抛出。
- `dynamic_cast` 依赖 RTTI 运行时支持。
- `static_cast` 版本通过 `assert` 要求类型匹配；发布构建若禁用断言，调用方仍须先保证 holder 类型正确，否则行为不安全。
- `args_holder_ref_wrap<T>` 不拥有被引用对象。外部对象必须在引用包装器及其使用期间保持有效。
- `args_holder<T>` 存储对象副本；复制/移动赋值行为由 `copy_or_move_or_swap` 及 `T` 自身的操作能力决定。
- `void` 特化没有实际值，相关提取指针函数返回空指针。
