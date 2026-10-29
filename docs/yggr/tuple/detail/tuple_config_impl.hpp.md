# `tuple_config_impl.hpp` 中文说明

## 用途

本头文件定义 Boost.Tuple 和 Yggdrasil Tuple 的最大元素数配置，并通过 `yggr::tuples::tuple_cfg` 提供对应的编译期常量。它属于 tuple 子系统的内部配置实现，通常由 `tuple_config.hpp` 间接包含。

```cpp
#include <yggr/tuple/tuple.hpp>
```

## 配置宏

| 宏 | 默认值/定义 | 说明 |
| --- | --- | --- |
| `BOOST_TUPLE_LIMIT_LENGTH_CFG` | 未定义时默认为 `10` | Boost.Tuple 的参数上限配置值。 |
| `BOOST_TUPLE_LIMIT_LENGTH()` | 展开为 `BOOST_TUPLE_LIMIT_LENGTH_CFG` | Boost.Tuple 上限的统一访问宏。 |
| `YGGR_TUPLE_LIMIT_DEFAULT_LENGTH()` | 展开为 `BOOST_TUPLE_LIMIT_LENGTH()` | Yggdrasil Tuple 默认沿用的上限。 |
| `YGGR_TUPLE_LIMIT_LENGTH_CFG` | 未定义时默认为 `YGGR_TUPLE_LIMIT_DEFAULT_LENGTH()` | Yggdrasil Tuple 的参数上限配置值。 |
| `YGGR_TUPLE_LIMIT_LENGTH()` | 展开为 `YGGR_TUPLE_LIMIT_LENGTH_CFG` | Yggdrasil Tuple 上限的统一访问宏。 |

Boost 和 Yggdrasil 的长度配置都必须不小于 `2`；否则预处理阶段会产生错误。若需要提高 Boost.Tuple 参数上限，应确保相关 Boost.Tuple 配置与 Yggdrasil 的预处理器生成上限一致，并在包含相关头文件前设置配置宏。

## 编译期配置类型

```cpp
yggr::tuples::tuple_cfg::E_boost_tuple_limit_length
yggr::tuples::tuple_cfg::E_yggr_tuple_limit_length
```

两个静态常量分别记录 `BOOST_TUPLE_LIMIT_LENGTH()` 和 `YGGR_TUPLE_LIMIT_LENGTH()` 的值，类型为 `std::size_t`，可用于编译期配置查询。

## 使用与注意事项

- 这是内部实现头文件；通常通过公开的 `yggr/tuple/tuple.hpp` 使用 tuple，不建议应用代码直接依赖此文件。
- 配置值会影响模板参数列表、预处理器生成的重载以及可构造的最大 tuple 长度。
- 配置宏应在首次包含 tuple/Boost.Tuple 相关配置头文件之前定义；包含后再修改宏不会改变已经生成的类型和声明。
- 增大上限时应验证项目使用的 Boost 版本、预处理器参数生成范围及编译器模板能力。
