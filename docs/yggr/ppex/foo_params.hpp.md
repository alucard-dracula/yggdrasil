**概述**
- **描述：** `foo_params.hpp` 提供了一组预处理宏，用于生成函数模板参数、参数类型、参数名称和参数转发表达式。它广泛用于模板元编程中构造可变参数列表、参数包装和函数调用序列。
- **头文件：** [yggr/ppex/foo_params.hpp](yggr/ppex/foo_params.hpp)

**核心目的**
- 简化函数模板中参数列表的定义。
- 支持不同参数传递方式：普通值、引用、常量引用、转发引用、右值引用、移动、boost::ref、boost::cref 等。
- 兼容 C++11 可变参数宏及非可变参数宏环境。

**主要宏分类**
- 参数类型和名称生成
  - `YGGR_PP_FOO_TYPES_DEF(__count__)`：生成 `typename T0, typename T1, ..., typename T{n-1}`。
  - `YGGR_PP_FOO_TYPES_GROUP(__count__)`：生成 `T0, T1, ..., T{n-1}`。
  - `YGGR_PP_FOO_ARG(__n__)`：生成参数名，如 `arg0`, `arg1` 等。

- 参数声明宏
  - `YGGR_PP_FOO_ANYREF_PARAMS`：将参数声明为 `BOOST_FWD_REF(Tn) argn`。
  - `YGGR_PP_FOO_RVREF_PARAMS` / `YGGR_PP_FOO_RVREF_PARAMS_TYPES`：声明右值引用参数和类型。
  - `YGGR_PP_FOO_FWDREF_PARAMS` / `YGGR_PP_FOO_FWDREF_PARAMS_TYPES`：声明转发引用参数。
  - `YGGR_PP_FOO_CREF_PARAMS` / `YGGR_PP_FOO_CREF_PARAMS_TYPES`：声明常量引用参数。
  - `YGGR_PP_FOO_REF_PARAMS` / `YGGR_PP_FOO_REF_PARAMS_TYPES`：声明非常量引用参数。
  - `YGGR_PP_FOO_VAR_PARAMS` / `YGGR_PP_FOO_VAR_PARAMS_TYPES`：声明按值参数。

- 自定义参数类型支持
  - `YGGR_PP_FOO_CUSTOM_ANYREF_PARAMS`：使用 `YGGR_PP_FOO_ARG_TYPE(__n__)` 定义的自定义类型，同时支持右值引用与常规引用。
  - `YGGR_PP_FOO_CUSTOM_RVREF_PARAMS`：自定义类型的右值引用声明。
  - `YGGR_PP_FOO_CUSTOM_CREF_PARAMS`：自定义类型的常量引用声明。
  - `YGGR_PP_FOO_CUSTOM_REF_PARAMS`：自定义类型的非常量引用声明。
  - `YGGR_PP_FOO_CUSTOM_VAR_PARAMS`：自定义类型的按值声明。

- 参数序列生成与操作
  - `YGGR_PP_FOO_PARAMS_DEF(__count__, __macro__)`：按数量调用指定宏生成参数列表。
  - `YGGR_PP_FOO_ARGS_OP(__z__, __n__, __op__)`：将参数名与操作符组合生成如 `arg0 __op__ arg1 ...`。
  - `YGGR_PP_FOO_PARAMS_OP(__count__, __op__)`：按数量生成参数列表并插入操作符。
  - `YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW(__count__, __foo_custom__, __op__)`：立即使用自定义宏生成参数与操作符。
  - `YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER(__count__, __op__)`：延迟使用 `YGGR_PP_FOO_CUSTOM_OP()` 生成参数与操作符。

- 默认参数与转发支持
  - `YGGR_PP_FOO_ARGS_FWD_REF(__class_name__)`：生成可转发参数类型 `__class_name__&&` 或 `const __class_name__&`，取决于是否支持右值引用。
  - `YGGR_PP_FOO_ARGS_DEFAULT_VALUE_OP`：为参数生成默认值初始化表达式。
  - `YGGR_PP_FOO_PARAM_AND_DEFAULT_VALUE(__count__, __class_name__)`：生成带默认值的参数声明序列。

- 标准转发 / 移动 / 引用包装
  - `YGGR_PP_FOO_PARAMS_OP_STD_FORWARD`：生成 `std::forward<Tn>(argn)` 表达式序列。
  - `YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD`：生成 `boost::forward<Tn>(argn)` 表达式序列。
  - `YGGR_PP_FOO_PARAMS_OP_STD_MOVE`：生成 `std::move(argn)` 表达式序列。
  - `YGGR_PP_FOO_PARAMS_OP_BOOST_MOVE`：生成 `boost::move(argn)` 表达式序列。
  - `YGGR_PP_FOO_PARAMS_OP_BOOST_REF`：生成 `boost::ref(argn)` 表达式序列。
  - `YGGR_PP_FOO_PARAMS_OP_BOOST_CREF`：生成 `boost::cref(argn)` 表达式序列。
  - `YGGR_PP_FOO_PARAMS_OP_BOOST_ANYREF`：根据是否支持右值引用，选择 `boost::ref` 或 `boost::cref`。

**环境兼容性**
- 支持 `YGGR_NO_CXX11_RVALUE_REFERENCES`：在不支持 C++11 右值引用时退回到 `BOOST_RV_REF`。
- 支持 `YGGR_PP_DEFAULT_PARAM_INIT()` 和 `YGGR_PP_FOO_ARG_NAME()` 等外部配置宏，以控制默认初始化和参数名生成方式。

**使用场景**
- 构建可扩展函数模板的参数列表，例如在 `BOOST_PP_LOCAL_ITERATE` 中自动生成多个重载版本。
- 在库内部统一参数命名、类型声明和参数转发逻辑，避免重复手写参数模板定义。

**示例**
```cpp
#define YGGR_PP_FOO_ARG_NAME() arg_
#define BOOST_PP_LOCAL_MACRO(__n__)
    template< YGGR_PP_FOO_TYPES_DEF(__n__) >
    void foo(YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS));
#define BOOST_PP_LOCAL_LIMITS (1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#include BOOST_PP_LOCAL_ITERATE()
#undef YGGR_PP_FOO_ARG_NAME
```

**相关文件**
- `yggr/ppex/foo_params.hpp`
- `yggr/ppex/typedef.hpp`
- `yggr/ppex/params_expand.hpp`
- `yggr/ppex/open.hpp`
- `yggr/ppex/cast.hpp`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件顶部。