# native_ex_decl_bson.hpp

## 概述

此头文件包含了 MongoDB BSON (Binary JSON) 相关类的向前声明。这些声明允许在不包含完整类定义的情况下引用这些类，主要用于解决循环依赖和减少编译时间。

## 命名空间

- `yggr::nsql_database_system`

## 类声明列表

### BSON 基础类型

##### `bson_string_native_ex`

BSON 字符串类型的原生扩展类。

##### `bson_oid_native_ex`

BSON ObjectId 类型的原生扩展类。

##### `bson_timeval_native_ex`

BSON 时间值类型的原生扩展类。

##### `bson_date_time_native_ex`

BSON 日期时间类型的原生扩展类。

##### `bson_decimal128_native_ex`

BSON Decimal128 高精度数值类型的原生扩展类。

### BSON 错误和正则表达式

##### `bson_error_native_ex`

BSON 错误处理的原生扩展类。

##### `bson_regex_native_ex`

BSON 正则表达式类型的原生扩展类。

### BSON 迭代和值

##### `bson_iterator_native_ex`

BSON 文档迭代器的原生扩展类。

##### `bson_native_ex`

BSON 文档的原生扩展类。

##### `bson_value_native_ex`

BSON 值的原生扩展类。

## 声明目的

这些向前声明用于：

- 解决类之间的循环依赖问题
- 减少头文件的包含依赖
- 提高编译速度
- 允许在接口中使用这些类型而不需要完整定义

## 依赖项

此头文件不依赖其他头文件，它是独立的声明文件。

## 使用示例

```cpp
#include <yggr/nsql_database_system/native_ex_decl_bson.hpp>

// 现在可以声明指向这些类的指针或引用，而不需要包含完整的类定义
namespace yggr {
namespace nsql_database_system {

class MyClass {
public:
    // 声明成员变量
    bson_native_ex* bson_doc_;
    bson_value_native_ex& bson_value_;
    
    // 声明函数参数
    void process_bson(const bson_string_native_ex& str);
    bson_oid_native_ex* create_oid(void);
};

} // namespace nsql_database_system
} // namespace yggr

// 在实现文件中包含实际的类定义头文件
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_string_native_ex.hpp>
#include <yggr/nsql_database_system/bson_oid_native_ex.hpp>
```

## 注意事项

- 这是一个纯声明头文件，不包含任何实现
- 使用这些声明的代码必须在其他地方包含相应的完整类定义
- 主要用于接口声明和指针/引用类型的使用
- 遵循 C++ 的向前声明最佳实践
- 减少了编译时的依赖关系
- 这些类都位于 `yggr::nsql_database_system` 命名空间中
- 类名以 `_native_ex` 结尾，表示是对 MongoDB C 驱动原生类型的扩展
- 声明顺序与实际头文件的包含顺序无关</content>
<parameter name="filePath">e:\devel\yggr_workspace\yggdrasil\yggr\nsql_database_system\native_ex_decl_bson_cn.md