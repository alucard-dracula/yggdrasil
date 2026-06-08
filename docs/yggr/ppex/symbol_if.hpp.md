**概述**
- **描述：** `symbol_if.hpp` 定义了一个用于条件展开符号的预处理宏 `YGGR_PP_SYMBOL_IF`，它基于 Boost.Preprocessor 的 `BOOST_PP_IF` 实现，并兼容不同预处理器配置。
- **头文件：** [yggr/ppex/symbol_if.hpp](yggr/ppex/symbol_if.hpp)

**核心宏**
- `YGGR_PP_SYMBOL_IF(__cond__, __symbol__)`
  - 当 `__cond__` 为真时展开 `__symbol__()`；否则展开为空。
  - 通过 `BOOST_PP_IF` 进行条件分支，避免直接写 `if`。

**兼容性处理**
- 对 `BOOST_PP_CONFIG_FLAGS()` 中含有 `BOOST_PP_CONFIG_EDG()` 的编译器，使用辅助宏 `YGGR_PP_SYMBOL_IF_I` 进行安全展开。
- 对其他预处理器直接展开为 `BOOST_PP_IF(__cond__, __symbol__, BOOST_PP_EMPTY)()`。

**实现依赖**
- 包含 `yggr/base/yggrdef.h`。
- 包含 Boost.Preprocessor 的 `config/config.hpp`、`control/if.hpp`、`facilities/empty.hpp`。

**用途**
- 在预处理宏中根据条件选择性生成一个符号调用。
- 常用于宏参数拼接、基于条件启用某个宏名称或表达式的场景。

**示例**
```cpp
#define TAG(x) x
YGGR_PP_SYMBOL_IF(1, TAG) // 展开为 TAG()
YGGR_PP_SYMBOL_IF(0, TAG) // 展开为空
```

**相关文件**
- `yggr/ppex/symbol_if.hpp`
- `yggr/base/yggrdef.h`
- `boost/preprocessor/control/if.hpp`
- `boost/preprocessor/facilities/empty.hpp`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件。