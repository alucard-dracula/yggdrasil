**概述**
- **描述：** `cast.hpp` 定义了一个简单的预处理宏 `YGGR_PP_CAST`，用于将一个表达式转换为指定类型或通过指定宏包装。
- **头文件：** [yggr/ppex/cast.hpp](yggr/ppex/cast.hpp)

**核心宏**
- `YGGR_PP_CAST(__x__, __to__)`
  - 将参数 `__x__` 作为输入传递给 `__to__`。
  - 展开为 `__to__(__x__)`。
  - 适用于在宏元编程中进行类型转换或应用包装宏。

**实现依赖**
- 包含头文件 `yggr/base/yggrdef.h`。

**用途**
- 可以用于将值传递给自定义转换宏或类型构造形式。
- 例如，用于统一宏风格的转换表达式。

**示例**
```cpp
#define TO_INT(x) static_cast<int>(x)
int value = YGGR_PP_CAST(3.14, TO_INT);
```

**相关文件**
- `yggr/ppex/cast.hpp`
- `yggr/base/yggrdef.h`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件。