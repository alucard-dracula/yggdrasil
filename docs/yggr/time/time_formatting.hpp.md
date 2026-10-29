# `time_formatting.hpp` 中文说明

## 用途

本头文件提供基于 `boost::xtime` 的 UTC 和本地时间格式化接口，支持将格式化结果写入字符流或 Yggdrasil 字符串对象。

```cpp
#include <yggr/time/time_formatting.hpp>
```

格式说明遵循 Boost.DateTime `time_facet` 规则；默认格式由 `time_format::tm_fmt_default<Char>()` 提供。

## 流格式化接口

```cpp
template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>&
time_formatting_utc(
    std::basic_ostream<Char, Traits>& os,
    const Char* format,
    const boost::xtime& tm);

template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>&
time_formatting_local(
    std::basic_ostream<Char, Traits>& os,
    const Char* format,
    const boost::xtime& tm);
```

- `time_formatting_utc()` 按 UTC 时间格式化。
- `time_formatting_local()` 先将 UTC 时间转换为本地时间，再格式化。
- `format` 为空指针时使用对应字符类型的默认时间格式。
- 成功时返回原输出流引用，便于继续链式写入。

除 `char` 外，`wchar_t`、`c8`、`c16`、`c32` 的重载受项目对应的 `YGGR_NO_*` 配置宏控制。

函数会为格式创建 Boost.DateTime `time_facet` 并临时安装到输出流 locale，写入时间后恢复原 locale。若 facet 创建失败，调试构建会触发断言；断言关闭时返回未完成格式化的原流。

## 字符串格式化接口

```cpp
template<typename String, typename Fmt>
String& time_formatting_utc(
    String& out,
    const Fmt& format,
    const boost::xtime& tm,
    const yggr::string& ret_charset_name,
    const yggr::string& fmt_charset_name);

template<typename String, typename Fmt>
String& time_formatting_local(
    String& out,
    const Fmt& format,
    const boost::xtime& tm,
    const yggr::string& ret_charset_name,
    const yggr::string& fmt_charset_name);
```

实际接口为模板，并为字符集名称提供默认值：

- `out` 必须是 Yggdrasil 字符串类型。
- `format` 必须是广义字符串类型。
- `ret_charset_name` 指定输出字符串的字符集，默认取自 `String`。
- `fmt_charset_name` 指定格式字符串的字符集，默认取自 `Fmt`。

函数先将格式字符串转换为相应的标准字符串，再通过字符串流执行格式化，最后转换为输出字符串的字符集，并更新字符串长度信息。空格式字符串会转成空指针传给流接口，从而使用默认格式。

另有 `int format` 的兼容重载；当前实现通过断言要求 `format == 0`，并按默认格式处理。

## 使用示例

### 写入流

```cpp
boost::xtime tm = /* 时间值 */;
std::ostringstream out;

yggr::time::time_formatting_utc(
    out,
    "%Y-%m-%d %H:%M:%S",
    tm);
```

### 写入 Yggdrasil 字符串

```cpp
boost::xtime tm = /* 时间值 */;
yggr::string out;

yggr::time::time_formatting_local(
    out,
    "%Y-%m-%d %H:%M:%S",
    tm);
```

## 错误与注意事项

- 字符串转换期间发生字符集错误或其他异常时，接口通过断言报告；转换格式失败时通常返回原有 `out`，输出转换失败时也会保留已有内容（转换器的具体修改行为由其实现决定）。
- 格式标记遵循 Boost.DateTime `time_facet`，并非完整的 `strftime` 语法。
- UTC 和本地格式化接口都以 `boost::xtime` 作为输入；本地格式化会执行 UTC 到本地时间的调整。
- 调用方应确保时间值有效，并按目标平台的字符集和字符类型选择兼容的格式字符串。
