**概述**
- **描述：** `swap_this_def.hpp` 定义了一组预处理宏，用于声明和实现基于对象自身 `swap` 成员函数的非成员 `swap` 函数。
- **头文件：** [yggr/ppex/swap_this_def.hpp](yggr/ppex/swap_this_def.hpp)

**核心宏**
- `YGGR_PP_SWAP_THIS_REF_DEF(__type__)`
  - 声明一个 `void swap(__type__& l, __type__& r);` 的非成员交换函数。
- `YGGR_PP_SWAP_THIS_REF_DEF_IMPL(__type__)`
  - 实现上述交换函数，调用 `l.swap(r)`。
- `YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(__type__)`
  - 以上实现的 `inline` 版本。

**支持不同类型的交换**
- `YGGR_PP_SWAP_THIS_SUPPORT_REF_DEF(__type__, __support_type__)`
  - 声明两个交换函数：
    - `void swap(__type__& l, __type__::__support_type__& r);`
    - `void swap(__type__::__support_type__& l, __type__& r);`
- `YGGR_PP_SWAP_THIS_SUPPORT_REF_DEF_IMPL(__type__, __support_type__)`
  - 提供这两个函数的实现，分别调用对应对象的成员 `swap`。
- `YGGR_PP_INLINE_SWAP_THIS_SUPPORT_REF_DEF_IMPL(__type__, __support_type__)`
  - 以上实现的 `inline` 版本。

**用途**
- 快速生成符合 C++ 交换约定的非成员 `swap` 函数，避免手写重复代码。
- 支持两个对象类型之间通过其中一个类型的成员 `swap` 执行交换操作。
- 便于在泛型代码中使用 ADL 查找到合适的 `swap` 实现。

**示例**
```cpp
YGGR_PP_SWAP_THIS_REF_DEF_IMPL(MyType)

YGGR_PP_SWAP_THIS_SUPPORT_REF_DEF_IMPL(MyType, SupportType)
```

**相关文件**
- `yggr/ppex/swap_this_def.hpp`
- `yggr/base/yggrdef.h`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，详细内容请参见源文件。