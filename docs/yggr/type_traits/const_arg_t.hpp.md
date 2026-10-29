# `const_arg_t.hpp` 中文说明

## 用途

本头文件定义编译期类型特征 `yggr::const_arg_t<T>`，根据 `T` 是否为类类型，选择适合作为函数参数的类型形式。

```cpp
#include <yggr/type_traits/const_arg_t.hpp>
```

## 类型映射

`const_arg_t<T>` 继承自项目的 `yggr::traits`，最终类型由其嵌套类型 `type` 表示：

- 若 `T` 是类类型，映射为 `const T&`，避免按值复制类对象。
- 若 `T` 不是类类型，映射为 `T`，按值传递。

示例：

```cpp
#include <yggr/type_traits/const_arg_t.hpp>

typedef yggr::const_arg_t<int>::type int_arg_type;
typedef yggr::const_arg_t<std::string>::type string_arg_type;
```

在通常的类型特征定义下，`int_arg_type` 为 `int`，`string_arg_type` 为 `const std::string&`。类型特征位于 `yggr::type_traits` 命名空间，并通过 `yggr` 命名空间导出。

## 注意事项

- 是否为类类型由 `boost::is_class<T>` 判断；枚举、标量、指针等非类类型按值映射。
- 该特征只选择参数类型，不会创建函数，也不负责参数生命周期或所有权管理。
- 具体结果还受 `yggr::traits` 的定义影响；使用 `std::string` 等类型时需包含相应标准库头文件。
