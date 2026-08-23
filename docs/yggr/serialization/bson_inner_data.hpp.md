# `bson_inner_data.hpp`

## 概述

该头文件为 yggdrasil BSON 内部数据类型配置 Boost.Serialization 支持。

主要职责包括：

- 将多个 BSON 类型标记为 Boost.Serialization 的原始类型实现；
- 为带作用域代码、二进制缓冲区和数据库指针等模板类型设置实现级别；
- 为 Boost.Serialization 的版本元数据注册 BSON 类型标识。

该文件主要提供编译期注册和特征配置，不定义新的运行时数据结构。

## 头文件

```cpp
#include <yggr/serialization/bson_inner_data.hpp>
```

## 依赖

主要依赖：

- `yggr/nsql_database_system/c_bson.hpp`：BSON 数据类型定义；
- `boost/serialization/string.hpp`：字符串序列化支持；
- `boost/serialization/collections_save_imp.hpp`：集合保存实现；
- `boost/serialization/collections_load_imp.hpp`：集合加载实现；
- `boost/serialization/split_free.hpp`：分离式序列化支持；
- `boost/serialization/collection_traits.hpp`：集合特征；
- `boost/serialization/version.hpp`：版本元数据类型。

## BSON 原始类型实现级别

文件使用 `BOOST_CLASS_IMPLEMENTATION` 将以下类型声明为
`boost::serialization::primitive_type`：

- `c_bson_decimal128::org_type`；
- `c_bson_decimal128`；
- `c_bson_date`；
- `c_bson_time`；
- `c_bson_timeval::org_type`；
- `c_bson_timeval`；
- `c_bson_oid::org_type`；
- `c_bson_oid`；
- `c_bson_null`；
- `c_bson_undefined`；
- `c_bson_minkey`；
- `c_bson_maxkey`；
- `c_bson_regex`；
- `c_bson_code`；
- `c_bson_symbol`；
- `c_bson_timestamp::base_type`；
- `c_bson_timestamp`；
- `c_bson_value::base_type`；
- `c_bson_value`；
- `c_bson`。

示例：

```cpp
BOOST_CLASS_IMPLEMENTATION(
    yggr::nsql_database_system::c_bson_date,
    boost::serialization::primitive_type)
```

这表示序列化框架将对应 BSON 类型视为原始值处理。具体字段访问和序列化函数仍由各 BSON 类型及其序列化实现提供。

## 模板类型实现级别

文件为以下模板类型特化 `implementation_level_impl`，并将实现级别设置为
`boost::serialization::primitive_type`：

```cpp
template<typename Scope>
struct implementation_level_impl<
    const yggr::nsql_database_system::c_bson_code_w_scope<Scope> >;

template<yggr::u32 BINARY_ID>
struct implementation_level_impl<
    const yggr::nsql_database_system::c_bson_binary_buffer<BINARY_ID> >;

template<yggr::u32 BINARY_ID, typename Buffer>
struct implementation_level_impl<
    const yggr::nsql_database_system::c_bson_binary_buffer_ref<
        BINARY_ID, Buffer> >;

template<typename BsonOid>
struct implementation_level_impl<
    const yggr::nsql_database_system::c_bson_dbpointer<BsonOid> >;
```

每个特化都提供：

```cpp
typedef mpl::integral_c_tag tag;
typedef mpl::int_<boost::serialization::primitive_type> type;
YGGR_STATIC_CONSTANT(int, value = implementation_level_impl::type::value);
```

这些特征允许 Boost.Serialization 在处理带 `const` 的对应 BSON 类型时采用原始类型级别。

## BSON 类型标识注册

文件为 Boost.Serialization 的两个版本元数据类型注册 BSON `int32` 类型标识：

```cpp
YGGR_PP_BSON_TYPEID_IMPL(
    boost::serialization::version_type,
    bson_typeid_def::E_BSON_TYPE_INT32)

YGGR_PP_BSON_TYPEID_IMPL(
    boost::serialization::item_version_type,
    bson_typeid_def::E_BSON_TYPE_INT32)
```

这表示：

- `boost::serialization::version_type` 使用 BSON `int32`；
- `boost::serialization::item_version_type` 使用 BSON `int32`。

该注册用于 BSON 归档在处理类版本和元素版本元数据时确定对应的 BSON 类型。

## 使用方式

通常不需要直接调用本文件中的任何函数。包含该文件后，相关编译期特征会自动生效：

```cpp
#include <yggr/serialization/bson_inner_data.hpp>
#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>
```

之后即可由项目 BSON 归档处理已配置的 BSON 数据类型。

## 注意事项

1. 该文件主要通过宏和模板特征修改 Boost.Serialization 行为，不提供独立运行时 API。
2. 标记为 `primitive_type` 只表示序列化实现级别，不代表 BSON 类型一定是 C++ 内置类型。
3. BSON 类型的实际编码、解码和字段布局由 `c_bson` 系列类型及 BSON 归档实现决定。
4. `c_bson_binary_buffer_ref` 可能引用外部缓冲区，生命周期和所有权仍由对应缓冲区管理。
5. 类型特征特化包含 `const` 类型，使用自定义 BSON 类型时应注意顶层 `const` 是否匹配。
6. 版本元数据被映射为 BSON `int32`，归档双方应使用兼容的 BSON 类型约定。
7. 文件末尾的 include guard 名称与文件名注释存在历史命名差异，使用时应依赖实际宏保护而不是注释名称。
8. 该文件依赖当前 Boost.Serialization 和 yggdrasil BSON 类型定义，升级相关组件后应验证特征兼容性。

## 相关组件

- `yggr/nsql_database_system/c_bson.hpp`
- `yggr/serialization/export.hpp`
- `yggr/serialization/nvp.hpp`
- `yggr/archive/bson_oarchive.hpp`
- `yggr/archive/bson_iarchive.hpp`
- `boost/serialization/implementation_level.hpp`
- `boost/serialization/version.hpp`
