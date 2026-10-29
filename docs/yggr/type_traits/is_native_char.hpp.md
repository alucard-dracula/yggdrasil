# `is_native_char.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::is_native_char<T>`，用于判断 `T` 是否与 Yggdrasil 支持的某种原生字符类型完全相同。

```cpp
#include <yggr/type_traits/is_native_char.hpp>
```

## 判定范围

`yggr::is_native_char<T>::value` 在以下任一条件成立时为 `true`：

- `T` 是 `char`；
- 未禁用宽字符支持时，`T` 是 `wchar_t`；
- 配置启用相应字符类型时，`T` 是 `yggr::c8`、`yggr::c16` 或 `yggr::c32`。

其中 `c8`、`c16`、`c32` 分别是 `char8_t`、`char16_t`、`char32_t` 的 Yggdrasil 别名。具体候选类型由 `YGGR_NO_CWCHAR`、`YGGR_NO_CHAR8_T`、`YGGR_NO_CHAR16_T` 和 `YGGR_NO_CHAR32_T` 等配置宏控制。

## 示例

```cpp
#include <yggr/type_traits/is_native_char.hpp>

static_assert(yggr::is_native_char<char>::value, "char is native");

#ifndef YGGR_NO_CHAR16_T
static_assert(yggr::is_native_char<yggr::c16>::value,
              "c16 is native when enabled");
#endif

static_assert(!yggr::is_native_char<unsigned char>::value,
              "unsigned char is not one of the listed character types");
```

## 注意事项

- 实现使用类型完全相同比较，不会自动移除 `const`、`volatile` 或引用限定。
- `signed char` 和 `unsigned char` 不会仅因其常用于存储字符数据而被识别为原生字符类型。
- 此特征只识别类型类别，不负责字符编码转换、字符有效性验证或字符串处理。
