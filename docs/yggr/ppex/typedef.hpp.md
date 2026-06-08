**概述**
- **描述：** `typedef.hpp` 提供了一组预处理宏，用于生成模板参数类型列表、默认类型参数、模板类型实例化以及重复宏操作。
- **头文件：** [yggr/ppex/typedef.hpp](yggr/ppex/typedef.hpp)

**主要目的**
- 封装模板参数类型列表的生成。
- 支持定义默认类型参数。
- 生成模板类型实例化表达式。
- 提供可重复的自定义操作宏。

**关键宏**
- `YGGR_PP_TEMPLATE_PARAMS_LEN()`
  - 返回模板参数最大数量，默认由 `YGGR_PP_TEMPLATE_PARAMS_LEN_CFG` 控制，默认值为 10。
- `YGGR_PP_TYPE(__class_name__, __x__)`
  - 连接 `__class_name__` 与 `__x__`，生成类型或标识符。
- `YGGR_PP_TEMPLATE_PARAMS_TYPES(__count__, __class_name__)`
  - 生成 `__class_name__0, __class_name__1, ...`。
- `YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES(__count__, __macro__)`
  - 对指定宏进行重复扩展，用于自定义类型序列生成。
- `YGGR_PP_TEMLATE_SAME_PARAMS(__count__, __class_name__)`
  - 生成重复的同一类型参数列表。
- `YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE(__count__, __class_name__ )`
  - 生成带默认类型参数的模板参数列表，例如 `T0 = DefaultType, T1 = DefaultType, ...`。
- `YGGR_PP_TEMPLATE_TYPE(__class_name__, __params__)`
  - 生成 `__class_name__<__params__>` 模板实例化形式。
- `YGGR_PP_TEMPLATE_TYPE2(__class_name__, __params__, __other__)`
  - 生成 `__class_name__<__params__, __other__>` 模板实例化形式。
- `YGGR_PP_CLASS_TYPE(__class_name__)`
  - 直接返回 `__class_name__`。
- `YGGR_PP_REPEAT_CUSTOM_OP_NOW(__count__, __foo_custom__, __op__)`
  - 立即重复执行自定义操作宏。
- `YGGR_PP_REPEAT_CUSTOM_OP_LATER(__count__, __op__)`
  - 延迟调用 `YGGR_PP_REPEAT_CUSTOM_OP()` 生成重复操作。
- `YGGR_PP_EXPR_IF`
  - 等价于 `BOOST_PP_EXPR_IF`，用于条件表达式生成。

**实现依赖**
- 包含 `yggr/ppex/symbols.hpp` 和 `yggr/ppex/symbol_if.hpp`。
- 使用 Boost.Preprocessor 的：
  - `cat.hpp`
  - `repetition/enum.hpp`
  - `enum_params.hpp`
  - `iteration/local.hpp`
  - `repetition/repeat.hpp`
  - `control/expr_if.hpp`
  - `expand.hpp`

**用途场景**
- 生成可扩展的模板参数列表和默认模板参数。
- 构造模板类型实例化语句。
- 与其他宏配合生成复杂的模板元编程结构。
- 支持在宏代码中统一处理参数拼接和重复逻辑。

**示例**
```cpp
#define YGGR_PP_TEMPLATE_CUSTOM_TYPE(n) T##n
#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() int

template<YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE(3, T)>
class MyClass {};

using MyClassType = YGGR_PP_TEMPLATE_TYPE(MyClass, YGGR_PP_TEMLATE_SAME_PARAMS(3, T));
```

**相关文件**
- `yggr/ppex/typedef.hpp`
- `yggr/ppex/symbols.hpp`
- `yggr/ppex/symbol_if.hpp`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件。