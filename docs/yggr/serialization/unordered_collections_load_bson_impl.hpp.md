# `unordered_collections_load_bson_impl.hpp`

## 概述

该头文件为无序集合提供 BSON 输入归档加载实现。它针对 BSON 输入归档特化 `unordered_collections_loader_switch`，负责读取无序集合的元数据、预留桶容量，并按 BSON 数字索引逐个加载元素。

## 头文件

```cpp
#include <yggr/serialization/unordered_collections_load_bson_impl.hpp>
```

该文件通常由无序集合序列化实现间接包含。

## 编译条件

实现仅在定义以下宏时启用：

```cpp
YGGR_HAS_BSON_IARCHIVE
```

启用时定义主 include guard：

```cpp
__YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_LOAD_BSON_IMPL_HPP__
```

如果未定义 `YGGR_HAS_BSON_IARCHIVE`，文件不会生成 BSON 加载特化，而是定义内部待处理宏：

```cpp
YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE
```

当 BSON 输入归档宏已经生效时，如果该待处理宏此前存在，文件会先将其取消定义。

## 依赖

启用实现时包含：

```cpp
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/array_bson_load_impl.hpp>
#include <sstream>
```

其中 `array_bson_load_impl.hpp` 提供相关 BSON 数组加载支持，`nvp.hpp` 用于按字段名称读取归档数据，`sstream` 用于生成数字字符串字段名。

## `unordered_collections_loader_switch` 特化

特化位于：

```cpp
yggr::serialization::container
```

接口形式为：

```cpp
template<bool skip_auto_id, typename Container, typename InputFunction>
struct unordered_collections_loader_switch<
    yggr::archive::bson_iarchive<skip_auto_id>,
    Container,
    InputFunction>
{
    void operator()(
        yggr::archive::bson_iarchive<skip_auto_id>& ar,
        Container& container) const;
};
```

该特化适用于所有匹配的 BSON 输入归档和无序容器类型，具体元素如何构造和插入由 `InputFunction` 决定。

## 加载流程

`operator()` 的执行步骤如下：

1. 清空目标容器：

   ```cpp
   container.clear();
   ```

2. 从归档读取集合元素数量 `count`。

3. 从归档读取桶数量 `bucket_count`。

4. 当归档库版本大于 3 时，读取元素版本 `item_version`。

5. 在非 `__MWERKS__` 环境下调用：

   ```cpp
   container.reserve(bucket_count);
   ```

6. 创建 `InputFunction` 实例。

7. 对每个元素调用：

   ```cpp
   ifunc(ar, container, item_version, idx);
   ```

其中 `idx` 从 0 开始递增，并用于生成 BSON 字段名。

## BSON 字段命名

元数据和元素字段均使用数字字符串作为字段名。字段名通过以下方式生成：

```cpp
std::stringstream ss;
ss << idx;
ar >> YGGR_SERIALIZATION_NAME_NVP(ss.str().data(), value);
```

因此 BSON 输入中的字段顺序通常对应如下逻辑索引：

```text
"0", "1", "2", ...
```

在读取 `count`、`bucket_count` 和 `item_version` 后，元素从当前索引继续读取。

## 归档版本兼容

实现根据 `ar.get_library_version()` 处理集合大小和桶数量的历史格式差异。

### 非版本 6

当归档库版本不是 6 时，直接按 `collection_size_type` 读取：

```cpp
collection_size_type count;
collection_size_type bucket_count;
```

### 版本 6

当归档库版本为 6 时，先读取两个 `unsigned int` 临时变量，再赋值给 `collection_size_type`：

```cpp
unsigned int c;
unsigned int bc;
count = c;
bucket_count = bc;
```

这是为了兼容版本 6 中 hashed collection 将集合大小错误实现为 `unsigned int` 的历史格式，特别覆盖 Windows 64 位环境可能产生的差异。

## 元素版本

当归档库版本大于 3 时，会读取：

```cpp
boost::serialization::item_version_type item_version(0);
```

否则保持默认值 0。该版本随后传递给 `InputFunction`，由具体元素输入逻辑使用。

## 容量预留

在非 `__MWERKS__` 编译环境下，加载器会根据归档中的桶数量调用：

```cpp
container.reserve(bucket_count);
```

这可以减少无序容器逐项插入时的重新哈希。`__MWERKS__` 环境会跳过该调用，以避免目标编译器或容器实现不支持对应操作。

## 输入函数分派

文件不直接实现元素读取，而是通过模板参数 `InputFunction` 分派：

```cpp
InputFunction ifunc;
for(; count-- > 0; ++idx)
{
    ifunc(ar, container, item_version, idx);
}
```

这样可以复用通用无序集合加载流程，并允许不同容器通过不同输入函数处理元素插入、对象构造和地址修复。

## 宏清理与注意事项

- 本文件只提供 BSON 输入归档实现，不处理 BSON 输出归档。
- `YGGR_HAS_BSON_IARCHIVE` 必须在包含本文件前正确设置。
- `count`、`bucket_count` 和 `item_version` 的字段顺序必须与对应 BSON 保存格式一致。
- `InputFunction` 必须接受归档对象、目标容器、元素版本和数字索引四个参数。
- `YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE` 属于内部重新包含控制宏，业务代码不应直接依赖。
