# `handler_container.hpp` 

## 用途

本头文件将可调用对象包装成统一的处理器容器。调用方通过 `basic_handler_container::invoke` 传入参数（以及可选的返回值存储），容器根据包装的处理器签名检查并转发调用。它位于 `yggr::handler_center` 命名空间，是 handler center 调用机制的底层组件。

```cpp
#include <yggr/handler_center/handler_container.hpp>
```

## 主要类型与工厂

### `basic_handler_container`

抽象基类，定义统一调用接口，并通过受保护的纯虚函数 `pro_invoke` 将调用分派给具体处理器。其 `invoke_result_type` 为 `detail::invoke_result_t`，包含 `bret` 状态字段，并可显式转换为 `bool`。

支持两类调用形式：

```cpp
invoke(invoke_result_type& out, Args... args);          // 调用无返回值形式
invoke(Ret& ret, invoke_result_type& out, Args... args); // 将返回值写入 ret
```

两种形式都返回 `out` 的 const 引用；调用是否成功由 `out.bret` 表示。调用方应在调用前初始化结果对象的 `bret`，尤其是无返回值形式会断言它当前为 `false`。带返回值的形式会将处理器结果写入 `ret`。

### `handler_container<Handler>`

保存一个 `Handler` 实例并实现 `pro_invoke`。处理器的返回类型、参数类型和可调用形式由 `func::foo_t_info` 提取；实际参数通过 packet info caster 与处理器所需参数进行匹配。

### 工厂函数

- `make_handler_container(handler)`：按值创建 `handler_container<Handler>`。
- `make_shared_handler_container(handler)`：创建并返回 `yggr::shared_ptr<basic_handler_container>`。
- `make_shared_handler_container_custom<SPtr>(handler)`：使用指定的单参数智能指针模板创建并返回指向基类的指针。

## 调用与匹配行为

- 普通函数对象、函数及成员函数等可调用对象由处理器特征识别，并通过 `caller::to_params_call` 调用。
- 对成员函数，接收者需要作为对象引用参数传入；通过对象指针调用成员函数的形式不受支持。成员函数的 const 性会影响所需对象引用类型。
- 返回值及参数类型必须与处理器声明匹配。默认使用 `args_holder_ref_wrap_dynamic_cast` 检查持有器类型；类型不匹配时调用返回失败状态，不会把返回值丢弃后伪装成成功。
- 对 `void` 返回处理器，返回值持有器必须表示 void；代码通过断言检查该条件。参数持有器转换失败时同样返回失败状态。
- 成功调用时状态设为 `true`；处理器本身抛出的异常没有在本头文件中捕获，会向调用方传播。

## 使用示例

```cpp
#include <yggr/handler_center/handler_container.hpp>

int add_one(int value)
{
    return value + 1;
}

yggr::handler_center::basic_handler_container_sptr_type handler =
    yggr::handler_center::make_shared_handler_container(&add_one);

yggr::handler_center::basic_handler_container::invoke_result_type result = { false };
int value = 0;
handler->invoke(value, result, 4);
// result.bret 为 true，value 为 5
```

示例展示了返回值形式的调用；调用参数类型应与处理器签名兼容。

## 配置与依赖

- 启用可变参数模板时，`invoke` 使用参数包生成接口；旧编译器路径通过预处理器按 `YGGR_PP_FOO_DEFAULT_PARAMS_LEN` 展开重载。
- `YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST` 未定义时默认为 `0`，即默认使用动态转换检查。设为非零时改用静态转换，调用方需自行保证持有器实际类型正确，否则可能产生未定义行为。
- 实现依赖 packet info/caster、参数调用器、参数持有器、处理器特征和 Yggdrasil shared pointer 等组件。
