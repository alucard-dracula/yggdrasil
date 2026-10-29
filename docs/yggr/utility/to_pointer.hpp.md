# `to_pointer.hpp` 中文说明

## 用途

本头文件在 `yggr::utility` 命名空间提供 `to_pointer` 和 `to_const_pointer`，用于将对象引用或已有指针转换为指针。常规重载保留对象类型；显式指定目标类型时，接口通过 `reinterpret_cast` 重新解释地址，不执行类型安全的转换或对象转换。

```cpp
#include <yggr/utility/to_pointer.hpp>
```

## 接口行为

### `to_pointer`

- `to_pointer(T* p)`：返回原指针。
- `to_pointer(T& value)`：返回 `value` 的地址。
- `to_pointer<Target>(source)`：在源类型与目标类型不同时，将源地址重新解释为 `Target*`。

### `to_const_pointer`

- `to_const_pointer(const T* p)`：返回原指针。
- `to_const_pointer(const T& value)`：返回对象地址，结果为指向 const 对象的指针。
- `to_const_pointer<Target>(source)`：在类型不同时，将地址重新解释为 `const Target*`。

示例：

```cpp
int value = 42;
int* p = yggr::utility::to_pointer(value);
const int* cp = yggr::utility::to_const_pointer(value);

void* raw = p;
int* restored = yggr::utility::to_pointer<int>(raw);
```

上例中的 `void*` 到 `int*` 属于显式重解释；调用方必须确保地址实际指向有效且对齐的 `int` 对象，且访问符合 C++ 对象生命周期和别名规则。

## 使用限制与风险

- 对象引用重载要求左值引用；不要将返回指针保存到已销毁对象的地址。
- `to_pointer<Target>` 与 `to_const_pointer<Target>` 的目标类型重载使用 `reinterpret_cast`，不会检查类型关系、对象布局、对齐或指针有效性。
- 将指针重新解释为不兼容类型后解引用，可能导致未定义行为。只有在调用方掌握底层存储和对象生命周期时才应使用此能力。
- `to_const_pointer` 保证返回指向 const 目标的指针，但这并不使无效的地址解释变得安全。
