# `time.hpp`

## 概述

该头文件为 `::yggr::time::time` 提供 Boost.Serialization 支持。时间对象由秒和纳秒两个字段组成，并通过命名值对进行保存与加载：

- `sec`：秒部分；
- `nsec`：纳秒部分。

## 头文件

```cpp
#include <yggr/serialization/time.hpp>
```

主要依赖包括：

```cpp
#include <yggr/time/time.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/is_bitwise_serializable.hpp>
#include <boost/mpl/and.hpp>
```

## 序列化接口

相关函数位于 `boost::serialization` 命名空间中。

### 保存

```cpp
template<typename Archive>
void save(
    Archive& ar,
    const ::yggr::time::time& value,
    const unsigned int file_version);
```

保存时，`sec` 和 `nsec` 会先转换为 `yggr::s64`，再分别使用以下名称写入归档：

```cpp
ar << YGGR_SERIALIZATION_NAME_NVP("sec", tsec);
ar << YGGR_SERIALIZATION_NAME_NVP("nsec", tnsec);
```

转换为 64 位有符号整数可以统一不同时间字段底层类型在归档中的表示。

### 加载

```cpp
template<typename Archive>
void load(
    Archive& ar,
    ::yggr::time::time& value,
    const unsigned int file_version);
```

加载时先将 `sec` 和 `nsec` 读入 `yggr::s64` 临时变量，然后转换为目标时间类型对应的字段类型：

```cpp
t.sec = static_cast<now_tm_type::xtime_sec_t>(tsec);
t.nsec = static_cast<now_tm_type::xtime_nsec_t>(tnsec);
```

临时变量初始化为零，因此归档读取过程从明确的初始值开始。

### 统一序列化入口

```cpp
template<typename Archive>
void serialize(
    Archive& ar,
    ::yggr::time::time& value,
    const unsigned int file_version);
```

`serialize` 使用 `boost::serialization::split_free` 将操作分派到对应的 `save` 或 `load` 函数：

```cpp
boost::serialization::split_free<
    Archive, ::yggr::time::time>(ar, value, file_version);
```

## 实现级别

文件为 `::yggr::time::time` 特化 `boost::serialization::implementation_level`，并将其设置为 `object_serializable`：

```cpp
template<>
struct implementation_level< ::yggr::time::time >
{
    typedef mpl::integral_c_tag tag;
    typedef mpl::int_<object_serializable> type;
    YGGR_STATIC_CONSTANT(int, value = object_serializable);
};
```

这表示该类型按对象级别进行序列化，而不是作为按位可复制的基本类型直接处理。

## 文件版本

`save` 和 `load` 接口接收 `file_version` 参数，但当前实现不使用该参数。序列化格式固定由 `sec` 和 `nsec` 两个命名字段构成。

## BSON 集成

文件主体结束后包含：

```cpp
#include <yggr/serialization/time_bson_impl.hpp>
```

该头文件负责根据 `YGGR_HAS_BSON_OARCHIVE` 和 `YGGR_HAS_BSON_IARCHIVE` 的状态协调时间 BSON 序列化实现的条件编译。具体 BSON 控制逻辑见 `time_bson_impl.hpp`。

## 注意事项

- 归档数据必须使用 `sec` 和 `nsec` 这两个字段名称，才能与当前加载逻辑匹配。
- `sec` 和 `nsec` 的归档中间表示均为 `yggr::s64`。
- 最终赋值会转换回 `::yggr::time::time` 定义的 `xtime_sec_t` 和 `xtime_nsec_t` 类型，应确保归档值位于目标类型可表示的范围内。
- 该文件只提供 `::yggr::time::time` 的序列化适配，不负责定义时间类型本身。
