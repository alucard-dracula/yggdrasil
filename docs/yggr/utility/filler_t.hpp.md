# `filler_t.hpp` 中文说明

## 用途

本头文件定义 `yggr::utility::filler_t<s>`，一个带有指定长度字节数组的填充结构体。构造时会将数组内容全部初始化为零。

```cpp
#include <yggr/utility/filler_t.hpp>
```

## 类型与成员

```cpp
template<yggr::u32 s>
struct filler_t;
```

- `filler_t<s>` 继承自 `boost::mpl::integral_c<yggr::u32, s>`，因此可通过 `value` 获取模板参数 `s`。
- 静态常量 `size` 等于 `s`。
- 公有成员 `filler_data` 是长度为 `s` 的 `yggr::u8` 数组。
- 默认构造函数将 `filler_data` 的全部字节设为 `0`。

## 示例

```cpp
#include <yggr/utility/filler_t.hpp>

yggr::utility::filler_t<16> reserved;
static_assert(yggr::utility::filler_t<16>::size == 16,
              "filler size is 16 bytes");
```

构造 `reserved` 后，其 `filler_data` 数组包含 16 个零字节。

## 注意事项

- `s` 同时决定数组长度和对象的填充数据大小；较大的值会增加对象占用空间。
- 该类型只提供并清零字节数组，不负责序列化、对齐保证或存储其他对象的生命周期管理。
- 源码使用 `memset` 清零；实际使用需确保项目包含链中已提供相应声明。
