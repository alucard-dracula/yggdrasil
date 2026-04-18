# bson_decimal128_native_ex.hpp 文档

## 概述

`bson_decimal128_native_ex.hpp` 是 Yggdrasil 项目中的一个 C++ 头文件，提供 BSON Decimal128 类型的本地扩展功能。该文件定义了 `bson_decimal128_native_ex` 类，用于处理高精度十进制数的转换、比较和操作。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`: 主要接口
- `yggr::nsql_database_system::detail`: 内部实现细节
- `yggr::nsql_database_system::swap_support`: 交换操作支持

## 文件描述

`bson_decimal128_native_ex.hpp` 提供了完整的 BSON Decimal128 类型支持，包括字符串转换、算术比较、流输出和交换操作。这些功能扩展了标准的 BSON Decimal128 类型，使其更容易在 C++ 代码中使用。

## Decimal128 类型定义

```cpp
typedef struct {
#if BSON_BYTE_ORDER == BSON_LITTLE_ENDIAN
   uint64_t low;
   uint64_t high;
#elif BSON_BYTE_ORDER == BSON_BIG_ENDIAN
   uint64_t high;
   uint64_t low;
#endif
} bson_decimal128_t;
```

## 类定义

### `bson_decimal128_native_ex` 类

这是一个静态方法类，提供 BSON Decimal128 的本地扩展功能。

#### 类型定义

##### `org_type`
```cpp
typedef bson_decimal128_t org_type;
```
- **说明**: BSON Decimal128 的原始类型别名

##### 常量定义
```cpp
YGGR_STATIC_CONSTANT(std::size_t, E_BSON_DECIMAL128_STRING = BSON_DECIMAL128_STRING); //43
typedef boost::mpl::string<'Infi', 'nity'> E_BSON_DECIMAL128_INF; // "Infinity"
typedef boost::mpl::string<'NaN'> E_BSON_DECIMAL128_NAN; // "NaN"
```
- `E_BSON_DECIMAL128_STRING`: Decimal128 字符串表示的最大长度 (43)
- `E_BSON_DECIMAL128_INF`: 无穷大字符串常量
- `E_BSON_DECIMAL128_NAN`: 非数值字符串常量

#### 静态方法

##### 字符串转换方法

###### `s_bson_decimal128_to_string()`
```cpp
static char* s_bson_decimal128_to_string(const bson_decimal128_t* dec, char* out);
```
- **参数**:
  - `dec`: 要转换的 Decimal128 值指针
  - `out`: 输出字符串缓冲区
- **返回值**: 输出字符串指针
- **说明**: 将 Decimal128 值转换为字符串表示

###### `s_bson_decimal128_from_string()`
```cpp
static bool s_bson_decimal128_from_string(const char* str, bson_decimal128_t* out);
static bool s_bson_decimal128_from_string(const char* str, std::size_t len, bson_decimal128_t* out);
```
- **参数**:
  - `str`: 输入字符串
  - `len`: 字符串长度 (第二个版本)
  - `out`: 输出 Decimal128 值指针
- **返回值**: 转换是否成功
- **说明**: 从字符串转换为 Decimal128 值

##### 比较方法

###### `s_bson_decimal128_compare_eq()` (算术类型)
```cpp
template<typename T> inline
static typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
    s_bson_decimal128_compare_eq(const bson_decimal128_t* pdec, T val);
```
- **模板参数**: `T` - 算术类型
- **参数**:
  - `pdec`: Decimal128 值指针
  - `val`: 要比较的算术值
- **返回值**: 是否相等
- **说明**: 比较 Decimal128 与算术值是否相等

###### `s_bson_decimal128_compare_eq()` (高低位)
```cpp
static bool s_bson_decimal128_compare_eq(const bson_decimal128_t* pdec, u64 nhigh, u64 nlow);
```
- **参数**:
  - `pdec`: Decimal128 值指针
  - `nhigh`: 高 64 位
  - `nlow`: 低 64 位
- **返回值**: 是否相等
- **说明**: 比较 Decimal128 与高低位值是否相等

###### `s_bson_decimal128_compare_eq()` (Decimal128)
```cpp
static bool s_bson_decimal128_compare_eq(const bson_decimal128_t* l, const bson_decimal128_t* r);
```
- **参数**:
  - `l`: 左侧 Decimal128 值指针
  - `r`: 右侧 Decimal128 值指针
- **返回值**: 是否相等
- **说明**: 比较两个 Decimal128 值是否相等

###### `s_bson_decimal128_compare()` (算术类型)
```cpp
template<typename T> inline
static typename boost::enable_if<boost::is_arithmetic<T>, s32>::type
    s_bson_decimal128_compare(const bson_decimal128_t* pdec, T val);
```
- **返回值**: 比较结果 (-1: 小于, 0: 等于, 1: 大于)
- **说明**: 比较 Decimal128 与算术值的大小

###### `s_bson_decimal128_compare()` (高低位)
```cpp
static s32 s_bson_decimal128_compare(const bson_decimal128_t* pdec, u64 nhigh, u64 nlow);
```
- **返回值**: 比较结果
- **说明**: 比较 Decimal128 与高低位值的大小

###### `s_bson_decimal128_compare()` (Decimal128)
```cpp
static s32 s_bson_decimal128_compare(const bson_decimal128_t* l, const bson_decimal128_t* r);
```
- **返回值**: 比较结果
- **说明**: 比较两个 Decimal128 值的大小

## 重载运算符

### 比较运算符

#### `operator==`
```cpp
// Decimal128 vs 算术类型
template<typename T>
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
operator==(const bson_decimal128_t& l, T r);

// 算术类型 vs Decimal128
template<typename T>
typename boost::enable_if<boost::is_arithmetic<T>, bool>::type
operator==(T l, const bson_decimal128_t& r);

// Decimal128 vs BSON 值
template<typename BsonValue>
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
operator==(const bson_decimal128_t& l, const BsonValue& r);

// BSON 值 vs Decimal128
template<typename BsonValue>
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
operator==(const BsonValue& l, const bson_decimal128_t& r);

// Decimal128 vs Decimal128 派生类型
template<typename BsonDeciml128>
typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDeciml128>, bool>::type
operator==(const bson_decimal128_t& l, const BsonDeciml128& r);
```

#### `operator!=`
```cpp
// 与 == 运算符对应的 != 重载
template<typename T> operator!=(const bson_decimal128_t& l, T r);
template<typename T> operator!=(T l, const bson_decimal128_t& r);
template<typename BsonValue> operator!=(const bson_decimal128_t& l, const BsonValue& r);
template<typename BsonValue> operator!=(const BsonValue& l, const bson_decimal128_t& r);
template<typename BsonDeciml128> operator!=(const bson_decimal128_t& l, const BsonDeciml128& r);
```

#### `operator<`, `operator<=`, `operator>`, `operator>=`
```cpp
// 所有比较运算符都有对应的模板重载
// 支持 Decimal128 与算术类型、BSON 值、Decimal128 派生类型的比较
```

### 流输出运算符

#### `operator<<`
```cpp
template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os, const bson_decimal128_t& right);
```
- **参数**:
  - `os`: 输出流
  - `right`: 要输出的 Decimal128 值
- **返回值**: 输出流引用
- **说明**: 将 Decimal128 值以 JSON 格式输出到流中
- **输出格式**: `{ "$numberDecimal" : "decimal_string" }`

## 交换支持

### `swap` 函数
```cpp
namespace swap_support {
    void swap(bson_decimal128_t& l, bson_decimal128_t& r);

    template<typename BsonDecimal128>
    typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDecimal128>, void>::type
    swap(bson_decimal128_t& l, BsonDecimal128& r);

    #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)
    template<typename BsonDecimal128>
    typename boost::enable_if<boost::is_base_of<bson_decimal128_t, BsonDecimal128>, void>::type
    swap(bson_decimal128_t& l, BOOST_RV_REF(BsonDecimal128) r);
    #endif
}
```

## 依赖项

该文件依赖于以下头文件：
- `<yggr/base/yggrdef.h>`: Yggdrasil 基础定义
- `<yggr/nsql_database_system/bson_config.hpp>`: BSON 配置
- `<yggr/nsql_database_system/bson_visit_t.hpp>`: BSON 访问器
- `<yggr/nsql_database_system/native_ex_decl_bson.hpp>`: BSON 本地扩展声明
- `<yggr/nsql_database_system/bson_value_native_ex.hpp>`: BSON 值扩展
- `<boost/ref.hpp>`: Boost 引用包装器
- `<boost/utility/enable_if.hpp>`: Boost enable_if
- `<boost/type_traits/is_arithmetic.hpp>`: Boost 算术类型特征
- `<boost/type_traits/is_base_of.hpp>`: Boost 继承特征
- `<boost/mpl/string.hpp>`: Boost MPL 字符串
- `<ostream>`: 标准输出流

## 使用示例

```cpp
#include <yggr/nsql_database_system/bson_decimal128_native_ex.hpp>

// 字符串转换
bson_decimal128_t dec;
const char* str = "123.456789012345678901234567890123456789";
bool success = bson_decimal128_native_ex::s_bson_decimal128_from_string(str, &dec);

char buffer[44] = {0};
bson_decimal128_native_ex::s_bson_decimal128_to_string(&dec, buffer);

// 比较操作
bool equal = bson_decimal128_native_ex::s_bson_decimal128_compare_eq(&dec, 123.456);

// 使用重载运算符
bson_decimal128_t dec2;
if (dec == dec2) { /* ... */ }
if (dec < 1000.0) { /* ... */ }

// 流输出
std::cout << dec << std::endl; // 输出: { "$numberDecimal" : "123.456789012345678901234567890123456789" }

// 交换操作
using namespace yggr::nsql_database_system;
swap(dec, dec2);
```

## 设计考虑

1. **模板元编程**: 广泛使用 Boost enable_if 和类型特征实现条件编译
2. **运算符重载**: 提供直观的比较和输出操作
3. **类型安全**: 通过模板约束确保类型安全
4. **性能优化**: 内联函数和静态方法减少运行时开销
5. **兼容性**: 支持多种字符类型和流类型

## 特殊值处理

- **Infinity**: 表示正无穷大
- **NaN**: 表示非数值
- **零值**: high=0, low=0

## 字节序考虑

Decimal128 结构根据 BSON 字节序定义不同：
- 小端字节序: low 在前，high 在后
- 大端字节序: high 在前，low 在后

## 兼容性

- C++11/14/17/20 兼容
- 支持 Boost 库
- 跨平台字节序处理
- GCC 右值引用支持 (C++11 之前)