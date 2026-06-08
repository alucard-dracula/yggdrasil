**概述**
- **描述：** `friend.hpp` 定义了一组预处理宏，用于在类内部声明友元类和友元类型，兼顾不同编译器对 `friend typename` 语法的支持差异。
- **头文件：** [yggr/ppex/friend.hpp](yggr/ppex/friend.hpp)

**核心宏**
- `YGGR_PP_FRIEND_CLASS(__type__)`
  - 展开为 `friend class __type__`。
  - 用于声明友元类。
- `YGGR_PP_FRIEND_TYPENAME(__type__)`
  - 在 MSVC 下展开为 `friend typename __type__`。
  - 在 Darwin/Clang 下展开为 `friend typename yggr::traits<__type__>::type`。
  - 在其他非 MSVC 平台下展开为 `friend class yggr::traits<__type__>::type`。
- `YGGR_PP_FRIEND_DEDUCED_TYPENAME(__type__)`
  - 用于声明通过 `typename` 或 `traits` 解析的友元类型。
  - 在 MSVC 下展开为 `friend typename __type__`。
  - 在 Darwin/Clang 下展开为 `friend typename yggr::traits<typename __type__>::type`。
  - 在其他非 MSVC 平台下展开为 `friend class yggr::traits<typename __type__>::type`。

**条件编译与兼容性**
- 对 MSVC、Darwin/Clang 和其他编译器分别提供不同实现。
- 非 MSVC 平台会额外包含 `yggr/type_traits/traits.hpp`，用于 `yggr::traits` 类型封装。

**用途**
- 统一声明友元类型的宏接口。
- 解决不同编译器对 `friend typename` 和 `friend class` 语法的兼容性问题。
- 适用于模板类内部声明对外部类型或类型萃取结果的友元访问权限。

**示例**
```cpp
class MyClass
{
    YGGR_PP_FRIEND_CLASS(FriendClass);
    YGGR_PP_FRIEND_TYPENAME(SomeType);
    YGGR_PP_FRIEND_DEDUCED_TYPENAME(AnotherType);
};
```

**相关文件**
- `yggr/ppex/friend.hpp`
- `yggr/type_traits/traits.hpp`
- `yggr/base/yggrdef.h`

**作者 / 许可证**
- 源文件顶部包含 MIT 风格许可声明，具体条款请参见源文件。