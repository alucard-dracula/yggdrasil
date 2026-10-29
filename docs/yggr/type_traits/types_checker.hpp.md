# `types_checker.hpp` 中文说明

## 用途

本头文件提供用于数值类型分类的编译期特征，主要用于判断类型是否可按“有符号值”处理。当前公开接口为 `yggr::signed_value_checker<T>`。

```cpp
#include <yggr/type_traits/types_checker.hpp>
```

## 类型特征

### `signed_value_checker<T>`

通过 `signed_value_checker<T>::value` 获取编译期布尔结果。默认情况下，若 `T` 是 Boost 识别的有符号类型或浮点类型，则结果为 `true`；其他类型由底层类型特征决定。

浮点类型也判为 true，是为了让依赖此特征的通用数值逻辑可以将负浮点数按有符号值处理。例如 Yggdrasil 数学符号函数据此区分有符号与无符号的符号判定方式。

头文件中还定义了内部特征 `unsigned_value_checker_detail<T>`，其直接继承 `boost::is_unsigned<T>`，但没有相应公开包装接口。

## 自定义有符号判定

宏 `YGGR_PP_SIGNED_CHECKER_TRUE_IMPL(Type)` 可为指定类型生成 `signed_value_checker_detail<Type>` 的 `true` 特化，供库内部兼容特殊数值类型使用。应在该类型相关模板实例化前使用，并避免重复定义同一类型的特化。

## 示例

```cpp
#include <yggr/type_traits/types_checker.hpp>

static_assert(yggr::signed_value_checker<int>::value,
              "signed integer is treated as signed");
static_assert(yggr::signed_value_checker<double>::value,
              "floating-point type is treated as signed");
```

## 注意事项

- 该特征是面向库逻辑的分类规则，不等价于“类型的底层表示具有符号位”。
- 浮点类型会被归入有符号值类别，即使某些底层 `is_signed` 实现并不将浮点类型判为 signed。
- 宏定义的特化只会将指定类型判定为 true；本头文件没有提供对应的自定义 false 特化宏。
- 判定逻辑依赖 Boost.TypeTraits 对类型的分类以及库的自定义特化。
