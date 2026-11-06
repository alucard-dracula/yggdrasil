# `handler_center.hpp` 

## 用途

本头文件提供按键管理和调用处理器的 `handler_center<Key, Mutex>`。它内部使用安全的 unordered map，将 key 映射到 `basic_handler_container`，并通过统一的 `call` 接口查找、校验参数并调用对应处理器。

```cpp
#include <yggr/handler_center/handler_center.hpp>
```

## 主要接口

### 类型与线程配置

```cpp
template<typename Key, typename Mutex = void>
class yggr::handler_center::handler_center;
```

`Key` 是处理器的查找键；`Mutex` 传给底层 `safe_unordered_map`，用于选择容器的同步配置。默认值为 `void`，具体并发行为取决于安全容器对该配置的处理。

### 注册与移除

- `register_handler(key, handler)`：将处理器包装为共享的 `basic_handler_container` 并按 key 插入；返回是否成功插入。key 已存在或容器构造失败时返回 `false`。
- `unregister_handler(key)`：删除指定 key，返回删除是否成功。
- `register_handler_object(obj)` / `unregister_handler_object(obj)`：转调对象的 `register_handler(*this)` / `unregister_handler(*this)`，用于由对象集中管理自身的注册项。
- `clear()`：移除所有注册项。

### 查询与调用

- `empty()`、`size()`：查询注册表是否为空及当前条目数。
- `call(key_anchor, args...)`：按 `key_anchor.get()` 查找处理器并以给定参数调用，适用于无返回值处理器。
- `call(ret, key_anchor, args...)`：调用有返回值的处理器，并将结果写入 `ret`。

`key_anchor` 必须是 `yggr::is_args_anchor_t` 可识别的参数锚点类型。key 不存在、参数或返回值类型与已注册处理器不匹配、或处理器调用容器无法完成调用时，`call` 返回 `false`；成功调用返回 `true`。处理器自身抛出的异常不由此类捕获。

无返回值调用形式不用于丢弃非 `void` 处理器的返回值；如处理器有返回值，应使用带 `ret` 参数的重载。参数和返回值类型应与注册处理器的签名相符。

## 使用示例

```cpp
#include <yggr/handler_center/handler_center.hpp>
#include <yggr/utility/args_anchor.hpp>

int increment(int value)
{
    return value + 1;
}

yggr::handler_center::handler_center<int> center;
center.register_handler(1, &increment);

int result = 0;
bool called = center.call(result, yggr::args_anchor(1), 4);
// called 为 true 时，result 为 5
```

注册同一 key 的第二个处理器不会覆盖既有项；应先移除旧项，或使用其他 key。

## 复制、移动与交换

该类支持拷贝和移动构造/赋值，也提供 `swap`。拷贝操作复制映射中的处理器共享指针，因此两个中心的注册项映射独立，但对应处理器对象由共享指针共同持有。可在不同 `Mutex` 模板参数的 handler center 之间执行相应的拷贝、移动和交换操作。

## 辅助宏

- `YGGR_PP_HANDLER_NAME(name)`：将处理器标识转换为字符串形式的名称。
- `YGGR_PP_HANDLER_NAME_T(type, name)`：将上述名称构造成指定类型。
- `YGGR_PP_MEM_HANDLER_NAME(class, name)`：组合类名和成员处理器名后生成字符串名称。
- `YGGR_PP_MEM_HANDLER_NAME_T(type, class, name)`：将成员处理器名称构造成指定类型。

这些宏用于生成名称值，不负责注册处理器。

## 配置与依赖

- 默认通过 `safe_unordered_map` 管理 key 与处理器容器，并依赖 handler container、参数锚点和共享指针组件。
- 未定义 `YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST` 时，本头文件将其默认设为 `0`，即使用处理器容器默认的参数/返回值类型检查方式。
- 启用可变参数模板时，`call` 使用参数包；否则接口通过预处理器按 `YGGR_PP_FOO_DEFAULT_PARAMS_LEN` 展开。
