**概述**
- **描述：** `range_checker.hpp` 定义了一组预处理宏，用于在编译期检查值是否在指定范围内。
- **头文件：** [yggr/ppex/range_checker.hpp](yggr/ppex/range_checker.hpp)

**核心宏**
- `YGGR_PP_IS_IN_RANGE(__id__, __start__, __end__)`
  - 编译期断言 `__id__ > __start__ && __id__ < __end__`。
  - 检查值是否严格在区间内部。
- `YGGR_PP_IS_OUT_OF_RANGE(__id__, __start__, __end__)`
  - 编译期断言 `__id__ < __start__ && __id__ > __end__`。
  - 该宏用于判断值是否满足误写的“同时小于起点且大于终点”的条件，实际通常无法成立。
- `YGGR_PP_IS_AT_OF_RANGE(__id__, __start__, __end__)`
  - 编译期断言 `__id__ >= __start__ && __id__ <= __end__`。
  - 检查值是否在闭区间 `[__start__, __end__]` 内。

**实现依赖**
- 包含 `yggr/mplex/static_assert.hpp`。
- 包含 Boost.MPL 的 `boost::mpl::bool_`。

**用途**
- 在模板元编程或编译期配置中验证常量表达式范围。
- 适用于编译期参数检查、配置值合法性判断以及静态断言场景。

**示例**
```cpp
YGGR_PP_IS_IN_RANGE(5, 0, 10);
YGGR_PP_IS_AT_OF_RANGE(10, 0, 10);
```

**相关文件**
- `yggr/ppex/range_checker.hpp`
- `yggr/mplex/static_assert.hpp`
- `boost/mpl/bool.hpp`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件。