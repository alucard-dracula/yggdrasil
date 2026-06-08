**概述**
- **描述：** `pointer_size.hpp` 定义了用于获取目标平台指针大小的预处理宏 `YGGR_PP_POINTER_SIZE()`。
- **头文件：** [yggr/ppex/pointer_size.hpp](yggr/ppex/pointer_size.hpp)

**核心宏**
- `YGGR_PP_POINTER_SIZE()`
  - 根据平台宏返回指针字节大小。
  - 当定义 `YGGR_SYSTEM_256` 时返回 `32`。
  - 当定义 `YGGR_SYSTEM_128` 时返回 `16`。
  - 当定义 `YGGR_SYSTEM_64` 时返回 `8`。
  - 其他情况默认返回 `4`。

**用途**
- 在平台无关的宏计算中获取指针大小，从而实现与指针对齐或缓冲区大小相关的逻辑。
- 常与对齐宏（如 `YGGR_PP_ALIGNED_OF_PTR_BEGIN` / `YGGR_PP_ALIGNED_OF_PTR_END`）结合使用。

**实现依赖**
- 包含头文件 `yggr/base/yggrdef.h`。

**示例**
```cpp
static_assert(YGGR_PP_POINTER_SIZE() == sizeof(void*), "pointer size mismatch");
```

**相关文件**
- `yggr/ppex/pointer_size.hpp`
- `yggr/ppex/aligned.hpp`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件。