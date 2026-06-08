**概述**
- **描述：** `aligned_storage_sizeof.hpp` 提供一个用于计算类型对齐存储大小的预处理宏 `YGGR_PP_ALIGNED_STORAGE_SIZEOF`。
- **头文件：** [yggr/ppex/aligned_storage_sizeof.hpp](yggr/ppex/aligned_storage_sizeof.hpp)

**核心内容**
- 包含头文件 `yggr/type_traits/aligned_storage_sizeof.hpp`。
- 定义宏 `YGGR_PP_ALIGNED_STORAGE_SIZEOF(__x__)`，其值为 `::yggr::aligned_storage_sizeof<__x__>::value`。

**用途**
- 宏用于在预处理阶段或模板元编程中获取类型 `__x__` 的对齐存储大小。
- 该宏是对 `yggr::aligned_storage_sizeof` 类型 trait 的简化封装。

**示例**
```cpp
std::size_t size = YGGR_PP_ALIGNED_STORAGE_SIZEOF(int);
```

**相关文件**
- `yggr/ppex/aligned_storage_sizeof.hpp`
- `yggr/type_traits/aligned_storage_sizeof.hpp`

**作者 / 许可证**
- 源文件头部包含 MIT 风格许可声明，许可条款请参见源文件顶部。