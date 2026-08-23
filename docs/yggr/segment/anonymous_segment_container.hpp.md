# `anonymous_segment_container.hpp`

## 概述

该头文件定义了 `yggr::segment::segment_container<Seg>`，用于封装一个匿名内存段对象，并通过 `anonymous_segment_op_proxy<Seg>` 管理该段的创建、查询和释放。

匿名段不具有命名对象语义，因此 `is_named_object()` 始终返回 `false`。

## 头文件

```cpp
#include <yggr/segment/anonymous_segment_container.hpp>
```

## 命名空间

```cpp
yggr::segment
```

## 类声明

```cpp
template<typename Seg>
class segment_container;
```

### 模板参数

| 参数 | 说明 |
| --- | --- |
| `Seg` | 被封装的段类型。该类型需要满足 `anonymous_segment_op_proxy<Seg>` 所要求的操作，并支持移动构造和移动赋值。 |

类型别名：

```cpp
typedef Seg seg_type;
```

## 生命周期与所有权

- 默认构造及带初始化参数的构造函数会通过操作代理创建匿名段。
- 从 `Seg` 移动构造时，容器接管传入段对象的状态。
- 容器不可复制，但支持移动构造和移动赋值。
- 析构时通过操作代理释放内部段。
- `free()` 可显式释放内部段。
- `segment()` 返回内部段对象的引用，不会创建副本。

## 构造函数

### 默认构造

```cpp
segment_container(void);
```

创建一个默认状态的匿名段。

### 使用初始化参数构造

```cpp
template<typename... Args>
segment_container(const Args&... args);
```

实际接口通过预处理器生成多个参数数量的重载。参数会转发给
`anonymous_segment_op_proxy<Seg>::create()`，用于按指定参数创建匿名段。

### 从段对象移动构造

```cpp
segment_container(BOOST_RV_REF(seg_type) seg);
```

使用已有的 `Seg` 对象移动构造容器。

### 移动构造

```cpp
segment_container(BOOST_RV_REF(this_type) right);
```

从另一个 `segment_container` 移动构造当前对象。

## 成员函数

### `segment`

```cpp
const seg_type& segment(void) const;
seg_type& segment(void);
```

返回内部段对象的只读或可修改引用。调用者可以通过返回的引用访问底层 `Seg`。

### `size`

```cpp
size_type size(void) const;
```

通过 `anonymous_segment_op_proxy` 查询当前匿名段的大小。

### `empty`

```cpp
bool empty(void) const;
```

判断当前匿名段是否为空。

### `free`

```cpp
void free(void);
```

通过 `anonymous_segment_op_proxy` 显式释放当前匿名段资源。

### `is_named_object`

```cpp
bool is_named_object(void) const;
```

返回 `false`，表示该容器管理的是匿名段，而不是命名对象。

### 移动赋值

```cpp
this_type& operator=(BOOST_RV_REF(this_type) right);
```

将另一个容器中的段状态移动到当前对象。对自身赋值时不执行任何操作。

## 使用示例

```cpp
#include <yggr/segment/anonymous_segment_container.hpp>

void use_segment()
{
    typedef yggr::segment::segment_container<my_segment> container_type;

    container_type container;

    if(!container.empty())
    {
        my_segment& segment = container.segment();
        // 使用 segment
    }

    size_type bytes = container.size();
    (void)bytes;
}
```

其中 `my_segment` 必须是项目中可由 `anonymous_segment_op_proxy` 操作的有效段类型。

## 注意事项

1. 该类继承 `nonable::noncopyable`，不能进行复制构造或复制赋值。
2. `segment()` 返回的是内部对象引用，引用的有效期受容器生命周期及其后续 `free()` 操作影响。
3. 调用 `free()` 后，继续使用底层段对象前应确认其状态符合对应段类型和操作代理的约定。
4. 具体的段创建、大小计算和释放行为由 `anonymous_segment_op_proxy<Seg>` 决定，而不是由容器自行实现。
5. `BOOST_RV_REF`、`BOOST_COPYABLE_AND_MOVABLE_ALT` 以及参数数量相关宏来自项目的 Boost/预处理器兼容层，使用时应保持项目既有编译配置。

## 相关组件

- `yggr/segment/segment_op_proxy.hpp`
- `yggr/segment/segment_config.hpp`
- `yggr/nonable/noncopyable.hpp`
