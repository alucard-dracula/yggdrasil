# `char8_t.hpp`

## 概述

该头文件为 yggdrasil 的 `yggr::c8` 字符类型提供 Boost.Serialization 兼容性配置。

它只在特定条件下启用两项编译期声明：

- 将 `yggr::c8` 标记为原始序列化类型；
- 将 `yggr::c8` 标记为按位可序列化类型。

## 头文件

```cpp
#include <yggr/serialization/char8_t.hpp>
```

## 启用条件

相关配置只在以下条件同时满足时生效：

```cpp
!defined(YGGR_NO_CHAR8_T) && (BOOST_VERSION < 108200)
```

也就是说：

- 未定义 `YGGR_NO_CHAR8_T`，表示项目启用了 `c8` 支持；
- Boost 版本低于 `1.82.0`。

当 `YGGR_NO_CHAR8_T` 已定义，或 Boost 版本不低于 1.82 时，文件不会生成任何相关序列化注册。

## 序列化配置

### 原始类型标记

```cpp
BOOST_CLASS_IMPLEMENTATION(yggr::c8, primitive_type)
```

将 `yggr::c8` 的 Boost.Serialization 实现级别声明为 `primitive_type`，使归档框架按原始值处理该类型。

### 按位可序列化标记

```cpp
BOOST_IS_BITWISE_SERIALIZABLE(yggr::c8)
```

声明 `yggr::c8` 可以按位复制进行序列化。该声明适用于字符类的简单值类型，可减少序列化过程中的额外处理。

## 使用示例

```cpp
#include <yggr/serialization/char8_t.hpp>

void serialize_char8(yggr::c8 value)
{
    // 在满足启用条件的 Boost 版本和项目配置下，
    // value 可按项目归档规则作为原始值处理。
    (void)value;
}
```

该文件不提供运行时函数，也不需要显式调用初始化接口。包含后，相关 Boost.Serialization 类型特征会自动生效。

## 版本兼容说明

文件只针对 Boost 1.82 之前的版本执行注册，说明较新 Boost 版本可能已经内置或采用了不同的 `char8_t` 序列化支持方式。

因此，在 Boost 1.82 及以上版本中，文件主动跳过旧版兼容声明，避免重复或冲突的类型特征定义。

## 注意事项

1. 该文件只提供编译期类型特征，不定义 `yggr::c8` 本身。
2. `YGGR_NO_CHAR8_T` 已定义时，不会注册 `yggr::c8` 的序列化特征。
3. Boost 版本判断使用 `BOOST_VERSION < 108200`，版本号 `108200` 对应 Boost 1.82.0。
4. `BOOST_CLASS_IMPLEMENTATION` 和 `BOOST_IS_BITWISE_SERIALIZABLE` 的具体行为由 Boost.Serialization 决定。
5. 按位可序列化要求类型适合按位复制；如果修改 `yggr::c8` 的内部表示，应重新评估该声明是否仍然成立。
6. 使用该头文件前应确保 `yggr::c8` 和 `BOOST_VERSION` 已由相关依赖正确定义。

## 相关组件

- `yggr/base/yggrdef.h`
- `yggr/serialization/export.hpp`
- `boost/serialization/level.hpp`
- `boost/serialization/is_bitwise_serializable.hpp`
