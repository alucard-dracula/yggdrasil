# `get_element_default.hpp` 中文说明

## 用途

本头文件为通用类型提供首、末元素访问函数的默认实现：当类型不是由其他重载专门支持的 pair 或 tuple 时，首元素和末元素都视为对象本身。

```cpp
#include <yggr/utility/get_element_default.hpp>
```

## 接口

```cpp
template<typename T>
T& get_first_element(T& value);

template<typename T>
T& get_last_element(T& value);
```

两个函数均位于 `yggr::utility` 命名空间，并通过 `yggr` 命名空间导出。它们返回传入对象的非常量左值引用，不复制对象。

## 示例

```cpp
#include <yggr/utility/get_element_default.hpp>

int value = 5;
int& first = yggr::get_first_element(value);
int& last = yggr::get_last_element(value);

first = 8; // 同时修改 value
```

在其他头文件提供专门重载时，pair 或 tuple 可返回其首个或末尾元素；本头文件的通用重载则将对象自身作为结果。

## 注意事项

- 仅提供非常量左值重载；const 对象、右值不能直接匹配这些函数。
- 返回引用与原对象具有相同生命周期；不得将引用保存到对象销毁之后。
- 本文件本身不检查容器长度，也不访问内部元素；容器、pair 和 tuple 的具体行为由其他专门重载提供。
