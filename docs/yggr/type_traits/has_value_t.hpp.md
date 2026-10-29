# `has_value_t.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::has_value_t<T>`，用于检测类型是否具有可经 `yggr::native_t` 处理的嵌套类型 `value_type`。

```cpp
#include <yggr/type_traits/has_value_t.hpp>
```

## 接口

```cpp
yggr::has_value_t<T>::value
```

`has_value_t<T>` 继承自 `boost::mpl::bool_`：

- 若 `T::value_type` 存在，且 `yggr::native_t<typename T::value_type>::type` 可用于检测函数参数，则 `value` 为 `true`。
- 否则，`value` 为 `false`。

该特征在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/has_value_t.hpp>

struct sequence_like
{
    typedef int value_type;
};

struct plain
{
};

static_assert(yggr::has_value_t<sequence_like>::value,
              "value_type is detected");
static_assert(!yggr::has_value_t<plain>::value, "value_type is absent");
```

## 检测机制与注意事项

- 实现通过重载检测和 `sizeof` 比较判断嵌套类型是否可用。
- 该特征只检查名为 `value_type` 的嵌套类型，不会验证对象是否为容器，也不会检查迭代能力或元素访问行为。
- `value_type` 必须能作为 `yggr::native_t` 的模板参数，且转换后的类型满足检测表达式要求。
- 本特征也可通过 `yggr/container_ex/has_value_t.hpp` 在 `yggr::container_ex` 命名空间使用。
