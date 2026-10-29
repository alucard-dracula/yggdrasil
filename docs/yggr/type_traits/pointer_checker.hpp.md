# `pointer_checker.hpp` 中文说明

## 用途

本头文件定义编译期指针类型特征，用于区分原生指针、具有指针式成员访问能力的类，以及 Yggdrasil/Boost 中已登记的智能指针包装类型。

```cpp
#include <yggr/type_traits/pointer_checker.hpp>
```

## 类型特征

以下特征在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出：

| 特征 | 判定含义 |
| --- | --- |
| `is_pointer_wrapper<T>` | 若类具有可检测的 `operator->`，或被专门登记为指针包装类型，则为 `true`。 |
| `is_pointer<T>` | 若 `T` 是原生指针或 `is_pointer_wrapper<T>` 为 `true`，则为 `true`。 |
| `is_native_pointer<T>` | 仅当 `T` 是原生指针时为 `true`。 |

判断前会移除顶层 `const`、`volatile` 限定。每个特征通过继承的 `value` 提供编译期布尔结果。

## 已登记的包装类型

头文件附带的 `detail/pointer_checker_of_wrapper.hpp` 将以下类型登记为指针包装类型：

- `yggr::shared_ptr<T>`
- `boost::scoped_ptr<T>`
- `boost::weak_ptr<T>`
- `boost::intrusive_ptr<T>`
- `yggr::interface_ptr<T, SPtr>`

此外，任意类只要具有可检测的 `operator->`，也会被 `is_pointer_wrapper` 识别，不需要在登记列表中。

## 自定义登记

可使用以下宏登记包装类型：

```cpp
YGGR_PP_IS_POINTER_WRAPPER_DEF(MyPointer)
YGGR_PP_IS_POINTER_WRAPPER_SMART_POINTER_DEF(MySmartPointer)
```

第一个宏为具体类型生成特化；第二个宏面向单一类型模板参数的智能指针模板。宏应在 `yggr/type_traits/pointer_checker.hpp` 已包含、且相关特征实例化前使用，并避免对同一类型重复登记。

## 示例

```cpp
#include <yggr/type_traits/pointer_checker.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

struct item {};

static_assert(yggr::is_native_pointer<item*>::value,
              "raw pointer is native");
static_assert(yggr::is_pointer<item*>::value,
              "raw pointer is a pointer");
static_assert(yggr::is_pointer_wrapper<yggr::shared_ptr<item> >::value,
              "shared_ptr is registered as a wrapper");
static_assert(!yggr::is_native_pointer<yggr::shared_ptr<item> >::value,
              "shared_ptr is not a native pointer");
```

## 注意事项

- `is_pointer_wrapper` 对具有 `operator->` 的类进行检测；它不验证运算符的返回类型、解引用行为或指针生命周期。
- `is_pointer` 是面向泛型代码的广义分类；若只接受 C++ 原生指针，应使用 `is_native_pointer`。
- `detail/pointer_checker_of_wrapper.hpp` 要求先包含本入口头文件，不应单独直接包含。
- 这些特征判断类型类别，不检查指针是否为空、资源是否有效或对象是否仍存活。
