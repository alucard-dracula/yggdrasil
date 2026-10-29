# `tuple_config.hpp` 中文说明

## 用途

本头文件是 Yggdrasil Tuple 的配置入口，集中引入 Yggdrasil 基础定义、tuple 长度配置实现和 Boost.Tuple 声明。它本身不定义 tuple 类型或操作，而是为 tuple 子系统准备所需的配置与依赖。

## 包含内容

```cpp
#include <yggr/tuple/detail/tuple_config.hpp>
```

该头文件依次包含：

- `yggr/base/yggrdef.h`：Yggdrasil 基础定义。
- `yggr/tuple/detail/tuple_config_impl.hpp`：定义 Boost.Tuple 和 Yggdrasil Tuple 的最大元素数配置，以及 `yggr::tuples::tuple_cfg` 编译期常量。
- `boost/tuple/tuple.hpp`：Boost.Tuple 的类型和相关接口。

长度配置的默认值、可覆盖宏及限制说明见 [`tuple_config_impl.hpp.md`](./tuple_config_impl.hpp.md)。

## 注意事项

- 这是 tuple 子系统内部使用的配置头文件；一般应用代码应通过公开的 tuple 头文件使用相关类型。
- 若要调整 tuple 长度，应在首次包含本头文件或相关 Boost.Tuple 配置之前定义相应配置宏。
- 文件中注释掉的 `boost/tuple/detail/tuple_basic_no_partial_spec.hpp` 仅保留为历史测试用途，不会被当前头文件包含。
