**概述**
- **描述：** `string_symbol.hpp` 定义了用于将符号函数调用转换为字符串字面量的预处理宏。
- **头文件：** [yggr/ppex/string_symbol.hpp](yggr/ppex/string_symbol.hpp)

**核心宏**
- `YGGR_PP_STRING_SYMBOL(__symbol__)`
  - 展开为 `YGGR_PP_CAST(__symbol__(), YGGR_PP_CAST_TO_STRING)`。
  - 将 `__symbol__` 视为返回可字符串化值的函数或宏，并生成普通 `char` 字符串字面量。
- `YGGR_PP_WSTRING_SYMBOL(__symbol__)`
  - 展开为 `YGGR_PP_CAST(__symbol__(), YGGR_PP_CAST_TO_WSTRING)`。
  - 将 `__symbol__` 生成宽字符字符串字面量。

**实现依赖**
- 包含头文件 `yggr/ppex/cast.hpp`。
- 包含头文件 `yggr/ppex/cast_to_string.hpp`。
- 包含头文件 `yggr/ppex/symbols.hpp`。

**用途**
- 用于将符号宏或函数的结果转换为调试字符串。
- 适合在宏元编程中生成可视化输出或产生编译时字符串描述。

**示例**
```cpp
#define MY_SYMBOL symbol_value
const char* s = YGGR_PP_STRING_SYMBOL(MY_SYMBOL);
const wchar_t* ws = YGGR_PP_WSTRING_SYMBOL(MY_SYMBOL);
```

**相关文件**
- `yggr/ppex/string_symbol.hpp`
- `yggr/ppex/cast.hpp`
- `yggr/ppex/cast_to_string.hpp`
- `yggr/ppex/symbols.hpp`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件。