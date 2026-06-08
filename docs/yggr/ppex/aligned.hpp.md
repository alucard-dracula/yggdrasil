**概述**
- **描述：** `aligned.hpp` 定义了一组编译器对齐属性宏，用于声明具有特定对齐规则的类型或对象。
- **头文件：** [yggr/ppex/aligned.hpp](yggr/ppex/aligned.hpp)

**核心宏**
- `YGGR_PP_ALIGNED_BEGIN(__size__)`
  - 在 MSVC 下展开为 `__declspec(align(__size__))`。
  - 在非 MSVC 下展开为空。
- `YGGR_PP_ALIGNED_END(__size__)`
  - 在 MSVC 下展开为空。
  - 在 GCC/Clang 等编译器下展开为 `__attribute__((aligned(__size__)))`。

**指针大小对齐宏**
- `YGGR_PP_ALIGNED_OF_PTR_BEGIN(__size__)`
  - 用于按最小值对齐：`__size__` 与 `YGGR_PP_POINTER_SIZE()` 中较小者。
  - 在 MSVC 下展开为 `__declspec(align(...))`。
  - 在非 MSVC 下展开为空。
- `YGGR_PP_ALIGNED_OF_PTR_END(__size__)`
  - 在 MSVC 下展开为空。
  - 在非 MSVC 下展开为 `__attribute__((aligned(...)))`。

**实现依赖**
- 包含头文件 `yggr/ppex/pointer_size.hpp`，用于获取目标平台的指针大小。

**用途**
- 跨编译器声明对齐类型时使用。
- `YGGR_PP_ALIGNED_OF_PTR_*` 系列用于将对齐大小限制在指针大小范围内，避免过度对齐问题。

**示例**
```cpp
struct YGGR_PP_ALIGNED_BEGIN(16) MyAlignedStruct
{
    int value;
} YGGR_PP_ALIGNED_END(16);
```

**相关文件**
- `yggr/ppex/aligned.hpp`
- `yggr/ppex/pointer_size.hpp`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件。