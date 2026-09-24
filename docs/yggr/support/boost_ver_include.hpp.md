# `boost_ver_include.hpp`

## 概述

该头文件提供 Boost 版本和版本化头文件包含路径的预处理器辅助宏。它不实现运行时逻辑，也不修改 Boost 版本，只负责集中包含 Boost 配置/版本头并暴露统一的宏接口。

主要功能包括：

- 确保 `boost/config.hpp` 已被包含；
- 包含 `boost/version.hpp` 获取 `BOOST_VERSION`；
- 通过 `YGGR_PP_BOOST_VERSION()` 暴露当前 Boost 版本；
- 通过 `YGGR_PP_BOOST_SRC_INCLUDE` 拼接版本化 Boost 头文件路径。

## 头文件

```cpp
#include <yggr/support/boost_ver_include.hpp>
```

## 依赖

```cpp
#ifndef BOOST_CONFIG_HPP
#include <boost/config.hpp>
#endif

#include <boost/version.hpp>
```

如果 `BOOST_CONFIG_HPP` 已经定义，则不会重复包含 `boost/config.hpp`。`boost/version.hpp` 始终由本文件直接包含。

## Include guard

主 include guard 为：

```cpp
__YGGR_SUPPORT_BOOST_VER_INCLUDE_HPP__
```

该 guard 防止辅助宏和 Boost 版本头被重复处理。

## `YGGR_PP_BOOST_VERSION`

文件定义：

```cpp
#define YGGR_PP_BOOST_VERSION() BOOST_VERSION
```

调用：

```cpp
YGGR_PP_BOOST_VERSION()
```

会展开为 Boost 官方定义的：

```cpp
BOOST_VERSION
```

Boost 的版本号通常编码为：

```text
major * 100000 + minor * 100 + patch
```

因此调用方可以使用该宏参与预处理器版本判断，例如：

```cpp
#if YGGR_PP_BOOST_VERSION() >= 106700
// 使用较新的 Boost 接口
#endif
```

实际版本编码规则由 Boost 版本头定义，本文件只是提供一层 Yggdrasil 命名空间风格的包装。

## `YGGR_PP_BOOST_SRC_INCLUDE`

文件定义：

```cpp
#define YGGR_PP_BOOST_SRC_INCLUDE(__d__, __v__, __f__) \
    < __d__/__v__/__f__ >
```

该宏用于拼接带目录、版本目录和文件名的尖括号头文件路径。

参数含义如下：

| 参数 | 含义 |
| --- | --- |
| `__d__` | 基础目录 |
| `__v__` | 版本或版本分支目录 |
| `__f__` | 目标头文件名 |

概念上的展开结果为：

```cpp
YGGR_PP_BOOST_SRC_INCLUDE(boost, version_dir, header.hpp)
```

展开为：

```cpp
<boost/version_dir/header.hpp>
```

该宏通常用于配合：

```cpp
#include YGGR_PP_BOOST_SRC_INCLUDE(...)
```

从而根据预处理器参数生成版本化头文件包含路径。

## 设计特点

- 只提供预处理器能力，不定义 C++ 类型或函数；
- 版本值直接来源于 Boost 的 `BOOST_VERSION`；
- 不自动根据当前 Boost 版本选择具体文件；
- 不验证拼接出的头文件是否存在；
- 通过尖括号形式使用编译器的系统/包含路径搜索规则。

## 使用示例

版本判断：

```cpp
#include <yggr/support/boost_ver_include.hpp>

#if YGGR_PP_BOOST_VERSION() < 106700
// 旧版 Boost 实现
#else
// 新版 Boost 实现
#endif
```

拼接版本化包含路径：

```cpp
#include <yggr/support/boost_ver_include.hpp>

#include YGGR_PP_BOOST_SRC_INCLUDE(
    boost,
    v1,
    feature.hpp)
```

这类用法要求对应的头文件路径在编译器 include 搜索路径中真实存在。

## 使用限制与注意事项

- `YGGR_PP_BOOST_VERSION()` 依赖 `boost/version.hpp`，必须在有效 Boost 环境中使用。
- 版本比较应使用完整的 Boost 版本编码，不要直接把编码当作普通小数比较。
- `YGGR_PP_BOOST_SRC_INCLUDE` 只做文本拼接，不负责目录合法性、文件存在性或版本映射。
- 使用该宏生成 `#include` 路径时，传入参数必须能够组成编译器可识别的头文件路径。
- 不要重新定义 `BOOST_VERSION` 或依赖本文件修改 Boost 的版本信息。
