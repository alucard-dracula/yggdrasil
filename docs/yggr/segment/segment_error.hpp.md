# `segment_error.hpp`

## 概述

该头文件为 Boost.Interprocess 的进程间通信异常提供项目统一名称：

```cpp
typedef boost::interprocess::interprocess_exception segment_error;
```

因此，`yggr::segment::segment_error` 并不是新的异常类，而是
`boost::interprocess::interprocess_exception` 的类型别名。

## 头文件

```cpp
#include <yggr/segment/segment_error.hpp>
```

## 命名空间

```cpp
yggr::segment
```

## 类型定义

```cpp
typedef boost::interprocess::interprocess_exception segment_error;
```

### 异常来源

底层异常类型定义在：

```cpp
#include <boost/interprocess/exceptions.hpp>
```

`segment_error` 继承关系、构造函数和成员函数均遵循当前 Boost.Interprocess 版本中
`interprocess_exception` 的定义。

## 常用接口

由于 `segment_error` 是类型别名，可以使用 `interprocess_exception` 提供的接口。常见接口包括：

```cpp
const char* what() const noexcept;
error_code_t get_error_code() const;
```

其中：

- `what()`：返回异常的描述文本；
- `get_error_code()`：返回 Boost.Interprocess 错误码，可用于项目统一异常转换或错误分支处理。

具体返回类型和异常规范以当前 Boost.Interprocess 版本为准。

## 使用示例

```cpp
#include <yggr/segment/segment_error.hpp>

void handle_segment_error()
{
    try
    {
        // 执行可能抛出 Boost.Interprocess 异常的操作
    }
    catch(const yggr::segment::segment_error& error)
    {
        const char* message = error.what();
        const auto code = error.get_error_code();

        (void)message;
        (void)code;
    }
}
```

也可以直接捕获底层类型；两者实际是同一类型：

```cpp
catch(const boost::interprocess::interprocess_exception& error)
{
    // 与捕获 segment_error 等价
}
```

## 在段组件中的用途

段操作代理通常使用该别名捕获 Boost.Interprocess 异常：

```cpp
try
{
    // 创建、打开或映射段
}
catch(const segment_error& error)
{
    // 执行清理、重试或转换为项目异常
}
```

典型处理流程包括：

1. 捕获首次创建或打开失败；
2. 调用错误修复器清理临时状态；
3. 重试一次操作；
4. 再次失败时读取 `get_error_code()` 和 `what()`，转换为项目异常。

## 注意事项

1. `segment_error` 只是别名，不改变底层异常的行为或 ABI。
2. 捕获该异常时应使用 `const segment_error&`，避免不必要的复制并保留多态异常信息。
3. 不同 Boost.Interprocess 版本可能提供不同的错误码类型或异常规范，代码应以实际依赖版本为准。
4. 该别名主要用于统一 yggdrasil 段相关代码的异常书写方式。
5. `segment_error` 只覆盖 Boost.Interprocess 异常；文件系统异常、标准库异常或项目自定义异常仍需按对应类型处理。

## 相关组件

- `yggr/segment/segment_op_proxy.hpp`
- `yggr/segment/segment_destroyer.hpp`
- `yggr/segment/segment_container.hpp`
- `boost/interprocess/exceptions.hpp`
