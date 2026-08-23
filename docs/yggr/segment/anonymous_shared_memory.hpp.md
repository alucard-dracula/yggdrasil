# `anonymous_shared_memory.hpp`

## 概述

该头文件定义了 `yggr::segment::anonymous_shared_memory`，用于表示一块匿名共享内存映射区域。

该类继承自 `boost::interprocess::mapped_region`，在保持 Boost.Interprocess 映射区域能力的基础上，补充了项目统一的移动语义、交换操作和类型别名。

匿名共享内存没有名称，不能通过名称进行跨进程查找；通常由创建它的进程或封装容器负责管理其生命周期。

## 头文件

```cpp
#include <yggr/segment/anonymous_shared_memory.hpp>
```

## 命名空间

```cpp
yggr::segment
```

## 类声明

```cpp
class anonymous_shared_memory
    : public boost::interprocess::mapped_region;
```

类型别名：

```cpp
typedef boost::interprocess::mapped_region base_type;
```

## 构造函数

### 默认构造

```cpp
anonymous_shared_memory(void);
```

构造一个空的匿名共享内存映射对象。

### 按大小构造

```cpp
anonymous_shared_memory(std::size_t size, void* address = 0);
```

创建指定大小的匿名共享内存映射。

参数：

| 参数 | 说明 |
| --- | --- |
| `size` | 要创建的映射区域大小。传入零值时不能创建有效区域。 |
| `address` | 可选的映射地址提示。默认值为 `0`，由底层实现选择地址。 |

创建失败时，具体异常行为遵循 `boost::interprocess::anonymous_shared_memory` 和 `mapped_region` 的约定。

### 从基类移动构造

```cpp
anonymous_shared_memory(BOOST_RV_REF(base_type) base);
```

从 `boost::interprocess::mapped_region` 移动构造对象，接管其映射资源。

### 从同类移动构造

```cpp
anonymous_shared_memory(BOOST_RV_REF(this_type) right);
```

从另一个 `anonymous_shared_memory` 移动构造对象。

## 赋值操作

### 从基类移动赋值

```cpp
this_type& operator=(BOOST_RV_REF(base_type) right);
```

将基类映射区域的资源移动到当前对象。自赋值时直接返回当前对象。

### 从同类移动赋值

```cpp
this_type& operator=(BOOST_RV_REF(this_type) right);
```

将另一个匿名共享内存对象的资源移动到当前对象。该类不支持复制赋值。

## 交换操作

### 与基类交换

```cpp
void swap(base_type& base);
void swap(BOOST_RV_REF(base_type) right);
```

与 `boost::interprocess::mapped_region` 对象交换映射资源。

### 与同类交换

```cpp
void swap(this_type& right);
void swap(BOOST_RV_REF(this_type) right);
```

与另一个 `anonymous_shared_memory` 对象交换映射资源。

### 非成员 `swap`

项目提供以下重载：

```cpp
void swap(anonymous_shared_memory::base_type& left,
          anonymous_shared_memory& right);
void swap(anonymous_shared_memory& left,
          anonymous_shared_memory::base_type& right);
void swap(anonymous_shared_memory& left,
          anonymous_shared_memory& right);
```

这些重载位于 `yggr::segment::swap_support`，并通过 `using` 声明导出到 `yggr::segment`、`std` 和 `boost` 命名空间，以便配合通用交换调用使用。

## 继承的接口

以下接口通过 `using base_type::...` 直接暴露：

```cpp
std::size_t get_size() const;
void* get_address() const;
boost::interprocess::mode_t get_mode() const;
void flush();
void shrink_by(std::size_t offset, std::size_t size);
void advise(void* address, std::size_t size, int advice);
```

具体签名和行为以当前 Boost.Interprocess 版本中的 `mapped_region` 定义为准。

常用接口说明：

| 接口 | 用途 |
| --- | --- |
| `get_size()` | 获取映射区域大小。 |
| `get_address()` | 获取映射区域起始地址。 |
| `get_mode()` | 获取映射访问模式。 |
| `flush()` | 将映射区域内容刷新到底层对象。 |
| `shrink_by()` | 缩小映射区域。 |
| `advise()` | 向系统提供映射访问模式建议。 |

## 使用示例

```cpp
#include <yggr/segment/anonymous_shared_memory.hpp>

void use_anonymous_memory()
{
    yggr::segment::anonymous_shared_memory memory(4096);

    void* address = memory.get_address();
    std::size_t size = memory.get_size();

    (void)address;
    (void)size;
}
```

### 移动与交换示例

```cpp
#include <yggr/segment/anonymous_shared_memory.hpp>

void move_memory()
{
    typedef yggr::segment::anonymous_shared_memory memory_type;

    memory_type source(4096);
    memory_type target(boost::move(source));

    memory_type another(8192);
    target.swap(another);
}
```

## 生命周期与所有权

- 该类不可复制，只支持移动构造和移动赋值。
- 移动操作会转移底层映射资源，移动后的源对象不应再被当作有效映射使用，除非底层类型明确允许相应操作。
- 析构时释放底层 `mapped_region` 映射资源。
- 通过 `get_address()` 得到的地址只在对应映射仍然有效时可使用。
- 交换操作会改变对象与底层映射资源的对应关系。

## 注意事项

1. `anonymous_shared_memory` 是匿名映射，不提供名称、名称创建或名称查找接口。
2. 使用 `get_address()` 返回的指针时，应确保对象未被移动、交换、释放或析构。
3. 具体的内存保护模式、刷新、缩小和系统建议行为由 Boost.Interprocess 决定。
4. 该头文件依赖 `segment_config.hpp` 的配置，应保持它作为相关段组件的首要配置包含项。
5. 需要与 `segment_op_proxy<anonymous_shared_memory>` 配合使用时，应通过 `segment_op_proxy.hpp` 引入相应代理接口。

## 相关组件

- `yggr/segment/anonymous_shared_memory_op_proxy.hpp`
- `yggr/segment/segment_op_proxy.hpp`
- `boost/interprocess/anonymous_shared_memory.hpp`
- `boost/interprocess/mapped_region.hpp`
