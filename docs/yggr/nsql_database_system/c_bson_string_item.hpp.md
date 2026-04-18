# c_bson_string_item.hpp 文档

## 概述

`c_bson_string_item.hpp` 是 Yggdrasil 项目中的 C++ 头文件，提供 BSON 字符串项的通用封装类 `c_bson_string_item`。该类基于 `utf8_string`，封装了字符串存取、比较、赋值、交换与类型转换等常见操作，供其他 BSON 值类型（如 `c_bson_regex`）复用。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`
- `yggr::nsql_database_system::swap_support`

## 类 `c_bson_string_item`

### 类型别名

- `typedef utf8_string string_type`：内部使用的字符串类型

### 构造与析构

- `c_bson_string_item()`：默认构造
- 模板构造函数：`c_bson_string_item(const String&)` 支持任意可转换为 UTF-8 的字符串类型
- `c_bson_string_item(BOOST_RV_REF(string_type))`：移动构造
- `c_bson_string_item(const string_type&)`：复制构造
- 移动与复制构造（对本类）
- 析构函数：`~c_bson_string_item()`

### 赋值运算符

- `operator=(const Char*)`：为本地字符指针赋值（仅当 `Char` 为原生字符类型）
- `operator=(const String&)`：支持任意可转换为 UTF-8 的字符串类型
- `operator=(BOOST_RV_REF(string_type))`：移动赋值
- `operator=(BOOST_RV_REF(this_type))`：移动赋值（本类）
- 复制赋值运算符

赋值操作内部使用 `copy_or_move_or_swap` 优化拷贝/移动/交换。

### 交换

- `swap(BOOST_RV_REF(this_type))` 与 `swap(this_type&)`：提供移动与普通交换
- 在 `swap_support` 命名空间内通过宏注册全局 `swap`

### 比较

- `compare_eq(const this_type& right) const`：比较两者是否相等（地址相同或字符串相等）
- 非成员 `operator==` / `operator!=` 基于 `compare_eq`

### 转换与访问

- `operator string_type&()` 与 `operator const string_type&() const`：隐式转换为 `utf8_string` 引用
- 模板转换 `operator Basic_String<Char, Traits, Alloc>() const`：转换为任意目标字符串类型（通过 `utf8_string::str`）
- `str()` 返回对内部字符串的引用，并提供一系列重载（通过宏 `YGGR_PP_C_MONGO_STRING_MEMBER_GET_*`）以支持多种访问模式

### 内部成员

- `string_type _str`：实际存储的 UTF-8 字符串

## 依赖项

- `<yggr/charset/utf8_string.hpp>`
- `<yggr/utility/copy_or_move_or_swap.hpp>`
- `<yggr/move/move.hpp>`
- `<yggr/ppex/swap_this_def.hpp>`
- `<yggr/mplex/tag_sfinae.hpp>`
- `<yggr/nsql_database_system/detail/string_member_var_get.hpp>`
- Boost `enable_if` / type traits / mpl

## 使用示例

```cpp
#include <yggr/nsql_database_system/c_bson_string_item.hpp>

using yggr::nsql_database_system::c_bson_string_item;

c_bson_string_item s1; // 默认构造
s1 = "hello"; // 赋值 C 字符串

c_bson_string_item s2(std::string("world")); // 从 std::string 构造

if(s1 != s2) {
    auto ss = s2.str(); // 访问内部 utf8_string
}

// 交换
s1.swap(s2);

// 隐式转换为 std::string（通过模板）
std::string out = static_cast<std::string>(s1);
```

## 设计考虑

- 将字符串操作封装成可复用组件，减少重复实现
- 通过 `utf8_string` 统一字符集处理，保证多语言兼容性
- 使用 `enable_if` 与 traits 控制模板重载的启用条件，保证类型安全
- 使用 `copy_or_move_or_swap` 提高赋值与移动的效率

## 保存路径

`e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\c_bson_string_item.md`
