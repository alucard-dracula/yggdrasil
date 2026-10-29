# `operator_result.hpp` 中文说明

## 用途

本头文件定义编译期类型特征，用于取得一元或二元运算表达式的结果类型。它通过 `YGGR_TYPEOF_TPL` 推导表达式类型，并以嵌套类型 `type` 暴露结果。

```cpp
#include <yggr/type_traits/operator_result.hpp>
```

## 提供的类型特征

| 特征 | 对应表达式 |
| --- | --- |
| `positive_operator_result<T>` | `+T` |
| `negate_operator_result<T>` | `-T` |
| `plus_operator_result<L, R>` | `L + R` |
| `minus_operator_result<L, R>` | `L - R` |
| `multiplies_operator_result<L, R>` | `L * R` |
| `divides_operator_result<L, R>` | `L / R` |
| `modulus_operator_result<L, R>` | 见下方源码注意事项 |
| `bitwise_and_operator_result<L, R>` | `L & R` |
| `bitwise_or_operator_result<L, R>` | `L \| R` |
| `bitwise_xor_operator_result<L, R>` | `L ^ R` |
| `bitwise_left_shift_operator_result<L, R>` | `L << R` |
| `bitwise_right_shift_operator_result<L, R>` | `L >> R` |
| `bitwise_not_operator_result<T>` | `~T` |

这些特征在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/operator_result.hpp>

typedef yggr::plus_operator_result<int, double>::type sum_type;
typedef yggr::bitwise_not_operator_result<unsigned int>::type inverted_type;
```

`sum_type` 与表达式 `int + double` 的类型相同，`inverted_type` 与 `~unsigned int` 的结果类型相同。

## 实现说明

- 内部 `operator_result_t_getter<T>` 提供到 `T` 的转换运算符，辅助编译器在运算表达式中推导结果类型。
- 二元和一元特征通过预处理器宏生成，底层实际运算符由 `yggr/ppex/symbols.hpp` 等定义。
- MSVC 下针对部分算术运算的警告在展开期间暂时关闭，结束后恢复。

## 注意事项

- 对应运算符必须能应用于给定类型；否则在实例化结果特征时会产生编译错误。
- 返回的 `type` 是表达式类型，不保证一定是普通值类型；例如运算符重载可能返回引用或代理类型。
- **取模特征存在实现与注释不一致：**源码中的 `modulus_operator_result` 虽以 `%` 为注释，但实际传入的是除法运算符宏 `/`。因此它目前推导的是除法表达式结果类型，不应依赖它获取取模表达式的类型。
- 这些特征只推导类型，不会执行运算或检查运行时数值。
