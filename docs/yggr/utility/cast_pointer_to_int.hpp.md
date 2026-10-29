# `cast_pointer_to_int.hpp` 中文说明

## 用途

本头文件提供将指针地址转换为整数结果的工具，底层整数类型使用 `yggr::mplex::pointer_int::type`，其宽度按平台指针位宽配置。

```cpp
#include <yggr/utility/cast_pointer_to_int.hpp>
```

## 接口

### `pointer_to_int_caster<T>`

函数对象。调用 `operator()(const void*)` 时，将指针重解释为平台指针整数类型，再构造并返回 `T`。

```cpp
yggr::utility::pointer_to_int_caster<std::size_t> caster;
std::size_t id = caster(ptr);
```

### `cast_pointer_to_int::cast<T>(ptr)`

静态模板函数，将 `const void*` 转换为 `yggr::mplex::pointer_int::type`，再构造并返回 `T`。

```cpp
std::size_t id =
    yggr::utility::cast_pointer_to_int::cast<std::size_t>(ptr);
```

两个工具均定义在 `yggr::utility` 命名空间。

## 使用场景

该转换可用于从对象地址生成进程内标识或映射键，例如将连接对象或 socket 对象的地址转为其项目定义的整数 ID。

## 注意事项

- 这是指针到整数的实现相关转换，不是可移植的持久化标识；结果可能随进程、运行或地址布局变化。
- 转换结果不应被当作对象所有权、有效性或唯一性的长期保证。
- 返回模板类型 `T` 必须能由 `mplex::pointer_int::type` 构造；若 `T` 更窄，可能发生截断。
- `mplex::pointer_int::type` 根据项目的 `sysbit_now` 选择整数类型；应确认目标平台的指针位宽有相应映射。
- 该工具不提供从整数恢复指针的接口，也不检查传入指针是否为空。
