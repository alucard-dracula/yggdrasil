# `shared_ptr.hpp`

## 概述

该头文件为共享指针提供 yggdrasil 的序列化入口，实际序列化能力直接来自 Boost.Serialization：

```cpp
#include <boost/serialization/shared_ptr.hpp>
```

文件本身不定义新的共享指针类型、序列化函数或宏。

## 头文件

```cpp
#include <yggr/serialization/shared_ptr.hpp>
```

## 依赖

```cpp
#include <yggr/base/yggrdef.h>
#include <boost/serialization/shared_ptr.hpp>
```

## 提供的能力

包含该文件后，可以使用 Boost.Serialization 对共享指针进行序列化，包括：

- 保存指针是否为空；
- 保存指针指向对象；
- 加载对象并恢复指针关系；
- 在归档支持的情况下处理对象跟踪和共享引用关系。

具体行为由 Boost.Serialization 的 `shared_ptr` 实现决定。

## 使用示例

```cpp
#include <yggr/serialization/shared_ptr.hpp>
#include <boost/shared_ptr.hpp>

struct record
{
    int value;

    template<typename Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & value;
        (void)version;
    }
};

void serialize_shared_pointer(boost::shared_ptr<record>& value)
{
    // 将 value 交给兼容的 Boost.Serialization 归档。
}
```

## 兼容的指针类型

该头文件直接引入 Boost 的共享指针序列化支持，因此实际支持范围取决于：

- Boost.Serialization 版本；
- 使用的共享指针类型；
- 目标归档类型；
- 被指向类型是否满足序列化要求。

yggdrasil 中的智能指针扩展类型如需专用适配，应使用其对应的序列化头文件。

## BSON 支持状态

文件中保留了以下注释：

```cpp
//#include <yggr/serialization/shared_ptr_bson_impl.hpp>
```

当前该包含语句未启用，因此本文件不会自动引入共享指针的 BSON 专用实现。使用 BSON 归档时，应确认目标工程是否通过其他入口提供所需适配。

## 注意事项

1. 该文件是 Boost.Serialization 共享指针支持的项目入口，不重新实现指针序列化。
2. 被指向对象必须提供有效的序列化接口或对应的非成员序列化函数。
3. 空指针、对象跟踪和共享引用恢复的具体语义由 Boost.Serialization 决定。
4. 多态对象序列化通常还需要配合类型导出宏，例如 `YGGR_CLASS_EXPORT` 系列宏。
5. 当前共享指针 BSON 实现包含语句处于注释状态，不应假定包含本文件就自动获得 BSON 适配。
6. 序列化和反序列化两端应使用兼容的指针类型、对象导出信息和归档配置。
7. 指针序列化不会自动解决原始指针所有权问题，资源生命周期仍由智能指针和对象设计负责。

## 相关组件

- `yggr/base/yggrdef.h`
- `yggr/serialization/export.hpp`
- `boost/serialization/shared_ptr.hpp`
- `boost/shared_ptr.hpp`
