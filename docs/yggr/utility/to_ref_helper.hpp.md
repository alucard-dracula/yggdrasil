# `to_ref_helper.hpp` 中文说明

## 用途

本头文件定义 `yggr::utility::to_ref_helper`，一个仅含静态函数的辅助类，用于在引用、const 引用及基类引用之间转换。它通过返回引用避免复制对象。

```cpp
#include <yggr/utility/to_ref_helper.hpp>
```

该类型也通过 `yggr::to_ref_helper` 名称导出。

## 接口

### 普通引用转换

- `to_cref(const T& value)`：返回 `const T&`，不改变对象的可写性。
- `to_ref(const T& value)`：通过 `const_cast` 返回 `T&`。

`to_ref` 的参数本身是 const 引用，但返回的可修改引用并不意味着底层对象一定可修改。若底层对象原本声明为 const，通过返回引用修改它会导致未定义行为；仅在能够确认对象实际可修改时使用。

### 基类引用转换

有两组 `to_base_cref` / `to_base_ref` 重载：

1. `to_base_cref(const T&)` 与 `to_base_ref(const T&)`：当 `T` 有 `base_type` 且确实派生自该类型时，自动转换为 `T::base_type` 引用。
2. `to_base_cref<Base>(const T&)` 与 `to_base_ref<Base>(const T&)`：显式指定基类 `Base`，仅在 `T` 派生自 `Base` 时参与重载。

`to_base_cref` 返回 `const Base&`；`to_base_ref` 返回 `Base&`，同样使用 `const_cast` 去除参数表达式的 const 限定。基类对象是派生对象的一部分，不会创建新对象或延长生命周期。

## 示例

```cpp
struct base {};
struct derived : base {
    typedef base base_type;
};

derived value;

const derived& readonly = yggr::to_ref_helper::to_cref(value);
derived& writable = yggr::to_ref_helper::to_ref(value);

const base& base_readonly =
    yggr::to_ref_helper::to_base_cref(value);
base& base_writable =
    yggr::to_ref_helper::to_base_ref<base>(value);
```

## 注意事项

- 所有接口都返回对原对象或其基类子对象的引用；被引用对象销毁后，结果引用失效。
- `to_ref` 和 `to_base_ref` 不进行运行时检查，并可能移除 const 限定。不得通过它们修改实际为 const 的对象。
- 自动选择 `base_type` 的重载需要该嵌套类型存在且满足真实继承关系；显式指定 `Base` 的重载则由 `boost::is_base_of` 约束。
- 这些函数不会执行复制、移动或动态类型转换。
