# `has_base_t.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::has_base_t<Container>`，用于检测类型是否具有可经 `yggr::native_t` 处理的嵌套类型 `base_type`。

```cpp
#include <yggr/type_traits/has_base_t.hpp>
```

## 接口

```cpp
yggr::has_base_t<T>::value
```

`has_base_t<T>` 继承自 `boost::mpl::bool_`：

- 若 `T::base_type` 存在，且 `yggr::native_t<typename T::base_type>::type` 可用于检测函数参数，则 `value` 为 `true`。
- 否则，`value` 为 `false`。

该特征在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/has_base_t.hpp>

struct wrapped
{
    typedef int base_type;
};

struct plain
{
};

static_assert(yggr::has_base_t<wrapped>::value, "base_type is detected");
static_assert(!yggr::has_base_t<plain>::value, "base_type is absent");
```

## 检测机制与注意事项

- 实现通过重载优先级和 `sizeof` 比较完成检测：存在可匹配的 `base_type` 时选择一字节标记类型的重载，否则选择较大的标记类型重载。
- 该特征检测的是名为 `base_type` 的嵌套类型及其可用性，并不直接判断 `T` 是否继承自某个基类。
- `base_type` 必须能作为 `yggr::native_t` 的模板参数，且其转换后的类型满足检测表达式要求。
- 对 `T` 的检测发生在模板实例化期；它不检查 `base_type` 的语义或与 `T` 的关系。
