# c_bson_maxkey.hpp 文档

## 概述

`c_bson_maxkey.hpp` 是 Yggdrasil 项目中的一个 C++ 头文件，提供 BSON MaxKey 类型的 C++ 包装器。该文件定义了 `c_bson_maxkey` 类，用于表示 MongoDB BSON 中的最大键值，在索引和排序操作中具有特殊意义。

## 许可证

该文件遵循 MIT 许可证，版权归 yggdrasil (2010-2024)。

## 命名空间

- `yggr::nsql_database_system`: 主要接口
- `yggr::nsql_database_system::detail`: 内部实现细节
- `yggr::nsql_database_system::swap_support`: 交换操作支持

## 文件描述

`c_bson_maxkey.hpp` 实现了 BSON MaxKey 类型的 C++ 封装。MaxKey 是 MongoDB 中的一个特殊值，在排序时总是大于任何其他值（除了其他 MaxKey）。它主要用于复合索引和范围查询的边界定义。

## 类型 ID 定义

```cpp
YGGR_PP_BSON_TYPEID_IMPL(c_bson_maxkey, bson_typeid_def::E_BSON_TYPE_MAXKEY)
```

- **类型 ID**: `bson_typeid_def::E_BSON_TYPE_MAXKEY`
- **说明**: 将 `c_bson_maxkey` 类注册为 BSON MaxKey 类型

## 类定义

### `c_bson_maxkey` 类

`c_bson_maxkey` 是一个表示 BSON MaxKey 值的类。由于 MaxKey 是一个特殊标记值，该类没有实际的数据成员。

#### 构造函数

##### 默认构造函数
```cpp
c_bson_maxkey(void);
```
- **说明**: 创建 MaxKey 对象

##### BSON 值构造函数
```cpp
c_bson_maxkey(const bson_value_t& val);
```
- **参数**: `val` - BSON 值对象
- **说明**: 从 BSON 值构造 MaxKey（会进行类型验证）

##### 移动构造函数
```cpp
c_bson_maxkey(BOOST_RV_REF(this_type));
```
- **参数**: 右值引用
- **说明**: 移动构造函数

##### 复制构造函数
```cpp
c_bson_maxkey(const this_type&);
```
- **参数**: 常量引用
- **说明**: 复制构造函数

##### 析构函数
```cpp
~c_bson_maxkey(void);
```
- **说明**: 析构函数

#### 赋值运算符

##### BSON 值赋值
```cpp
template<typename BsonValue>
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
operator=(const BsonValue& val);
```
- **模板参数**: `BsonValue` - 继承自 `bson_value_t` 的类型
- **参数**: `val` - BSON 值
- **返回值**: 自身引用
- **说明**: 从 BSON 值赋值（会进行类型验证）

##### 移动赋值
```cpp
this_type& operator=(BOOST_RV_REF(this_type));
```
- **参数**: 右值引用
- **返回值**: 自身引用
- **说明**: 移动赋值运算符

##### 复制赋值
```cpp
this_type& operator=(const this_type&);
```
- **参数**: 常量引用
- **返回值**: 自身引用
- **说明**: 复制赋值运算符

#### 交换操作

##### 移动交换
```cpp
inline void swap(BOOST_RV_REF(this_type));
```
- **参数**: 右值引用
- **说明**: 与右值交换

##### 引用交换
```cpp
inline void swap(this_type&);
```
- **参数**: 引用
- **说明**: 与引用交换

#### 类型 ID 方法

##### `value_typeid()`
```cpp
YGGR_CONSTEXPR_OR_INLINE static u32 value_typeid(void);
```
- **返回值**: 类型 ID (`bson_typeid_def::E_BSON_TYPE_MAXKEY`)
- **说明**: 获取类型 ID

##### `s_value_typeid()`
```cpp
YGGR_CONSTEXPR_OR_INLINE static u32 s_value_typeid(void);
```
- **返回值**: 类型 ID
- **说明**: 静态版本的类型 ID 获取方法

#### 比较方法

##### `compare_eq()` (BSON 值)
```cpp
template<typename BsonValue>
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
compare_eq(const BsonValue& val) const;
```
- **参数**: `val` - BSON 值
- **返回值**: 是否相等
- **说明**: 比较是否为 MaxKey 值

##### `compare_eq()` (MaxKey)
```cpp
bool compare_eq(const this_type& right) const;
```
- **参数**: `right` - 另一个 MaxKey
- **返回值**: `true` (所有 MaxKey 都相等)
- **说明**: MaxKey 之间的比较总是返回 true

## 序列化方法

### `save()` 方法
```cpp
template<typename Bson, typename String>
typename boost::enable_if<...>::type
save(Bson& bs, const String& name) const;
```
- **模板参数**:
  - `Bson`: BSON 对象类型
  - `String`: 字符串类型
- **参数**:
  - `bs`: BSON 对象
  - `name`: 字段名
- **返回值**: 保存是否成功
- **说明**: 将 MaxKey 保存到 BSON 对象中

### `save_value()` 方法
```cpp
template<typename Bson>
typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
save_value(Bson& bs) const;
```
- **参数**: `bs` - BSON 对象
- **返回值**: 保存是否成功
- **说明**: 将 MaxKey 值保存到 BSON 对象

### `load()` 方法
```cpp
template<typename BsonIter, typename String>
typename boost::enable_if<...>::type
load(const BsonIter& iter, const String& name);
```
- **参数**:
  - `iter`: BSON 迭代器
  - `name`: 字段名
- **说明**: 从 BSON 加载 MaxKey（实际上不需要加载，因为 MaxKey 无状态）

### `load_value()` 方法
```cpp
template<typename BsonIter>
typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
load_value(const BsonIter& iter);
```
- **参数**: `iter` - BSON 迭代器
- **说明**: 从 BSON 加载 MaxKey 值（实际上不需要加载）

## 全局运算符重载

### 比较运算符

#### `operator==`
```cpp
// MaxKey vs MaxKey
inline bool operator==(const c_bson_maxkey&, const c_bson_maxkey&);

// BSON 值 vs MaxKey
template<typename BsonValue>
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
operator==(const BsonValue& l, const c_bson_maxkey& r);

// MaxKey vs BSON 值
template<typename BsonValue>
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
operator==(const c_bson_maxkey& l, const BsonValue& r);
```

#### `operator!=`
```cpp
// MaxKey vs MaxKey
inline bool operator!=(const c_bson_maxkey&, const c_bson_maxkey&);

// BSON 值 vs MaxKey
template<typename BsonValue>
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
operator!=(const BsonValue& l, const c_bson_maxkey& r);

// MaxKey vs BSON 值
template<typename BsonValue>
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
operator!=(const c_bson_maxkey& l, const BsonValue& r);
```

## 流输出支持

### `operator<<`
```cpp
template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os, c_bson_maxkey);
```
- **参数**:
  - `os`: 输出流
  - 第二个参数: MaxKey 对象 (按值传递)
- **返回值**: 输出流引用
- **输出格式**: `{ "$maxKey" : 1 }`
- **说明**: 将 MaxKey 以 JSON 格式输出到流

## 交换支持

### 全局交换函数
```cpp
namespace swap_support {
    inline void swap(c_bson_maxkey& l, c_bson_maxkey& r);
}
```
- **说明**: MaxKey 交换操作（实际上不做任何操作，因为无状态）

## 依赖项

该文件依赖于以下头文件：
- `<yggr/base/yggrdef.h>`: Yggdrasil 基础定义
- `<yggr/ppex/friend.hpp>`: 友元类支持
- `<yggr/charset/utf8_string.hpp>`: UTF-8 字符串支持
- `<yggr/nsql_database_system/bson_typeid.hpp>`: BSON 类型 ID
- `<yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>`: BSON 内部数据访问器声明
- `<yggr/nsql_database_system/bson_native_ex.hpp>`: BSON 本地扩展
- `<yggr/nsql_database_system/bson_value_native_ex.hpp>`: BSON 值扩展
- `<yggr/nsql_database_system/bson_visit_t.hpp>`: BSON 访问器
- `<boost/utility/enable_if.hpp>`: Boost enable_if
- `<boost/type_traits/is_base_of.hpp>`: Boost 继承特征
- `<cassert>`: 断言

## 使用示例

```cpp
#include <yggr/nsql_database_system/c_bson_maxkey.hpp>

// 创建 MaxKey 对象
yggr::nsql_database_system::c_bson_maxkey maxkey1; // 默认构造
yggr::nsql_database_system::c_bson_maxkey maxkey2; // 另一个 MaxKey

// 比较操作
bool equal = (maxkey1 == maxkey2); // true，所有 MaxKey 都相等
bool not_equal = (maxkey1 != maxkey2); // false

// 检查类型 ID
u32 type_id = yggr::nsql_database_system::c_bson_maxkey::value_typeid();

// 保存到 BSON
bson_t bson;
bson_init(&bson);
maxkey1.save(bson, "max_key_field");

// 从 BSON 值构造
bson_value_t val;
val.value_type = BSON_TYPE_MAXKEY;
// ... 设置值
yggr::nsql_database_system::c_bson_maxkey maxkey3(val);

// 流输出
std::cout << maxkey1 << std::endl; // 输出: { "$maxKey" : 1 }

// 赋值操作
maxkey2 = maxkey1; // 复制赋值
```

## MaxKey 的特性

1. **排序特性**: MaxKey 在排序时总是大于任何其他值（除了其他 MaxKey）
2. **相等性**: 所有 MaxKey 值都相互相等
3. **无状态**: MaxKey 没有实际的数据内容，是一个标记值
4. **索引应用**: 常用于复合索引的边界定义

## 设计考虑

1. **无状态设计**: 由于 MaxKey 没有实际数据，类设计为无状态
2. **模板元编程**: 广泛使用 Boost enable_if 实现条件编译
3. **序列化友好**: 提供完整的保存和加载接口
4. **运算符重载**: 支持直观的比较操作
5. **类型安全**: 通过模板约束确保类型安全

## BSON 表示

在 BSON 中的 JSON 表示：
```json
{ "$maxKey" : 1 }
```

## 应用场景

- **复合索引边界**: 定义索引范围的上界
- **范围查询**: 指定查询范围的最大值
- **排序控制**: 确保某些文档始终排在最后

## 兼容性

- C++11/14/17/20 兼容
- 支持 Boost 库
- 与 MongoDB BSON 规范完全兼容