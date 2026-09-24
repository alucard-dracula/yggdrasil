# `conv_params_and_packet_info.hpp`

## 概述

该头文件提供 `any_val::params` 与 `packet::packet_info` 之间的双向静态转换工具：

- 参数对象转换为 packet 信息：根据给定 key 从参数中读取值，并生成由键值 pair 组成的 `packet_info`；
- packet 信息转换为参数对象：遍历 `packet_info` 中的键值 pair，将值写回参数对象。

外层头文件根据是否支持 C++11 variadic templates，选择不同的 detail 实现：

- 支持 variadic templates：`detail/conv_params_and_packet_info_vtpl.hpp`；
- 不支持 variadic templates：`detail/conv_params_and_packet_info_vmacro.hpp`。

两个实现的公开类和转换语义一致，差别主要在模板参数展开方式。

## 头文件

```cpp
#include <yggr/struct_conv/conv_params_and_packet_info.hpp>
```

detail 实现不应直接包含；两个 detail 文件都要求先包含外层头文件，否则会触发预处理器错误。

## 编译分支

外层文件根据以下宏选择实现：

```cpp
#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
#include <yggr/struct_conv/detail/conv_params_and_packet_info_vtpl.hpp>
#else
#include <yggr/struct_conv/detail/conv_params_and_packet_info_vmacro.hpp>
#endif
```

选择规则如下：

| 条件 | 使用的实现 |
| --- | --- |
| 未定义 `YGGR_NO_CXX11_VARIADIC_TEMPLATES` | `v_tpl`，使用 variadic templates 和 index sequence |
| 定义 `YGGR_NO_CXX11_VARIADIC_TEMPLATES` | `v_macro`，使用 Boost.Preprocessor/Yggdrasil 参数宏 |

## 命名空间与类型

转换器位于：

```cpp
yggr::struct_conv
```

核心类型为：

```cpp
struct conv_params_and_packet_info;
```

该类型只提供静态函数，不保存运行时状态，通常通过类名直接调用：

```cpp
yggr::struct_conv::conv_params_and_packet_info::conv(...);
```

## 相关数据类型

### `any_val::params`

参数对象的基本形式为：

```cpp
yggr::any_val::params<
    Key,
    Op,
    Cmp,
    Alloc,
    Map>
```

转换器使用其：

```cpp
params.template get_value<ValueType>(key)
```

读取指定 key 对应的值，并在反向转换中使用下标操作写回：

```cpp
params[key] = value;
```

### `packet::packet_info`

packet 信息由键值 pair 参数包组成，例如：

```cpp
packet::packet_info<
    Pair<Key, Value1>,
    Pair<Key, Value2>>
```

每个元素包含：

```cpp
pair.first
// 参数 key

pair.second
// 对应参数值
```

空 `packet_info<>` 表示没有参数项。

## 参数到 packet_info

### 空参数转换

接口为：

```cpp
template<
    typename Key,
    typename Op,
    typename Cmp,
    typename Alloc,
    template<
        typename _Key,
        typename _Val,
        typename _Cmp,
        typename _Alloc>
    class Map>
static packet::packet_info<> conv(
    const any_val::params<
        Key, Op, Cmp, Alloc, Map>& params);
```

该重载返回空的：

```cpp
packet::packet_info<>
```

参数对象内容不会参与处理。

### 带 key 的转换

variadic templates 版本的核心接口为：

```cpp
template<
    template<typename _First, typename _Second> class Pair,
    typename ...Args,
    typename Key,
    typename Op,
    typename Cmp,
    typename Alloc,
    template<
        typename _Key,
        typename _Val,
        typename _Cmp,
        typename _Alloc>
    class Map>
static packet::packet_info<Pair<Key, Args>...> conv(
    const any_val::params<Key, Op, Cmp, Alloc, Map>& params,
    const typename boost::mpl::pair<Args, Key>::second& ...keys);
```

对每个 key：

1. 使用 `params.get_value<Args>(key)` 读取对应类型的值；
2. 构造 `Pair<Key, Args>(key, value)`；
3. 将所有 pair 按参数顺序组合为 `packet::packet_info<Pair<Key, Args>...>`。

逻辑形式为：

```cpp
packet_info_type(
    Pair<Key, Args>(
        keys,
        params.template get_value<Args>(keys))...);
```

其中 `Args...` 表示各个值的类型，`keys...` 表示对应参数键。

## packet_info 到参数

### 空 packet_info 转换

接口为：

```cpp
template<typename Params>
static Params conv(
    const packet::packet_info<>& packet_info);
```

该重载返回默认构造的 `Params`：

```cpp
return Params();
```

### 带参数项的转换

接口形式为：

```cpp
template<typename Params, typename ...Args>
static Params conv(
    const packet::packet_info<Args...>& packet_info);
```

转换过程如下：

1. 默认构造 `Params params`；
2. 按索引遍历 `packet_info` 的每个参数项；
3. 取得 pair：

   ```cpp
   const type& item = packet_info.template get<I>();
   ```

4. 使用 `item.first` 作为参数键；
5. 使用 `ANY_SET` 将 `item.second` 转换为目标值类型；
6. 写入：

   ```cpp
   params[item.first] = converted_value;
   ```

7. 返回填充后的 `Params`。

### 类型转换

写回参数时使用：

```cpp
ANY_SET(
    typename type::second_type,
    item.second)
```

因此 packet 中的值不要求与目标参数存储类型完全相同，但必须满足 `ANY_SET` 支持的转换规则。

## variadic templates 实现

`v_tpl` 版本依赖：

```cpp
#include <yggr/type_traits/integer_sequence.hpp>
#include <yggr/support/return_arg.hpp>
```

反向转换通过 `make_index_sequence` 生成索引包：

```cpp
make_index_sequence<sizeof...(Args)>{}
```

再使用参数包展开逐项调用内部函数：

```cpp
prv_s_set_params_detail<I>(params, packet_info)...
```

`support::return_first_arg` 用于保证多个写入操作按表达式展开完成，并返回原始 `Params&`。

## 宏实现

在定义 `YGGR_NO_CXX11_VARIADIC_TEMPLATES` 时，`v_macro` 版本通过 Yggdrasil/Boost.Preprocessor 宏生成多个固定参数数量的重载：

```cpp
BOOST_PP_LOCAL_LIMITS(
    1,
    YGGR_PP_TEMPLATE_PARAMS_LEN())
```

宏版本分别生成：

- 参数到 `packet_info` 的多参数转换函数；
- `packet_info` 到 `Params` 的多参数转换函数；
- 每个 packet 元素的类型、索引和参数赋值表达式。

宏版本在生成接口层面模拟 variadic templates，但可兼容不支持 C++11 参数包的编译器。

## 数据流示意

参数到 packet：

```text
any_val::params
  + key1, key2, ...
  -> get_value<ValueType>(key)
  -> Pair<Key, Value>(key, value)
  -> packet::packet_info<Pair<...>, Pair<...>, ...>
```

packet 到参数：

```text
packet::packet_info
  -> get<index>()
  -> 读取 pair.first / pair.second
  -> ANY_SET 进行目标类型转换
  -> Params[pair.first] = value
```

## 使用示例

参数转 packet：

```cpp
using converter =
    yggr::struct_conv::conv_params_and_packet_info;

packet_type packet = converter::conv<
    std::pair,
    int,
    std::string>(params, key_int, key_string);
```

packet 转参数：

```cpp
params_type params = converter::conv<params_type>(packet);
```

实际 `Pair`、值类型、参数容器和 packet 类型应与项目中对应的 `any_val::params` 与 `packet_info` 定义匹配。

## 使用限制与注意事项

- detail 文件必须通过 `conv_params_and_packet_info.hpp` 间接包含，不能脱离外层头文件直接使用。
- 参数到 packet 的 key 顺序决定 `packet_info` 中元素的顺序。
- `get_value<ValueType>` 的值类型必须与参数对象支持的类型一致。
- packet 到参数的写回依赖 `ANY_SET`，转换失败行为由 any-value 类型系统决定。
- 空参数或空 packet 会返回默认构造对象，不会自动复制其他内容。
- `YGGR_NO_CXX11_VARIADIC_TEMPLATES` 只改变实现方式，不改变公开转换语义。
