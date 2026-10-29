# `has_char_t.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::has_char_t<T>`，用于检测类型是否具有可经 `yggr::native_t` 处理的嵌套类型 `char_type`。

```cpp
#include <yggr/type_traits/has_char_t.hpp>
```

## 接口

```cpp
yggr::has_char_t<T>::value
```

`has_char_t<T>` 继承自 `boost::mpl::bool_`：

- 若 `T::char_type` 存在，且 `yggr::native_t<typename T::char_type>::type` 可用于检测函数参数，则 `value` 为 `true`。
- 否则，`value` 为 `false`。

该特征在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/has_char_t.hpp>

struct text_like
{
    typedef char char_type;
};

struct plain
{
};

static_assert(yggr::has_char_t<text_like>::value, "char_type is detected");
static_assert(!yggr::has_char_t<plain>::value, "char_type is absent");
```

## 检测机制与注意事项

- 实现通过重载检测和 `sizeof` 比较判断嵌套类型是否可用。
- 该特征只检查名为 `char_type` 的嵌套类型，不会判断它是否为 `char`，也不验证类型是否为字符容器。
- `char_type` 必须能作为 `yggr::native_t` 的模板参数，且转换后的类型满足检测表达式要求。
- `T` 的检测发生于模板实例化期；它不会检查该类型的具体用途或语义。
