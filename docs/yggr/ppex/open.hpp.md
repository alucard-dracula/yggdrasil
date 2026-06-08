**概述**
- **描述：** `open.hpp` 定义了一个简单的预处理宏 `YGGR_PP_OPEN`，用于将传入的宏参数作为函数名并追加空参数列表调用。
- **头文件：** [yggr/ppex/open.hpp](yggr/ppex/open.hpp)

**核心宏**
- `YGGR_PP_OPEN(__x__)`
  - 展开为 `__x__()`。
  - 用于在宏生成的表达式中统一将标识符转换为调用形式。

**实现依赖**
- 包含头文件 `yggr/base/yggrdef.h`。

**用途**
- 适用于宏元编程中将参数视为函数对象或函数名并执行调用的场景。
- 常与其他参数生成宏配合使用，以构建可变参数函数调用表达式。

**示例**
```cpp
#define FOO() do_something()
YGGR_PP_OPEN(FOO); // 展开为 FOO()
```

**相关文件**
- `yggr/ppex/open.hpp`
- `yggr/base/yggrdef.h`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件。