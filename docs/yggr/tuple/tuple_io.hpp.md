# `tuple_io.hpp` 中文说明

## 用途

本头文件为 Yggdrasil Tuple 的链式 `cons` 类型提供输入/输出流运算符，并在支持标准 tuple 时为 `std::tuple` 提供相应运算符。格式控制与 Boost.Tuple 的 I/O 操纵器兼容。

```cpp
#include <yggr/tuple/tuple_io.hpp>
```

## 输出格式

tuple 默认以括号包围元素，并使用空格分隔，例如：

```text
(first second third)
```

格式中的左括号、右括号和分隔符可通过以下操纵器修改：

| 操纵器 | 作用 |
| --- | --- |
| `set_open(ch)` | 设置起始括号字符。 |
| `set_close(ch)` | 设置结束括号字符。 |
| `set_delimiter(ch)` | 设置元素分隔字符。 |

该头文件将 `tuple_manipulator`、`operator<<` 和上述操纵器从 `boost::tuples` 引入 `yggr::tuples`，可用于配置流的 tuple 格式。

## 输入格式

输入运算符按同一组括号和分隔符读取元素。读取时依次检查开括号、各元素之间的分隔符以及闭括号；如果流状态不正常，运算符直接返回该流。

元素自身必须支持相应字符流的 `operator>>` 或 `operator<<`。格式符不匹配或元素读取失败会反映在流状态中，调用方应检查 `fail()` / `good()`。

## 支持的类型

- Yggdrasil Tuple 的 `cons` / `null_type` 链。
- 在未定义 `YGGR_NO_CXX11_HDR_TUPLE` 时，`std::tuple<Args...>`。
- Boost.Tuple 的操纵器与内部格式辅助函数被复用；本文件定义的主要 I/O 运算符针对 `yggr::tuples` 中的 `cons` 以及标准 tuple。

对于 `std::tuple`，实现通过索引递归打印或读取每个元素；在配置不支持模板化流或部分模板特化时，代码提供相应兼容分支。

## 使用示例

```cpp
#include <sstream>
#include <yggr/tuple/tuple.hpp>
#include <yggr/tuple/tuple_io.hpp>

std::tuple<int, int> value(3, 8);
std::ostringstream out;
out << value; // 默认格式：(3 8)

std::tuple<int, int> parsed;
std::istringstream in("(3 8)");
in >> parsed;
if(!in)
{
    // 处理格式错误或元素读取失败
}
```

## 注意事项

- 输入和输出必须使用一致的括号与分隔符设置，否则输入解析可能失败。
- 自定义元素类型需实现相应字符类型下的流操作符；不支持流操作的元素无法直接进行 tuple I/O。
- `std::tuple` 的流运算符仅在配置启用标准 tuple 支持时可用。
- 该实现使用标准流状态报告读取/写入错误；它不会额外返回解析成功标志。
