# mongo_accesser_database.hpp

## 文件概述

`mongo_accesser_database.hpp` 是 Yggdrasil 数据库系统中的 MongoDB 数据库访问器头文件。该文件定义了 `mongo_accesser_database` 类，提供了一套完整的 MongoDB 数据库级别操作接口，包括聚合、集合管理、用户管理、监听等功能。

该类继承自 `basic_mongo_accesser_database`，并通过静态模板方法提供类型安全的 MongoDB 数据库操作。所有方法都支持多种字符串类型参数，并使用 Boost MPL 进行编译时类型检查。

## 许可证

该文件使用 MIT 许可证。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    // mongo_accesser_database 类定义
} // namespace nsql_database_system
} // namespace yggr
```

## 类定义

### mongo_accesser_database

```cpp
class mongo_accesser_database
    : public basic_mongo_accesser_database
{
public:
    // 类型定义和静态方法
};
```

#### 继承关系
- **基类**: `basic_mongo_accesser_database`

#### 模板参数说明
该类主要通过静态模板方法提供功能，不需要实例化。主要使用模板参数进行类型推导和编译时检查。

## 主要功能

### 1. 数据库聚合操作

#### s_database_aggregate
执行数据库级别的聚合管道操作。

**函数签名**:
```cpp
template<typename Connection_T, typename String1,
         typename BsonPipeline, typename BsonOpts, typename ReadPrefs,
         typename HandlerCollector>
static bool s_database_aggregate(Connection_T& conn, const String1& str_db,
                                const BsonPipeline& pipeline, const BsonOpts& opts,
                                const ReadPrefs& read_prefs, org_bson_error_type* perr,
                                const HandlerCollector& handler_collector)
```

**参数**:
- `conn`: 数据库连接对象
- `str_db`: 数据库名称
- `pipeline`: 聚合管道定义
- `opts`: 聚合选项
- `read_prefs`: 读取偏好设置
- `perr`: 错误对象指针
- `handler_collector`: 结果处理器

**返回值**: `bool` - 操作是否成功

### 2. 集合管理操作

#### s_database_create_collecion
在数据库中创建新集合。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2, typename BsonOpts>
static typename boost::enable_if<...>::type
s_database_create_collecion(Connection_T& conn, const String1& str_db, const String2& str_coln,
                           const BsonOpts& opts, org_bson_error_type* perr = 0)
```

**参数**:
- `conn`: 数据库连接对象
- `str_db`: 数据库名称
- `str_coln`: 要创建的集合名称
- `opts`: 创建选项
- `perr`: 错误对象指针

#### s_database_find_collections
查找数据库中的所有集合。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename BsonOpts, typename HandlerCollector>
static typename boost::enable_if<...>::type
s_database_find_collections(Connection_T& conn, const String1& str_db,
                           const BsonOpts& opts, org_bson_error_type* perr,
                           const HandlerCollector& handler_collector)
```

#### s_database_get_collection_names
获取数据库中所有集合的名称列表。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename BsonOpts, typename HandlerCollector>
static typename boost::enable_if<...>::type
s_database_get_collection_names(Connection_T& conn, const String1& str_db,
                               const BsonOpts& opts, org_bson_error_type* perr,
                               const HandlerCollector& handler_collector)
```

#### s_database_has_collection
检查数据库中是否存在指定的集合。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2>
static typename boost::enable_if<...>::type
s_database_has_collection(Connection_T& conn, const String1& str_db, const String2& str_coln,
                         org_bson_error_type* perr = 0)
```

**返回值**: `bool` - 集合是否存在

### 3. 数据库管理操作

#### s_database_drop
删除指定的数据库。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename BsonOpts>
static typename boost::enable_if<...>::type
s_database_drop(Connection_T& conn, const String1& str_db, const BsonOpts& opts,
               org_bson_error_type* perr = 0)
```

**参数**:
- `conn`: 数据库连接对象
- `str_db`: 要删除的数据库名称
- `opts`: 删除选项
- `perr`: 错误对象指针

### 4. 用户管理操作

#### s_database_add_user
向数据库添加用户。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2, typename String3,
         typename BsonRoles, typename BsonCustomData>
static typename boost::enable_if<...>::type
s_database_add_user(Connection_T& conn, const String1& str_db, const String2& str_uname,
                   const String3& str_pwd, const BsonRoles& roles,
                   const BsonCustomData& custom_data, org_bson_error_type* perr = 0)
```

**参数**:
- `conn`: 数据库连接对象
- `str_db`: 数据库名称
- `str_uname`: 用户名
- `str_pwd`: 密码
- `roles`: 用户角色定义
- `custom_data`: 用户自定义数据
- `perr`: 错误对象指针

#### s_database_remove_user
从数据库中移除指定用户。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename String2>
static typename boost::enable_if<...>::type
s_database_remove_user(Connection_T& conn, const String1& str_db, const String2& str_uname,
                      org_bson_error_type* perr = 0)
```

#### s_database_remove_all_users
移除数据库中的所有用户。

**函数签名**:
```cpp
template<typename Connection_T, typename String1>
static typename boost::enable_if<...>::type
s_database_remove_all_users(Connection_T& conn, const String1& str_db,
                           org_bson_error_type* perr = 0)
```

### 5. 数据库监听操作

#### s_database_watch / s_watch
监听数据库的变化。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename BsonPipeline, typename BsonOpts,
         typename HandlerCollector>
static bool s_database_watch(Connection_T& conn, const String1& str_db,
                            const BsonPipeline& pipeline, const BsonOpts& opts,
                            org_bson_type* preply, org_bson_error_type* perr,
                            const HandlerCollector& handler_collector)
```

**参数**:
- `conn`: 数据库连接对象
- `str_db`: 数据库名称
- `pipeline`: 监听管道定义
- `opts`: 监听选项
- `preply`: 回复 BSON 对象指针
- `perr`: 错误对象指针
- `handler_collector`: 结果处理器

**返回值**: `bool` - 操作是否成功

### 6. 通用处理器操作

#### s_database_using_handler / s_using_handler
使用自定义处理器执行数据库操作。

**函数签名**:
```cpp
template<typename Connection_T, typename String1, typename Handler>
static typename boost::enable_if<...>::type
s_database_using_handler(Connection_T& conn, const String1& str_db,
                        org_bson_error_type* perr, const Handler& handler)
```

**参数**:
- `conn`: 数据库连接对象
- `str_db`: 数据库名称
- `perr`: 错误对象指针
- `handler`: 自定义处理器对象

**返回值**: 根据处理器类型确定的返回值类型

## 类型要求

所有模板方法都要求字符串参数满足 `charset::utf8_string_constructable` 特性，这意味着参数可以转换为 UTF-8 编码的字符串。

## 错误处理

所有操作都接受 `org_bson_error_type*` 或 `org_bson_error_type&` 参数用于错误报告。当操作失败时，会在错误对象中设置相应的错误信息。

## 依赖关系

该文件依赖于以下头文件和组件：

1. **Yggdrasil 组件**:
   - `yggr/nsql_database_system/basic_mongo_accesser_database.hpp` - 基类定义
   - `yggr/nsql_database_system/detail/to_pointer.hpp` - 指针转换工具
   - `yggr/charset/charset.hpp` - 字符集处理
   - `yggr/func/foo_t.hpp` - 函数对象工具

2. **Boost 库**:
   - `boost/mpl/and.hpp` - MPL 逻辑与
   - `boost/mpl/or.hpp` - MPL 逻辑或
   - `boost/enable_if.hpp` - SFINAE 启用条件
   - `boost/is_base_of.hpp` - 类型检查
   - `boost/is_same.hpp` - 类型相等检查
   - `boost/addressof.hpp` - 地址获取

3. **MongoDB C 驱动**:
   - `mongoc.h` - MongoDB C 驱动头文件

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongo_accesser_database.hpp>

// 假设已有连接对象 conn
using namespace yggr::nsql_database_system;

// 创建集合
mongo_accesser_database::s_database_create_collecion(
    conn, "testdb", "testcol", bson_opts, &error);

// 检查集合是否存在
bool exists = mongo_accesser_database::s_database_has_collection(
    conn, "testdb", "testcol", &error);

// 删除数据库
mongo_accesser_database::s_database_drop(
    conn, "testdb", bson_opts, &error);

// 添加用户
mongo_accesser_database::s_database_add_user(
    conn, "testdb", "username", "password",
    bson_roles, bson_custom_data, &error);

// 监听数据库变化
mongo_accesser_database::s_database_watch(
    conn, "testdb", bson_pipeline, bson_opts,
    &reply, &error, handler);
```

## 设计特点

1. **类型安全**: 使用模板和 SFINAE 确保编译时类型检查
2. **灵活性**: 支持多种字符串类型和 BSON 类型参数
3. **错误处理**: 统一的错误报告机制
4. **性能优化**: 直接调用底层 MongoDB C 驱动，避免额外开销
5. **编码转换**: 自动处理 UTF-8 字符串编码转换
6. **资源管理**: 正确管理 BSON 对象和错误对象的生命周期
7. **用户管理**: 完整的数据库用户管理功能
8. **监听支持**: 支持数据库级别的变更监听