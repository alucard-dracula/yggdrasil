# `deduce_result.hpp` 中文说明

## 用途

本头文件声明辅助函数 `yggr::deduce_result`，用于配合 `YGGR_TYPEOF` 推导处理器或可调用对象的返回类型。由于部分 Boost.Typeof 实现不能直接表示 `void` 返回类型，该函数将其映射为 `boost::mpl::void_`。

```cpp
#include <yggr/typeof/deduce_result.hpp>
```

## 接口

```cpp
template<typename Handler>
deduced_result_type yggr::deduce_result(const Handler&);
```

返回类型根据 `yggr::func::foo_t_info<Handler>::result_type` 条件选择：

- 若处理器结果类型为 `void`，返回类型为 `boost::mpl::void_`；
- 否则，返回类型与处理器的 `result_type` 相同。

`deduce_result` 仅作声明，用于编译期表达式类型推导；不应作为运行时函数调用。

## 使用示例

```cpp
#include <yggr/typeof/deduce_result.hpp>
#include <yggr/typeof/typeof.hpp>
#include <boost/bind/bind.hpp>

typedef YGGR_TYPEOF(
    yggr::deduce_result(boost::bind(&some_function, 1))
) result_type;
```

该模式通过 `foo_t_info` 获取处理器的结果类型，并使其可供 typeof 机制处理。具体可调用对象形式及绑定参数需符合对应 Boost.Bind 和 Yggdrasil 函数信息特征的支持范围。

## 注意事项

- 对 `void` 返回值，推导结果是占位类型 `boost::mpl::void_`，不是 `void` 本身。
- 此头文件不执行处理器，不创建或调用函数对象。
- 正确推导依赖 `yggr::func::foo_t_info<Handler>` 能够识别对应处理器类型。
