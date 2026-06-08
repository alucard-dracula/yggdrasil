# mongoc_index_opt_geo_native_ex.hpp

## 概述

此头文件定义了 `mongoc_index_opt_geo_native_ex` 类，该类提供静态方法来处理 MongoDB 地理空间索引选项。这些方法是对 MongoDB C 驱动程序 `mongoc_index_opt_geo_t` 结构体的扩展管理接口。

## 命名空间

- `yggr::nsql_database_system`

## 类描述

### mongoc_index_opt_geo_native_ex

一个工具类，提供地理空间索引选项的管理方法。

#### 静态方法

##### `s_mongoc_index_opt_geo_get_default`

```cpp
inline static const mongoc_index_opt_geo_t* s_mongoc_index_opt_geo_get_default(void);
```

获取默认的地理空间索引选项。

**参数：**
- 无

**返回值：**
- 指向默认地理索引选项的常量指针

##### `s_mongoc_index_opt_geo_init`

```cpp
inline static mongoc_index_opt_geo_t* s_mongoc_index_opt_geo_init(mongoc_index_opt_geo_t* popt);
```

初始化地理空间索引选项结构体。

**参数：**
- `popt`：指向要初始化的地理索引选项结构体的指针

**返回值：**
- 初始化后的结构体指针，如果输入无效则返回 0

##### `s_mongoc_index_opt_geo_copy`

```cpp
inline static mongoc_index_opt_geo_t* 
s_mongoc_index_opt_geo_copy(mongoc_index_opt_geo_t* pdst, const mongoc_index_opt_geo_t* psrc);
```

复制地理空间索引选项。

**参数：**
- `pdst`：指向目标结构体的指针
- `psrc`：指向源结构体的常量指针

**返回值：**
- 复制后的目标结构体指针，如果输入无效则返回 0

##### `s_mongoc_index_opt_geo_swap`

```cpp
inline static void s_mongoc_index_opt_geo_swap(mongoc_index_opt_geo_t* pl, mongoc_index_opt_geo_t* pr);
```

交换两个地理空间索引选项结构体。

**参数：**
- `pl`：指向第一个结构体的指针
- `pr`：指向第二个结构体的指针

**返回值：**
- 无

## 依赖项

- `<yggr/nsql_database_system/mongodb_config.hpp>`
- `<cassert>`

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongoc_index_opt_geo_native_ex.hpp>

using namespace yggr::nsql_database_system;

// 获取默认选项
const mongoc_index_opt_geo_t* default_opts = mongoc_index_opt_geo_native_ex::s_mongoc_index_opt_geo_get_default();

// 初始化新选项
mongoc_index_opt_geo_t opts;
mongoc_index_opt_geo_native_ex::s_mongoc_index_opt_geo_init(&opts);

// 复制选项
mongoc_index_opt_geo_t copied_opts;
mongoc_index_opt_geo_native_ex::s_mongoc_index_opt_geo_copy(&copied_opts, &opts);

// 交换选项
mongoc_index_opt_geo_native_ex::s_mongoc_index_opt_geo_swap(&opts, &copied_opts);
```

## 注意事项

- 所有方法都是静态的，不需要创建类的实例
- 这些方法提供对地理空间索引选项的初始化、复制和交换操作
- 使用这些方法时需要确保传入的指针有效
- 该类是对 MongoDB C 驱动程序地理索引选项管理的包装
- 文件中注释掉了原始的 `mongoc_index_opt_geo_t` 结构体定义，现在通过这些方法进行操作
- 地理空间索引选项包括 2dsphere、2d 等类型的索引参数
- 方法使用 `memcpy` 进行结构体复制，确保高效的内存操作</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\mongoc_index_opt_geo_native_ex_cn.md