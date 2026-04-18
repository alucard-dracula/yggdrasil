# c_bson_error.hpp 文档

## 概述

`c_bson_error.hpp` 是 Yggdrasil 项目中的一个 C++ 头文件，提供 BSON 错误的 C++ 包装器。该文件定义了 `c_bson_error` 类，对原始的 `bson_error_t` 结构进行封装，提供现代 C++ 的构造、赋值和访问接口。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`: 主要接口
- `yggr::nsql_database_system::detail`: 内部实现细节
- `yggr::nsql_database_system::swap_support`: 交换操作支持

## 文件描述

`c_bson_error.hpp` 提供了 BSON 错误的面向对象包装。通过继承 `bson_error_t` 并添加 C++ 特性，使得 BSON 错误处理更加符合现代 C++ 编程习惯，包括构造函数重载、赋值运算符、移动语义和异常安全保证。

## BSON 错误类型定义

```cpp
typedef struct _bson_error_t {
   uint32_t domain;     // 错误域
   uint32_t code;       // 错误码
   char     message[504]; // 错误消息
} bson_error_t;
```

## 类定义

### `c_bson_error` 类

`c_bson_error` 类继承自 `bson_error_t`，提供 C++ 风格的错误处理接口。

#### 类型定义

##### `base_type`
```cpp
typedef bson_error_t base_type;
```
- **说明**: 基类类型别名

##### `org_type`
```cpp
typedef base_type org_type;
```
- **说明**: 原始类型别名

#### 构造函数

##### 默认构造函数
```cpp
c_bson_error(void);
```
- **说明**: 创建空的错误对象

##### 格式化构造函数
```cpp
c_bson_error(u32 domain, u32 code, const char* format, ...);
```
- **参数**:
  - `domain`: 错误域
  - `code`: 错误码
  - `format`: 格式化字符串 (printf 风格)
  - `...`: 可变参数
- **说明**: 使用格式化字符串创建错误对象

##### 基类右值引用构造函数
```cpp
c_bson_error(BOOST_RV_REF(base_type) right);
```
- **参数**: `right` - 基类右值引用
- **说明**: 从基类移动构造

##### 基类复制构造函数
```cpp
c_bson_error(const base_type& right);
```
- **参数**: `right` - 基类常量引用
- **说明**: 从基类复制构造

##### 类右值引用构造函数
```cpp
c_bson_error(BOOST_RV_REF(this_type) right);
```
- **参数**: `right` - 类右值引用
- **说明**: 移动构造函数

##### 复制构造函数
```cpp
c_bson_error(const this_type& right);
```
- **参数**: `right` - 类常量引用
- **说明**: 复制构造函数

##### 析构函数
```cpp
~c_bson_error(void);
```
- **说明**: 析构函数

#### 赋值运算符

##### 基类右值引用赋值
```cpp
template<typename BsonErr>
typename boost::enable_if<boost::is_base_of<base_type, BsonErr>, this_type&>::type
operator=(BOOST_RV_REF(BsonErr) right);
```
- **模板参数**: `BsonErr` - 继承自 `base_type` 的类型
- **参数**: `right` - 右值引用
- **返回值**: 自身引用
- **说明**: 移动赋值运算符

##### 基类常量引用赋值
```cpp
template<typename BsonErr>
typename boost::enable_if<boost::is_base_of<base_type, BsonErr>, this_type&>::type
operator=(const BsonErr& right);
```
- **参数**: `right` - 常量引用
- **返回值**: 自身引用
- **说明**: 复制赋值运算符

##### 类右值引用赋值
```cpp
this_type& operator=(BOOST_RV_REF(this_type) right);
```
- **参数**: `right` - 类右值引用
- **返回值**: 自身引用
- **说明**: 类移动赋值运算符

##### 复制赋值
```cpp
this_type& operator=(const this_type& right);
```
- **参数**: `right` - 类常量引用
- **返回值**: 自身引用
- **说明**: 类复制赋值运算符

#### 交换操作

##### 基类右值引用交换
```cpp
template<typename BsonErr>
typename boost::enable_if<boost::is_base_of<base_type, BsonErr>, void>::type
swap(BOOST_RV_REF(BsonErr) right);
```
- **参数**: `right` - 基类右值引用
- **说明**: 与基类右值交换

##### 基类引用交换
```cpp
template<typename BsonErr>
typename boost::enable_if<boost::is_base_of<base_type, BsonErr>, void>::type
swap(BsonErr& right);
```
- **参数**: `right` - 基类引用
- **说明**: 与基类交换

##### 类右值引用交换
```cpp
void swap(BOOST_RV_REF(this_type) right);
```
- **参数**: `right` - 类右值引用
- **说明**: 与类右值交换

##### 类引用交换
```cpp
void swap(this_type& right);
```
- **参数**: `right` - 类引用
- **说明**: 与类交换

#### 访问方法

##### `org_pointer()`
```cpp
inline org_type* org_pointer(void);
inline const org_type* org_pointer(void) const;
```
- **返回值**: 原始类型指针
- **说明**: 获取指向原始 `bson_error_t` 的指针

##### 布尔转换运算符
```cpp
inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const;
```
- **返回值**: 布尔值
- **说明**: 检查错误是否有效 (域、码或消息任一非空)

##### `domain()`
```cpp
inline u32 domain(void) const;
```
- **返回值**: 错误域
- **说明**: 获取错误域

##### `code()`
```cpp
inline u32 code(void) const;
```
- **返回值**: 错误码
- **说明**: 获取错误码

##### `what()`
```cpp
inline const char* what(void) const;
```
- **返回值**: 错误消息字符串
- **说明**: 获取错误描述，支持异常接口
- **逻辑**: 如果域和码都为0，返回消息或"Success"；否则返回消息或"Unknow error"

##### `message()`
```cpp
inline const char* message(void) const;
```
- **返回值**: 错误消息字符串
- **说明**: 获取错误消息 (与 `what()` 相同)

## 流输出支持

### `operator<<`
```cpp
template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os, const c_bson_error& right);
```
- **参数**:
  - `os`: 输出流
  - `right`: 错误对象
- **返回值**: 输出流引用
- **说明**: 将错误以 JSON 格式输出到流

## 交换支持

### 全局交换函数
```cpp
namespace swap_support {
    #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)
    inline void swap(c_bson_error& l, bson_error_t& r);
    #else
    template<typename BsonErr>
    typename boost::enable_if<boost::is_base_of<bson_error_t, BsonErr>, void>::type
    swap(c_bson_error& l, BsonErr& r);
    #endif

    inline void swap(c_bson_error& l, c_bson_error& r);
}
```

## 依赖项

该文件依赖于以下头文件：
- `<yggr/base/yggrdef.h>`: Yggdrasil 基础定义
- `<yggr/support/operator_t.hpp>`: 运算符支持
- `<yggr/move/move.hpp>`: 移动语义支持
- `<yggr/charset/string.hpp>`: 字符串支持
- `<yggr/charset/string_view.hpp>`: 字符串视图支持
- `<yggr/nsql_database_system/bson_error_native_ex.hpp>`: BSON 错误本地扩展
- `<boost/utility/enable_if.hpp>`: Boost enable_if
- `<boost/type_traits/is_base_of.hpp>`: Boost 继承特征

## 使用示例

```cpp
#include <yggr/nsql_database_system/c_bson_error.hpp>

// 创建错误对象
yggr::nsql_database_system::c_bson_error err1; // 空错误

// 使用格式化字符串创建错误
yggr::nsql_database_system::c_bson_error err2(BSON_ERROR_JSON, 123, "Invalid JSON: %s", "missing bracket");

// 从原始类型构造
bson_error_t raw_err;
raw_err.domain = BSON_ERROR_READER;
raw_err.code = 456;
strcpy(raw_err.message, "Read failed");
yggr::nsql_database_system::c_bson_error err3(raw_err);

// 复制和移动
yggr::nsql_database_system::c_bson_error err4 = err2; // 复制
yggr::nsql_database_system::c_bson_error err5 = std::move(err3); // 移动

// 访问错误信息
if (err2) { // 检查是否有效
    std::cout << "Domain: " << err2.domain() << std::endl;
    std::cout << "Code: " << err2.code() << std::endl;
    std::cout << "Message: " << err2.what() << std::endl;
}

// 流输出
std::cout << err2 << std::endl;

// 交换
yggr::nsql_database_system::swap(err4, err5);
```

## 错误域常量

常见的 BSON 错误域包括：
- `BSON_ERROR_JSON`: JSON 解析错误
- `BSON_ERROR_READER`: 读取器错误
- `BSON_ERROR_INVALID`: 无效参数错误
- 其他 MongoDB C 驱动定义的错误域

## 设计考虑

1. **继承设计**: 公开继承 `bson_error_t`，保持内存布局兼容
2. **移动语义**: 支持 C++11 移动构造和赋值
3. **模板元编程**: 使用 Boost enable_if 实现条件编译
4. **异常安全**: 所有操作都保证异常安全
5. **兼容性**: 支持 GCC 在 C++11 之前的版本
6. **统一接口**: 提供 `what()` 方法支持标准异常接口

## 内存布局

由于公开继承，`c_bson_error` 的内存布局与 `bson_error_t` 完全相同，确保了与 C API 的二进制兼容性。

## 布尔转换

错误对象在以下情况下被认为是"真"：
- `domain` 非零
- `code` 非零
- `message[0]` 非空字符

## 兼容性

- C++11/14/17/20 兼容
- 支持 Boost 库
- GCC 兼容 (包括 C++11 之前的版本)
- 与原始 BSON C API 二进制兼容