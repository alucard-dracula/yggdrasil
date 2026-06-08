**概述**
- **描述：** `pp_debug.hpp` 定义了一组仅在调试模式下可用的预处理宏，用于将宏参数转换为字符串形式，便于调试输出或宏展开检查。
- **头文件：** [yggr/ppex/pp_debug.hpp](yggr/ppex/pp_debug.hpp)

**条件启用**
- 该头文件仅在 `_DEBUG` 或 `YGGR_TEST_PP_DEBUG` 定义时生效。
- 在发布版本中使用会触发编译错误：`release version do not use YGGR_PP_DEBUG series`。

**核心宏**
- `YGGR_PP_DEBUG(__x__)`
  - 将参数 `__x__` 通过 `YGGR_PP_CAST_TO_STRING` 转换为普通字符串字面量。
- `YGGR_PP_WDEBUG(__x__)`
  - 将参数 `__x__` 转换为宽字符字符串字面量（`L"..."`）。
- `YGGR_PP_DEBUG_AORW(__x__, __a_or_w__)`
  - 将参数 `__x__` 转换为由 `__a_or_w__` 指定的字符串字面量类型。

**实现依赖**
- 包含 `yggr/ppex/cast.hpp`
- 包含 `yggr/ppex/cast_to_string.hpp`

**用途**
- 在调试时，将宏参数字符串化以便观察其展开结果。
- 适用于生成调试日志、断言消息或宏扩展诊断。

**示例**
```cpp
YGGR_PP_DEBUG(SOME_MACRO);
YGGR_PP_WDEBUG(OTHER_MACRO);
YGGR_PP_DEBUG_AORW(MACRO_NAME, YGGR_PP_CAST_TO_WSTRING);
```

**相关文件**
- `yggr/ppex/pp_debug.hpp`
- `yggr/ppex/cast.hpp`
- `yggr/ppex/cast_to_string.hpp`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件。