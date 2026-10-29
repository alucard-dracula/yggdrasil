# `has_operator_index_member_access.hpp` 中文说明

## 用途

本头文件提供编译期类型特征，用于检测类类型是否具有下标运算符 `operator[]`，并识别可进行下标访问的对象指针和数组类型。

```cpp
#include <yggr/type_traits/has_operator_index_member_access.hpp>
```

## 类型特征

### `has_operator_index_member_access_class<T>`

检测移除 cv 限定和引用后的类类型是否具有可检测的 `operator[]`。结果通过 `value` 提供。

### `has_operator_index_member_access<T>`

在类检测基础上，对指针和数组类型作额外处理：

- 类类型：通过类的 `operator[]` 检测逻辑判定。
- 指针类型：指向非 `void`、非函数类型的指针返回 `true`。
- 数组类型：返回 `true`。
- 其他非类类型：返回 `false`。

这两个特征均在 `yggr::type_traits` 命名空间定义，并通过 `yggr` 命名空间导出。

## 示例

```cpp
#include <yggr/type_traits/has_operator_index_member_access.hpp>

struct indexed
{
    int& operator[](std::size_t);
};

struct plain
{
};

static_assert(yggr::has_operator_index_member_access_class<indexed>::value,
              "indexed has operator[]");
static_assert(!yggr::has_operator_index_member_access_class<plain>::value,
              "plain has no operator[]");
static_assert(yggr::has_operator_index_member_access<int*>::value,
              "object pointer supports indexing");
static_assert(yggr::has_operator_index_member_access<int[4]>::value,
              "array supports indexing");
```

## 检测实现与边界

- 类类型检测通过派生探测类型和回退 `operator[]` 参与重载解析来实现，是对成员运算符存在性的启发式检测。
- 检测不验证某个具体索引参数是否可用，也不验证返回类型或表达式 `object[index]` 是否满足调用方要求。
- 类中重载或模板化的 `operator[]`、不可访问成员及复杂继承关系可能影响检测结果；建议对目标类型进行实际验证。
- 指针的 `true` 仅表示指针指向的对象类型可进行内建下标访问；该特征不检查指针是否为空、是否指向有效对象或索引是否越界。
- `void*` 和函数指针不会被识别为可下标访问的对象指针。
