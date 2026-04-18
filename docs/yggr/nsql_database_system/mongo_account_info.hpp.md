# mongo_account_info.hpp

## 概述

`mongo_account_info.hpp` 是 Yggdrasil 数据库系统中 MongoDB 账户信息管理器的头文件。该文件定义了 `mongo_account_info` 模板类，用于封装 MongoDB 连接信息、用户名和密码，提供统一的账户管理接口。

该类是 MongoDB 访问器的辅助组件，主要用于存储和管理数据库连接的认证信息，支持多种构造方式和资源管理操作。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    template<typename Conn, typename String = ::yggr::string>
    class mongo_account_info;
}
}
```

## 模板参数

- **Conn**: 连接类型，通常是 MongoDB 客户端连接类型
- **String**: 字符串类型，默认为 `::yggr::string`，用于用户名和密码

## 类定义

### mongo_account_info

```cpp
template<typename Conn, typename String = ::yggr::string>
class mongo_account_info
{
public:
    typedef Conn conn_type;
    typedef String string_type;
    typedef typename conn_type::init_type conn_init_type;
    typedef ::yggr::shared_ptr<conn_type> conn_ptr_type;

    // ...
private:
    conn_ptr_type _pconn;
    string_type _uname;
    string_type _pwd;
};
```

## 构造函数

### 默认构造函数
```cpp
mongo_account_info(void);
```
创建一个空的账户信息对象。

### 带初始化参数的构造函数
```cpp
explicit mongo_account_info(const conn_init_type& init);
```
使用连接初始化参数创建账户信息。

**参数：**
- `init`: 连接初始化对象

### 带认证信息的构造函数
```cpp
mongo_account_info(const conn_init_type& init, const string_type& uname, const string_type& pwd);
```
使用连接初始化参数和认证信息创建账户信息。

**参数：**
- `init`: 连接初始化对象
- `uname`: 用户名
- `pwd`: 密码

### 带连接指针的构造函数
```cpp
explicit mongo_account_info(const conn_ptr_type& ptr);
```
使用现有的连接指针创建账户信息。

**参数：**
- `ptr`: 连接对象的共享指针

### 带连接指针和认证信息的构造函数
```cpp
mongo_account_info(const conn_ptr_type& ptr, const string_type& uname, const string_type& pwd);
```
使用连接指针和认证信息创建账户信息。

**参数：**
- `ptr`: 连接对象的共享指针
- `uname`: 用户名
- `pwd`: 密码

## 拷贝和移动操作

### 拷贝构造函数
```cpp
mongo_account_info(const this_type& right);
```
从另一个账户信息对象拷贝构造。

### 移动构造函数
```cpp
mongo_account_info(BOOST_RV_REF(this_type) right);
```
从另一个账户信息对象移动构造。

### 拷贝赋值操作符
```cpp
this_type& operator=(const this_type& right);
```
从另一个账户信息对象拷贝赋值。

### 移动赋值操作符
```cpp
this_type& operator=(BOOST_RV_REF(this_type) right);
```
从另一个账户信息对象移动赋值。

## 交换操作

### swap 方法
```cpp
void swap(this_type& right);
void swap(BOOST_RV_REF(this_type) right);
```
与另一个账户信息对象交换内容。

## 类型转换操作符

### 转换为连接指针
```cpp
operator const conn_ptr_type&(void) const;
operator conn_ptr_type&(void);
```
转换为连接对象的共享指针（常量和非常量版本）。

### 转换为连接对象指针
```cpp
operator conn_type*(void) const;
```
转换为连接对象的原始指针。

## 辅助函数

### swap_support 命名空间
```cpp
namespace swap_support
{
    template<typename Conn, typename String> inline
    void swap(mongo_account_info<Conn, String>& l, mongo_account_info<Conn, String>& r);
}
```

提供全局 swap 函数支持。

### std 命名空间扩展
```cpp
namespace std
{
    using ::yggr::nsql_database_system::swap_support::swap;
}
```

将 swap 函数引入标准命名空间，支持标准库算法。

## 依赖项

- `yggr/charset/string.hpp` - 字符串类型
- `yggr/smart_ptr_ex/shared_ptr.hpp` - 共享指针
- `yggr/move/move.hpp` - 移动语义支持
- `yggr/utility/copy_or_move_or_swap.hpp` - 拷贝/移动/交换工具

## 设计理念

### 资源管理
- **智能指针**: 使用 `shared_ptr` 管理连接对象的生命周期
- **RAII**: 构造函数获取资源，析构函数释放资源
- **共享语义**: 多个账户信息对象可以共享同一个连接

### 认证信息管理
- **可选认证**: 支持无认证和有认证两种模式
- **字符串类型**: 模板化的字符串类型，支持不同的字符串实现
- **安全存储**: 用户名和密码作为私有成员存储

### 操作语义
- **值语义**: 支持完整的拷贝和赋值操作
- **移动优化**: 提供移动构造和移动赋值以提高性能
- **交换支持**: 提供高效的交换操作

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongo_account_info.hpp>

// 使用默认字符串类型
typedef mongo_account_info<mongo_client_type> account_info_type;

// 创建无认证的账户信息
account_info_type account1(init_params);

// 创建带认证的账户信息
account_info_type account2(init_params, "username", "password");

// 使用现有连接
shared_ptr<mongo_client_type> conn_ptr = ...;
account_info_type account3(conn_ptr, "username", "password");

// 拷贝和赋值
account_info_type account4 = account2;
account1 = account3;

// 转换为连接指针使用
mongo_client_type* conn = account1; // 隐式转换
const shared_ptr<mongo_client_type>& ptr = account1; // 隐式转换

// 交换操作
account1.swap(account2);
std::swap(account1, account2); // 使用全局 swap
```

## 注意事项

1. **模板参数**: 确保 `Conn` 类型具有 `init_type` 类型定义
2. **连接管理**: 账户信息对象共享连接，注意生命周期管理
3. **认证安全**: 密码以明文形式存储，实际使用中应考虑加密
4. **线程安全**: 该类本身不提供线程安全，需要外部同步
5. **异常安全**: 构造函数可能抛出异常，确保正确处理

## 许可证

该文件遵循项目的许可证协议，基于 MIT 许可证。