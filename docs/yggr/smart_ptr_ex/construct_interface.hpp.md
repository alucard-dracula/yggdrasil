# `construct_interface.hpp`

## 概述

该文件为接口类型构造提供统一的共享指针包装入口。它由外层公开头和 detail 实现组成：

- 外层 `construct_interface.hpp`：包含 detail 实现，并将函数导出到公开命名空间；
- `detail/construct_interface.hpp`：构造 `interface_ptr<T, SPtr>`，并将异常转换为空接口指针；
- `detail/construct_shared_boost.hpp` 和 `detail/construct_shared_stl.hpp`：为 `interface_ptr` 的原始指针构造路径提供 Boost/STL 共享指针创建能力。

该接口适合使用具体实现类型构造对象，并通过 `T` 作为对外接口类型返回共享指针包装。

## 头文件

```cpp
#include <yggr/smart_ptr_ex/construct_interface.hpp>
```

外层文件内部包含：

```cpp
#include <yggr/smart_ptr_ex/detail/construct_interface.hpp>
```

## 公开名称导出

外层文件将 detail 函数引入：

```cpp
namespace yggr::smart_ptr_ex
{
using detail::construct_interface;
}
```

随后再导出到 `yggr` 根命名空间：

```cpp
using smart_ptr_ex::construct_interface;
```

因此以下调用形式都可用：

```cpp
yggr::smart_ptr_ex::construct_interface<T, SPtr>(args...);
yggr::construct_interface<T, SPtr>(args...);
```

外层文件不包含具体构造逻辑，实际行为由 detail 文件提供。

## `construct_interface`

核心接口为：

```cpp
template<
    typename T,
    template<typename _T> class SPtr,
    typename ...Args>
inline interface_ptr<T, SPtr>
construct_interface(BOOST_FWD_REF(Args)... args);
```

模板参数含义如下：

| 参数 | 含义 |
| --- | --- |
| `T` | 对外暴露的接口、基类或视图类型 |
| `SPtr` | 共享指针模板，例如 `boost::shared_ptr` 或 `std::shared_ptr` |
| `Args...` | 传给 `interface_ptr` 构造函数的参数 |

返回类型为：

```cpp
interface_ptr<T, SPtr>
```

`interface_ptr` 是基于所选共享指针模板的接口包装，具体所有权、别名指针和转换行为由 `detail/interface_ptr.hpp` 定义。

## 构造流程

在支持 variadic templates 时，函数核心逻辑为：

```cpp
typedef interface_ptr<T, SPtr> ret_type;

try
{
    return ret_type(
        boost::forward<Args>(args)...);
}
catch(...)
{
    return ret_type();
}
```

处理步骤如下：

1. 确定返回类型 `interface_ptr<T, SPtr>`；
2. 完美转发所有构造参数；
3. 调用 `interface_ptr` 的对应构造函数；
4. 构造成功时返回接口指针；
5. 任意异常发生时返回默认构造的空接口指针。

该函数本身不直接调用 `boost::make_shared` 或 `std::make_shared`，但当传入参数匹配 `interface_ptr` 的原始指针构造路径时，`interface_ptr` 内部会通过 `construct_shared` 创建共享对象。

## 与 `construct_shared` 的关系

`detail/interface_ptr.hpp` 的部分构造函数使用：

```cpp
construct_shared<obj_type>(...);
```

`construct_shared.hpp` 再根据宏选择 detail 后端：

```text
construct_interface<T, SPtr>(args...)
  -> interface_ptr<T, SPtr>(args...)
      -> construct_shared<obj_type>(...)
          -> construct_shared_stl / construct_shared_boost
              -> std::shared_ptr 或 boost::shared_ptr
```

### Boost 后端

`detail/construct_shared_boost.hpp` 中的：

```cpp
construct_shared_boost<T>(args...)
```

通过共享指针构造：

```cpp
boost::shared_ptr<T>(
    boost::forward<Args>(args)...)
```

如果构造抛出异常，则返回空的 `boost::shared_ptr<T>`。

### STL 后端

当未定义 `YGGR_NO_CXX11_SMART_PTR` 时，`detail/construct_shared_stl.hpp` 提供：

```cpp
construct_shared_stl<T>(args...)
```

通过：

```cpp
std::shared_ptr<T>(
    boost::forward<Args>(args)...)
```

创建对象，异常时返回空的 `std::shared_ptr<T>`。

## 智能指针选择

`construct_shared.hpp` 的公开 `construct_shared` 根据以下条件选择返回类型：

```cpp
!defined(YGGR_NO_CXX11_SMART_PTR)
&& defined(YGGR_SMART_PTR_USING_STL)
```

满足时使用：

```cpp
std::shared_ptr<T>
```

否则使用：

```cpp
boost::shared_ptr<T>
```

因此 `construct_interface<T, SPtr>` 可以显式指定 `SPtr`，而 `interface_ptr` 内部通过 `construct_shared` 构造对象时，则遵循项目的默认智能指针选择。

## 异常行为

`construct_interface` 在接口指针构造阶段捕获所有异常：

```cpp
catch(...)
{
    return ret_type();
}
```

底层 `construct_shared_boost` 和 `construct_shared_stl` 也分别捕获对象构造异常并返回空共享指针。调用方应检查返回值是否为空，而不应依赖异常继续向外传播。

## 旧编译器兼容

当定义以下宏时：

```cpp
YGGR_NO_CXX11_VARIADIC_TEMPLATES
```

detail 文件通过 Boost.Preprocessor 生成固定参数数量的重载：

```cpp
BOOST_PP_LOCAL_LIMITS(
    0,
    YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
```

这些重载仍然保持参数转发和异常转空指针语义，但最大参数数量由 `YGGR_PP_FOO_DEFAULT_PARAMS_LEN` 决定。

## 典型调用形式

使用 Boost.SharedPtr 作为接口包装：

```cpp
auto p = yggr::smart_ptr_ex::construct_interface<
    Interface,
    boost::shared_ptr>(constructor_args...);
```

使用标准库 SharedPtr：

```cpp
auto p = yggr::smart_ptr_ex::construct_interface<
    Interface,
    std::shared_ptr>(constructor_args...);
```

这里的 `Interface` 应与具体构造对象存在可用的指针转换关系，传入参数则必须匹配 `interface_ptr` 支持的构造形式。

## 使用限制与注意事项

- `SPtr` 必须是接受一个类型模板参数的共享指针模板。
- 传入参数必须匹配 `interface_ptr<T, SPtr>` 的构造函数；原始指针路径还需要满足底层共享指针构造要求。
- 构造失败会返回空 `interface_ptr`，调用方必须检查结果。
- `YGGR_NO_CXX11_SMART_PTR` 定义时不会提供 STL 后端。
- 非 variadic templates 模式下，参数数量不能超过 `YGGR_PP_FOO_DEFAULT_PARAMS_LEN`。
- 修改 `interface_ptr`、`construct_shared` 或默认智能指针选择时，应同步检查本接口的返回类型和构造链。
