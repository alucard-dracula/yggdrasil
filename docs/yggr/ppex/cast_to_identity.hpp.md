**概述**
- **描述：** `cast_to_identity.hpp` 提供了一个简单的预处理宏 `YGGR_PP_CAST_TO_IDENTITY`，用于返回传入的参数本身。
- **头文件：** [yggr/ppex/cast_to_identity.hpp](yggr/ppex/cast_to_identity.hpp)

**核心宏**
- `YGGR_PP_CAST_TO_IDENTITY(__x__)`
  - 直接展开为 `__x__`。
  - 主要用于宏编程场景中保留传入参数内容而不改变其形式。
- `YGGR_PP_IDENTITY`
  - 定义为 `YGGR_PP_CAST_TO_IDENTITY` 的别名。
  - 提供更简洁的身份宏名称。

**实现依赖**
- 包含头文件 `yggr/base/yggrdef.h`。
- 包含 `boost/preprocessor/cat.hpp`，但本文件当前未直接使用该依赖。

**用途**
- 在预处理宏扩展时，作为“身份函数”保留原始输入。
- 可用于宏展开过程中的占位符、参数传递或避免额外层级展开。

**示例**
```cpp
#define X 123
const int value = YGGR_PP_IDENTITY(X);
```

**相关文件**
- `yggr/ppex/cast_to_identity.hpp`
- `yggr/base/yggrdef.h`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参考源文件。