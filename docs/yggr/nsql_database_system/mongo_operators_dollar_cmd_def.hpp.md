# mongo_operators_dollar_cmd_def.hpp

## 概述

`mongo_operators_dollar_cmd_def.hpp` 是 Yggdrasil 数据库系统中 MongoDB 操作符常量定义的头文件。该文件定义了 `mongo_operators_dollar_cmd_def` 结构体，集中声明了 MongoDB 查询、更新、聚合等操作中使用的所有 `$` 开头的操作符常量。

这些操作符是 MongoDB 查询语言的核心组成部分，包括比较操作符、逻辑操作符、数组操作符、聚合操作符等。通过集中定义这些常量，可以确保代码的一致性和避免硬编码字符串的问题。

## 命名空间

```cpp
namespace yggr
{
namespace nsql_database_system
{
    struct mongo_operators_dollar_cmd_def;
}
}
```

## 结构体定义

### mongo_operators_dollar_cmd_def

```cpp
struct mongo_operators_dollar_cmd_def
{
    // 各种操作符定义...
};
```

该结构体使用预处理器宏定义了大量的 MongoDB 操作符常量。

## 操作符分类

### 1. 查询和投影操作符 (Query and Projection Operators)

#### 比较操作符 (Comparison)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_eq, "$eq")      // 等于
YGGR_TMP_PP_MAKE_FUNCTION(dollar_gt, "$gt")      // 大于
YGGR_TMP_PP_MAKE_FUNCTION(dollar_gte, "$gte")    // 大于等于
YGGR_TMP_PP_MAKE_FUNCTION(dollar_in, "$in")      // 在数组中
YGGR_TMP_PP_MAKE_FUNCTION(dollar_lt, "$lt")      // 小于
YGGR_TMP_PP_MAKE_FUNCTION(dollar_lte, "$lte")    // 小于等于
YGGR_TMP_PP_MAKE_FUNCTION(dollar_ne, "$ne")      // 不等于
YGGR_TMP_PP_MAKE_FUNCTION(dollar_nin, "$nin")    // 不在数组中
```

#### 逻辑操作符 (Logical)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_and, "$and")    // 逻辑与
YGGR_TMP_PP_MAKE_FUNCTION(dollar_not, "$not")    // 逻辑非
YGGR_TMP_PP_MAKE_FUNCTION(dollar_nor, "$nor")    // 逻辑或非
YGGR_TMP_PP_MAKE_FUNCTION(dollar_or, "$or")      // 逻辑或
```

#### 元素操作符 (Element)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_exists, "$exists")  // 字段存在
YGGR_TMP_PP_MAKE_FUNCTION(dollar_type, "$type")      // 类型检查
```

#### 求值操作符 (Evaluation)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_expr, "$expr")              // 表达式
YGGR_TMP_PP_MAKE_FUNCTION(dollar_json_schema, "$jsonSchema") // JSON模式
YGGR_TMP_PP_MAKE_FUNCTION(dollar_mod, "$mod")                // 取模
YGGR_TMP_PP_MAKE_FUNCTION(dollar_regex, "$regex")            // 正则表达式
YGGR_TMP_PP_MAKE_FUNCTION(dollar_text, "$text")              // 文本搜索
YGGR_TMP_PP_MAKE_FUNCTION(dollar_where, "$where")            // JavaScript条件
```

#### 地理空间操作符 (Geospatial)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_geo_intersects, "$geoIntersects") // 地理相交
YGGR_TMP_PP_MAKE_FUNCTION(dollar_geo_within, "$geoWithin")         // 地理范围内
YGGR_TMP_PP_MAKE_FUNCTION(dollar_near, "$near")                    // 附近
YGGR_TMP_PP_MAKE_FUNCTION(dollar_near_sphere, "$nearSphere")       // 球面附近
```

#### 数组操作符 (Array)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_all, "$all")            // 匹配所有元素
YGGR_TMP_PP_MAKE_FUNCTION(dollar_elem_match, "$elemMatch") // 元素匹配
YGGR_TMP_PP_MAKE_FUNCTION(dollar_size, "$size")          // 数组大小
```

#### 位操作符 (Bitwise)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_bits_all_clear, "$bitsAllClear") // 位全清
YGGR_TMP_PP_MAKE_FUNCTION(dollar_bits_all_set, "$bitsAllSet")     // 位全置
YGGR_TMP_PP_MAKE_FUNCTION(dollar_bits_any_clear, "$bitsAnyClear") // 位任清
YGGR_TMP_PP_MAKE_FUNCTION(dollar_bits_any_set, "$bitsAnySet")     // 位任置
```

#### 注释操作符 (Comments)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_comment, "$comment") // 查询注释
```

### 2. 投影操作符 (Projection Operators)

```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar, "$")              // 位置操作符
YGGR_TMP_PP_MAKE_FUNCTION(dollar_meta, "$meta")     // 元数据
YGGR_TMP_PP_MAKE_FUNCTION(dollar_slice, "$slice")   // 数组切片
```

### 3. 更新操作符 (Update Operators)

#### 字段操作符 (Field)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_current_date, "$currentDate") // 当前日期
YGGR_TMP_PP_MAKE_FUNCTION(dollar_inc, "$inc")                  // 递增
YGGR_TMP_PP_MAKE_FUNCTION(dollar_min, "$min")                  // 最小值
YGGR_TMP_PP_MAKE_FUNCTION(dollar_max, "$max")                  // 最大值
YGGR_TMP_PP_MAKE_FUNCTION(dollar_mul, "$mul")                  // 乘法
YGGR_TMP_PP_MAKE_FUNCTION(dollar_rename, "$rename")            // 重命名字段
YGGR_TMP_PP_MAKE_FUNCTION(dollar_set, "$set")                  // 设置字段值
YGGR_TMP_PP_MAKE_FUNCTION(dollar_set_on_insert, "$setOnInsert") // 插入时设置
YGGR_TMP_PP_MAKE_FUNCTION(dollar_unset, "$unset")              // 删除字段
```

#### 数组操作符 (Array)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_array_identifier, "$[]")      // 数组标识符
YGGR_TMP_PP_MAKE_FUNCTION(dollar_add_to_set, "$addToSet")      // 添加到集合
YGGR_TMP_PP_MAKE_FUNCTION(dollar_pop, "$pop")                  // 弹出元素
YGGR_TMP_PP_MAKE_FUNCTION(dollar_pull, "$pull")                // 拉取元素
YGGR_TMP_PP_MAKE_FUNCTION(dollar_push, "$push")                // 推送元素
YGGR_TMP_PP_MAKE_FUNCTION(dollar_pull_all, "$pullAll")         // 拉取所有
```

#### 修改器 (Modifiers)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_each, "$each")        // 每个元素
YGGR_TMP_PP_MAKE_FUNCTION(dollar_position, "$position") // 位置
YGGR_TMP_PP_MAKE_FUNCTION(dollar_sort, "$sort")        // 排序
YGGR_TMP_PP_MAKE_FUNCTION(dollar_bit, "$bit")          // 位操作
```

### 4. 聚合管道阶段 (Aggregation Pipeline Stages)

#### 集合聚合阶段 (Collection Aggregation Stages)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_add_fields, "$addFields")    // 添加字段
YGGR_TMP_PP_MAKE_FUNCTION(dollar_bucket, "$bucket")           // 分桶
YGGR_TMP_PP_MAKE_FUNCTION(dollar_bucket_auto, "$bucketAuto")  // 自动分桶
YGGR_TMP_PP_MAKE_FUNCTION(dollar_coll_stats, "$collStats")    // 集合统计
YGGR_TMP_PP_MAKE_FUNCTION(dollar_count, "$count")             // 计数
YGGR_TMP_PP_MAKE_FUNCTION(dollar_facet, "$facet")             // 分面
YGGR_TMP_PP_MAKE_FUNCTION(dollar_geo_near, "$geoNear")        // 地理邻近
YGGR_TMP_PP_MAKE_FUNCTION(dollar_graph_lookup, "$graphLookup") // 图查找
YGGR_TMP_PP_MAKE_FUNCTION(dollar_group, "$group")             // 分组
YGGR_TMP_PP_MAKE_FUNCTION(dollar_index_stats, "$indexStats")  // 索引统计
YGGR_TMP_PP_MAKE_FUNCTION(dollar_limit, "$limit")             // 限制
YGGR_TMP_PP_MAKE_FUNCTION(dollar_list_sessions, "$listSessions") // 会话列表
YGGR_TMP_PP_MAKE_FUNCTION(dollar_lookup, "$lookup")           // 查找
YGGR_TMP_PP_MAKE_FUNCTION(dollar_match, "$match")             // 匹配
YGGR_TMP_PP_MAKE_FUNCTION(dollar_out, "$out")                 // 输出
YGGR_TMP_PP_MAKE_FUNCTION(dollar_project, "$project")         // 投影
YGGR_TMP_PP_MAKE_FUNCTION(dollar_redact, "$redact")           // 编辑
YGGR_TMP_PP_MAKE_FUNCTION(dollar_replace_root, "$replaceRoot") // 替换根
YGGR_TMP_PP_MAKE_FUNCTION(dollar_sample, "$sample")           // 采样
YGGR_TMP_PP_MAKE_FUNCTION(dollar_skip, "$skip")               // 跳过
YGGR_TMP_PP_MAKE_FUNCTION(dollar_sort_by_count, "$sortByCount") // 按计数排序
YGGR_TMP_PP_MAKE_FUNCTION(dollar_unwind, "$unwind")           // 展开
```

### 5. 聚合管道操作符 (Aggregation Pipeline Operators)

#### 算术表达式操作符 (Arithmetic Expression)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_abs, "$abs")           // 绝对值
YGGR_TMP_PP_MAKE_FUNCTION(dollar_add, "$add")           // 加法
YGGR_TMP_PP_MAKE_FUNCTION(dollar_ceil, "$ceil")         // 向上取整
YGGR_TMP_PP_MAKE_FUNCTION(dollar_divide, "$divide")     // 除法
YGGR_TMP_PP_MAKE_FUNCTION(dollar_exp, "$exp")           // 指数
YGGR_TMP_PP_MAKE_FUNCTION(dollar_floor, "$floor")       // 向下取整
YGGR_TMP_PP_MAKE_FUNCTION(dollar_ln, "$ln")             // 自然对数
YGGR_TMP_PP_MAKE_FUNCTION(dollar_log, "$log")           // 对数
YGGR_TMP_PP_MAKE_FUNCTION(dollar_log10, "$log10")       // 常用对数
YGGR_TMP_PP_MAKE_FUNCTION(dollar_multiply, "$multiply") // 乘法
YGGR_TMP_PP_MAKE_FUNCTION(dollar_pow, "$pow")           // 幂
YGGR_TMP_PP_MAKE_FUNCTION(dollar_sqrt, "$sqrt")         // 平方根
YGGR_TMP_PP_MAKE_FUNCTION(dollar_subtract, "$subtract") // 减法
YGGR_TMP_PP_MAKE_FUNCTION(dollar_trunc, "$trunc")       // 截断
```

#### 数组表达式操作符 (Array Expression)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_array_elem_at, "$arrayElemAt")     // 数组元素
YGGR_TMP_PP_MAKE_FUNCTION(dollar_array_to_object, "$arrayToObject") // 数组转对象
YGGR_TMP_PP_MAKE_FUNCTION(dollar_concat_arrays, "$concatArrays")   // 连接数组
YGGR_TMP_PP_MAKE_FUNCTION(dollar_filter, "$filter")                 // 过滤
YGGR_TMP_PP_MAKE_FUNCTION(dollar_index_of_array, "$indexOfArray")   // 数组索引
YGGR_TMP_PP_MAKE_FUNCTION(dollar_is_array, "$isArray")             // 是否为数组
YGGR_TMP_PP_MAKE_FUNCTION(dollar_map, "$map")                       // 映射
YGGR_TMP_PP_MAKE_FUNCTION(dollar_object_to_array, "$objectToArray") // 对象转数组
YGGR_TMP_PP_MAKE_FUNCTION(dollar_range, "$range")                   // 范围
YGGR_TMP_PP_MAKE_FUNCTION(dollar_reduce, "$reduce")                 // 归约
YGGR_TMP_PP_MAKE_FUNCTION(dollar_reverse_array, "$reverseArray")   // 反转数组
YGGR_TMP_PP_MAKE_FUNCTION(dollar_zip, "$zip")                       // 拉链
```

#### 条件表达式操作符 (Conditional Expression)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_cond, "$cond")       // 条件
YGGR_TMP_PP_MAKE_FUNCTION(dollar_if_null, "$ifNull")  // 如果为空
YGGR_TMP_PP_MAKE_FUNCTION(dollar_switch, "$switch")   // 开关
```

#### 日期表达式操作符 (Date Expression)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_date_from_parts, "$dateFromParts")   // 从部分创建日期
YGGR_TMP_PP_MAKE_FUNCTION(dollar_date_from_string, "$dateFromString") // 从字符串创建日期
YGGR_TMP_PP_MAKE_FUNCTION(dollar_date_to_parts, "$dateToParts")       // 日期转部分
YGGR_TMP_PP_MAKE_FUNCTION(dollar_date_to_string, "$dateToString")     // 日期转字符串
YGGR_TMP_PP_MAKE_FUNCTION(dollar_day_of_month, "$dayOfMonth")         // 月中的日
YGGR_TMP_PP_MAKE_FUNCTION(dollar_day_of_week, "$dayOfWeek")           // 周中的日
YGGR_TMP_PP_MAKE_FUNCTION(dollar_day_of_year, "$dayOfYear")           // 年中的日
YGGR_TMP_PP_MAKE_FUNCTION(dollar_hour, "$hour")                       // 小时
YGGR_TMP_PP_MAKE_FUNCTION(dollar_minute, "$minute")                   // 分钟
YGGR_TMP_PP_MAKE_FUNCTION(dollar_month, "$month")                     // 月
YGGR_TMP_PP_MAKE_FUNCTION(dollar_second, "$second")                   // 秒
YGGR_TMP_PP_MAKE_FUNCTION(dollar_week, "$week")                       // 周
YGGR_TMP_PP_MAKE_FUNCTION(dollar_year, "$year")                       // 年
```

#### 字符串表达式操作符 (String Expression)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_concat, "$concat")             // 连接字符串
YGGR_TMP_PP_MAKE_FUNCTION(dollar_index_of_bytes, "$indexOfBytes") // 字节索引
YGGR_TMP_PP_MAKE_FUNCTION(dollar_index_of_cp, "$indexOfCP")     // 码点索引
YGGR_TMP_PP_MAKE_FUNCTION(dollar_split, "$split")               // 分割字符串
YGGR_TMP_PP_MAKE_FUNCTION(dollar_str_len_bytes, "$strLenBytes") // 字节长度
YGGR_TMP_PP_MAKE_FUNCTION(dollar_str_len_cp, "$strLenCP")       // 码点长度
YGGR_TMP_PP_MAKE_FUNCTION(dollar_substr, "$substr")             // 子字符串
YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_lower, "$toLower")          // 转小写
YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_upper, "$toUpper")          // 转大写
```

#### 类型转换操作符 (Type Conversion)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_convert, "$convert")       // 类型转换
YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_bool, "$toBool")        // 转布尔
YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_date, "$toDate")        // 转日期
YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_double, "$toDouble")    // 转双精度
YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_int, "$toInt")          // 转整数
YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_long, "$toLong")        // 转长整数
YGGR_TMP_PP_MAKE_FUNCTION(dollar_to_string, "$toString")    // 转字符串
```

#### 累加器操作符 (Accumulator)
```cpp
YGGR_TMP_PP_MAKE_FUNCTION(dollar_avg, "$avg")             // 平均值
YGGR_TMP_PP_MAKE_FUNCTION(dollar_first, "$first")         // 第一个
YGGR_TMP_PP_MAKE_FUNCTION(dollar_last, "$last")           // 最后一个
YGGR_TMP_PP_MAKE_FUNCTION(dollar_std_dev_pop, "$stdDevPop")   // 总体标准差
YGGR_TMP_PP_MAKE_FUNCTION(dollar_std_dev_samp, "$stdDevSamp") // 样本标准差
YGGR_TMP_PP_MAKE_FUNCTION(dollar_sum, "$sum")             // 求和
```

### 6. 特殊功能

#### 数组标识符生成器
```cpp
template<typename T> static
utf8_string dollar_array_identifier(const T& id);

template<typename String, typename T> static
String dollar_array_identifier(const T& id);
```

生成数组标识符字符串，如 `$[0]`, `$[1]` 等。

**参数：**
- `id`: 数组索引或标识符

**返回值：** 格式为 `$[id]` 的字符串

## 类型别名

```cpp
typedef mongo_operators_dollar_cmd_def mongo_operators_dollar_cmd_def_type;
```

提供简化的类型别名。

## 设计理念

### 常量集中管理
- **避免硬编码**: 将所有 MongoDB 操作符常量集中定义
- **一致性保证**: 确保所有地方使用相同的操作符字符串
- **维护便利**: 修改操作符时只需在一处更改

### 分类组织
- **按功能分组**: 将操作符按查询、更新、聚合等功能分类
- **版本标注**: 对特定版本引入的操作符进行版本标注
- **重复标记**: 使用注释标记重复定义以避免冲突

### 类型安全
- **宏生成**: 使用预处理器宏生成类型安全的常量
- **模板函数**: 为动态生成的操作符提供模板函数
- **编译时检查**: 宏展开后生成编译时可检查的代码

## 依赖项

- `yggr/nsql_database_system/detail/const_string_make_function_macro.hpp` - 字符串常量生成宏

## 使用示例

```cpp
#include <yggr/nsql_database_system/mongo_operators_dollar_cmd_def.hpp>

// 使用操作符常量
mongo_operators_dollar_cmd_def_type ops;

// 构建查询条件
bson_t query = BSON_INITIALIZER;
BSON_APPEND_INT32(&query, ops.dollar_gt(), 100); // { $gt: 100 }

// 构建更新操作
bson_t update = BSON_INITIALIZER;
{
    bson_t set_doc;
    BSON_APPEND_DOCUMENT_BEGIN(&update, ops.dollar_set(), &set_doc);
    BSON_APPEND_UTF8(&set_doc, "status", "active");
    bson_append_document_end(&update, &set_doc);
}

// 使用数组标识符
std::string array_id = mongo_operators_dollar_cmd_def_type::dollar_array_identifier(0); // "$[0]"

// 聚合管道
bson_t pipeline = BSON_INITIALIZER;
{
    bson_t match_stage;
    BSON_APPEND_DOCUMENT_BEGIN(&pipeline, ops.dollar_match(), &match_stage);
    BSON_APPEND_INT32(&match_stage, "age", 25);
    bson_append_document_end(&pipeline, &match_stage);
}
```

## 注意事项

1. **宏依赖**: 该文件依赖于 `const_string_make_function_macro.hpp` 中的宏定义
2. **版本兼容性**: 某些操作符需要特定版本的 MongoDB 支持（如标注的版本要求）
3. **重复定义**: 文件中注释标记了一些重复的操作符定义，这是为了避免宏重复定义
4. **动态生成**: `dollar_array_identifier` 函数用于动态生成数组操作符
5. **UTF-8编码**: 所有字符串常量都使用 UTF-8 编码

## 许可证

该文件遵循项目的许可证协议，基于 MIT 许可证。