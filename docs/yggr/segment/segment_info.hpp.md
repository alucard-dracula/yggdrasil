# `segment_info.hpp`

## 概述

该头文件定义了 `yggr::segment::segment_info`，用于查询与段映射相关的系统信息。

当前提供的唯一信息是系统内存页大小，具体值由 Boost.Interprocess 根据当前平台返回。

## 头文件

```cpp
#include <yggr/segment/segment_info.hpp>
```

## 命名空间

```cpp
yggr::segment
```

## 类型声明

```cpp
struct segment_info;
```

`segment_info` 是无状态工具结构，不包含数据成员，也不需要保存实例状态。

## 静态接口

### `s_get_page_size`

```cpp
static std::size_t s_get_page_size(void);
```

返回当前系统的内存页大小，内部调用：

```cpp
boost::interprocess::mapped_region::get_page_size();
```

返回值单位为字节。

## 使用示例

```cpp
#include <yggr/segment/segment_info.hpp>

void query_page_size()
{
    const std::size_t page_size =
        yggr::segment::segment_info::s_get_page_size();

    (void)page_size;
}
```

## 注意事项

1. `segment_info` 是工具结构，通常直接调用静态函数，不需要实例化。
2. 返回值是当前平台的系统内存页大小，单位为字节。
3. 返回值可能因操作系统、硬件架构或运行环境而不同。
4. 具体查询行为由 Boost.Interprocess 的 `mapped_region` 实现决定。

## 相关组件

- `yggr/base/yggrdef.h`
- `boost/interprocess/mapped_region.hpp`
