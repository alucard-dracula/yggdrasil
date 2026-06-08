**概述**
- **描述：** `va_args.hpp` 提供了一组预处理宏，用于获取可变参数列表的长度和按索引访问参数元素。
- **头文件：** [yggr/ppex/va_args.hpp](yggr/ppex/va_args.hpp)

**核心宏**
- `YGGR_PP_ARGS_SIZE(...)`
  - 计算传入参数列表的元素数量。
  - 在支持 C++11 可变参数宏的环境下，使用 `__VA_ARGS__`。
  - 在不支持时，使用兼容形式 `__args__`。
- `YGGR_PP_ARGS_ELEM(__n__, ...)`
  - 获取参数列表中索引为 `__n__` 的元素。
  - 支持同样的可变参数宏和老式 `__args__` 两种形式。

**实现依赖**
- 包含 `boost/preprocessor/tuple/size.hpp`
- 包含 `boost/preprocessor/tuple/elem.hpp`

**兼容性说明**
- 该文件支持 `YGGR_NO_CXX11_VARIADIC_MACROS` 宏，以兼容不支持 C++11 `__VA_ARGS__` 的编译器。
- 在支持可变参数宏的情况下，直接使用 `...` 语法；否则通过单一 `__args__` 传递整个参数序列。

**用途**
- 在宏元编程中获取可变参数数量。
- 在宏内按位置访问传入参数。
- 适用于模板参数展开、函数宏生成和参数序列处理。

**示例**
```cpp
#define COUNT_ARGS(...) YGGR_PP_ARGS_SIZE(__VA_ARGS__)
#define GET_ARG(n, ...) YGGR_PP_ARGS_ELEM(n, __VA_ARGS__)

static_assert(COUNT_ARGS(a, b, c) == 3, "arg count");
const char* second = GET_ARG(1, a, b, c);
```

**相关文件**
- `yggr/ppex/va_args.hpp`
- `boost/preprocessor/tuple/size.hpp`
- `boost/preprocessor/tuple/elem.hpp`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件。