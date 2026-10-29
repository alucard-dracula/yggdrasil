# `task_store_op.hpp` 中文说明

## 用途

本头文件提供任务保存策略，用于把任务转换为底层容器保存的数据，并按键删除数据。策略按容器数据布局分为键值存储、随机访问存储、顺序存储，以及不支持删除的存储形式。

```cpp
#include <yggr/task_center/task_store_op.hpp>
```

这些策略通常作为 `task_saver` 的 `Store_OP` 模板参数，由 `task_saver` 调用 `get_store_data()` 和 `remove()`。

## 策略概览

| 策略 | 存储数据形式 | 删除行为 |
| --- | --- | --- |
| `key_value_store<Store_Val, KeyGetter>` | `(key, task)` 键值对 | 使用容器的 `remove(key)` 删除一个或多个键。 |
| `random_access_store<Store_Val>` | 直接保存任务 | 使用 `std::remove_if` 按任务值或提取键匹配。 |
| `sequenced_store<Store_Val>` | 直接保存任务 | 使用容器的 `remove_if` 按任务值或提取键匹配。 |
| `nonremove_key_value_store<Store_Val, KeyGetter>` | `(key, task)` 键值对 | 删除接口为空操作。 |
| `nonremove_linner_store<Store_Val>` | 直接保存任务 | 删除接口为空操作。 |

## `key_value_store`

```cpp
template<typename Store_Val, typename KeyGetter>
struct key_value_store;
```

要求 `Store_Val` 提供 `first_type` 和 `second_type`：

- `key_type` 是 `Store_Val::first_type`。
- `task_type` 是 `Store_Val::second_type`。
- `key_getter_type` 是 `KeyGetter`。

`get_store_data(task)` 默认构造一个键提取器，调用 `key_getter(task)`，并返回由提取键和任务组成的 `Store_Val`。右值任务重载会移动任务，常量任务重载会复制任务。

删除接口支持单个键和键集合：

```cpp
remove(container, key);
remove(container, keys);
```

它们分别调用容器的 `remove(key)`，因此实际删除语义取决于底层容器的 `remove` 实现。

## `random_access_store`

```cpp
template<typename Store_Val>
struct random_access_store;
```

该策略直接将任务作为存储数据返回。非常量左值重载返回任务引用，常量重载返回常量引用。

删除接口可按任务值或由 `KeyGetter` 从任务提取的键进行匹配，也支持键集合：

```cpp
remove(container, key);
remove(container, keys);
remove(container, key, key_getter);
remove(container, keys, key_getter);
```

比较规则分别是 `task == key`，或 `key_getter(task) == key`。

**实现注意：**这些重载调用 `std::remove_if`，但没有使用返回的新逻辑尾部迭代器调用 `erase()`。对标准随机访问容器而言，这通常只会移动/覆盖匹配范围内的元素，不会缩短容器大小；不要仅凭调用 `remove()` 推断任务已从容器中物理删除。若要求真正擦除，应确认底层容器或相关实现提供了后续擦除操作。

## `sequenced_store`

```cpp
template<typename Store_Val>
struct sequenced_store;
```

该策略也直接保存任务，并提供与 `random_access_store` 相同的按值、按提取键及键集合删除接口。不同之处在于它调用容器自身的 `remove_if()`，是否移除元素及其行为由底层容器定义。

## 不支持删除的策略

### `nonremove_key_value_store`

```cpp
template<typename Store_Val, typename KeyGetter>
struct nonremove_key_value_store;
```

其存储数据结构与 `key_value_store` 相同：根据默认构造的 `KeyGetter` 提取键，并保存 `(key, task)`。但它提供的 `remove()` 重载均为空操作，不会删除容器中的数据。

### `nonremove_linner_store`

```cpp
template<typename Store_Val>
struct nonremove_linner_store;
```

该策略直接保存任务。所有按键或键集合删除重载（包括带 `KeyGetter` 的形式）均为空操作。

使用不支持删除的策略时，调用 `task_saver::drop_task()` 不会从缓冲中移除任务。

## 与 `task_saver` 配合

`task_saver` 推入非空任务时，会先调用策略的 `get_store_data()`，再将返回值放入容器；删除任务时则把请求委托给策略的 `remove()`。因此需要根据容器布局选择匹配策略，并确认其删除语义符合预期。

概念示例：

```cpp
typedef yggr::task_center::random_access_store<my_task>
    my_task_store_op;

// task_saver<Mark, my_task, Container, my_task_store_op>
// 保存任务本身，并由随机访问策略决定删除匹配项的具体行为。
```

## 实现约束

- 键值策略要求 `Store_Val` 是具有 `first_type`、`second_type` 的键值类型，并且 `KeyGetter` 可从任务生成键。
- 直接任务策略的 `Store_Val` 必须能作为任务类型使用；相关比较操作必须满足所选删除重载的要求。
- 多键删除接口接受模板模板参数形式的键集合，并遍历其中的 `const_iterator`。
- `KeyGetter` 在键值存储的 `get_store_data()` 中默认构造；需确保该类型可默认构造并可调用。
