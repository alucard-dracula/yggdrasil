**概述**
- **描述：** `concat.hpp` 定义了一个预处理宏 `YGGR_PP_CONCAT`，用于直接展开传入的宏参数列表。
- **头文件：** [yggr/ppex/concat.hpp](yggr/ppex/concat.hpp)

**核心宏**
- `YGGR_PP_CONCAT(...)`
  - 在支持 C++11 可变参数宏的编译环境下，直接将传入的变长参数展开。
  - 用于宏元编程中按原样传递或组合参数。
- `YGGR_PP_CONCAT(__args__)`
  - 在不支持 C++11 可变参数宏的环境下，使用单参数形式展开传入的参数。

**条件宏控制**
- `YGGR_NO_CXX11_VARIADIC_MACROS` 控制宏定义形式。
  - 未定义时使用 `__VA_ARGS__`。
  - 已定义时使用兼容旧编译器的单参数展开方式。

**实现依赖**
- 包含头文件 `yggr/base/yggrdef.h`。

**用途**
- 作为宏参数传递的辅助工具，在宏展开阶段保持参数顺序和内容不变。
- 可用于连接多个宏表达式或在其他宏中嵌套参数展开。

**示例**
```cpp
#define F(a, b) a + b
int value = YGGR_PP_CONCAT(F(1, 2));
```

**相关文件**
- `yggr/ppex/concat.hpp`
- `yggr/base/yggrdef.h`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件。