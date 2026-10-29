# `timer_object_container.hpp` 中文说明

## 用途

本头文件定义内部模板容器 `yggr::time::detail::timer_object_container<Val>`，使用 `yggr::list<Val>` 保存定时任务对象、任务 ID 等值，并提供插入、删除、搬移及回调访问接口。它通常配合 `timer_mgr` 中的安全缓冲对象使用，不是独立的线程安全容器。

```cpp
#include <yggr/time/detail/timer_object_container.hpp>
```

## 模板参数与类型

| 名称 | 含义 |
| --- | --- |
| `Val` / `value_type` | 列表中存放的值类型。 |
| `list_type` | 底层容器类型 `yggr::list<value_type>`。 |
| `basic_args_holder_nv_type` | 用于将回调引用包装并传递给类型擦除访问接口的参数持有类型。 |

## 构造、赋值与交换

容器支持默认构造、复制和移动构造、复制和移动赋值，以及 `swap()`。复制操作会复制底层列表；移动操作会移动列表内容。`swap()` 交换两个容器的列表。

```cpp
timer_object_container<value_type> values;
values.insert(value);
```

## 元素操作

| 接口 | 说明 |
| --- | --- |
| `insert(const value_type&)` | 将值复制到列表尾部。 |
| `insert(BOOST_RV_REF(value_type))` | 将值移动到列表尾部。 |
| `remove(cmp)` | 用 `cmp` 作为谓词执行 `remove_if`，返回删除的元素数。 |
| `clear()` | 清空列表。 |
| `size()` | 返回当前元素数。 |
| `empty()` | 判断列表是否为空。 |
| `splice_to(right)` | 将本容器的元素转移到 `right`，最终本容器为空。 |

两个 `insert()` 重载都会先判断值的真假；当 `value_type` 可进行布尔判断且值为假时，不会插入。该行为适用于任务共享指针等可判空值类型。

## 回调访问

```cpp
template<typename Handler>
void get(const Handler& handler);

template<typename Handler>
void get_of_any_handler(const basic_args_holder_nv_type& any_handler);
```

- `get(handler)` 直接将底层列表以可变引用形式传给回调，由回调决定如何读取或修改列表。
- `get_of_any_handler<Handler>(any_handler)` 从参数持有对象中恢复指定的 `Handler` 类型，并以底层列表调用该处理器。类型恢复失败时会触发断言；断言关闭时不调用处理器。

`get()` 和 `get_of_any_handler()` 本身不加锁。并发保护应由调用方或外层 `safe_buffered_object` 等安全包装提供；直接并发访问同一个容器可能产生数据竞争。

## 注意事项

- 这是 `detail` 命名空间中的实现容器；通常由 `timer_mgr` 管理，而不建议应用代码直接依赖。
- `remove()` 的谓词应能接受 `value_type` 并返回可转换为 `bool` 的结果。
- `splice_to()` 的实现将本列表的全部元素拼接到目标列表末尾，再交换两个列表，因此调用完成后原容器为空。
- 插入时对值执行布尔判断，限制了适用的 `Val` 类型；若 `Val` 不支持该判断，该接口无法实例化。
