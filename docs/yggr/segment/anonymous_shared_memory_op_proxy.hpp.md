# `anonymous_shared_memory_op_proxy.hpp`

## 概述

该头文件为 `yggr::segment::anonymous_shared_memory` 提供
`yggr::segment::segment_op_proxy` 特化，实现匿名共享内存段的创建、释放、大小查询和空状态判断。

该特化继承 `boost::mpl::bool_<false>`，因此匿名共享内存不是命名对象。

## 包含方式

该文件要求在包含前已定义 `YGGR_SEGMENT_OPERATOR_INCLUDE`。通常应包含统一入口：

```cpp
#include <yggr/segment/segment_op_proxy.hpp>
```

不要把该文件作为独立的底层实现头直接使用，否则可能触发预处理器错误：

```text
include error managed_mapped_file_op_proxy.hpp please include segment_op_proxy.hpp.
```

## 命名空间

```cpp
yggr::segment
```

## 类声明

```cpp
template<>
class segment_op_proxy<anonymous_shared_memory>;
```

类型别名：

```cpp
typedef anonymous_shared_memory seg_type;
typedef boost::mpl::bool_<false> base_type;
typedef base_type is_named_object_type;
```

静态常量 `E_is_named_object` 的值为 `false`。

## 公共接口

### `s_create`

```cpp
static bool s_create(seg_type& seg);
```

将 `seg` 交换为空的匿名共享内存段，返回 `true`。该重载用于创建默认状态的段。

```cpp
template<typename EArg, typename ...Args>
static bool s_create(seg_type& seg,
                     BOOST_FWD_REF(EArg) earg,
                     BOOST_FWD_REF(Args)... args);
```

接收创建参数，并转发到内部创建逻辑。具体生成的重载数量取决于项目是否启用 C++11 可变参数模板；未启用时由预处理器生成对应参数数量的重载。

常用参数形式包括：

```cpp
static bool s_create(seg_type& seg, std::size_t size);
static bool s_create(seg_type& seg, std::size_t size, void* addr);
```

其中：

- `size`：要申请的共享内存大小，不能为零。
- `addr`：可选的映射地址，传入 `nullptr` 或省略时由 Boost.Interprocess 选择地址。

### `s_remove`

```cpp
static void s_remove(seg_type& seg);
```

释放当前段。实现通过重新执行无参 `s_create(seg)`，将段交换为空状态。

### `s_seg_is_empty`

```cpp
static bool s_seg_is_empty(const seg_type& seg);
```

当 `s_seg_size(seg)` 为零时返回 `true`，否则返回 `false`。

### `s_seg_size`

```cpp
static size_type s_seg_size(const seg_type& seg);
```

返回 `seg.get_size()`，即当前匿名共享内存段的大小。

### `s_is_named_enable_object`

```cpp
static bool s_is_named_enable_object(void);
```

返回 `false`，表示该段不支持命名对象语义。

## 创建流程与错误处理

内部创建逻辑由 `s_prv_create_detail` 完成：

1. 当 `size == 0` 时立即返回 `false`。
2. 调用 `boost::interprocess::anonymous_shared_memory(size, addr)` 创建段，并与 `seg` 交换。
3. 创建成功后，以 `seg.get_size()` 是否非零作为结果。
4. 首次捕获 `segment_error` 时调用错误修复处理器，清理当前段后重试一次。
5. 若重试仍然抛出 `segment_error`，则通过 `exception::exception::throw_error()` 抛出错误。

默认错误修复器会调用 `s_remove(seg)`，确保失败后的段状态被清理。

## 使用示例

```cpp
#include <yggr/segment/segment_op_proxy.hpp>

void create_anonymous_memory()
{
    typedef yggr::segment::anonymous_shared_memory segment_type;
    typedef yggr::segment::segment_op_proxy<segment_type> proxy_type;

    segment_type segment;
    const std::size_t size = 4096;

    if(proxy_type::s_create(segment, size))
    {
        const yggr::size_type actual_size = proxy_type::s_seg_size(segment);
        (void)actual_size;
    }

    proxy_type::s_remove(segment);
}
```

## 注意事项

1. `size` 为零时不会创建段，`s_create` 返回 `false`。
2. 该代理管理的是匿名共享内存，不提供命名对象查找或命名对象创建能力。
3. 创建失败时可能先清理已有段，再进行一次重试；重试失败会抛出项目异常，而不只是返回 `false`。
4. `seg_type` 的具体行为来自 `anonymous_shared_memory` 和 Boost.Interprocess，使用时应遵循这两者的生命周期约束。
5. `s_remove` 会修改传入的段对象；调用后不要继续依赖释放前的映射状态或地址。
6. `YGGR_SEGMENT_OPERATOR_INCLUDE` 由统一代理头设置，推荐始终包含 `segment_op_proxy.hpp`，不要直接依赖该实现文件。

## 相关组件

- `yggr/segment/segment_op_proxy.hpp`
- `yggr/segment/anonymous_shared_memory.hpp`
- `yggr/segment/segment_error.hpp`
- `boost/interprocess/anonymous_shared_memory.hpp`
