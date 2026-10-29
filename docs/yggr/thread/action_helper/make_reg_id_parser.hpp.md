# `make_reg_id_parser.hpp` 中文说明

## 用途

本头文件定义宏 `YGGR_PP_THREAD_ACTRION_HELPER_MAKE_REG_ID_PARSER`，用于在 `yggr::thread::action_helper` 命名空间中生成注册 ID 解析器类型。该解析器将类型模板调用转发给指定的基础解析器。

```cpp
#include <yggr/thread/action_helper/make_reg_id_parser.hpp>
```

## 宏参数

```cpp
YGGR_PP_THREAD_ACTRION_HELPER_MAKE_REG_ID_PARSER(
    TypeName,
    ResultType,
    BaseParser,
    Operation)
```

| 参数 | 含义 |
| --- | --- |
| `TypeName` | 生成的解析器结构体名称。 |
| `ResultType` | 解析结果类型，作为生成类型的 `result_type`。 |
| `BaseParser` | 提供底层静态解析操作的类型。 |
| `Operation` | `BaseParser` 上的静态成员模板名称。 |

宏展开后，生成的类型大致如下：

```cpp
struct TypeName
{
    typedef ResultType result_type;

    template<typename T>
    result_type operator()() const
    {
        return BaseParser::Operation<T>();
    }
};
```

生成的结构体位于 `yggr::thread::action_helper` 命名空间，调用时默认构造即可。

## 使用示例

假设已有解析器 `base_parser`，其中 `get_id<T>()` 返回兼容的 ID：

```cpp
YGGR_PP_THREAD_ACTRION_HELPER_MAKE_REG_ID_PARSER(
    my_reg_id_parser,
    my_id_type,
    base_parser,
    get_id)
```

之后可通过 `my_reg_id_parser::result_type` 获取返回类型，并以 `my_reg_id_parser().operator()<SomeType>()` 查询类型对应的 ID。

## 与 `action_id_parser` 的关系

宏生成的类型可作为 `action_id_parser<Reg_ID_Parser, Task_ID_Parser>` 的 `Reg_ID_Parser` 参数。生成的 `result_type` 应与任务 ID 解析器提供的结果类型兼容，以满足 `action_id_parser` 的编译期类型约束。

## 注意事项

- 宏名中的 `ACTRION` 是源码中的拼写，使用时必须保持一致。
- `TypeName` 在 `yggr::thread::action_helper` 命名空间中定义；不要在同一作用域重复生成相同名称。
- `BaseParser::Operation<T>()` 必须是可调用的静态成员模板，且返回值可转换为 `ResultType`。
- 本头文件只定义生成宏，不会自动实例化解析器或校验 ID 唯一性。
