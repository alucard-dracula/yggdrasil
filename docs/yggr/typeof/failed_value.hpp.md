# `failed_value.hpp` 中文说明

## 用途

本头文件定义 `yggr::failed_value<T>(...)`，用于在类型推导或错误处理路径中取得一个“失败值”。对非 `void` 类型，该函数返回由给定参数构造的、按类型缓存的静态对象；对 `void` 类型则直接返回。

```cpp
#include <yggr/typeof/failed_value.hpp>
```

## 接口与行为

- `failed_value<void>(...)` 返回 `void`，不构造或缓存对象，传入参数不参与结果。
- `failed_value<T>(args...)`（`T` 非 `void`）将参数转发给 `create_object`，并返回类型为 `T` 的结果。
- 非 `void` 路径先用 `remove_cv_ref<T>::type` 得到静态缓存对象类型；该对象在首次调用时初始化，之后的调用仍以该对象作为返回值来源。
- 不支持可变模板的构建使用预处理器生成有限参数数量的重载。

## 示例

```cpp
#include <yggr/typeof/failed_value.hpp>

struct error_value
{
    explicit error_value(int code) : code(code) {}
    int code;
};

error_value error = yggr::failed_value<error_value>(-1);
yggr::failed_value<void>();
```

## 注意事项

- 函数声明的返回类型是 `T`，不是引用。实现通过静态对象缓存构造结果，再按 `T` 返回；返回过程可能涉及复制或转换。
- 对相同模板实例的首次调用参数决定静态对象的初始化值，之后的调用参数不会重新初始化该对象。
- 静态对象可能在多线程中首次访问；其初始化安全性取决于项目采用的 C++ 标准和编译器运行时。
- `T` 的去 cv/ref 类型必须能由传入参数构造；构造异常可能在首次调用时传播。
- 此工具不适用于需要每次独立创建错误值或需要携带逐次变化状态的场景。
