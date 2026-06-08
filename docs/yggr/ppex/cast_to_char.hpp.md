**概述**
- **描述：** `cast_to_char.hpp` 定义了一组预处理宏，用于将宏参数转换成不同字符类型的字符串字面量。
- **头文件：** [yggr/ppex/cast_to_char.hpp](yggr/ppex/cast_to_char.hpp)

**核心宏**
- `YGGR_PP_CAST_TO_CHAR(__x__)`
  - 将参数 `__x__` 转换为普通 `char` 字符串字面量。
  - 使用 `#@__x__` 进行字符串化。
- `YGGR_PP_CAST_TO_WCHAR(__x__)`
  - 在支持宽字符的环境下定义。
  - 将参数转换为 `wchar_t` 宽字符串字面量，等价于 `L"..."`。
- `YGGR_PP_CAST_TO_U8_CHAR(__x__)`
  - 在支持 `char8_t` 的环境下定义。
  - 将参数转换为 UTF-8 字符串字面量，等价于 `u8"..."`。
- `YGGR_PP_CAST_TO_U16_CHAR(__x__)`
  - 在支持 `char16_t` 的环境下定义。
  - 将参数转换为 UTF-16 字符串字面量，等价于 `u"..."`。
- `YGGR_PP_CAST_TO_U32_CHAR(__x__)`
  - 在支持 `char32_t` 的环境下定义。
  - 将参数转换为 UTF-32 字符串字面量，等价于 `U"..."`。

**条件宏控制**
- 宏 `YGGR_NO_CWCHAR` 控制是否定义 `YGGR_PP_CAST_TO_WCHAR`。
- 宏 `YGGR_NO_CHAR8_T` 控制是否定义 `YGGR_PP_CAST_TO_U8_CHAR`。
- 宏 `YGGR_NO_CHAR16_T` 控制是否定义 `YGGR_PP_CAST_TO_U16_CHAR`。
- 宏 `YGGR_NO_CHAR32_T` 控制是否定义 `YGGR_PP_CAST_TO_U32_CHAR`。

**实现依赖**
- 包含头文件 `yggr/base/yggrdef.h`。
- 包含 Boost.Preprocessor 的 `BOOST_PP_CAT`，用于拼接字符串前缀。

**用途**
- 在宏元编程中生成不同字符编码的字符串字面量。
- 适用于跨平台、多编码环境下需要统一字符串字面量生成的场景。

**示例**
```cpp
const char* s1 = YGGR_PP_CAST_TO_CHAR(hello);
const wchar_t* s2 = YGGR_PP_CAST_TO_WCHAR(hello);
const char8_t* s3 = YGGR_PP_CAST_TO_U8_CHAR(hello);
const char16_t* s4 = YGGR_PP_CAST_TO_U16_CHAR(hello);
const char32_t* s5 = YGGR_PP_CAST_TO_U32_CHAR(hello);
```

**相关文件**
- `yggr/ppex/cast_to_char.hpp`
- `yggr/base/yggrdef.h`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，许可条款请参见源文件。