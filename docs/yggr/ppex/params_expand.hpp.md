**概述**
- **描述：** `params_expand.hpp` 定义了用于展开宏参数的预处理宏 `YGGR_PP_PARAMS_EXPAND` 和 `YGGR_PP_MACRO_PARAMS_EXPAND`，支持 C++11 可变参数宏与旧版兼容写法。
- **头文件：** [yggr/ppex/params_expand.hpp](yggr/ppex/params_expand.hpp)

**核心宏**
- `YGGR_PP_PARAMS_EXPAND(__x__)`
  - 直接展开参数 `__x__`。
  - 用于在宏中显式展开另一个宏或参数表达式。
- `YGGR_PP_MACRO_PARAMS_EXPAND(__m__, ...)`
  - 在支持 C++11 可变参数宏的编译器下定义。
  - 展开为 `YGGR_PP_PARAMS_EXPAND(__m__(__VA_ARGS__))`。
- `YGGR_PP_MACRO_PARAMS_EXPAND(__m__, __args__)`
  - 在不支持可变参数宏的环境下定义。
  - 展开为 `YGGR_PP_PARAMS_EXPAND(__m__(__args__))`。

**兼容性说明**
- 该文件特别提醒：不要在宏循环中使用 `YGGR_PP_MACRO_PARAMS_EXPAND`，因为 MSVC 和 GCC 对 `__VA_ARGS__` 的解析行为存在差异。
- 使用 `YGGR_NO_CXX11_VARIADIC_MACROS` 宏选择兼容旧编译器的参数传递方式。

**用途**
- 在宏编程中展开嵌套宏调用。
- 支持统一的宏参数展开方式，避免直接嵌套时产生语法或解析差异。

**示例**
```cpp
#define F(x, y) x + y
int sum = YGGR_PP_MACRO_PARAMS_EXPAND(F, 1, 2);
```

**相关文件**
- `yggr/ppex/params_expand.hpp`
- `yggr/base/yggrdef.h`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件顶部。