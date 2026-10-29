# `sao_buffer.hpp` 中文说明

## 用途

本头文件定义 `yggr::utility::sao_buffer<T, MinBufSize>`，一种按需容量的连续缓冲区。它在请求元素数不超过 `MinBufSize` 时使用对象内部的固定数组存储；请求数更大时则分配堆数组。类型不可复制、不可移动。

```cpp
#include <yggr/utility/sao_buffer.hpp>

yggr::sao_buffer<int> buffer(64);
```

模板参数：

- `T`：缓冲区元素类型。
- `MinBufSize`：内部缓冲区的元素容量，默认为 `1024`。

## 构造与存储策略

- `sao_buffer(req_size)`：创建指定长度的缓冲区。元素按底层数组的默认初始化方式构造。
- `sao_buffer(req_size, value)`：创建指定长度的缓冲区，并将每个元素填充为 `value` 转换得到的 `T`。
- 请求长度不超过 `MinBufSize` 时，使用内部 `container::array`；超过时使用 `new T[req_size]` 分配动态数组。
- 析构时仅在使用动态数组时执行 `delete[]`。

构造要求 `req_size` 大于零。代码通过断言检查该条件；断言关闭时，初始化逻辑会将零长度调整为 `1`。

## 主要接口

| 接口 | 说明 |
|---|---|
| `begin()` / `end()` | 返回首元素和尾后位置，支持可写迭代。 |
| `cbegin()` / `cend()` | const 成员版本的首尾迭代器接口。 |
| `rbegin()` / `rend()` | 反向迭代器范围。 |
| `crbegin()` / `crend()` | const 成员版本的反向迭代器范围。 |
| `buffer_pointer()` | 返回底层缓冲区指针。 |
| `buffer_size()` | 返回请求的元素数。 |
| `buffer_byte_size()` | 使用 `yggr::bytes::byte_size` 计算缓冲区范围的字节数。 |
| `is_optimized()` | 若当前使用内部固定数组则返回 `true`。 |
| `min_buffer_size()` | 返回模板参数指定的内部容量。 |

示例：

```cpp
yggr::sao_buffer<int, 16> buffer(8, 7);

for (yggr::sao_buffer<int, 16>::iterator it = buffer.begin();
     it != buffer.end(); ++it) {
    // 每个元素初始值为 7
}

bool uses_internal_storage = buffer.is_optimized();
```

## 生命周期与类型注意事项

- 缓冲区拥有其元素存储；对象销毁后，指针、迭代器及其引用均失效。
- 内部数组的容量由模板参数决定，但 `buffer_size()` 和迭代器范围反映的是请求长度，而非内部数组的全部容量。
- 本头文件将 `const_pointer` 定义为对 `pointer` 添加顶层 `const`，而不是 `const T*`。因此不要仅凭 const 成员函数或 `const_iterator` 名称推断元素不可修改；使用前应确认此类型别名在当前接口中的实际效果。
- `T` 必须满足数组存储所需的构造、赋值及析构要求；带初值构造还要求可由 `Val` 构造 `T`，且 `T` 可被填充赋值。
- 分配较大缓冲区时，堆分配失败的处理遵循项目的 `yggr_nothrow_new` 配置。
