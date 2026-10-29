# `floating_native_cast.hpp` 中文说明

## 用途

本头文件定义 `yggr::utility::floating_native_cast`，用于将浮点对象的存储表示按位重新解释为对应宽度的无符号整数引用。它不执行数值转换。

```cpp
#include <yggr/utility/floating_native_cast.hpp>
```

## 接口

```cpp
floating_native_cast(const T&)  // 返回对应无符号整数类型的 const 引用
floating_native_cast(T&)        // 返回对应无符号整数类型的可写引用
```

两个重载均仅对浮点类型启用。目标整数类型由 `sizeof(T)` 经 `YGGR_PP_UINTEGER_T_TYPE` 选择；例如常见平台上，4 字节浮点数映射到 `u32`，8 字节浮点数映射到 `u64`。

## 示例

```cpp
#include <yggr/utility/floating_native_cast.hpp>

double value = 1.5;
yggr::u64& bits = yggr::utility::floating_native_cast(value);
const double other = 2.0;
const yggr::u64& other_bits =
    yggr::utility::floating_native_cast(other);
```

可写重载返回底层表示的可修改引用，因此通过 `bits` 写入会直接修改 `value` 的存储位模式。

## 注意事项

- 该函数通过 `reinterpret_cast` 将浮点对象地址视为整数类型地址并解引用；这种访问可能受严格别名规则、对齐、对象表示和平台 ABI 影响，不能视为完全可移植的位转换方式。
- 返回的是引用，与原浮点对象共享存储；引用不得超出原对象生命周期。
- 可写重载可能制造目标整数类型的位模式，某些模式对浮点类型可能是无效或特殊值。使用前应确认目标平台的浮点格式和所写位模式。
- 浮点值与对应整数数值并无数值转换关系；如需数值转换，应使用普通类型转换。若需要可移植地复制位表示，应使用符合项目语言标准的安全位复制方案。
- `sizeof(T)` 必须对应 `number_t.hpp` 中可用的无符号宽度映射。
