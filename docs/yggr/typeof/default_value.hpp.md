# `default_value.hpp` 中文说明

## 用途

本头文件定义 `yggr::default_value<T>(...)`，用于根据模板参数 `T` 返回默认值或通过给定参数构造的 `T` 对象。参数通过 Yggdrasil/Boost 的转发机制传递。

```cpp
#include <yggr/typeof/default_value.hpp>
```

## 接口与行为

- 当 `T` 为 `void` 时，`default_value<void>(...)` 返回 `void`，不构造对象；提供的参数会被忽略。
- 当 `T` 非 `void` 时，函数将参数转发给 `create_object<T>(...)`，并返回构造得到的 `T`。
- 启用可变模板时，接口接受参数包；兼容实现则通过预处理器生成有限参数数量的重载。

## 示例

```cpp
#include <yggr/typeof/default_value.hpp>

struct item
{
    explicit item(int value) : value(value) {}
    int value;
};

item result = yggr::default_value<item>(42);
yggr::default_value<void>();
```

## 注意事项

- 对非 `void` 类型，`T` 必须能由传入参数构造；构造失败会导致编译错误或运行时构造异常。
- 该函数返回一个对象值，不是类型特征，也不单独负责引用包装或对象生命周期管理。
- `void` 重载返回后不保留或处理参数。
- 旧编译器分支可接受的参数数量受项目预处理器参数配置限制。
