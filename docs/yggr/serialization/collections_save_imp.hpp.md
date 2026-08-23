# `collections_save_imp.hpp`

## 概述

该头文件提供通用集合保存实现，用于将满足标准容器接口的集合对象写入 Boost.Serialization 归档。

主要功能包括：

- 保存集合元素数量；
- 保存元素版本信息；
- 保存每个元素的构造数据；
- 以统一字段名保存集合元素；
- 提供 `save_collection()` 作为调用入口；
- 在文件末尾接入 BSON 专用集合保存实现。

## 头文件

```cpp
#include <yggr/serialization/collections_save_imp.hpp>
```

## 命名空间

实现位于：

```cpp
yggr::serialization::container
```

外层命名空间实际为：

```cpp
yggr::serialization
```

## 依赖

主要依赖：

- `yggr/serialization/nvp.hpp`：名称-值对序列化支持；
- `yggr/serialization/collection_size_type.hpp`：集合大小包装类型；
- `boost/serialization/serialization.hpp`：基础序列化设施；
- `boost/serialization/version.hpp`：类型版本信息；
- `boost/serialization/item_version_type.hpp`：元素版本类型；
- `boost/ref.hpp`：地址和引用辅助支持。

## `collections_saver_switch`

```cpp
template<typename Archive, typename Container>
struct collections_saver_switch;
```

该模板是通用集合保存器。BSON 归档可以通过专用特化覆盖默认行为。

### 调用操作符

```cpp
void operator()(Archive& ar, const Container& s) const;
```

将集合 `s` 保存到归档 `ar`。

### 容器要求

`Container` 通常需要提供：

```cpp
size_type size() const;
const_iterator begin() const;
typedef ... value_type;
```

此外，元素类型必须满足 Boost.Serialization 的保存要求，并支持必要的构造数据保存操作。

## 保存流程

通用实现按以下顺序写入数据：

1. 创建集合大小对象：

   ```cpp
   collection_size_type count(s.size());
   ```

2. 获取元素版本：

   ```cpp
   boost::serialization::version<
       typename Container::value_type>::value
   ```

3. 保存集合数量：

   ```cpp
   ar << YGGR_SERIALIZATION_NVP(count);
   ```

4. 保存元素版本：

   ```cpp
   ar << YGGR_SERIALIZATION_NVP(item_version);
   ```

5. 从 `s.begin()` 开始遍历集合；
6. 对每个元素调用 `save_construct_data_adl()` 保存构造数据；
7. 使用名称 `"item"` 保存当前元素；
8. 递减计数并继续遍历，直到所有元素保存完成。

元素保存代码等价于：

```cpp
boost::serialization::save_construct_data_adl(
   ar,
   boost::addressof(*it),
   item_version);
ar << YGGR_SERIALIZATION_NAME_NVP("item", *it++);
```

## `save_collection`

```cpp
template<typename Archive, typename Container>
void save_collection(Archive& ar, const Container& s);
```

创建 `collections_saver_switch<Archive, Container>`，然后调用其函数对象。对于 BSON 归档，匹配的特化会替代通用保存器。

## 元素版本和构造数据

元素版本通过 `boost::serialization::version<typename Container::value_type>::value` 获取。
每个元素正式保存前都会调用 `save_construct_data_adl()`，用于支持需要额外构造数据的元素类型。

## 通用归档布局

默认归档通常包含集合数量、元素版本，以及使用名称 `"item"` 按顺序保存的集合元素。
实际字段表示由归档类型和名称-值对宏的实现决定。

## BSON 分派

文件末尾包含 `collections_save_bson_impl.hpp`。启用 `YGGR_HAS_BSON_OARCHIVE` 时，BSON 输出归档使用专用特化：

- `bson_oarchive<false>` 使用数字字段保存数量、元素版本和元素；
- `bson_oarchive<true>` 省略数量和元素版本，直接保存数字字段元素。

## 使用示例

```cpp
#include <yggr/serialization/collections_save_imp.hpp>
#include <vector>

template<typename Archive>
void save_values(Archive& archive, const std::vector<int>& values)
{
   yggr::serialization::container::save_collection(archive, values);
}
```

## 注意事项

1. `Container` 的 `size()` 应与从 `begin()` 开始可遍历的元素数量一致。
2. 元素类型必须满足 Boost.Serialization 的序列化要求。
3. 元素构造数据保存会在元素本体保存之前执行。
4. 通用归档使用名称 `"item"` 保存元素；BSON 特化可能采用数字字符串字段名。
5. 元素版本由 `Container::value_type` 的 Boost.Serialization 版本特征决定。
6. `save_collection()` 的归档类型分派由 `collections_saver_switch` 完成。
7. 归档读取端必须与保存端使用兼容的集合布局、元素版本和构造数据约定。

## 相关组件

- `yggr/serialization/nvp.hpp`
- `yggr/serialization/collection_size_type.hpp`
- `yggr/serialization/collections_save_bson_impl.hpp`
- `boost/serialization/serialization.hpp`
- `boost/serialization/version.hpp`
- `boost/serialization/item_version_type.hpp`
- `boost/serialization/collections_save_imp.hpp`

   ```cpp
   ar << YGGR_SERIALIZATION_NVP(item_version);
   ```

5. 从 `s.begin()` 开始遍历集合；
6. 对每个元素调用 `save_construct_data_adl()` 保存构造数据；
7. 使用名称 `