# `time_cast.hpp` 中文说明

## 用途

本头文件提供时间类型转换函数 `yggr::time::time_cast<Dst>(src)`。对于不同类型的时间值，默认通过 `boost::xtime` 作为中间类型完成转换；源类型和目标类型相同时，则直接返回源对象的常量引用。

```cpp
#include <yggr/time/time_cast.hpp>
```

## 转换辅助模板

```cpp
template<typename Dst, typename Src>
struct time_cast_helper;
```

通用实现定义：

- `src_type`：`Src`
- `dst_type` 和 `result_type`：`Dst`

调用辅助对象时，按以下步骤执行：

1. 将源值转换为 `boost::xtime`。
2. 使用 `boost::xtime` 构造目标类型 `Dst`。
3. 返回目标类型值。

因此，参与转换的源类型需能转换为 `boost::xtime`，目标类型需能由 `boost::xtime` 构造。

## 同类型特化

对于 `Dst` 与 `Src` 相同的情况，`time_cast_helper<T, T>` 返回：

```cpp
typedef const T& result_type;
```

调用时直接返回输入对象的常量引用，不执行中间类型转换，也不创建副本。该引用的有效期受源对象生命周期限制。

## 主函数

```cpp
template<typename Dst, typename Src>
typename time_cast_helper<Dst, Src>::result_type
time_cast(const Src& src);
```

通过模板参数指定目标类型：

```cpp
auto converted = yggr::time::time_cast<target_time_type>(source_time);
```

返回类型由 `time_cast_helper<Dst, Src>::result_type` 决定：不同类型时返回 `Dst` 值；同类型时返回 `const Dst&`。

## 转换示意

```cpp
source_time_type source = /* 源时间值 */;
target_time_type target =
    yggr::time::time_cast<target_time_type>(source);
```

源、目标类型及其与 `boost::xtime` 的互转能力由所使用的时间类型定义。该函数不进行时区、历法或精度策略选择，只通过中间类型执行类型转换。
