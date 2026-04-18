# c_mongo_iovec.hpp 文档

## 文件概述

`c_mongo_iovec.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB I/O 向量的 C++ 类。该类提供了对 MongoDB I/O 向量结构的包装，支持缓冲区数据的长度和指针管理。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 结构体定义

### mongoc_iovec_t

该结构体是 MongoDB C 驱动的 I/O 向量结构体，包含以下成员：

- `u_long iov_len`：缓冲区长度。
- `char *iov_base`：缓冲区指针。

## 类定义

### c_mongo_iovec

该类继承自 `mongoc_iovec_t`，提供了对 I/O 向量的封装和管理。它支持复制和移动操作。

#### 类型定义

- `base_type`：基类 `mongoc_iovec_t`。
- `org_type`：原始类型 `mongoc_iovec_t`。
- `iov_len_type`：I/O 向量长度类型。

#### 构造函数

- `c_mongo_iovec(void)`：默认构造函数。
- `c_mongo_iovec(u32 byte_size, char* buf_ptr)`：使用长度和缓冲区指针构造。
- `c_mongo_iovec(Buffer& buf)`：从缓冲区对象构造（模板）。
- `c_mongo_iovec(const base_type& right)`：从基类复制构造。
- `c_mongo_iovec(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `c_mongo_iovec(const this_type& right)`：复制构造函数。

#### 析构函数

- `~c_mongo_iovec(void)`：析构函数。

#### 赋值运算符

- `operator=(Buffer& buf)`：从缓冲区对象赋值（模板）。
- `operator=(const base_type& right)`：从基类赋值。
- `operator=(BOOST_RV_REF(this_type) right)`：移动赋值。
- `operator=(const this_type& right)`：复制赋值。

#### 交换函数

- `swap(BOOST_RV_REF(base_type) right)`：与基类移动交换。
- `swap(base_type& right)`：与基类交换。
- `swap(BOOST_RV_REF(this_type) right)`：移动交换。
- `swap(this_type& right)`：交换函数。

#### 类型转换函数

- `org_pointer(void)`：返回原始类型指针。
- `org_pointer(void) const`：返回常量原始类型指针。

#### 缓冲区访问函数

- `buffer<T>(void)`：返回指定类型的缓冲区指针（模板）。
- `buffer<T>(void) const`：返回指定类型的常量缓冲区指针（模板）。

#### 成员访问函数

- `size(void)`：返回长度的引用。
- `size(void) const`：获取长度。

## 交换支持

在 `swap_support` 命名空间中定义了多个 `swap` 函数，用于支持与不同类型对象的交换操作。

## 依赖

- `yggr/base/yggrdef.h`：基础定义。
- `yggr/charset/string.hpp`：字符串。
- `yggr/move/move.hpp`：移动语义。
- `yggr/typeof/typeof.hpp`：类型推导。
- `yggr/mplex/static_assert.hpp`：静态断言。
- `yggr/bytes/byte_size.hpp`：字节大小。
- `yggr/iterator_ex/iterator_category_check.hpp`：迭代器类别检查。
- `<boost/range/value_type.hpp>`：Boost 范围值类型。
- `<boost/range/functions.hpp>`：Boost 范围函数。
- `<boost/range/iterator.hpp>`：Boost 范围迭代器。
- `<boost/type_traits/is_class.hpp>`：Boost 类型特征。
- `<mongoc-iovec.h>`：MongoDB I/O 向量头文件（条件包含）。

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。