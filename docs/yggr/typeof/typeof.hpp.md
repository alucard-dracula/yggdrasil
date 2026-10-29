# `typeof.hpp` 中文说明

## 用途

本头文件为 Yggdrasil 提供统一的表达式类型推导和 `auto` 宏接口。根据编译器是否支持 `decltype`，底层使用 C++ `decltype` 或 Boost.Typeof。

```cpp
#include <yggr/typeof/typeof.hpp>
```

## 类型推导宏

| 宏 | 用途 |
| --- | --- |
| `YGGR_TYPEOF(expr)` | 获取表达式类型。 |
| `YGGR_TYPEOF_TPL(expr)` | 在模板上下文中获取表达式类型。 |
| `YGGR_TYPEOF_NESTED_TYPEDEF(name, expr)` | 声明一个结构体，并以嵌套 `type` 暴露表达式类型。 |
| `YGGR_TYPEOF_NESTED_TYPEDEF_TPL` | 模板上下文中的嵌套类型定义形式。 |
| `YGGR_TYPEOF_REGISTER_TYPE(type)` | 为旧版 Boost.Typeof 注册类型；`decltype` 分支中展开为空。 |
| `YGGR_TYPEOF_REGISTER_TEMPLATE(template, params)` | 为旧版 Boost.Typeof 注册模板；`decltype` 分支中展开为空。 |

当定义 `YGGR_NO_CXX11_DECLTYPE` 时，上述接口映射到 Boost.Typeof 宏；否则 `YGGR_TYPEOF` 映射为 `decltype`，相关嵌套类型宏通过 typedef 实现。

## `auto` 宏

- `YGGR_AUTO(Var, Expr)` 展开为 `auto Var = Expr`，或在旧版分支展开为 `BOOST_AUTO`。
- `YGGR_AUTO_TPL` 是模板上下文对应形式；`decltype` 分支中与 `YGGR_AUTO` 相同，旧版分支映射到 `BOOST_AUTO_TPL`。

## 辅助声明

头文件还声明 `yggr::declval<T>()` 和 `yggr::gen_value_t(T)`，供类型推导表达式构造使用。`declval` 的返回类型移除 `T` 的引用限定；这些函数主要用于编译期类型表达式，不应依赖其运行时定义。

## 示例

```cpp
#include <yggr/typeof/typeof.hpp>

int value = 1;
typedef YGGR_TYPEOF(value + 1.0) result_type;
YGGR_TYPEOF_NESTED_TYPEDEF(value_type, value);

YGGR_AUTO(copy, value);
```

## 注意事项

- 两个实现分支的能力和限制由编译器、Boost.Typeof 注册状态及项目配置决定。
- 注释明确指出旧版 `BOOST_TYPEOF` 不支持 `void` 表达式类型；此类场景需使用专门的兼容辅助，例如 `deduce_result.hpp`。
- `YGGR_AUTO` 是宏展开形式，不是独立的类型特征；应在支持该宏对应语法的上下文使用。
