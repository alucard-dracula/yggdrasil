# c_mongo_index_opt_geo.hpp 文档

## 文件概述

`c_mongo_index_opt_geo.hpp` 是 Yggdrasil 项目中的一个头文件，定义了用于封装 MongoDB 地理索引选项的 C++ 类。该类提供了对 MongoDB 地理索引配置的包装，包括 2D 球体、2D 位置和 Haystack 索引的选项。

## 命名空间

- `yggr::nsql_database_system`：包含数据库系统相关的类和函数。

## 结构体定义

### mongoc_index_opt_geo_t

该结构体是 MongoDB C 驱动的地理索引选项结构体，包含以下成员：

- `uint8_t twod_sphere_version`：2D 球体版本。
- `uint8_t twod_bits_precision`：2D 位精度。
- `double twod_location_min`：2D 位置最小值。
- `double twod_location_max`：2D 位置最大值。
- `double haystack_bucket_size`：Haystack 桶大小。
- `uint8_t *padding[32]`：填充字节。

## 类定义

### c_mongo_index_opt_geo

该类继承自 `mongoc_index_opt_geo_t`，提供了对地理索引选项的封装和管理。它支持复制和移动操作。

#### 类型定义

- `base_type`：基类 `mongoc_index_opt_geo_t`。
- `org_type`：原始类型 `mongoc_index_opt_geo_t`。

#### 构造函数

- `c_mongo_index_opt_geo(void)`：默认构造函数。
- `c_mongo_index_opt_geo(const base_type* ptr)`：从基类指针构造。
- `c_mongo_index_opt_geo(BOOST_RV_REF(base_type) right)`：从基类移动构造。
- `c_mongo_index_opt_geo(const base_type& right)`：从基类复制构造。
- `c_mongo_index_opt_geo(BOOST_RV_REF(this_type) right)`：移动构造函数。
- `c_mongo_index_opt_geo(const this_type& right)`：复制构造函数。

#### 析构函数

- `~c_mongo_index_opt_geo(void)`：析构函数。

#### 赋值运算符

- `operator=(BOOST_RV_REF(base_type) right)`：从基类移动赋值。
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

#### 成员访问函数

- `var_twod_sphere_version(void)`：返回 2D 球体版本的引用。
- `var_twod_sphere_version(void) const`：获取 2D 球体版本。
- `var_twod_bits_precision(void)`：返回 2D 位精度的引用。
- `var_twod_bits_precision(void) const`：获取 2D 位精度。
- `var_twod_location_min(void)`：返回 2D 位置最小值的引用。
- `var_twod_location_min(void) const`：获取 2D 位置最小值。
- `var_twod_location_max(void)`：返回 2D 位置最大值的引用。
- `var_twod_location_max(void) const`：获取 2D 位置最大值。
- `var_haystack_bucket_size(void)`：返回 Haystack 桶大小的引用。
- `var_haystack_bucket_size(void) const`：获取 Haystack 桶大小。

## 交换支持

在 `swap_support` 命名空间中定义了多个 `swap` 函数，用于支持与不同类型对象的交换操作。

## 依赖

- `yggr/move/move.hpp`：移动语义。
- `yggr/ppex/swap_this_def.hpp`：交换定义宏。
- `yggr/nsql_database_system/mongoc_index_opt_geo_native_ex.hpp`：MongoDB 地理索引选项扩展。
- `<boost/ref.hpp>`：Boost 引用。

## 版权信息

Copyright (c) 2010-2024 yggdrasil。遵循 MIT 许可证。