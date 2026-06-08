**概述**
- **描述：** `symbols.hpp` 定义了一组预处理符号宏，用于在宏元编程中以名称化方式表示 C++ 运算符、分隔符、括号和常用符号。
- **头文件：** [yggr/ppex/symbols.hpp](yggr/ppex/symbols.hpp)

**核心宏**
- `YGGR_PP_SYMBOL`：等价于 `YGGR_PP_OPEN`，将参数视为可调用的符号宏。
- `YGGR_PP_SYMBOL_PLUS()`、`YGGR_PP_SYMBOL_MINUS()`、`YGGR_PP_SYMBOL_MUL()` 等：表示数学运算符 `+`, `-`, `*`。
- `YGGR_PP_SYMBOL_EQUAL_TO()`、`YGGR_PP_SYMBOL_LESS()` 等：表示比较运算符。
- `YGGR_PP_SYMBOL_LOGIC_AND()`、`YGGR_PP_SYMBOL_LOGIC_OR()` 等：表示逻辑运算符。
- `YGGR_PP_SYMBOL_BITWISE_AND()`、`YGGR_PP_SYMBOL_BITWISE_OR()` 等：表示按位运算符。
- `YGGR_PP_SYMBOL_SET()`、`YGGR_PP_SYMBOL_PLUS_SET()` 等：表示赋值和复合赋值运算符。

**括号与分隔符**
- `YGGR_PP_SYMBOL_PARENTHESES()` / `_L()` / `_R()`：圆括号。
- `YGGR_PP_SYMBOL_BRACKETS()` / `_L()` / `_R()`：方括号。
- `YGGR_PP_SYMBOL_BRACES()` / `_L()` / `_R()`：花括号。
- `YGGR_PP_SYMBOL_ANGLE_BRACKETS()` / `_L()` / `_R()`：尖括号。
- `YGGR_PP_SYMBOL_COMMA`：逗号。
- `YGGR_PP_SYMBOL_SEMICOLON()`：分号。
- `YGGR_PP_SYMBOL_QUESTION_MARK()` / `YGGR_PP_SYMBOL_COLON()`：条件运算符三元符号。
- `YGGR_PP_SYMBOL_AREA()`：作用域解析运算符 `::`。

**指针与引用**
- `YGGR_PP_SYMBOL_REF()`：`&`
- `YGGR_PP_SYMBOL_IND()`：`*`
- `YGGR_PP_SYMBOL_MEMBER_REF()`：`.`
- `YGGR_PP_SYMOBL_MEMBER_DER()`：`->`
- `YGGR_PP_SYMBOL_POINTER_TO_MEMBER_REF()`：`.*`
- `YGGR_PP_SYMOBL_POINTER_TO_MEMBER_DER()`：`->*`
- `YGGR_SYMBOL_RVALUE_REF()`：`&&`（支持 C++11 右值引用时）

**其他符号**
- `YGGR_PP_SYMBOL_EMPTY`：展开为空。
- `YGGR_PP_SYMBOL_EMPTY_N(__n__, ...)`：用于可变参数宏兼容的空语法占位。
- `YGGR_PP_SYMBOL_SLASH()`：斜杠 `/`。
- `YGGR_PP_SYMBOL_ELLIPSIS()`：省略号 `...`。

**数字与布尔值**
- `YGGR_PP_SYMBOL_NUMBER_ZERO()` 至 `YGGR_PP_SYMBOL_NUMBER_NINE()`：表示 `0` 到 `9`。
- 德语数字别名：`YGGR_PP_SYMBOL_NUMBER_NULL`, `YGGR_PP_SYMBOL_NUMBER_EINS`, `YGGR_PP_SYMBOL_NUMBER_ZWEI`, `YGGR_PP_SYMBOL_NUMBER_DREI`, `YGGR_PP_SYMBOL_NUMBER_VIER`, `YGGR_PP_SYMBOL_NUMBER_FUNF`, `YGGR_PP_SYMBOL_NUMBER_SECHS`, `YGGR_PP_SYMBOL_NUMBER_SIEBEN`, `YGGR_PP_SYMBOL_NUMBER_ACHT`, `YGGR_PP_SYMBOL_NUMBER_NEUN`。
- `YGGR_PP_SYMBOL_BOOL_FALSE()` / `YGGR_PP_SYMBOL_BOOL_TRUE()`：表示布尔值 `0` 和 `1`。

**实现依赖**
- 包含 `yggr/base/yggrdef.h`
- 包含 `yggr/move/config.hpp`
- 包含 `yggr/ppex/open.hpp`
- 使用 Boost.Preprocessor 的 `empty.hpp` 和 `comma.hpp`

**用途**
- 在宏定义中保持符号表达一致性，并支持通过名称引用符号。
- 可用于构建更可读的宏代码，如生成表达式、条件判断、赋值语句等。
- 适合与 `YGGR_PP_OPEN`、`YGGR_PP_SYMBOL_IF` 等宏配合使用。

**示例**
```cpp
YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_PLUS) // 生成 +
YGGR_PP_SYMBOL(YGGR_PP_SYMBOL_PARENTHESES_L) int x YGGR_PP_SYMBOL_PARENTHESES_R ;
```

**相关文件**
- `yggr/ppex/symbols.hpp`
- `yggr/ppex/open.hpp`
- `yggr/base/yggrdef.h`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参考源文件。