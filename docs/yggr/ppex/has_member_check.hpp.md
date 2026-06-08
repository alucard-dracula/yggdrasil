**概述**
- **描述：** `has_member_check.hpp` 定义了一组基于 SFINAE 的预处理宏，用于检测类型是否包含特定成员函数、静态成员函数、成员类型、成员变量或静态成员变量。
- **头文件：** [yggr/ppex/has_member_check.hpp](yggr/ppex/has_member_check.hpp)

**核心功能**
- 提供成员函数检测：`YGGR_PP_HAS_MEMBER_FOO_DEF`、`YGGR_PP_HAS_ANY_MEMBER_FOO_DEF` 及其调用辅助宏。
- 提供静态成员函数检测：`YGGR_PP_HAS_STATIC_MEMBER_FOO_DEF`、`YGGR_PP_HAS_STATIC_MEMBER_FOO_DEF_CC` 及其辅助宏。
- 提供成员类型检测：`YGGR_PP_HAS_MEMBER_TYPE_DEF`。
- 提供成员变量检测：`YGGR_PP_HAS_MEMBER_VAR_DEF`、`YGGR_PP_HAS_ANY_MEMBER_VAR_DEF` 及其辅助宏。
- 提供静态成员变量检测：`YGGR_PP_HAS_STATIC_MEMBER_VAR_DEF`。

**实现细节**
- 通过模板偏特化和函数重载选择实现 `true_type/false_type` 判定。
- 使用 `boost::mpl::bool_` 将结果封装为类型 `type`。
- 利用 `YGGR_PP_MAKE_FOO_POINTER` / `YGGR_PP_MAKE_VAR_POINTER` 处理成员指针生成，兼容旧版 C++ 和多种调用约定。
- 对于 `has_any` 版本，使用派生结构和 `fallback_t` 防止函数名隐藏，检测成员是否存在于类层次结构中。

**主要宏说明**
- `YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_BEGIN()` / `_END()`
  - 定义检测宏所使用的命名空间 `yggr::has_some`。
- `YGGR_PP_HAS_MEMBER_FOO_DEF(__name__, __type__, __ret_type__, __foo_name__, __is_const__, __params__)`
  - 检测类型 `__type__` 是否包含成员函数 `__foo_name__`，返回类型为 `__ret_type__`，参数列表为 `__params__`。
  - `__is_const__` 决定是否检测 const 成员函数。
- `YGGR_PP_HAS_ANY_MEMBER_FOO_DEF(__name__, __foo_name__)`
  - 检测类型是否包含任意版本的成员函数 `__foo_name__`（而不关心具体签名）。
- `YGGR_PP_HAS_MEMBER_TYPE_DEF(__name__, __type_name__)`
  - 检测类型是否包含嵌套类型别名 `__type_name__`。
- `YGGR_PP_HAS_MEMBER_VAR_DEF(__name__, __type__, __mem_var_type__, __var_name__)`
  - 检测类型是否包含成员变量 `__var_name__`，类型为 `__mem_var_type__`。
- `YGGR_PP_HAS_ANY_MEMBER_VAR_DEF(__name__, __var_name__)`
  - 检测类型是否包含任意成员变量 `__var_name__`。
- `YGGR_PP_HAS_STATIC_MEMBER_FOO_DEF` / `YGGR_PP_HAS_STATIC_MEMBER_VAR_DEF`
  - 检测静态成员函数或静态成员变量是否存在。

**辅助宏**
- `YGGR_PP_HAS_MEMBER_FOO_TYPE(__name__, __type__)`
- `YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(__name__, __type__)`
- `YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_VALUE(__name__, __type__)`
- `YGGR_PP_HAS_MEMBER_T_TYPE_VALUE(__name__, __type__)`
- `YGGR_PP_HAS_MEMBER_VAR_TYPE_VALUE(__name__, __type__)`
- `YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE_VALUE(__name__, __type__)`

**平台与编译器兼容性**
- 支持 `BOOST_MEM_FN_ENABLE_CDECL`、`BOOST_MEM_FN_ENABLE_STDCALL`、`BOOST_MEM_FN_ENABLE_FASTCALL` 等调用约定检测。
- 使用 `boost::is_class` 和 `yggr::native_t` 处理非类类型与成员类型检测。
- 兼容旧版 C++ 语法，使用 `YGGR_CPP_VERSION` 控制成员指针生成规则。

**示例**
```cpp
YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_BEGIN()
YGGR_PP_HAS_MEMBER_FOO_DEF(begin, T, typename T::iterator, begin, YGGR_PP_SYMBOL_BOOL_TRUE(), YGGR_PP_CONCAT(void))
YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_END()

static_assert(YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(begin, std::vector<int>), "vector has begin");
```

**相关文件**
- `yggr/ppex/has_member_check.hpp`
- `yggr/ppex/concat.hpp`
- `yggr/ppex/symbols.hpp`
- `yggr/type_traits/native_t.hpp`
- `yggr/typeof/typeof.hpp`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件。